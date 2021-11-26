/*
 * System_Startup.c
 *
 * Created: 18.11.2021 16:16:55
 *  Author: harishnv
 */ 

#include "sam.h"
#include "uart.h"
#include "./can/can_controller.h"
#include "ADC.h"
#include "Motor.h"
#include "./can/can_interrupt.h"
#include "PID_controller.h"
#include "Solenoid.h"
#include "PWM.h"
#include "LED.h"
/* 
Description: Calling the initialisation of all the modules to be performed during init of the system
Input: Nothing
Output: Nothing
*/
void system_start()
{
	/* Initialize the SAM system */
	SystemInit();
	//Disable watchdog timer
	WDT->WDT_MR=WDT_MR_WDDIS;
	configure_uart();
	//Baudrate =250kbps, Tq=16, BRP=41, PS1=6, PS2=1, Propagation delay = 5
	can_init_def_tx_rx_mb(0x00140561);
	pwm_init();
	adc_init();
	init_LED();
	motor_init();
	solenoid_init();
	sys_init();	   
}