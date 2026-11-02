/*
 * kernel.c
 *
 *  Created on: Nov 2, 2026
 *      Author: zecha
 */

#include "main.h"
#include "kernel.h"
#include <stdio.h> //You are permitted to use this library, but currently only printf is implemented. Anything else is up to you!
#include <stdbool.h>

#define RUN_FIRST_THREAD 0x3
#define STACK_SIZE 0x400
#define STACK_POOL_SIZE 0x4000

uint32_t* msp_init;
uint32_t* LAST_ALLOCATED_ADDRESS;

thread kernel_thread;

extern void runFirstThread(void);

void print_success(void) {
      __asm("SVC #17");
}

void print_hello(void) {
      __asm("SVC #18");
}

void osKernelStart(void) {
      __asm("SVC #3");
}

void osKernelInitialize(void) {
    msp_init = *(uint32_t**)0x0;
    LAST_ALLOCATED_ADDRESS = msp_init;
}

uint32_t* allocate_space(void) {
	uint32_t new_address = (uint32_t)LAST_ALLOCATED_ADDRESS - STACK_SIZE;
	uint32_t pool_floor = (uint32_t)msp_init - STACK_POOL_SIZE;

    if (new_address < pool_floor) { return NULL; }

    LAST_ALLOCATED_ADDRESS = (uint32_t*)new_address;
    return LAST_ALLOCATED_ADDRESS;
}

int osCreateThread(void (*fn)(void*)) {
	uint32_t* space = allocate_space();
	if (space == NULL) { return 0; }
	else {
		space = space -1;
        *space = 1 << 24;

        space = space - 1;
        *space = (uint32_t)fn;

        for (int i = 0; i < 14; i++) {
                space = space - 1;
                *space = 0xA;
        }

        kernel_thread.sp = space;
        kernel_thread.thread_function = fn;
        return 1;
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
            	  	  __set_PSP((uint32_t)kernel_thread.sp);
                      runFirstThread();
                      break;
              default: /* unknown SVC */
                      break;
      }
}

