/*
 * pcInterface.h
 *
 *  Created on: Dec 30, 2013
 *      Author: jonas
 */

#ifndef PCINTERFACE_H_
#define PCINTERFACE_H_

#include "project.h"
#include "checksum.h"
#ifdef __arm__
#include <xpcc/architecture.hpp>

extern xpcc::stm32::BufferedUsart2 uart;
#endif

static const bool ENABLE_STREAMS = true;

struct Prefix{
	static const char DEBUG = 'D';
	static const char POSITION = 'P';
	static const char MEASUREMENT = 'M';
	static const char INFO = 'I';
	static const char ERROR = 'E';
	static const char COMMAND = 'C';
	static const char CONTROL = 'K';
};

class PC{
public:
	struct ucCommand{
		constexpr static const char* FLUSH_DATA = "flush data";
	};

	/**
	 * Setup UART for communication
	 * Should be the first init function to call
	 * @return
	 */
	static bool init();
	static void scanForCommands();
	static void registerCommandHandler(const char* command, bool (*handler)(const char*, int));

	struct Command {
	    const char* name;
	    bool (*handler)(const char*, int); //if true is returned, "ok - command" is printed on uart
	};

	static bool defaultHandler(const char*, int);
	static Command* getCommand(const char* command);
};

#ifdef __arm__

/**
 * Wrapper class to provide various streams
 */
template<char prefix, bool useChecksum, bool enable = ENABLE_STREAMS, int TEXT_BUFFER_SIZE = 300>
class Stream{
private:
	static char text[TEXT_BUFFER_SIZE];
	static int pos;
public:
	static void flush();
	static void writeMeasurement(char* measurement, int length);
	static void write(char c);
	static bool read(char &c){ (void)c; return false;}
	static bool read(uint8_t &c){ (void)c; return false;}
private:
	static void calcChecksum();
};

template<char prefix, bool useChecksum, bool enable, int TEXT_BUFFER_SIZE>
char Stream<prefix, useChecksum, enable, TEXT_BUFFER_SIZE>::text[TEXT_BUFFER_SIZE];

template<char prefix, bool useChecksum, bool enable, int TEXT_BUFFER_SIZE>
int Stream<prefix, useChecksum, enable, TEXT_BUFFER_SIZE>::pos;



template<char prefix, bool useChecksum, bool enable, int TEXT_BUFFER_SIZE>
void Stream<prefix, useChecksum, enable, TEXT_BUFFER_SIZE>::write(char c){
	if(!enable){
		return;
	}
	//if(prefix != 'M'){
		if(pos == TEXT_BUFFER_SIZE-3){
			text[pos++] = c;
			//text[pos+1] = '\n';
			//text[pos+2] = '\0';
			flush();
			return;
		}
		if(c == '\n'){
			flush();
			return;
		}

		text[pos++] = c;
	/*}else{ //zero overhead version for measurement
		//text[pos++] = c;
		uart.write(c);
	}*/
}

template<char prefix, bool useChecksum, bool enable, int TEXT_BUFFER_SIZE>
void Stream<prefix, useChecksum, enable, TEXT_BUFFER_SIZE>::flush(){
	if(!enable){
		return;
	}

	uart.write(prefix);
	if(useChecksum){
		calcChecksum();
	}
	uart.write(',');

	const char* ptr = &text[0];
	const char* end = &text[pos];

	while(ptr != end){
		uart.write(*ptr);
		ptr++;
	}

	//coutRaw << " l: " << pos;

	uart.write('\n');

	pos = 0;
}

template<char prefix, bool useChecksum, bool enable, int TEXT_BUFFER_SIZE>
void Stream<prefix, useChecksum, enable, TEXT_BUFFER_SIZE>::calcChecksum(){
	char check = Checksum::getFor(text, pos, prefix);
	uart.write(check);
}

#endif

#endif /* PCINTERFACE_H_ */
