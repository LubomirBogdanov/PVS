/*
 * sbrk.c
 *
 *  Created on: Nov 26, 2020
 *      Author: lbogdanov
 */
#include <errno.h>
#include <stdio.h>

extern int errno;

caddr_t _sbrk(int incr){
	static char *heap_end = 0;
	char *prev_heap_end;
	extern char _sheap; /* Defined by the linker */
	char *stack_pointer;

	asm("mov %[dst], sp" :[dst] "=r" (stack_pointer));

	if (heap_end == 0) {
		heap_end = &_sheap;
	}

	prev_heap_end = heap_end;

	if (heap_end + incr > stack_pointer) {
		/* Heap and stack collision */
		errno = ENOMEM;
		return (caddr_t) -1;
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

