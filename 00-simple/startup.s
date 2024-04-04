
  .syntax unified
  .cpu cortex-m4
  .thumb

.global  g_pfnVectors
.global  Default_Handler

  .section  .text.Reset_Handler,"ax",%progbits
  .type  Reset_Handler, %function
Reset_Handler:  
  ldr   sp, =_estack     /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */  
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit
  
/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

  bl  main    
.size  Reset_Handler, .-Reset_Handler

    .section  .text.Default_Handler,"ax",%progbits
	.type  Default_Handler, %function
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler

  .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
    
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
.size  g_pfnVectors, .-g_pfnVectors
  
   .weak  Reset_Handler
	
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler
  
   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler
  
   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler
  
   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

