#include <xpcc/architecture.hpp>

#include "control.h"
#include "encoder.h"
#include "motor.h"
#include "hallSensor.h"
#include "project.h"
#include "pcInterface.h"
#include "laserInterface.h"
#include "Trigger.h"
#include "pattern.h"
#include "utils.h"

using namespace xpcc::stm32;

static bool initClock(){
	// use external 8MHz crystal
	if (!Clock::enableHse(Clock::HseConfig::HSE_CRYSTAL)) {
		return false;
	}
	Clock::enablePll(Clock::PllSource::PLL_HSI, 8, 168);
	return Clock::switchToPll();
}


template<Axis axis>
void printControl(){
	static long long unsigned int step = 0;
	controlOut << step << " " << Control<axis>::getShallPosition() << " " << Control<axis>::getIsPosition() << " "
			<< Control<axis>::getShallSpeed() << " " << Control<axis>::getIsSpeed() << " "
			<< Control<axis>::getSpeedU() << " " << Control<axis>::getSpeedRealU() << endl;
	step++;
}

//int last u;
static int counter = 0;
void systick(){
	//if(Control)
	Control<ALL>::update(false);
	counter++;
#if !USE_LASER_DMA
	if(counter == 1000){
		Laser::sendMeasurementRequest();
		counter = 0;
	}
#endif
	/*counter++;
	if(counter > 500){
		Laser::sendMeasurementRequest();
		counter = 0;
	}*/
	//printControl<X>();
}

GPIO__OUTPUT(LASER_TIMING, E, 1);
GPIO__INPUT(LASER_STATUS, E, 5);


MAIN_FUNCTION{
	bool ok;
	ok = initClock(); //if this fails, we are going to have a bad time

	//--------------------------------------------------------------
	//Init routines
	//--------------------------------------------------------------
	State::init(); //state handling, can visualize init failures via LEDs
	ok = PC::init(); //starts up COM interface to computer
	dout << "COM INIT " << ok << endl;
	ok = Encoder::init();
	dout << "ENCODER INIT " << ok << endl;
	ok = Motor<ALL>::init();
	dout << "MOTOR INIT " << ok << endl;
	ok = HallSensor<ALL>::init();
	dout << "HALLSENSOR INIT " << ok << endl;
	ok = Control<ALL>::init();
	dout << "CONTROL INIT " << ok << endl;
	ok = Laser::init();
	dout << "LASER INTERFACE INIT " << ok << endl;

	//--------------------------------------------------------------
	//go go go
	//--------------------------------------------------------------
	SysTickTimer::attachInterrupt(systick);

	Control<ALL>::enablePositionControl(false);
	Control<ALL>::enableSpeedControl(false);
	Utils::calibrateZero();

	Utils::enableSystick();
	Laser::enableRequests();
	State::set(State::READY);

	Control<ALL>::enableSpeedControl(true);
	Control<ALL>::enablePositionControl(true);

	Trigger::forceTrigger();

	//LASER_TIMING::setOutput(xpcc::stm32::OPEN_DRAIN);

	//Trigger::enable(false);
int i = 0;
	while(1){
		/*LASER_TIMING::set(true);
		xpcc::delay_ms(20);
		LASER_TIMING::set(false);
		xpcc::delay_ms(20);

		coutRaw << "last m " << Laser::DoubleBuffer::get() << endl;
		coutRaw << "rec " << Laser::DoubleBuffer::inSize << endl;*/

		Trigger::sample();
		PC::scanForCommands();
		Trigger::sample();
		Pattern::follow();

		//coutRaw << "my " << Motor<Y>::getFaultStatus() << endl;
		//if(Control<Y>::getIsSpeed() != 0)
			//coutRaw << Control<Y>::getIsSpeed() << endl;
		//coutRaw << "X: " << Control<X>::getIsPosition();
		//coutRaw << " sy "  << Control<Y>::getIsPosition() << " vy " << Control<Y>::getIsSpeed() << endl;
		//coutRaw << " ::: " << Laser::DoubleBuffer::asInt() << " " << Laser::DoubleBuffer::get() << endl;
	}
}
