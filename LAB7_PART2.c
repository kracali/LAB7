/*
 ============================================================================
 Name        : LAB7_PART2.c
 Author      : Katherine Rac
 Version     :
 Copyright   : Your copyright notice
 ============================================================================
 */


#include "IE3059lab7.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>


#define VERDE 17
#define AMARILLO 27
#define ROJO 5
#define BOTON 26

int cont = 0;
#define INIT_VALUE 1
#define MI_PRIORIDAD 1;
#define MI_PRIORIDADVERDE 1;
#define MI_PRIORIDADOTRA 1;
sem_t my_semaphore; // counting semaphore


void LEDVERDE(void *ptr); // prototipo
void LEDOTRA(void *ptr); // prototipo
void LEDOAMARILLA(void *ptr); // prototipo
int check_button(void);
void clear_button(void);



int main(int argc, char *argv[]) {
	wiringPiSetupGpio(); //
	pinMode(VERDE ,OUTPUT); //VERDE
	pinMode(AMARILLO ,OUTPUT); //AMARILLO
	pinMode(ROJO , OUTPUT); //ROJO
	pinMode(BOTON , INPUT); //BOTON

	int PRIORIDAD_VERDE = MI_PRIORIDADVERDE;
	int PRIORIDAD_ROJO = MI_PRIORIDADOTRA;
	int PRIORIDAD_PEATONAL = MI_PRIORIDAD;

	if(argc >= 2){
		PRIORIDAD_VERDE = atoi(argv[1]);
		PRIORIDAD_ROJO = atoi(argv[2]);
		PRIORIDAD_PEATONAL = atoi(argv[3]);
	}


	int initial = INIT_VALUE;
	sem_init(&my_semaphore, 0, initial);

	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;

	pthread_create(&thread1, NULL, (void*)&LEDVERDE, (void*)PRIORIDAD_VERDE);
	pthread_create(&thread2, NULL, (void*)&LEDOTRA, (void*)PRIORIDAD_ROJO);
	pthread_create(&thread3, NULL, (void*)&LEDOAMARILLA, (void*)PRIORIDAD_PEATONAL);

	pthread_join(thread1, NULL); // espeara que que el hilo termine.
	pthread_join(thread2, NULL); // espeara que que el hilo termine.
	pthread_join(thread3, NULL); // espeara que que el hilo termine.


	return 0;
}


void LEDVERDE(void *ptr){
	int ledpl1;
	ledpl1 = (int ) ptr;
	//int *ledpl1 = (int *)ptr;
	//int value;
	printf("LED ROJO; %d\n", ledpl1);

	struct sched_param param;
	param.sched_priority = ledpl1;

	if (sched_setscheduler(0, SCHED_RR, &param) == -1){
		perror("sched_setscheduler falló, thread1");
		exit(20);
	}

	while(1){
		sem_wait(&my_semaphore);
		digitalWrite(VERDE, HIGH);
		//printf("led verde");
		sleep(1);
		digitalWrite(VERDE, LOW);
		//printf("no peaton");
		sem_post(&my_semaphore);
		usleep(1000);
	}
	pthread_exit(0); //Se sale del hilo
}

void LEDOTRA(void *ptr){
	int ledpl2;
	ledpl2 = (int ) ptr;
	//int *ledpl1 = (int *)ptr;
	//int value;
	printf("LED ROJO; %d\n", ledpl2);

	struct sched_param param;
	param.sched_priority = ledp2;

	if (sched_setscheduler(0, SCHED_RR, &param) == -1){
		perror("sched_setscheduler falló, thread2");
		exit(20);
	}


	while(1){
		sem_wait(&my_semaphore);
		digitalWrite(ROJO, HIGH);
		//printf("led rojo");
		sleep(1);
		digitalWrite(ROJO, LOW);
		//printf("no peaton");
		sem_post(&my_semaphore);
		usleep(1000);
	}
	pthread_exit(0); //Se sale del hilo
}

void LEDOAMARILLA(void *ptr){
	int ledplP;
	ledplP = (int ) ptr;
	//int *ledpl1 = (int *)ptr;
	//int value;
	printf("LED ROJO; %d\n", ledplP);

	struct sched_param param;
	param.sched_priority = ledplP;

	if (sched_setscheduler(0, SCHED_RR, &param) == -1){
		perror("sched_setscheduler falló, thread3");
		exit(20);
	}


	while(1){
		sem_wait(&my_semaphore);
		if(check_button() == 1){
		digitalWrite(AMARILLO, HIGH);
		//printf("led peaton");
		sleep(1);
		digitalWrite(AMARILLO, LOW);
		//printf("no peaton");
		usleep(1000);
		//printf("peaton");
		clear_button();
		}
		sem_post(&my_semaphore);
	}
	pthread_exit(0); //Se sale del hilo
}
