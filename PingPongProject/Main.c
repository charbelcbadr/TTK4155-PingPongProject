
/* CFile1.c
 *
 * Created: 15.09.2021 15:30:32
 *  Author: charbelb
 */ 
#define F_CPU 4.9152e6
#define BAUD 9600
#define MYUBRR  F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "USART.h"
#include "SRAM.h"
#include "adc.h"
#include "Joystick.h"
#include "OLED.h"
#include "Menu.h"
#include "SPI.h"
#include "CAN.h"
#include "Can_Driver.h"
#include "mcp2515.h"
#include "Replay.h"
#include "System_StartUp.h"

extern Node* head;

int main(void){
	//Initialization of UART And external memory
	USART_Init(MYUBRR);
	MEM_Init();
	
	//Testing SRAM
	//SRAM_test();
	
	//Initialization OLED
 	init_oled();
	OLED_Start();
 	head=displayMenu();
	displayArrow(0,0);

	//Initialization Joystick and CAN
	initialize_clock_adc();
	CAN_init();
	MEM_Init();
	Joystick_init();
 	Joystick current;

 	while(1)
	 {
		// Depending on where the arrow is in the OLED and button is pressed, specific sub modules will be processed
   		char* direction = Joystick_info(current);
	 	if (strcmp(direction,"RIGHT") == 0)
	 	{
	 		clearArrow(head->line,head->column-10);
	 		head=head->next;
	 		displayArrow(head->line,head->column-10);
	 	}
	 	else if (strcmp(direction,"LEFT") == 0)
	 	{
	 		clearArrow(head->line,head->column-10);
	 		head=head->prev;
	 		displayArrow(head->line,head->column-10);
	 	}
	 	else if (strcmp(direction,"UP") == 0)
	 	{
	 		clearArrow(head->line,head->column-10);
	 		head=head->prev;
	 		head=head->prev;
	 		displayArrow(head->line,head->column-10);
	 	}
	 	else if (strcmp(direction,"DOWN") == 0)
	 	{
	 		clearArrow(head->line,head->column-10);
	 		head=head->next;
	 		head=head->next;
	 		displayArrow(head->line,head->column-10);
	 	}
		Button(head);		
	 	_delay_ms(250);
	 }
}