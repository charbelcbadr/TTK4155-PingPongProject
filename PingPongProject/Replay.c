/*
 * Replay.c
 *
 * Created: 12.11.2021 16:52:10
 *  Author: harishnv
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "CAN.h"

#define SRAM_BASE 0x1800
//Declare the start address of the SRAM
volatile char *ext_ram = (char *) SRAM_BASE; 
//Storage limit of the SRAM
uint16_t ext_ram_size = 0x800;

uint16_t current_ram_address;
//Consumed memory
uint16_t total_usage ;
/* 
Description: Stores the current user game values of Joystick and Silder in SRAM
Input: The data of the CAN message to be stored
Output: Nothing
*/
void replay_store(CAN_Msg msg)
{
	//Storing each of the CAN message in one address
	ext_ram[current_ram_address] = msg.Msg_ID;
	ext_ram[current_ram_address+1] = msg.Msg_length;
	ext_ram[current_ram_address+2] = msg.Data[0];
	ext_ram[current_ram_address+3] = msg.Data[1];
	ext_ram[current_ram_address+4] = msg.Data[2];
	current_ram_address +=5;
	total_usage +=5;
}
/* 
Description: Replays how the current user played the game, when Replay is selected from the display Menu
Input: Nothing
Output: Nothing
*/
void replay_retrieve()
{
	current_ram_address = 0;
	CAN_Msg retreived_msg;
	//Transmit the CAN message until consumed memory in SRAM and transmit the data
	while(current_ram_address < total_usage)
	{
		retreived_msg.Msg_ID = ext_ram[current_ram_address];
		retreived_msg.Msg_length = ext_ram[current_ram_address+1];
		retreived_msg.Data[0] = ext_ram[current_ram_address+2];
		retreived_msg.Data[1] = ext_ram[current_ram_address+3];
		retreived_msg.Data[2] = ext_ram[current_ram_address+4];
		_delay_ms(250);
		current_ram_address+=5;
		can_transmit(retreived_msg);
	}
	printf("Replay is over");
}

