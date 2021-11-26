/*
 * ADC.c
 *
 * Created: 03.11.2021 15:01:20
 *  Author: harishnv
 */ 

#include "ADC.h"
#include "sam.h"
#include "./can/can_controller.h"
#include "printf-stdarg.h"
#include "Motor.h"
#include "LED.h"

//If the values of IR is less than this value, game is over
#define IR_Threshold 1000
/* 
Description: Initialisation of ADC
Input: Nothing
Output: Nothing
*/
void adc_init()
{
	//Setting the mode register to FREERUN
	ADC->ADC_MR = ADC_MR_FREERUN;
	//REG_ADC_MR = ADC_MR_FREERUN;
	//Setting to Channel 7
	ADC->ADC_CHER = ADC_CHER_CH7;
	//To start the conversion
	ADC->ADC_CR = ADC_CR_START;
	//Enabling control register with clock
	PMC->PMC_PCR = (PMC_PCR_EN|PMC_PCR_DIV_PERIPH_DIV_MCK|ID_ADC);
	//Enable ADC controller
	REG_PMC_PCER1 |= PMC_PCER1_PID37;
	//During the start of the game, the total lives lost is 0
}
/* 
Description: Read the ADC values
Input: Nothing
Output: Nothing
*/
void read_adc()
{
	//Read the data from channel 7
	uint16_t temp = ADC->ADC_CDR[7];
	//printf("temp value %d",temp);
	if((temp < IR_Threshold))
	{		
		while (temp < IR_Threshold+500)
		{
			 temp = ADC->ADC_CDR[7];
		 }		 
		current_counter++;
		//printf("ADC VALUE: %d TEMP:%d\n",current_counter,temp);
		CAN_MESSAGE msg;
		msg.id=2;
		msg.data_length = 1;
		//If the counter value is 3, then send a CAN message to node 1 by terminating the game
		if(current_counter == 3)
		{
			msg.data[0] = (uint8_t)(current_counter);
			//Transmit the CAN Message
			can_send(&msg,0);
			current_counter = 0;
			//Switch on LEDs and disable the systicks
			Switch_only_LED1();
			disable_sysTick();
		}	
	}
}

