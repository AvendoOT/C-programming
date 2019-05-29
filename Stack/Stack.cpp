#include <stdlib.h>
#include <stdio.h>
#include "stack_funct.c"


int main(void) {
	Stack mystack;
	int element = 3;
	stack_init(&mystack, 0);
	stack_push(&mystack, element);
	stack_push(&mystack, element+1);
	stack_push(&mystack, element+2);
	peek_find(&mystack, &element);
	stack_pop(&mystack, &element);
	return 0;
}

