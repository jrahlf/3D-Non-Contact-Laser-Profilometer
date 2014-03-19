/*
 * encoder.cpp
 *
 *  Created on: Nov 26, 2013
 *      Author: jonas
 */


#include "encoder.h"
#include "control.h"
#include <xpcc/architecture.hpp>
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/misc.h"
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_tim.h"
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h"
#include "../ext/stm32f4xx_lib/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
#include "../ext/stm32f4xx_lib/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_rcc.h"

using namespace xpcc::stm32;

/*
 * xpcc does not support timer pin remap
 * Source: http://www.micromouseonline.com/2013/02/16/quadrature-encoders-with-the-stm32f4/
 * definitions for the quadrature encoder pins
 */
// Left Motor Channels
#define ENCLA_PIN               GPIO_Pin_15
#define ENCLA_GPIO_PORT         GPIOA
#define ENCLA_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENCLA_SOURCE            GPIO_PinSource15
#define ENCLA_AF                GPIO_AF_TIM2

#define ENCLB_PIN               GPIO_Pin_3
#define ENCLB_GPIO_PORT         GPIOB
#define ENCLB_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define ENCLB_SOURCE            GPIO_PinSource3
#define ENCLB_AF                GPIO_AF_TIM2

// Right Motor Channels
#define ENCRA_PIN               GPIO_Pin_0
#define ENCRA_GPIO_PORT         GPIOA
#define ENCRA_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENCRA_SOURCE            GPIO_PinSource0
#define ENCRA_AF                GPIO_AF_TIM5

#define ENCRB_PIN               GPIO_Pin_1
#define ENCRB_GPIO_PORT         GPIOA
#define ENCRB_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENCRB_SOURCE            GPIO_PinSource1
#define ENCRB_AF                GPIO_AF_TIM5

// determine the timers to use
#define ENCL_TIMER              TIM2
#define ENCL_TIMER_CLK          RCC_APB1Periph_TIM2
#define ENCR_TIMER              TIM5
#define ENCR_TIMER_CLK          RCC_APB1Periph_TIM5

#define LEFT_COUNT()            ENCL_TIMER->CNT
#define RIGHT_COUNT()           ENCR_TIMER->CNT

namespace Encoder{

void encodersInit();

bool init(){
	encodersInit();
	Encoder::zero();
	return true;
}


/*
 * Configure two timers as quadrature encoder counters.
 * Details of which timers should be used are
 * in the project hardware header file.
 * Most timers can be used if channels 1 and 2 are available on pins.
 * The timers are mostly 16 bit. Timers can be set to 32 bit but they are
 * not very convenient for IO pins so the counters are simply set to to
 * 16 bit counting regardless.
 * A mouse needs 32 bits of positional data and, since it also needs the
 * current speed, distance is not maintained by the encoder code but will
 * be looked after by the motion control code.
 * The counters are set to X4 mode. The only alternative is X2 counting.
 */
void encodersInit (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // turn on the clocks for each of the ports needed
  RCC_AHB1PeriphClockCmd (ENCLA_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENCLB_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENCRA_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd (ENCRB_GPIO_CLK, ENABLE);

  // now configure the pins themselves
  // they are all going to be inputs with pullups
  GPIO_StructInit (&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = ENCLA_PIN;
  GPIO_Init (ENCLA_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = ENCLB_PIN;
  GPIO_Init (ENCLB_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = ENCRA_PIN;
  GPIO_Init (ENCRA_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = ENCRB_PIN;
  GPIO_Init (ENCRB_GPIO_PORT, &GPIO_InitStructure);

  // Connect the pins to their Alternate Functions
  GPIO_PinAFConfig (ENCLA_GPIO_PORT, ENCLA_SOURCE, ENCLA_AF);
  GPIO_PinAFConfig (ENCLB_GPIO_PORT, ENCLB_SOURCE, ENCLB_AF);
  GPIO_PinAFConfig (ENCRA_GPIO_PORT, ENCRA_SOURCE, ENCRA_AF);
  GPIO_PinAFConfig (ENCRB_GPIO_PORT, ENCRB_SOURCE, ENCRB_AF);

  // Timer peripheral clock enable
  RCC_APB1PeriphClockCmd (ENCL_TIMER_CLK, ENABLE);
  RCC_APB1PeriphClockCmd (ENCR_TIMER_CLK, ENABLE);

  // set them up as encoder inputs
  // set both inputs to rising polarity to let it use both edges
  TIM_EncoderInterfaceConfig (ENCL_TIMER, TIM_EncoderMode_TI12,
                              TIM_ICPolarity_Rising,
                              TIM_ICPolarity_Rising);
  TIM_SetAutoreload (ENCL_TIMER, 4294967295);
  TIM_EncoderInterfaceConfig (ENCR_TIMER, TIM_EncoderMode_TI12,
                              TIM_ICPolarity_Rising,
                              TIM_ICPolarity_Rising);
  TIM_SetAutoreload (ENCR_TIMER, 4294967295);

  // turn on the timer/counters
  TIM_Cmd (ENCL_TIMER, ENABLE);
  TIM_Cmd (ENCR_TIMER, ENABLE);

}

void zero(){
	Timer2::setValue(0);
	Timer5::setValue(0);
}

};

