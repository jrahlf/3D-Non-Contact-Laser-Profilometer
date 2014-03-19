#include "serial_port.hpp"
#include <iostream>

xpcc::pc::SerialPort::SerialPort():
	shutdown(true),
	port(io_service)
{
}

xpcc::pc::SerialPort::~SerialPort()
{
	this->close();
}

void
xpcc::pc::SerialPort::write(char c)
{
	this->io_service.post(boost::bind(&xpcc::pc::SerialPort::doWrite, this, c));
}


void
xpcc::pc::SerialPort::flush()
{
}

void
xpcc::pc::SerialPort::readStart()
{
	port.async_read_some(boost::asio::buffer(&this->tmpRead, sizeof(this->tmpRead)),
			boost::bind(&xpcc::pc::SerialPort::readComplete,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

bool
xpcc::pc::SerialPort::read(char& value)
{
	if(this->readBuffer.empty())
		return false;
	else
	{
		MutexGuard queueGuard( this->readMutex);
		value=this->readBuffer.front();
		this->readBuffer.pop();
		return true;
	}
}

bool
xpcc::pc::SerialPort::open(std::string deviceName, unsigned int baudRate)
{
	if (!this->isOpen())
	{
		this->deviceName = deviceName;
		this->baudRate = baudRate;
		
		//std::cout << "open port" << std::endl;

		this->shutdown = false;
		this->port.open(this->deviceName);
		if (!this->port.is_open()) {
			std::cerr << "Failed to open serial port " << deviceName << "\n";
			return false;
		}

		this->port.set_option(boost::asio::serial_port_base::baud_rate(this->baudRate));
		this->port.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
		this->port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
		this->port.set_option(boost::asio::serial_port_base::character_size(8));
		this->port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

		this->io_service.post(boost::bind(&SerialPort::readStart, this));

		this->thread = new boost::thread(boost::bind(&boost::asio::io_service::run, &this->io_service));
	}
	else {
		std::cerr << "Port already open!" << std::endl;
	}

	return true;
}


bool
xpcc::pc::SerialPort::isOpen()
{
	return this->port.is_open() && !this->shutdown;
}

void
xpcc::pc::SerialPort::close()
{
	if (!this->isOpen())
		return;

	this->io_service.post(boost::bind(
			&xpcc::pc::SerialPort::doClose,
			this,
			boost::system::error_code()));

	this->thread->join();
	delete this->thread;
	this->io_service.reset();
}

void
xpcc::pc::SerialPort::kill()
{
	if (!this->isOpen())
		return;

	this->io_service.post(boost::bind(
				&xpcc::pc::SerialPort::doAbort,
				this,
				boost::system::error_code()));
	this->shutdown = true;
	this->thread->join();
	delete this->thread;
	this->io_service.reset();
}

void
xpcc::pc::SerialPort::doAbort(const boost::system::error_code& error)
{

	if (error)
		std::cerr << "Error: " << error.message() << std::endl;
	this->port.close();
	if (error)
		std::cerr << "Error: " << error.message() << std::endl;
}

void
xpcc::pc::SerialPort::doClose(const boost::system::error_code& error)
{
	if( this->writeBuffer.empty() ) {
		this->doAbort(error);
	}
	this->shutdown = true;
}

void
xpcc::pc::SerialPort::doWrite(const char c) {
	if (!this->shutdown)
	{
		std::cout<<"get 0x"<< std::hex << (int) c << std::dec 
		<< "(" << c << ")" << std::endl;

		MutexGuard mutex(this->writeMutex);
		bool idle = this->writeBuffer.empty();
		this->writeBuffer.push(c);

		if (idle) {
			this->writeStart();
		}
	}
}

void
xpcc::pc::SerialPort::writeStart(void)
{
	boost::asio::async_write(this->port,
			boost::asio::buffer(&this->writeBuffer.front(), 1),
			boost::bind(&xpcc::pc::SerialPort::writeComplete, this,
					boost::asio::placeholders::error));
}

void
xpcc::pc::SerialPort::writeComplete(const boost::system::error_code& error)
{
	if (!error) {
		MutexGuard mutex(this->writeMutex);
		this->writeBuffer.pop();
		if (!this->writeBuffer.empty()) {
			std::cout << "restart " << this->writeBuffer.size() << std::endl;
			this->writeStart();
		}
		else if (this->shutdown) {
			this->doAbort(error);
		}
	}
	else {
		std::cerr << "Error in write: " << error.message() << std::endl;
		this->doAbort(error);
	}
}

void
xpcc::pc::SerialPort::readComplete(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
    	{
			MutexGuard queueGuard( this->readMutex);
			for(unsigned int i=0; i<bytes_transferred; ++i)
			{
				std::cout<<"get 0x"<< std::hex << (int) this->tmpRead[i] << std::dec 
				<< "(" << this->tmpRead[i] << ")" << std::endl;
				this->readBuffer.push(this->tmpRead[i]);
			}
    	}
        this->readStart();
    }
    else
    {
		doClose(error);
    }
}

void
xpcc::pc::SerialPort::clearReadBuffer()
{
	MutexGuard queueGuard( this->readMutex);
	while(!this->readBuffer.empty()) {
		this->readBuffer.pop();
	}
}

void
xpcc::pc::SerialPort::clearWriteBuffer()
{
	while(!this->writeBuffer.empty()) {
		this->writeBuffer.pop();
	}
}
