/*
 * atof.c
 *
 *  Created on: Jan 3, 2014
 *      Author: jonas
 */


#include "atof.h"

//
// Simple and fast atof (ascii to float) function.
//
// - Executes about 5x faster than standard MSCRT library atof().
// - An attractive alternative if the number of calls is in the millions.
// - Assumes input is a proper integer, fraction, or scientific format.
// - Matches library atof() to 15 digits (except at extreme exponents).
// - Follows atof() precedent of essentially no error checking.
//
// 09-May-2009 Tom Van Baak (tvb) www.LeapSecond.com
//

/**
* source: http://www.leapsecond.com/tools/fast_atof.c
* Modified by: Jonas Rahlf
*
* see atof.h for documentation
*/

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')
#define valid_char(c) (((c) >= '0' && (c) <= '9') || c == '+' || c == '-')
int atoi2 (const char *p, const char** stopPosition){
    int sign = 0;
    int value = 0;
    const char* const start = p;

    // Skip leading garbage

    while (!valid_char(*p) ) {
    	if(p == '\0'){
    		goto exit;
    	}
        p += 1;
    }

    // Get sign, if any.

    sign = 1.0;
    if (*p == '-') {
        sign = -1.0;
        p += 1;

    } else if (*p == '+') {
        p += 1;
    }

    //skip leading zeros
    /*while(*p == '0')
    	p++;*/

    // Get digits before decimal point or exponent, if any.

    for (value = 0.0; valid_digit(*p); p += 1){
        value = value * 10.0 + (*p - '0');
    }

    if (*p == '.'){
    	p++;
		for (; valid_digit(*p); p += 1){
			value = value * 10.0 + (*p - '0');
		}
		value *= 10;
    }

    // Get digits after decimal point, if any.

    /*if (*p == '.') {
        double pow10 = 10.0;
        p += 1;
        while (valid_digit(*p)) {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p += 1;
        }
    }*/

    // Return signed and scaled floating point result.
    exit: ;
    if(stopPosition != 0){
    	*stopPosition = p;
    }
    return sign * value;
}

int atoi2 (const char *p, const int maxLen, const int garbage){
    int sign;
    int value;
    const char* pEnd = p+maxLen;

    // Skip leading garbage
    if(p == pEnd){
    	return garbage;
    }

    while (!valid_char(*p) ) {
        p += 1;
        if(p == pEnd){
        	return garbage;
        }
    }

    // Get sign, if any.

    sign = 1.0;
    if (*p == '-') {
        sign = -1.0;
        p += 1;

    } else if (*p == '+') {


        p += 1;
    }

    if(p == pEnd){
		return garbage;
	}

    //skip leading zeros
    while(*p == '0'){
    	p++;
    	if(p == pEnd){
			return 0; //not garbage
		}
    }

    // Get digits before decimal point or exponent, if any.

    for (value = 0.0; valid_digit(*p); p += 1){
    	if(p == pEnd){
			return sign * value;
		}
        value = value * 10.0 + (*p - '0');

    }

    if (*p == '.'){
    	p++;
		for (; valid_digit(*p); p += 1){
			if(p == pEnd){
				return sign * value;
			}
			value = value * 10.0 + (*p - '0');
		}
		value *= 10;
    }

    // Get digits after decimal point, if any.

    /*if (*p == '.') {
        double pow10 = 10.0;
        p += 1;
        while (valid_digit(*p)) {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p += 1;
        }
    }*/

    // Return signed and scaled floating point result.

    return sign * value;
}

float atof2 (const char *p, const char** stopPosition, const float garbage){
    int sign;
    float value;

    // Skip leading garbage

    while (!valid_char(*p) ) {
        p += 1;
        if(*p == '\0'){
        	goto exit;
        }
    }

    // Get sign, if any.

    sign = 1.0;
    if (*p == '-') {
        sign = -1.0;
        p += 1;

    } else if (*p == '+') {
        p += 1;
    }

    //skip leading zeros
    while(*p == '0'){
    	p++;
    }

    // Get digits before decimal point or exponent, if any.

    for (value = 0.0; valid_digit(*p); p += 1){
        value = value * 10.0 + (*p - '0');
    }

    // Get digits after decimal point, if any.

    if (*p == '.') {
        double pow10 = 10.0;
        p += 1;
        while (valid_digit(*p)) {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p += 1;
        }
    }

    // Return signed and scaled floating point result.
    exit: ;
	if(stopPosition != 0){
		*stopPosition = p;
	}
    return sign * value;
}

float atof2 (const char *p, const int maxLen, const float garbage){
    int sign;
    float value;
    const char* pEnd = p+maxLen;

    // Skip leading garbage
    if(p == pEnd){
    	return garbage;
    }

    while (!valid_char(*p) ) {
        p += 1;
        if(p == pEnd){
        	return garbage;
        }
    }

    // Get sign, if any.

    sign = 1.0;
    if (*p == '-') {
        sign = -1.0;
        p += 1;

    } else if (*p == '+') {
        p += 1;
    }

    if(p == pEnd){
		return garbage;
	}

    //skip leading zeros
    while(*p == '0'){
    	p++;
    	if(p == pEnd){
			return 0;
		}
    }

    // Get digits before decimal point or exponent, if any.

    for (value = 0.0; valid_digit(*p); p += 1){
    	if(p == pEnd){
			return sign * value;
		}
        value = value * 10.0 + (*p - '0');
    }

    // Get digits after decimal point, if any.

    if (*p == '.') {
        double pow10 = 10.0;
        p += 1;
        if(p == pEnd){
			return sign * value;
		}
        while (valid_digit(*p)) {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p += 1;
            if(p == pEnd){
				return sign * value;
			}
        }
    }

    // Return signed and scaled floating point result.

    return sign * value;
}

