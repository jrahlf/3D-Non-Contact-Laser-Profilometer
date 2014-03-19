/*
 * laserInterface.h
 *
 *  Created on: Dec 30, 2013
 *      Author: jonas
 */

#ifndef LASERINTERFACE_H_
#define LASERINTERFACE_H_

#include "project.h"

class Laser{
public:

	class DoubleBuffer{
	public:
		static unsigned char buf1[LASER_INPUT_BUFFER_SIZE+1];
		static unsigned char buf2[LASER_INPUT_BUFFER_SIZE+1];
		//swap pointers instead of copying
		static unsigned char* bufInPtr;
		static unsigned char* bufOutPtr;
		static int inSize;
		static int outSize;

	public:

		static inline int getInSize(){
			return inSize;
		}

		static inline void addChar(unsigned char c){
			bufInPtr[inSize] = c;
			inSize++;
		}

		static inline unsigned char* inPtr(){
			return bufInPtr;
		}

		static inline unsigned char* outPtr(){
			return bufOutPtr;
		}

		/**
		 * Returns outPtr as an integer
		 * @return the current measurement in nm
		 */
		static int asInt();

		/**
		 * Also resets input buffer and adds trailing
		 * null byte to new output buffer
		 */
		static inline void swap(){
			unsigned char* temp = bufInPtr;
			bufInPtr = bufOutPtr;
			bufOutPtr = temp;
			bufOutPtr[inSize] = '\0';
			outSize = inSize;
			inSize = 0;
		}

		static inline void reset(){
			inSize = 0;
		}

		static char* get(){
			return (char*)bufOutPtr;
		}

		static int good();
	};

public:
	static bool init();
	static int getLastError();
	static void enableRequests();
	static void disableRequests();
	static void sendString(const char* cmd);
	static const char* exec(const char* cmd);
	static const char* getMeasurement();

	static void handleChar(char c);
	static void sendMeasurementRequest();
};

#endif /* LASERINTERFACE_H_ */
