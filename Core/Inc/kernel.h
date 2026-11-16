/*
 * kernel.h
 *
 *  Created on: Nov 2, 2026
 *      Author: zecha
 */

#ifndef INC_KERNEL_H_
#define INC_KERNEL_H_
#include <stdint.h>

extern uint32_t* msp_init;
extern uint32_t* LAST_ALLOCATED_ADDRESS;

extern void osYield(void);

extern void print_success(void);
extern void print_hello(void);

extern void osKernelInitialize(void);
extern void osKernelStart(void);

extern void osSched(void);

typedef struct k_thread{
	uint32_t* sp; // sp js means stack pointer
	void (*thread_function)(void*);
}thread;

#define MAX_THREADS 16

extern thread threadArray[MAX_THREADS];
extern int currentThread;
extern int numThreadsRunning;

extern int osCreateThread(void (*fn)(void*));

extern void SVC_Handler_Main(unsigned int *svc_args);

#endif /* INC_KERNEL_H_ */
