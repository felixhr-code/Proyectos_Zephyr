#include <stdio.h>
#include <zephyr/kernel.h>

#define FIB_LIMIT 400

void fibonacci(void){
	unsigned long  long int fib1=0, fib2=1, next;
	printf("Fibonacci Series uo to %d terms:\n", FIB_LIMIT);

	for (int i=0; i< FIB_LIMIT; i++){
		printf("%llu\n",fib1);
		next = fib1+fib2;
		fib1 = fib2;
		fib2 = next;
		k_msleep(500);
	}
}
void main(void){
	printf("Starting Fibonacci calculation in Zephyr ...\n");
	fibonacci();
	printf("Fibonacci calculation completed.\n");
}

