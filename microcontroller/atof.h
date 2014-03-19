/*
 * atof.h
 *
 *  Created on: Jan 3, 2014
 *      Author: jonas
 */

#ifndef ATOF_H_
#define ATOF_H_

/**
 * Takes a pointer to a string and converts the
 * first number found into an int and returns it
 * If the number is a fractional number, it is expected to
 * have exactly 2 decimal digits. The value is multiplied by
 * 1000 prior returning it
 *
 * @param p pointer to string
 * @param stopPosition position where parsing was stopped due to non numerical character
 * 				this is a pointer to the original string and can be reused to parse several arguments
 * 				0 can be passed if position is not needed
 * @return Integer of string, if fractional 1000 times the value
 */
int atoi2(const char *p, const char** stopPosition);

/**
 * Same as atoi2(const char *p), but scans by at most maxLen characters
 * @param p pointer to string
 * @param maxLen
 * @param errorValue value which is returned if no number is found
 * @return Integer of string, or errorValue
 */
int atoi2(const char *p, const int maxLen, const int errorValue);

/**
 * @param p pointer to string
 * @param maxLen
 * @param errorValue value which is returned if no number is found
 * @return float of string, or errorValue
 */
float atof2(const char *p, const int maxLen, const float errorValue);

/**
 * @param p pointer to string
 * @param maxLen
 * @param errorValue value which is returned if no number is found
 * @return float of string, or errorValue
 */
float atof2(const char *p, const char** stopPosition, const float errorValue);



#endif /* ATOF_H_ */
