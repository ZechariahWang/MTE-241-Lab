/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h> //You are permitted to use this library, but currently only printf is implemented. Anything else is up to you!
#include <stdbool.h>

#define RUN_FIRST_THREAD 0x3

uint32_t* stackptr;

extern void runFirstThread(void);

void jumpAssembly(void* fcn) {
	__asm("MOV PC, R0");
}

void print_continuously(void) {
	while (true) {
		printf("Hello, PC! \n");
	}
}

void SVC_Handler_Main(unsigned int *svc_args)
{
      unsigned int svc_number;
      /*
       * Stack contains:
       * r0, r1, r2, r3, r12, r14, the return address and xPSR
       * First argument (r0) is svc_args[0]
       */
      svc_number = ((char *)svc_args[6])[-2];
      switch (svc_number)
      {
              case 17:
                      printf("Success!\r\n");
                      break;
              case 18:
                      printf("Hello from system call 18!\r\n");
                      break;
              case RUN_FIRST_THREAD:
                      __set_PSP((uint32_t)stackptr);
                      runFirstThread();
                      break;
              default: /* unknown SVC */
                      break;
      }
}

void print_success(void) {
      __asm("SVC #17");
}

void print_hello(void) {
      __asm("SVC #18");
}

void run_first_thread(void) {
      __asm("SVC #3");
}

/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{

	/* MCU Configuration: Don't change this or the whole chip won't work!*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	/* MCU Configuration is now complete. Start writing your code below this line */

	uint32_t* MSP_INIT_VAL = *(uint32_t**)0x0;
	printf("MSP Init is: %p\r\n",MSP_INIT_VAL); // note the %p to print a pointer. It will be in hex

    print_success();
    print_hello();

    stackptr = (uint32_t*)((uint32_t)MSP_INIT_VAL - 0x400);

    // fill the stack, top down
    stackptr = stackptr - 1;
    *stackptr = 1 << 24;

    stackptr = stackptr - 1;
    *stackptr = (uint32_t)print_continuously;

    // LR, R12, R3, R2, R1, R0, then R11 down to R4: 14 registers
    for (int i = 0; i < 14; i++)
    {
            stackptr = stackptr - 1;
            *stackptr = 0xA;
    }

    run_first_thread();

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
	/* USER CODE END WHILE */
	//	  printf("Hello, world!\r\n");
	/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

