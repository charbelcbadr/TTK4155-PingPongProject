/*
 * Motor.c
 *
 * Created: 07.11.2021 15:19:48
 *  Author: harishnv
 */ 

#include "sam.h"
#include "Motor.h"
#include "./can/can_interrupt.h"
#include "PID_controller.h"
#include "Delay.h"


//Global variable for knowing the current motor position
extern int Current_motor_position;
/* 
Description: Initialization function for the motor init
Input: Nothing
Output: Nothing
*/
void motor_init()
{
	//Enabling control register with clock
	REG_PMC_PCR = (PMC_PCR_EN|PMC_PCR_DIV_PERIPH_DIV_MCK|ID_DACC);
	//Enable DACC controller
	REG_PMC_PCER1 |= PMC_PCER1_PID38;
	//Enable channel 1 
	REG_DACC_MR = DACC_MR_USER_SEL_CHANNEL1;
	REG_DACC_CHER = DACC_CHER_CH1;
	//REG_DACC_CDR = 0;
	//enable motor box encoder as output
	PIOD->PIO_PER |= (DIR|EN|SEL|NOT_RST|NOT_OE);
	PIOD->PIO_OER |= (DIR|EN|SEL|NOT_RST|NOT_OE);
	//enable motor encoder as input
	PIOC->PIO_IFER |=(DO0|DO1|DO2|DO3|DO4|DO5|DO6|DO7);
	PIOC->PIO_PUDR |=(DO0|DO1|DO2|DO3|DO4|DO5|DO6|DO7);
	//Enabling control register with clock
	REG_PMC_PCR = (PMC_PCR_EN|PMC_PCR_DIV_PERIPH_DIV_MCK|ID_PIOC);
	//Enable DACC controller
	REG_PMC_PCER0 |= PMC_PCER0_PID13 | PMC_PCER0_PID14;
	//Enabling the pin
	PIOD->PIO_SODR = EN;
	set_initial_reference();
	//Toggle !RST to reset encoder
	PIOD->PIO_CODR |= NOT_RST;
	delay_us(30);
	PIOD->PIO_SODR |= NOT_RST;
}
/* 
Description: Setting the initial reference for the motor before starting to be in one end of the motorbox
Input: Nothing
Output: Nothing
*/
void set_initial_reference()
{
	//setting to initial point
	PIOD->PIO_SODR = DIR;
	//Run the motor with specified speed
	REG_DACC_CDR=1000;
	delay_ms(5000);
}
/* 
Description: Derives the current position of the encoder from the motorbox
Input: Nothing
Output: Returns the current position of the encoder
*/
int16_t motor_encoder()
{	
	int16_t encoded_data=0;
	//Set !OE low to enable output of encoder
	PIOD->PIO_CODR |= NOT_OE;
	//Set SEL low to get high byte
	PIOD->PIO_CODR |= SEL;
	//Wait about 20 microseconds
	delay_us(30);
	//Read MSB
	encoded_data |= (((PIOC->PIO_PDSR)>>1) & 0xFF)<<8;
	//Set SEL high to get low byte
	PIOD->PIO_SODR |= SEL;
	//Wait about 20 microseconds
	delay_us(30);
	//Read LSB
	encoded_data |=((PIOC->PIO_PDSR)>>1) & 0xFF;	
	//Set !OE high to disable output of encoder
	PIOD->PIO_SODR |= NOT_OE;
	//To get rid of overflow
	if (encoded_data<-20000)
	{
		encoded_data=0;
	}
	return -encoded_data;
}
/* 
Description: Scaling down the encoded value
Input: Current position of the encoder
Output: Reduced value of the current position of the encoder
*/
int16_t scale_motor_enocder(int encoded_value)
{
	//Scaling the encoded values in the range of 0 to 200
	return (encoded_value/44);
}

/* 
Description: Initialization of the System Ticks to generate a timer of approx 30ms
Input: Nothing
Output: Nothing
*/
void sys_init()
{
	//Creating a 30ms timer
	SysTick->LOAD=0x19A280;
	enable_sysTick();	
}
/* 
Description: Enable the SysTick
Input: Nothing
Output: Nothing
*/
void enable_sysTick()
{
	//Enabling the SysTick
	SysTick->CTRL=0x7;
}
/* 
Description: Calls the SysTick handler which is called every 30ms approx and calls the motor function for controlling the motor position
Input: Nothing
Output: Nothing
*/
void SysTick_Handler()
{
	//Calling the motor function every 30ms
	write_motor(Current_motor_position);
}

/* 
Description: Disable the SysTick
Input: Nothing
Output: Nothing
*/
void disable_sysTick()
{
	//disabling the Systick
	SysTick->CTRL=0x4;
}
/* 
Description: Specifying the value to the motor to run in a specified direction after the calculation from the Joystick and encoder
Input: Receives the current position from the Joystick/ Slider
Output: Nothing
*/
void write_motor(int current_position)
{
	int current_pos =current_position;
	//Get the current encoder position
	int encoder_value= motor_encoder();
	//Scale down the encoder values
	encoder_value =scale_motor_enocder(encoder_value);
	//Get the Values of the multifunction board in the range of -100 to 100
	if(current_pos>100)
	{
		current_pos = current_pos-201;
	}
	//Mapping the multifunction board slider/ joystick to 0 -200
	current_pos=current_pos+100;
	//Calculating the speed of the motor and checking the error using PI Controller
  	int  speed = ((pid_controller(encoder_value,current_pos)));
	//If a positive speed is return, turn the motor towards RIGHT
 	if(speed > 0)
 	{
 		PIOD->PIO_CODR = DIR;
 		REG_DACC_CDR=speed;
 	}
	//If a negative speed is return, turn the motor towards LEFT
 	else
	{
 		PIOD->PIO_SODR = DIR;
 		REG_DACC_CDR=-speed;
 	}
}

