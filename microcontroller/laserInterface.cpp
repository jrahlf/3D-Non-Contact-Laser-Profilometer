/*
 * laserInterface.cpp
 *
 *  Created on: Dec 30, 2013
 *      Author: jonas
 *
 *  UART DMA source: http://www.mikrocontroller.net/topic/243265
 *  UART interrupt source: http://eliaselectronics.com/stm32f4-usart-example-with-interrupt/
 */

#pragma GCC push_options
#pragma GCC optimize ("O0")

#include <xpcc/architecture.hpp>
#include <xpcc/workflow/timeout.hpp>

#include "laserInterface.h"
#include "state.h"
#include "atof.h"
#include "Trigger.h"
#include "../ext/stm32f4xx_lib/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rcc.h"
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_dma.h"
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_usart.h"
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h"
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/misc.h"

//source for dma: http://www.mikrocontroller.net/topic/251293

//==================================
//=== Output ====
//==================================
static unsigned char normalRequest[] = "M1,1\r";//{ 'M', '0', ',', '1', '\r'};
static const int normalRequestLength = 5;
static const int normalRequestAnswerLength = 15; //TODO

static unsigned char* bufferOut = normalRequest;
static int expectedAnswerLength = normalRequestAnswerLength;
static int requestLength = normalRequestLength;

GPIO__INPUT(LASER_STATUS, E, 5);

static int lastError = -1;

//==================================
//=== Input ====
//==================================

//==================================
//=== Double buffer ====
//==================================
unsigned char Laser::DoubleBuffer::buf1[LASER_INPUT_BUFFER_SIZE+1];
unsigned char Laser::DoubleBuffer::buf2[LASER_INPUT_BUFFER_SIZE+1] = {'\0'};
unsigned char* Laser::DoubleBuffer::bufInPtr = Laser::DoubleBuffer::buf1;
unsigned char* Laser::DoubleBuffer::bufOutPtr = Laser::DoubleBuffer::buf2;
int Laser::DoubleBuffer::inSize = 0;
int Laser::DoubleBuffer::outSize = 0;

#if USE_LASER_DMA
xpcc::stm32::Usart3 uart3(115200);
xpcc::IODeviceWrapper<xpcc::stm32::Usart3> uartWrap3(uart3);
#else
xpcc::stm32::BufferedUsart3 uart3(115200, 4);
xpcc::IODeviceWrapper<xpcc::stm32::BufferedUsart3> uartWrap3(uart3);
#endif
xpcc::IOStream laserOutStream(uartWrap3);

using namespace xpcc::stm32;

//==================================
//=== Functions
//==================================
void USART3_Init(void);
void DMAStream3_Channel4_Init();
void enable_USART3_Interrupts();

bool Laser::init(){
	//uart2.configurePins(xpcc::stm32::USART3::Mapping::REMAP_PA2_PA3);
	LASER_STATUS::setInput(xpcc::stm32::PULLUP);
#if USE_LASER_DMA
	enable_USART3_Interrupts();
	USART3_Init();
	DMAStream3_Channel4_Init();
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
#else
	uart3.configurePins(xpcc::stm32::BufferedUsart3::Mapping::REMAP_PC10_PC11);
#endif
	return true;
}

int Laser::getLastError(){
	return lastError;
}

void Laser::enableRequests(){
#if USE_LASER_DMA
	DMAStream3_Channel4_Init();
	DMA_Cmd(DMA1_Stream3, ENABLE);
#else
	sendMeasurementRequest();
#endif
}

void Laser::disableRequests(){
	DMA_Cmd(DMA1_Stream3, DISABLE);
}

void Laser::sendString(const char* cmd){
	laserOutStream << cmd << '\r';
}

const char* Laser::exec(const char* cmd){
	disableRequests();
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	xpcc::delay_ms(500);
	static char response[256];
	char c = 0;
	//clear receive buffer
	for(int i = 0; i < 100; i++){
		uart3.read((uint8_t&)c);
	}
	sendString(cmd);
	int i = 0;
	xpcc::Timeout<> timeout(1000);
	while(true){
		if(uart3.read((uint8_t&)c)){
			response[i] = c;
			i++;
			if(i > 254 || c == '\r'){
				response[i-1] = '\0';
				break;
			}
		}
		if(timeout.isExpired()){
			break;
		}
	}

	xpcc::delay_ms(500);
	enableRequests();
	enable_USART3_Interrupts();

	if(c == '\r'){
		return response;
	}
	if(i > 254){
		return "Buffer overrun while executing laser command";
	}
	if(timeout.isExpired()){
		return "Timeout expired while executing laser command";
	}
	cerr << " LaserInterface.cpp: " << __LINE__ << " : this point should not be reached" << endl;
	return "";
}

int Laser::DoubleBuffer::asInt(){
	return good() ? atoi2(Laser::DoubleBuffer::get()+3, 0) : 9000;
}

int Laser::DoubleBuffer::good(){
	return lastError == -1 && outSize == expectedAnswerLength;
}

void USART3_Init(void){
  GPIO_InitTypeDef GPIOC_10_11_InitStructure;
  USART_InitTypeDef USART3_InitStructure;

  //GPIO C 10/11 Init
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

  GPIOC_10_11_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIOC_10_11_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIOC_10_11_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIOC_10_11_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIOC_10_11_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIOC_10_11_InitStructure);

  //USART 3 Init
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  USART3_InitStructure.USART_BaudRate = 115200;
  USART3_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART3_InitStructure.USART_StopBits = USART_StopBits_1;
  USART3_InitStructure.USART_Parity = USART_Parity_No;
  USART3_InitStructure.USART_HardwareFlowControl =
USART_HardwareFlowControl_None;
  USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART3_InitStructure);
  USART_Cmd(USART3, ENABLE);
}

void DMAStream3_Channel4_Init(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

  DMA_InitTypeDef DMA_InitStruct;
  DMA_StructInit(&DMA_InitStruct);
  DMA_InitStruct.DMA_Channel = DMA_Channel_4;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
  DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)bufferOut;
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = requestLength;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream3, &DMA_InitStruct);
}

void enable_USART3_Interrupts(){

	/* Here the USART3 receive interrupt is enabled
	 * and the interrupt controller is configured
	 * to jump to the USART3_IRQHandler() function
	 * if the USART3 receive interrupt occurs
	 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // enable the USART3 receive interrupt

	NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		 // we want to configure the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART3 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	// finally this enables the complete USART3 peripheral
	USART_Cmd(USART3, ENABLE);
}

#pragma GCC pop_options

#if USE_LASER_DMA

// this is the interrupt request handler (IRQ) for ALL USART3 interrupts
extern "C" void
USART3_IRQHandler(void){

	// check if the USART3 receive interrupt flag was set
	if( USART_GetITStatus(USART3, USART_IT_RXNE) ){

		char c = USART3->DR;

		Laser::handleChar(c);

		//USART_ClearITPendingBit(USART3, USART_IT_RXNE); not needed
	}
}

#endif

void Laser::handleChar(char c){
	Laser::DoubleBuffer::addChar(c);

	if(c == '\r'){

#if !USE_LASER_DMA
		Laser::sendMeasurementRequest();
#endif

		//do some sanity checks
		//active low -> high means invalid measurement
		bool nok = LASER_STATUS::read();
		State::setOrange(nok);
		if(nok){
			Laser::DoubleBuffer::reset();
			lastError = 3000;
			return;
		}

		if(Laser::DoubleBuffer::get()[0] == 'E'
				&& Laser::DoubleBuffer::get()[1] == 'R'){
			lastError = atoi2(Laser::DoubleBuffer::get()+6, 2, -1);
			Laser::DoubleBuffer::reset();
		}else{
			lastError = -1; //no error
			if(Laser::DoubleBuffer::getInSize() == normalRequestAnswerLength){
				Laser::DoubleBuffer::swap();
				//Trigger::forceTrigger();
			}else{
				Laser::DoubleBuffer::reset();
				lastError = 2000+Laser::DoubleBuffer::getInSize();
			}
		}
	}

	if(Laser::DoubleBuffer::getInSize() >= LASER_INPUT_BUFFER_SIZE){
		Laser::DoubleBuffer::reset(); //reset everything
	}
}

void Laser::sendMeasurementRequest(){
	uart3.write(bufferOut, requestLength);
}



