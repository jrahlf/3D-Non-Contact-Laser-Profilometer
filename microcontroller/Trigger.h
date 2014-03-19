/*
 * Trigger.h
 *
 *  Created on: Jan 16, 2014
 *      Author: jonas
 */

#ifndef TRIGGER_H_
#define TRIGGER_H_

/**
 * Used in conjunction with the laser and sends the current
 * measurement + position if the stage moved by a certain threshold
 */
class Trigger {
private:
public:
	/**
	 * Should be called from main loop at maximum frequency
	 */
	static void sample();
	static void forceTrigger();
	/**
	 * Sets a new threshold in encoder counts
	 * @param threshold
	 */
	static void setThreshold(int threshold);
	static int getThreshold();
	/**
	 * Enable or disables triggering
	 * @param enable
	 */
	static void enable(bool enable);
	/**
	 * If disabled, only the position will be output
	 * @param enable
	 */
	static void enableMeasurementOutput(bool enable);

};


#endif /* TRIGGER_H_ */
