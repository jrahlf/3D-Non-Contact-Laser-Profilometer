// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------

#include "serial_interface.hpp"

#include <iostream>
#include <ios>
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>		// file control
#include <sys/ioctl.h>	// I/O control routines
#include <termios.h>	// POSIX terminal control
#include <unistd.h>
#include <sys/socket.h>
#include <asm/socket.h>

#include <errno.h>

#include <xpcc/debug/logger.hpp>

#undef XPCC_LOG_LEVEL
#define XPCC_LOG_LEVEL 	xpcc::log::ERROR

// ----------------------------------------------------------------------------
xpcc::pc::SerialInterface::SerialInterface() :
	isConnected(false),
	deviceName("unknown"),
	baudRate(0),
	fileDescriptor(0)
{
}

xpcc::pc::SerialInterface::SerialInterface(
		const std::string& device, unsigned int baudRate) :
	isConnected(false),
	deviceName(device),
	baudRate(baudRate),
	fileDescriptor(0)
{
}

// ----------------------------------------------------------------------------
xpcc::pc::SerialInterface::~SerialInterface()
{
	this->close();
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::setDeviceName(const std::string& name)
{
	this->deviceName = name;
}

// ----------------------------------------------------------------------------
const std::string&
xpcc::pc::SerialInterface::getDeviceName() const
{
	return this->deviceName;
}

// ----------------------------------------------------------------------------
bool
xpcc::pc::SerialInterface::setBaudRate(unsigned int rate)
{
	struct termios configuration;
	int result1, result2, result3;
	
	// Read current configuration structure
	tcgetattr(this->fileDescriptor, &configuration);
	
	XPCC_LOG_INFO << "Set baud rate to '" << this->baudRate << "'" << xpcc::endl;
	
	this->baudRate = rate;
	
	speed_t baudRateConstant =
		(rate == 2400) ? B2400 :
		(rate == 4800) ? B4800 :
		(rate == 9600) ? B9600 :
		(rate == 19200) ? B19200 :
		(rate == 38400) ? B38400 :
		(rate == 57600) ? B57600 :
		(rate == 115200) ? B115200 : B0;
	
	if ((baudRateConstant & CBAUD) == 0)
	{
		XPCC_LOG_ERROR << "Invalid baud rate!" << xpcc::endl;
		return false;
	}
	
	// Change the configuration structure
	result1 = cfsetispeed(&configuration, baudRateConstant);
	result2 = cfsetospeed(&configuration, baudRateConstant);
	
	// Set the modified configuration structure
	result3 = tcsetattr(this->fileDescriptor, TCSANOW, &configuration);
	
	if (result1 < 0 || result2 < 0 || result3 < 0)
	{
		XPCC_LOG_ERROR << "Could not set the baud rate!" << xpcc::endl;
		return false;
	}
	
	return true;
}


// ----------------------------------------------------------------------------
unsigned int
xpcc::pc::SerialInterface::getBaudRate() const
{
	return this->baudRate;
}

// ----------------------------------------------------------------------------
bool
xpcc::pc::SerialInterface::open()
{
	XPCC_LOG_INFO
		<< XPCC_FILE_INFO
		<< "Opening port '"	<< this->deviceName.c_str()
		<< "' at speed '" << this->baudRate << "'"
		<< xpcc::endl;
	
	// Complain if device is already opened.
	if (this->isConnected)
	{
		XPCC_LOG_ERROR << "Port is already open." << xpcc::endl;
	}
	else
	{
		XPCC_LOG_INFO << "Trying to create file descriptor ... ";
		
		this->fileDescriptor = ::open(this->deviceName.c_str(), O_RDWR | O_NOCTTY | O_EXCL | O_NDELAY);
		
		XPCC_LOG_INFO << this->fileDescriptor << xpcc::endl;
		
		if (this->fileDescriptor == -1) {
			XPCC_LOG_ERROR << "Invalid file descriptor!" << xpcc::endl;
		}
		else {
			// Set parameter for this port
			this->initSerial();
			
			// FNDELAY : Wenn keine Daten verfuegbar sind,
			//           soll 'read' 0 zurueckliefern
			fcntl(this->fileDescriptor, F_SETFL, FNDELAY);
			this->isConnected = true;
			
			XPCC_LOG_INFO << "Connected!" << xpcc::endl;
			return true;
		}
	}
	
	return false;
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::initSerial()
{
	struct termios configuration;
	
	// Read old configuration
	tcgetattr(this->fileDescriptor, &configuration);
	
	XPCC_LOG_INFO
		<< "Read old configuration:" << xpcc::endl
		<< " - iflag = " << configuration.c_iflag << xpcc::endl
		<< " - oflag = " << configuration.c_oflag << xpcc::endl
		<< " - cflag = " << configuration.c_cflag << xpcc::endl
		<< " - lflag = " << configuration.c_lflag << xpcc::endl;

	XPCC_LOG_INFO << "Set new configuration" << xpcc::endl;
	
	configuration.c_cflag &= ~CBAUD;		// clear old baud rate
	configuration.c_cflag &= ~PARENB;	// no parity
	configuration.c_cflag &= ~CSIZE;		// clear old data bit value
	configuration.c_cflag |= CS8;		// 8 data bits
	configuration.c_cflag &= ~CSTOPB;	// 1 stop bit
	configuration.c_cflag |= CLOCAL;		// don't take ownership of the port (should not be necessary)
	configuration.c_cflag |= CREAD;		// set port ready to receive data
	configuration.c_cflag &= ~CRTSCTS;	// no flow-control
	configuration.c_cflag &= ~HUPCL;		// don't hung up when the process is terminated
	configuration.c_lflag &= ~ISIG;		// don't allow signals
	configuration.c_lflag &= ~ICANON;	// RAW mode
	configuration.c_lflag &= ~ECHO;		// no echo
	configuration.c_lflag &= ~ECHOE;		// no echo for the backspace character
	configuration.c_lflag &= ~(XCASE | ECHOK | ECHONL | NOFLSH | IEXTEN | ECHOCTL | ECHOPRT | ECHOKE | FLUSHO | PENDIN | TOSTOP); // ??
	configuration.c_iflag &= ~INPCK;		// no parity check
	configuration.c_iflag |= IGNPAR;		// ignore parity errors (as we don't use parity)
	configuration.c_iflag &= ~(IXOFF | IXON | IXANY);		// no soft-handshake
	configuration.c_iflag |= IGNBRK;		// ignore connection break
	configuration.c_iflag &= ~(INLCR | IGNCR | ICRNL | IUCLC);	// don't do anything funny with my data :)
	configuration.c_oflag &= ~OPOST;		// no post-processing
	configuration.c_oflag &= ~ONLCR;		// 
//	configStatus.c_cc[VMIN] = 1;		// read() should block until a character is read
	
	// write new configuration
	tcsetattr(this->fileDescriptor, TCSANOW, &configuration);
	
	int status = 0;
	ioctl(this->fileDescriptor, TIOCMGET, &status);
	
	this->setBaudRate(this->baudRate);
	
	status |= TIOCM_DTR;
	status &= ~TIOCM_RTS;
	
	ioctl(this->fileDescriptor, TIOCMSET, &status);
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::close()
{
	if (this->isConnected) {
		XPCC_LOG_INFO << "Closing port!!" << xpcc::endl;

		int result = ::close(this->fileDescriptor);
		(void) result;
		
		this->isConnected = false;
	}
}

// ----------------------------------------------------------------------------
bool
xpcc::pc::SerialInterface::isOpen()
{
	return this->isConnected;
}

// ----------------------------------------------------------------------------
bool
xpcc::pc::SerialInterface::read(char& c)
{
	if (::read(this->fileDescriptor, &c, 1) > 0)
	{
		XPCC_LOG_DEBUG << "0x" << xpcc::hex << c << " " << xpcc::endl;
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::readBytes(char* data, std::size_t length)
{
	int delta = length;
	int result = 0 ;
	while (delta > 0)
	{
		result = ::read(this->fileDescriptor, (data + (length - delta)), delta);
		if (result < 0) {
			usleep(20);	// swap the thread, so that something could be done while waiting
			continue;
		}
		delta -= result;
	}
	
	for (std::size_t i = 0; i < length; i++) {
		XPCC_LOG_DEBUG << "0x" << xpcc::hex << data[i] << xpcc::ascii << " ";
	}
	XPCC_LOG_DEBUG << xpcc::endl;
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::write(char c)
{
/*	SUB_LOGGER_LOG(logger, Logger::ERROR, "writeByte")
		<< "0x" << std::hex << (int)data << "; ";
 */
	int reply = ::write(this->fileDescriptor, &c, 1);
	if (reply <= 0) {
		this->dumpErrorMessage();
	}
//	SUB_LOGGER_LOG( logger, Logger::DEBUG, "writeByte" )
//		<< "0x" << std::hex << data << " => " << reply << std::dec;
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::write(const char* str)
{
	char c;
	while ((c = *str++)) {
		this->write(c);
	}
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::writeBytes(const char* data, std::size_t length)
{
	for (std::size_t i = 0; i < length; ++i) {
		this->write(*data++);
	}
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::dumpErrorMessage()
{
	switch(errno)
	{
		case EBADF:
			XPCC_LOG_ERROR << "The argument is not a valid file descriptor." << xpcc::endl;
			break;
			
		case EINVAL:
			XPCC_LOG_ERROR << "Invalid argument." << xpcc::endl;
			break;

		case EFAULT:
			XPCC_LOG_ERROR << "Bad Address. Pointer address outside the address "\
							  "space of the process." << xpcc::endl;
			break;

		case EAGAIN:
			XPCC_LOG_ERROR << "Normally, when no input is immediately available, "\
				"read waits for some input. But if the O_NONBLOCK flag is set "\
				"for the file (see section File Status Flags), read returns "\
				"immediately without reading any data, and reports this error." << xpcc::endl;
			break;
			
		case ENOSPC:
			XPCC_LOG_ERROR << "The device is full." << xpcc::endl;
			break;
		
		case EPIPE:
			XPCC_LOG_ERROR << "Trying to write to a pipe or FIFO that isn't "\
							  "open for reading by any process" << xpcc::endl;
			break;
		
		case EINTR:
			XPCC_LOG_ERROR << "The call was interrupted by a signal." << xpcc::endl;
			break;
			
		default:
			XPCC_LOG_ERROR	<< "Unknown error: " << errno << xpcc::endl;
	}
}
// ----------------------------------------------------------------------------
std::size_t
xpcc::pc::SerialInterface::bytesAvailable() const
{
	std::size_t bytesAvailable;
	
	ioctl(this->fileDescriptor, FIONREAD, &bytesAvailable);
	
	return bytesAvailable;
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::flush()
{
	// TODO flush buffers
}

// ----------------------------------------------------------------------------
void
xpcc::pc::SerialInterface::dump()
{
	if (!this->isConnected) {
		XPCC_LOG_DEBUG	<< "Port is not connected device!!" << xpcc::endl;
	}
	else {
		struct termios configStatus;
		
		// Fill the configuration structure
		tcgetattr( this->fileDescriptor, &configStatus);
		
		XPCC_LOG_DEBUG
			<< "parity:       " << ((configStatus.c_cflag & PARENB) ? "yes" : "no") << xpcc::endl
			<< "8 data bits:  " << ((configStatus.c_cflag & CS8) 	? "yes" : "no") << xpcc::endl
			<< "ready:        " << ((configStatus.c_cflag & CREAD) 	? "yes" : "no") << xpcc::endl
			<< "input speed:  " << configStatus.c_ispeed << xpcc::endl
			<< "output speed: " << configStatus.c_ospeed << xpcc::endl
			<< "speed:        " << ( configStatus.c_cflag & CBAUD ) << xpcc::endl;
	}
}

// ----------------------------------------------------------------------------
/*std::ostream&
operator << (std::ostream& os, const xpcc::pc::SerialInterface& c)
{
	os << "\nPort-Identifier: " << c.portIdentifier_;
	os << "\nBaud-Rate:       "	<< ::std::dec << c.baudRate_;
	os << "\nIs Connected:    " << c.isConnected_;
	
	return os;
}*/
