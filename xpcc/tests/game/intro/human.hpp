
#ifndef HUMAN_HPP
#define HUMAN_HPP

#include <xpcc/workflow/protothread.hpp>
#include <xpcc/workflow/timeout.hpp>
#include <xpcc/driver/lcd/graphic_display.hpp>

class Human : public xpcc::Protothread
{
public:
	Human(xpcc::glcd::Point start, xpcc::GraphicDisplay *display);
	
	void
	draw();
	
protected:
	bool
	run();
	
	xpcc::glcd::Point position;
	xpcc::GraphicDisplay *display;
	xpcc::Timeout<> timer;
	uint8_t i;
	
	enum { STAND, WAVE0, WAVE1 } state;
};

#endif // HUMAN_HPP
