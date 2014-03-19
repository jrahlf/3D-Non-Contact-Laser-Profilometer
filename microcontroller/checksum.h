/*
 * checksum.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: jonas
 */

#pragma once

struct Checksum{
    /**
     * @brief getFor: Returns the checksum for a message
     * @param data message the checksum shall be calculated for
     * @param length number of characters, the newline character shall not be included
     * @param specifier the specifier of the message
     * @return computed checksum
     */
    static char getFor(const char* data, int length, char specifier){
		char checksum = 0x0;
		const char* const pEnd = data+length;
		while(data != pEnd){
			checksum ^= *data;
			data++;
		}
        checksum ^= specifier;
		return checksum;
	}
};


