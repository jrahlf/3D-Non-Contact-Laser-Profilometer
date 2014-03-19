/*
 * pcInterface.cpp
 *
 *  Created on: Dec 30, 2013
 *      Author: jonas
 */

#include "pcInterface.h"
#include "project.h"
#include "atof.h"
#include "control.h"
#include "limits.h"
#include <xpcc/architecture.hpp>
#include "commandHandlers.h"

xpcc::stm32::BufferedUsart2 uart(921600, 10);
xpcc::IODeviceWrapper<xpcc::stm32::BufferedUsart2> uartWrap(uart);
xpcc::IOStream coutRaw(uartWrap);

xpcc::IODeviceWrapper<Stream<Prefix::POSITION, false> > poutWrap;
xpcc::IOStream pout(poutWrap);

xpcc::IODeviceWrapper<Stream<Prefix::MEASUREMENT, true> > moutWrap;
xpcc::IOStream mout(moutWrap);

xpcc::IODeviceWrapper<Stream<Prefix::DEBUG, false> > doutWrap;
xpcc::IOStream dout(doutWrap);

xpcc::IODeviceWrapper<Stream<Prefix::ERROR, false> > cerrWrap;
xpcc::IOStream cerr(cerrWrap);

xpcc::IODeviceWrapper<Stream<Prefix::INFO, false> > iWrap;
xpcc::IOStream iout(iWrap);

xpcc::IODeviceWrapper<Stream<Prefix::COMMAND, false> > cWrap;
xpcc::IOStream commandOut(cWrap);

xpcc::IODeviceWrapper<Stream<Prefix::CONTROL, false> > kWrap;
xpcc::IOStream controlOut(kWrap);

namespace{
const int BUFFER_LENGTH = 512;
char buffer[BUFFER_LENGTH] = {0};
int bufPos = 0;

PC::Command CommandTable[] = {
    { "GO X", Handlers::gotoHandler<X> } //goto x [mm]
   ,{ "GO Y", Handlers::gotoHandler<Y> } //goto y [mm]
   ,{ "MOVX", Handlers::moveHandler<X> } //move x by [mm]
   ,{ "MOVY", Handlers::moveHandler<Y> } //move y by [mm]
   ,{ "SPEX", Handlers::setSpeedHandler<X> } //Speed X [-1,1]
   ,{ "SPEY", Handlers::setSpeedHandler<Y> } //Speed Y [-1,1]
   ,{ "PWMX", Handlers::setPwmHandler<X> } //PWM X [-1200,1200]
   ,{ "PWMY", Handlers::setPwmHandler<Y> } //PWM Y [-1200,1200]
   ,{ "MAXS", Handlers::setMaxSpeedHandler } //sets max speed for patterns [0,1]
   ,{ "EN S", Handlers::enableSystickHandler } //enable systick
   ,{ "ER L", Handlers::getLaserError } //get current laser error
   ,{ "EN T", Handlers::enableTrigger } //enable trigger, param is 0 or 1
   ,{ "SETT", Handlers::setTriggerThreshold } //set threshold for trigger in encoder counts
   ,{ "BOOT", Handlers::resetHandler } //reset microController
    //Rectangle pattern, parameters: x,y,width,height,sampling resolution;
    //First call demonstrates the size, second call confirms
   ,{ "PAR1", Handlers::rectanglePatternHandler }
    //Rectangle pattern, parameters: start x, start y, stop x , stop y, sampling resolution;
	//First call demonstrates the size, second call confirms
   ,{ "PAR2", Handlers::rectanglePatternHandler2 }
   ,{ "PAPA", Handlers::pausePatternHandler } //pauses the current pattern
   ,{ "PAST", Handlers::stopPatternHandler } //stops the current pattern
   ,{ "PARE", Handlers::resumePatternHandler } //resumes execution of current pattern
    //shall be called 4 consecutive times for coordinates, requires sampling resolution as parameter
   ,{ "PAR3", Handlers::rectanglePatternHandler3 }
   ,{ "PAL1", Handlers::linePatternHandler} //shall be called 2 times, requires sampling resolution as parameter
   ,{ "GETB", Handlers::getPushButtonStates }
   ,{ "GETM", Handlers::getMeasurement }
   ,{ "GECX", Handlers::getControlOutputs<X> }
   ,{ "GECY", Handlers::getControlOutputs<Y> }
   ,{ "EXEC", Handlers::execLaserCommand } //execute a laser command
   ,{ "MIRR", Handlers::mirrorCommand } //outputs the parameter / mirrors input to the output as a command
};
}

bool PC::init(){
	uart.configurePins(xpcc::stm32::BufferedUsart2::Mapping::REMAP_PA2_PA3);
	//sanity check:
	int size = sizeof(CommandTable)/sizeof(CommandTable[0]);
	for(int x = 0; x < size; x++){
		for(int y = x+1; y < size; y++){
			int word1 = *(int*)&CommandTable[x].name[0];
			int word2 = *(int*)&CommandTable[y].name[0];
			if(word1 == word2){
				cerr << "duplicate entry in command table: " << CommandTable[x].name << endl;
				return false;
			}
		}
	}
	return true;
}

//TODO binary search
PC::Command* PC::getCommand(const char* command){
	int size = sizeof(CommandTable)/sizeof(CommandTable[0]);
	for(int i = 0; i < size; i++){
		int word1 = *(int*)&CommandTable[i].name[0];
		int word2 = *(int*)&command[0];
		if(word1 == word2){
			return &CommandTable[i];
		}
	}
	return 0;
}

void PC::scanForCommands(){
	uint8_t c;
	if(uart.read(c)){
		if(c != '\n'){
			buffer[bufPos++] = char(c);
			if(bufPos >= BUFFER_LENGTH - 4){
				bufPos = 0;
			}
		}else{
			buffer[bufPos] = 0;
			buffer[bufPos+1] = 0;
			buffer[bufPos+2] = 0;
			buffer[bufPos+3] = 0;
			if(bufPos < 3){
				dout << "Ignoring " << buffer;
				bufPos = 0;
				return;
			}
			Command* cmd = getCommand(buffer);
			if(!cmd){
				cerr << "cannot interpret " << buffer << endl;
				bufPos = 0;
				return;
			}
			bool ok = cmd->handler(&buffer[5], bufPos-5);
			if(ok){
				iout << "OK: " << buffer << endl;
			}
			bufPos = 0;
		}
	}
}

void PC::registerCommandHandler(const char* command, bool (*handler)(const char*, int)){
	Command* cmd = PC::getCommand(command);
	if(!cmd){
		cerr << "Error tried to register handler for unknown command: " << command << endl;
		return;
	}
	cmd->handler = handler;
}


bool PC::defaultHandler(const char* command, int length){
	cerr << "Unhandled command: " << command << endl;
	return false;
}
