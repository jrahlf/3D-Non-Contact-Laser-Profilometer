/*
 * faultHandlers.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: jonas
 */



#include <xpcc/architecture.hpp>
#include "project.h"
#include "state.h"

GPIO__OUTPUT(ledRed, D, 14);
GPIO__OUTPUT(ledBlue, D, 15);
GPIO__OUTPUT(ledOrange, D, 13);
GPIO__OUTPUT(ledGreen, D, 12);

static void blink(){
	ledRed::setOutput(xpcc::stm32::PUSH_PULL);
	ledBlue::setOutput(xpcc::stm32::PUSH_PULL);
	ledOrange::setOutput(xpcc::stm32::PUSH_PULL);
	ledGreen::setOutput(xpcc::stm32::PUSH_PULL);
	while(true){
		ledRed::toggle();
		ledBlue::toggle();
		xpcc::delay_ms(100);
		ledOrange::toggle();
		ledGreen::toggle();
		xpcc::delay_ms(100);
	}
}

// From Joseph Yiu, minor edits by FVH
// hard fault handler in C,
// with stack frame location as input parameter
// called from HardFault_Handler in file xxx.s
// source: http://blog.frankvh.com/2011/12/07/cortex-m3-m4-hard-fault-handler/
extern "C" void
HardFault_Handler_CPP (unsigned int * hardfault_args){
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;

  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);

  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr = ((unsigned long) hardfault_args[5]);
  stacked_pc = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);

  for(int i = 0; i < 10; i++){
	  cerr << "\n\n[Hard fault handler - all numbers in hex]\n";
	  cerr << "R0 = " << stacked_r0 << endl;
	  cerr << "R1 = " << stacked_r1 << endl;
	  cerr << "R2 = " << stacked_r2 << endl;
	  cerr << "R3 = " << stacked_r3 << endl;
	  cerr << "R12 = " << stacked_r12 << endl;
	  cerr << "LR [R14] = " << stacked_lr << "  subroutine call return address" << endl;
	  cerr << "PC [R15] = " << stacked_pc << "  program counter" << endl;
	  cerr << "PSR = " <<  stacked_psr << endl;
	  cerr << "BFAR = " << (*((unsigned long *)(0xE000ED38))) << endl;
	  cerr << "CFSR = " <<  (*(( unsigned long *)(0xE000ED28))) << endl;
	  cerr << "HFSR = " << (*(( unsigned long *)(0xE000ED2C))) << endl;
	  cerr << "DFSR = " << (*(( unsigned long *)(0xE000ED30))) << endl;
	  cerr << "AFSR = " << (*(( unsigned long *)(0xE000ED3C))) << endl;
	  //printf ("SCB_SHCSR = %x\n", SCB->SHCSR);
  }
  blink();
}

extern "C" void
NMI_Handler(void){
	for(int i = 0; i < 10; i++){
		cerr << __PRETTY_FUNCTION__ << endl;
	}
	blink();
}

extern "C" void
MemManage_Handler(void){
	for(int i = 0; i < 10; i++){
		cerr << __PRETTY_FUNCTION__ << endl;
	}
	blink();
}

extern "C" void
BusFault_Handler(void){
	for(int i = 0; i < 10; i++){
		cerr << __PRETTY_FUNCTION__ << endl;
	}
	blink();
}

extern "C" void
UsageFault_Handler(void){
	for(int i = 0; i < 10; i++){
		cerr << __PRETTY_FUNCTION__ << endl;
	}
	blink();
}
