/*
 * PWM.c
 *
 * Created: 27.10.2021 16:14:08
 *  Author: harishnv
 */ 


#include "sam.h"
#include "PWM.h"

/* 
Description: Initialization function of PWM
Input: Nothing
Output: Nothing
*/
void pwm_init()
{
	//Select Peripheral AB
	PIOC->PIO_ABSR|= PIO_ABSR_P19;	
	//Disable PIO
	PIOC->PIO_PDR |= PIO_PDR_P19;
	//Set the PWM clock frequency to 2Mz
	REG_PWM_CLK =  PWM_CLK_PREA(0)|PWM_CLK_DIVA(42);
	//Enable the clock for a particular PID
	REG_PMC_PCER1 |= PMC_PCER1_PID36;
	//Set the clock to A for CMR5 and period center aligned
	REG_PWM_CMR5 = PWM_CMR_CPRE_CLKA; 
	//2Mz/(2*20000)=50Hz
	REG_PWM_CPRD5 = 20000;
	REG_PWM_CDTY5 =18000;
	//enable pwm channel
	REG_PWM_ENA = PWM_ENA_CHID5;
}
/* 
Description: Set the duty cycle of PWM depending on which the servo motor has to be rotated
Input: The data for setting the duty cycle
Output: Nothing
*/
void set_duty_cycle(char data)
{
	int temp = 0;
	//Depending on the value, the servo motor is rotated to left or right
	if(data <= 100)
	{
		temp = 3000+(int)(data*10);
		REG_PWM_CDTY5= 20000 - temp;
	}
	else
	{
		temp = 	 3000 +	((int) (data - 201))*10;
		REG_PWM_CDTY5= 20000 - temp;	
	}
}