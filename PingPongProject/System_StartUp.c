/*
 * System_StartUp.c
 *
 * Created: 18.11.2021 14:36:11
 *  Author: harishnv
 */ 

#include "System_StartUp.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OLED.h"
#include "CAN.h"

/* 
Description: Starts the OLED with a welcome message by doing an animation
Input:  Nothing
Output: Nothing
*/
void OLED_Start()
{
	reset_oled();
	oled_goto_line(3);
	oled_goto_column(5);
	oled_print("WELCOME TO GAME",LARGE);
	_delay_ms(5000);
	reset_oled();
	oled_goto_line(3);
	oled_goto_column(30);	
	oled_print("WELCOME TO GAME",MEDIUM);
	_delay_ms(5000);
	reset_oled();
	oled_goto_line(3);
	oled_goto_column(30);
	oled_print("WELCOME TO GAME",SMALL);
	_delay_ms(5000);
	reset_oled();
}
/* 
Description: Sends a message to Node 2 for doing a soft system reset during calibration
Input:  Nothing
Output: Nothing
*/
void reset_system_node_2()
{
	//Configuring a CAN message to send to node 2 for doing soft rest
	CAN_Msg msg_reset;
	msg_reset.Msg_ID = 5;
	msg_reset.Msg_length = 1;
	msg_reset.Data[0]=1;
	can_transmit(msg_reset);	
}