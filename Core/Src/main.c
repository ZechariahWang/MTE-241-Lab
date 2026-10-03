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

void jumpAssembly(void* fcn) {
	__asm("MOV PC, R0");
}

void print_success(void) {
    __asm volatile ("SVC #0");
}

void print_failure(void) {
    __asm volatile ("SVC #1");
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
              default: /* unknown SVC */
                      break;
      }
}

void print_success(void)
{
      __asm("SVC #17");
}

void print_hello(void)
{
      __asm("SVC #18");
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

	uint32_t PSP_val = (uint32_t)MSP_INIT_VAL - 0x400;
	__set_PSP(PSP_val);
	__set_CONTROL(2);

    print_success();
    print_hello();

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

