
.syntax unified
.cpu cortex-m3
.thumb

.global HardFault_Handler
.extern HardFault_Handler_CPP

HardFault_Handler:
  TST LR, #4
  ITE EQ
  MRSEQ R0, MSP
  MRSNE R0, PSP
  B HardFault_Handler_CPP
