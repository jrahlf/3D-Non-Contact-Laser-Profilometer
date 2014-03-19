/*
 * Example for a simple IO extension
 * 
 * This program enables a master to use the A/D converter and PORTD
 * from extern.
 */

#include <xpcc/architecture/platform.hpp>
#include <xpcc/driver/connectivity/sab/slave.hpp>

// ----------------------------------------------------------------------------
// wrapper class for the A/D converter
class AnalogDigital : public xpcc::sab::Callable
{
public:
	AnalogDigital()
	{
		// initialize the analog to digital converter
		xpcc::atmega::Adc::initialize(
				xpcc::atmega::Adc::REFERENCE_INTERNAL_AVCC, xpcc::atmega::Adc::PRESCALER_64);
	}
    
	void
	readChannel(xpcc::sab::Response& response, const uint8_t *channel)
	{
		uint16_t value = xpcc::atmega::Adc::readChannel(*channel);
		response.send(value);
	}
};

// ----------------------------------------------------------------------------
// wrapper for PORTD
class InOut : public xpcc::sab::Callable
{
public:
	void
	setDirection(xpcc::sab::Response& response, const uint8_t *direction)
	{
		DDRD = *direction;
		
		// send a response without any attached data
		response.send();
	}
	
	void
	readInput(xpcc::sab::Response& response)
	{
		uint8_t value = PIND;
		response.send(value);
	}
	
	void
	setOutput(xpcc::sab::Response& response, const uint8_t *output)
	{
		PORTD = *output;
		response.send();
	}
};

// ----------------------------------------------------------------------------
// create instances from the wrapper classes
AnalogDigital analogDigital;
InOut inOut;

// ----------------------------------------------------------------------------
// create a list of all possible actions
FLASH_STORAGE(xpcc::sab::Action actionList[]) =
{
	SAB__ACTION( 'A', analogDigital,	AnalogDigital::readChannel,	1 ),
	SAB__ACTION( 'D', inOut,			InOut::setDirection,		1 ),
	SAB__ACTION( 'I', inOut,			InOut::readInput,			0 ),
	SAB__ACTION( 'O', inOut,			InOut::setOutput,			1 ),
};

// wrap the type definition inside a typedef to make the code more readable
typedef xpcc::sab::Slave< xpcc::sab::Interface< xpcc::atmega::BufferedUart0 > > Slave;

// ----------------------------------------------------------------------------
MAIN_FUNCTION
{
	// initialize ABP interface, set baudrate etc.
	Slave slave(0x02,
			xpcc::accessor::asFlash(actionList),
			sizeof(actionList) / sizeof(xpcc::sab::Action));
	
	// enable interrupts
	sei();
	
	while (1)
	{
		// decode received messages etc.
		slave.update();
	}
}
