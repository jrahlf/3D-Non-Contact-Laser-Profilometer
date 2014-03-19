#include "uart_1.hpp"

#include <xpcc/architecture/driver/atomic.hpp>

#include <lpc11xx/cmsis/LPC11xx.h>
#include "lpc11_uart_registers.hpp"

#include <xpcc_config.hpp>

// ----------------------------------------------------------------------------

namespace
{
// If requested buffer size is larger than hardware buffer size create a
// software queue for the remaining bytes.
#if (LPC11_UART1_RX_BUFFER_SIZE > 16)
	static xpcc::atomic::Queue<uint8_t, LPC11_UART1_RX_BUFFER_SIZE - 16> rxBuffer;
#endif
#if (LPC11_UART1_TX_BUFFER_SIZE > 16)
	static xpcc::atomic::Queue<uint8_t, LPC11_UART1_TX_BUFFER_SIZE - 16> txBuffer;
#endif

	static bool isBlocking = true;

	/**
	 * The FIFO only tells when it is entirly empty by setting THRE.
	 * So after putting one char to the FIFO it is not possible to know
	 * how many other chars can be send to the FIFO.
	 * NXP wisely did not include a flag FIFO-is-full.
	 */
	static uint8_t
	charsLeft = 16;
}

// ----------------------------------------------------------------------------

void
xpcc::lpc::Uart1::write(uint8_t data)
{
	while ( !(LPC_UART->LSR & LSR_THRE) );
	LPC_UART->THR = data;
}

// ----------------------------------------------------------------------------

void
xpcc::lpc::Uart1::write(const uint8_t *buffer, uint8_t n)
{
	  while (n != 0)
	  {
			while ( !(LPC_UART->LSR & LSR_THRE) );
			LPC_UART->THR = *buffer;
			--n; ++buffer;
	  }
}

// ----------------------------------------------------------------------------

bool
xpcc::lpc::Uart1::read(uint8_t & c)
{
	if (LPC_UART->LSR & LSR_RDR) {
		// Receive data available
		c = LPC_UART->RBR;
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------

uint8_t
xpcc::lpc::Uart1::read(uint8_t *buffer, uint8_t n)
{
	uint8_t ret = 0;

	while ((n != 0) && (LPC_UART->LSR & LSR_RDR))
	{
		// Receive data available and space in buffer left
		*buffer = LPC_UART->RBR;
		--n; ++buffer; ++ret;
	}

	return ret;
}

// ----------------------------------------------------------------------------

xpcc::lpc::Uart1::Uart1(uint32_t baudrate)
{
	initialize(baudrate);
} // Uart1 constructor

void
xpcc::lpc::Uart1::initialize(uint32_t baudrate)
{
	/*  UART I/O config. PIO1_6 and PIO1_7 are always Rx/Tx */
	LPC_IOCON->PIO1_6 &= ~0x07;
	LPC_IOCON->PIO1_6 |= 0x01;     /* UART RXD */
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01;     /* UART TXD */

	/* Enable UART clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
	LPC_SYSCON->UARTCLKDIV = 0x1;     /* divided by 1 */

	setBaudrate(baudrate);

	/* Enable and reset TX and RX FIFO. */
	LPC_UART->FCR = FCR_TXFIFORES | FCR_RXFIFORES | FCR_FIFOEN;

	/* Read to clear the line status. */
	uint32_t regVal = LPC_UART->LSR;
	(void)regVal; // unused, just read to clear.

	/* Ensure a clean start, no data in either TX or RX FIFO. */
	while (( LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
	while ( LPC_UART->LSR & LSR_RDR )
	{
		regVal = LPC_UART->RBR;	/* Dump data from RX FIFO */
	}

} // Uart1 initialize

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

xpcc::lpc::BufferedUart1::BufferedUart1(uint32_t baudrate)
{
	/*  UART I/O config. PIO1_6 and PIO1_7 are always Rx/Tx */
	LPC_IOCON->PIO1_6 &= ~0x07;
	LPC_IOCON->PIO1_6 |= 0x01;     /* UART RXD */
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01;     /* UART TXD */

	/* Enable UART clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
	LPC_SYSCON->UARTCLKDIV = 0x1;     /* divided by 1 */

	Uart1::setBaudrate(baudrate);

	/* Enable and reset TX and RX FIFO. */
	LPC_UART->FCR = FCR_TXFIFORES | FCR_RXFIFORES | FCR_FIFOEN;

	/* Read to clear the line status. */
	uint32_t regVal = LPC_UART->LSR;
	(void)regVal; // unused, just read to clear.

	/* Ensure a clean start, no data in either TX or RX FIFO. */
	while (( LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
	while ( LPC_UART->LSR & LSR_RDR )
	{
		regVal = LPC_UART->RBR;	/* Dump data from RX FIFO */
	}

#if (LPC11_UART1_TX_BUFFER_SIZE > 16) && (LPC11_UART1_RX_BUFFER_SIZE <= 16)
	/* If TX buffering with more than 16 bytes is requested a software queue
	 * must be used for sending. This involves the Tx Interrupt only. */
	LPC_UART->IER = IER_THREIE;
#endif

#if (LPC11_UART1_TX_BUFFER_SIZE <= 16) && (LPC11_UART1_RX_BUFFER_SIZE > 16)
	/* If RX buffering with more than 16 bytes is requested a software queue
	 * must be used for receiving. This involves the Rx Interrupt only. */
	LPC_UART->IER = IER_RBRIE;
#endif

#if (LPC11_UART1_TX_BUFFER_SIZE > 16) && (LPC11_UART1_RX_BUFFER_SIZE > 16)
	/* If both Rx and Tx buffering with more than 16 bytes is requested
	 * two software queues muste be used. This involves the Rx and Tx interrupt. */
	LPC_UART->IER = IER_RBRIE | IER_THREIE;
#endif

#if (LPC11_UART1_TX_BUFFER_SIZE > 16) || (LPC11_UART1_RX_BUFFER_SIZE > 16)
	/* Enable the UART Interrupt */
	NVIC_EnableIRQ(UART_IRQn);
#endif

	// TODO Care about Line status and its interrupt.
} // BufferedUart1 constructor

// ----------------------------------------------------------------------------

void
xpcc::lpc::BufferedUart1::write(uint8_t data)
{
	if (LPC_UART->LSR & LSR_THRE) {
		// If the FIFO is empty 16 bytes can be send.
		charsLeft = 16;
	}

#if (LPC11_UART1_TX_BUFFER_SIZE <= 16)
	// No software buffer necessary, use hardware buffer only.
	if (charsLeft > 0) {
		// There is at least charsLeft bytes free in the FIFO
		LPC_UART->THR = data;
		--charsLeft;
	}
	else {
		// Well, it's not known how many chars are really available in the FIFO.
		// Since the FIFO was completly empty 16 bytes have been sent to the FIFO.
		// Even though some bytes are already transmitted and some space is
		// available again we only can detect if the FIFO is completely empty.
		if (isBlocking) {
			// If blocking was selected we need to wait until the FIFO
			// is completly empty to guarantee that the byte is sent.
			while ( !(LPC_UART->LSR & LSR_THRE) );
			// FIFO is empty, 16 bytes can be send.
			charsLeft = 16;
		}
		// Send the byte.
		// In non-blocking mode it is pure luck if it can be transmitted or not.
		// There may are may not be some space in the FIFO.
		// That is absolutly what you want in your system!
		LPC_UART->THR = data;
		--charsLeft;
	}
#else
	// Use hardware buffer and additional software buffer.

	if (txBuffer.isEmpty()) {
		// If software buffer is empty try to write to hardware buffer directly.
		// Do not write to hardware buffer while there is some data in the
		// software buffer to maintain byte order.
		if (charsLeft > 0) {
			// There is at least charsLeft bytes free in the FIFO
			LPC_UART->THR = data;
			--charsLeft;
			return; // success
		}
		else {
			// Hardware buffer is full, so try software buffer.
			// Software buffer is empty so this will succeed.
			// Hardware buffer is not empty so at least one Tx interrupt
			// will be generated soon.
			txBuffer.push(data);
			return; // success
		}
	}
	else {
		// Software buffer is not empty. Try to write to software buffer and
		// block if configured to do so.
		while ( !txBuffer.push(data) ) {
			if (!isBlocking) {
				return; // data dropped
			}
		}
		return; // success
	} // if(txBuffer.empty())
#endif
} // write

// ----------------------------------------------------------------------------

void
xpcc::lpc::BufferedUart1::write(const uint8_t *buffer, uint8_t n)
{
	if (LPC_UART->LSR & LSR_THRE) {
		// If the FIFO is empty 16 bytes can be send.
		charsLeft = 16;
	}

	#if (LPC11_UART1_TX_BUFFER_SIZE <= 16)
	// No software buffer necessary, use hardware buffer only.

	while (n > 0) {
		// Copy max(charsLeft, n) chars from buffer to hardware FIFO.
		while ((charsLeft > 0) && (n > 0)) {
			LPC_UART->THR = *buffer;
			--n; ++buffer; --charsLeft;
		}
		if (charsLeft == 0) {
			// no space left in FIFO
			if (isBlocking) {
				// If blocking was selected we need to wait until the FIFO
				// is completly empty to guarantee that the byte is sent.
				while ( !(LPC_UART->LSR & LSR_THRE) ) {
				}
				// FIFO is empty, 16 bytes can be send.
				charsLeft = 16;
			}
			else {
				// Return in non-blocking mode with some data dropped.
				return;
			} // blocking
		} // charsLeft == 0
	} // while (n > 0)

	// Fine, everything put into FIFO
	return;
#else
	// Use hardware buffer and additional software buffer.

	if (txBuffer.isEmpty()) {
		// If software buffer is completly empty try to write to hardware buffer directly.
		// Do not write to hardware buffer while there is some data in the
		// software buffer to maintain byte order.

		// First Copy max(charsLeft, n) chars from buffer to hardware FIFO.
		while ((charsLeft > 0) && (n > 0)) {
			LPC_UART->THR = *buffer;
			--n; ++buffer; --charsLeft;
		}
	}

	// If there is remaining data, put it into the software buffer
	if (n > 0) {
		// Try software buffer
		while (n > 0) {
			while (!txBuffer.push(*buffer)) {
				if (!isBlocking) {
					// Hardware and Software buffer full.
					// Non-blocking, so return
					return;
				}
			}
			--n; ++buffer;
		}
	} // n > 0

	// All data put into Hardware and Software buffers
	return;
#endif
} // write buffer

bool
xpcc::lpc::BufferedUart1::read(uint8_t & c)
{
	return xpcc::lpc::Uart1::read(c);
}

// ----------------------------------------------------------------------------

extern "C" void
UART_IRQHandler()
{
	// read IIR to clear Interrupt Status and mask
	uint8_t IIRValue = LPC_UART->IIR & IIR_INTID_MASK;

	switch (IIRValue)
	{
	case IIR_INTID_RDA:
#if (LPC11_UART1_RX_BUFFER_SIZE > 16)
		// Receive data available: Move data to queue.
		// As the Rx Interrupt is disabled when the rxBuffer is full it should
		// never happen that this is called with rxBuffer full.
		rxBuffer.push(LPC_UART->RBR);

		if (rxBuffer.isFull()) {
			// Disable Rx Interrupt so that the hardware buffer is used, too.
			// Enable Rx Interrupt in read() as soon as rxBuffer is not full anymore.
			LPC_UART->IER &= ~(IER_RBRIE);
		}
#endif
		break;
	case IIR_INTID_THRE:
#if (LPC11_UART1_TX_BUFFER_SIZE > 16)
		// Transmit Holding Register Empty == FIFO completly empty.
		// Write up to 16 bytes to FIFO.
		charsLeft = 16;

		if (txBuffer.isEmpty()) {
			// Transmission to FIFO finished, FIFO empty, software buffer empty.
			// No need to switch of TRHE interrupt.
			return;
		}

		while (!txBuffer.isEmpty() && (charsLeft-- > 0)) {
			// Write to the hardware buffer
			LPC_UART->THR = txBuffer.get();
			txBuffer.pop();
		}
#endif
		break;
	case IIR_INTID_CTI:
		// Character Time-out
		break;
	case IIR_INTID_MI:		// Modem interrupt
	case IIR_INTID_RLS: 	// Receive line status
	default:
		// don't care
		break;
	} // switch

}
