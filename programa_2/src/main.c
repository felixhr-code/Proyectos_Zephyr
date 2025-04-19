#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>


#define STACK_SIZE 1024
#define PRIORITY 5

// Definicion de stacks para los hilos
K_THREAD_STACK_DEFINE(stack_par, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_impar, STACK_SIZE);

// Definicion de estructuras de control para los hilos
static struct k_thread thread_par;
static struct k_thread thread_impar;

// Funcion para imprimir numeros pares 
void par_thread(void *arg1, void *arg2, void *arg3){
	int num = 0;
	while(1){
		if(num % 2 == 0){
			printk("Par: %d\n", num);
		}
		num++;
		k_msleep(500);
	}
}

void impar_thread(void *arg1, void *arg2, void *arg3){
	int num = 1;
	while(1){
		if(num % 2 != 0){
			printk("IMpar: %d\n", num);
		}
		num++;
		k_msleep(500);
	}
}

int main(void){
	printk("Inicio del programa con dos hilos\n");

	// Crear hilo para numeros pares
	k_thread_create(&thread_par, stack_par, STACK_SIZE,
									par_thread, NULL, NULL, NULL,
									PRIORITY, 0, K_NO_WAIT);

	// Crear hilo para numeros impares
	k_thread_create(&thread_impar, stack_impar, STACK_SIZE,
									impar_thread, NULL, NULL, NULL,
									PRIORITY, 0, K_NO_WAIT);
}
