/*
 * PID_controller.c
 *
 * Created: 09.11.2021 19:42:57
 *  Author: harishnv
 */ 

// #define Kp 20
// #define Ki 1


#include "PID_controller.h"
#include <stdio.h>


//PID_MAX with 1000 works slowly
//Initializing the parameters
parameters present={0,0,0,0,0,0,0,1750};
/* 
Description: Depending on the user chosen mode, the PID values are chosen for Easy mode
Input: Nothing
Output: Nothing
*/
void set_easy_mode()
{
	present.Kp=20;
	present.Ki=15;
	present.Kd= 0.5;
	present.sum_error=0;
	present.prev_error=0;
}
/* 
Description: Depending on the user chosen mode, the PID values are chosen for Difficult mode
Input: Nothing
Output: Nothing
*/
void set_difficult_mode()
{
	present.Kp=30;
	present.Ki=10;
	present.Kd= 1;
	present.sum_error=0;
	present.prev_error=0;
}

/* 
Description: Calculates the speed at which the motor should be rotated depending on the current position from the Joystick/ Slider and the motor position from the enocder
Input: Receives the current joystick/ slider position and encoder position
Output: Returns the speed at which motor is rotated
*/
int pid_controller(int encoder_value,int current_position)
{
	//Speed to be calculated
	int u=0;
	//Current position of the joystick/ slider value
	present.current_value = current_position;
	//Calculating the error for the propagation term
	int error = present.current_value - encoder_value;
	//Summing the total error for the Integral term
	present.sum_error =present.sum_error + error;
	//Formula for calculating the speed of the motor
	u= (present.Kp*error)+ ((present.Ki*present.sum_error)*0.03)+ ((present.Kd*(error-present.prev_error))/0.03);
	//u= (present.Kp*error)+ (present.Ki*1*present.sum_error)+ ((present.Kd*(error-present.prev_error))/1);
	//Assigning the present error to the prev error
	present.prev_error=error;
	//Limiting the speed of the motor
	if (u > present.PID_MAX)
	{
		u = present.PID_MAX;
	}
	else if (u < -present.PID_MAX) 
	{
		u = -present.PID_MAX;
	}
	return u;
}


