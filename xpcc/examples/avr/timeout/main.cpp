#include <xpcc/architecture/platform.hpp>

#include <xpcc/architecture/driver/gpio.hpp>
#include <xpcc/workflow/timeout.hpp>

// create a output device for the led
GPIO__OUTPUT(Led, B, 0);

// timer interrupt routine
ISR(TIMER2_COMPA_vect)
{
	xpcc::Clock::increment();
}

int
main()
{
	Led::setOutput();
	Led::reset();
	
	// timer initialization
	// compare-match-interrupt every 1 ms at 14.7456 MHz
	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << CS22);
	TIMSK2 = (1 << OCIE2A);
	OCR2A = 230;
	
	// enable interrupts
	sei();
	
	xpcc::Timeout<> timeout(200);
	while (1)
	{
		if (timeout.isExpired())
		{
			timeout.restart(200);
			Led::toggle();
		}
	}
}
