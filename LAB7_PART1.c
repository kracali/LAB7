//============================================================================
// Name        : Lab7.cpp
// Author      : Katherine Rac
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "IE3059lab7.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void LED(void *ptr); // prototipo
int check_button(void);
void clear_button(void);

int main() {
	wiringPiSetupGpio(); //
	pinMode(17 ,OUTPUT); //
	pinMode(27 ,OUTPUT); //
	pinMode(5 , OUTPUT); //
	pinMode(26 , INPUT); //

	pthread_t thread1;

	pthread_create(&thread1, NULL, (void*)&LED, NULL);
	pthread_join(thread1, NULL); // espeara que que el hilo termine.

}

void LED(void *ptr){
	while(1){
		if(check_button() == 0){
			digitalWrite(17, HIGH);
			digitalWrite(27, LOW);
			sleep(1);
			digitalWrite(17, LOW);
			digitalWrite(27,HIGH);
			sleep(1);
			//printf("no peaton");
		}
		if(check_button() == 1){
			digitalWrite(17, LOW);
			digitalWrite(27, LOW);
			digitalWrite(5,HIGH);
			sleep(1);
			digitalWrite(17, HIGH);
			digitalWrite(27,LOW);
			digitalWrite(5,LOW);
			sleep(1);
			//printf("peaton");
			clear_button();
			}
		}
}

