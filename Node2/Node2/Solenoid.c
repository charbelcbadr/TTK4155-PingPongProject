/*
 * Solenoid.c
 *
 * Created: 10.11.2021 10:10:08
 *  Author: harishnv
 */ 

#include "sam.h"
#include "Solenoid.h"
#include <stdio.h>
#include "ADC.h"
#include "printf-stdarg.h"
#include "Delay.h"
/* 
Description: Initialisation function of Soleniod
Input: Nothing
Output: Nothing
*/
void solenoid_init()
{
	//Enable the Solenoid Pin
	PIOC->PIO_PER |= Solenoid_PIN;
	//Set the pin as output
	PIOC->PIO_OER |= Solenoid_PIN;
	//Set the register
	PIOC->PIO_SODR = Solenoid_PIN;
	}
/* 
Description: Controlling the solenoid
Input: Nothing
Output: Nothing
*/
 void solenoid_controller(int value)
 {
	//If Soleniod is activated, the pin in made set and cleared
 	if(value == 10)
 	{
		//Set the register
		PIOC->PIO_CODR = Solenoid_PIN;
		delay_ms(100);
		//Clear the register
		PIOC->PIO_SODR = Solenoid_PIN;	
	}
}