/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"

#include "../printf-stdarg.h"
#include "../System_Startup.h"
#include "can_controller.h"
#include "../PWM.h"
#include "../Solenoid.h"
#include "../LED.h"
#include "../ADC.h"

#define DEBUG_INTERRUPT 0

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */

int Current_motor_position;
int lostGame;


void CAN0_Handler( void )
{
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		//Message received from Node 1 and process the functions
		if(message.id == 1)
		{
 			set_duty_cycle(message.data[0]);
			Current_motor_position = message.data[1];
			solenoid_controller(message.data[2]);
			//Reading the ADC values, when the value goes below the threshold, game is over	
			read_adc();
			Switch_only_LED2();
		}
		//Message received from Node 1 and to restart the system
		else if(message.id == 5)
		{
			sys_init();
			pwm_init();
			adc_init();
			motor_init();
		}
		//Depending on the Mode chosen from the user, the PI values are chosen
		else if(message.id == 9)
		{
			if(message.data[0] == 1)
			{
				set_easy_mode();
			}
			if(message.data[0] ==2)
			{
				set_difficult_mode();
			}
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}
