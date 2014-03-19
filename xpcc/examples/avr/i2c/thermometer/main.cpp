
#include <xpcc/architecture.hpp>
#include <xpcc/driver/temperature/ds1631.hpp>


#include <xpcc/io/iostream.hpp>

GPIO__IO(Scl, C, 0);
GPIO__IO(Sda, C, 1);

// Create a new UART object and configure it to a baudrate of 9600
xpcc::atmega::BufferedUart0 uart(9600);

//#define USE_SOFTWARE
#define USE_HARDWARE

#if defined USE_SOFTWARE
#include <xpcc/driver/connectivity/i2c/software_i2c.hpp>
typedef xpcc::SoftwareI2C<Scl, Sda> I2C;
#endif

#if defined USE_HARDWARE
typedef xpcc::atmega::I2cMaster I2C;
#endif

void
die()
{
	while (1) {
		// wait forever
	}
}

int
main()
{
	// Enable interrupts, this is needed for every buffered UART
	sei();
	
	// Create a IOStream for complex formatting tasks
	xpcc::IODeviceWrapper<xpcc::atmega::BufferedUart0> device(uart);
	xpcc::IOStream output(device);
	
	output << "Thermometer" << xpcc::endl;
	
#if defined USE_SOFTWARE
	// Initialize the I2C interface.
	I2C::initialize();
#endif

#if defined USE_HARDWARE
	I2C::initialize(65, 0);
#endif
	
	uint8_t data[2];
	xpcc::Ds1631< I2C > ds1631(data, 0x90);

	// Enable the 12 bit resolution
	ds1631.configure(xpcc::ds1631::RESOLUTION_12BIT, false);

	xpcc::delay_ms(1);

	// start a new conversion
	ds1631.startConversion();

	while (1)
	{
		if (ds1631.isConversionDone())
			ds1631.readTemperature();

		if (ds1631.isNewDataAvailable())
		{
			int16_t temperature = ds1631.getTemperature();
			// print the integer part
			output << (temperature >> 8) << ".";

			// print the fractional part
			uint16_t low  = temperature & 0xff;
			for (uint8_t i = 0; i < 4; ++i)
			{
				low *= 10;
				uint8_t c = low >> 8;
				low &= 0x00ff;
				output << c;
			}
			output << xpcc::endl;

			// start a new conversion
			ds1631.startConversion();
		}
	}
}
