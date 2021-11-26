/*
 * CAN.c
 *
 * Created: 13.10.2021 11:13:57
 *  Author: harishnv
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "Can_Driver.h"
#include "mcp2515.h"
#include "CAN.h"
#include "Joystick.h"
#include "OLED.h"

extern Node* head;

//Global state to handle the tramission and reception of data
extern int state;
extern int present_score;
extern int default_mode;
extern int default_mode_end;
extern int replay_status;

//Interrupt hanlde INT0 is called when new CAN recieve messages arrives from the other nodes
ISR (INT0_vect){
	CAN_Msg msg=can_receive();
	//If the message ID from Node 2 is 2, process the request otherwise no
	if(msg.Msg_ID == 2){
		state =2;
		printf("ID: %d, Length:%d, Data:%d receive \n",msg.Msg_ID,msg.Msg_length,msg.Data[0]);
		if(replay_status ==0)
		{
			update_HighScore();
		}
		
		reset_oled();
		oled_goto_line(3);
		oled_goto_column(20);
		oled_print("GAME OVER",LARGE);
		_delay_ms(5000);
		//Reseting the score and mode
		present_score = 0;
		default_mode_end = 1;
		reset_oled();
		displayMenu();
		replay_status=1;
	}
}

/* 
Description: Initialisation function for CAN
Input: Nothing
Output: Nothing
*/
void CAN_init(){
	mcp2515_init();
	// enable interrupt generation for successful reception
	mcp2515_write(MCP_CANINTE,MCP_RX_INT_B0);
	//Clear the global interrupts
	cli();
	//The falling edge of INT0 generates an interrupt request
	MCUCR = (1<<ISC01)|(0<<ISC00);
	//Set INT0 interrupts through the GICR register
	GICR = (1<<INT0);
	//Set the global interrupts
	sei();
}
/* 
Description: The function is used to send the CAN messages
Input: CAN message which needs to be transmitted
Output: Nothing
*/
void can_transmit(CAN_Msg msg){
	//Classifying the High byte and low byte of Message ID of 16 bits by right shifting high byte and left shifting the low byte and writing to the MCP Tx Buffer 
	int msg_ID = msg.Msg_ID;
	uint8_t ID_High = msg_ID>>3;
	uint8_t ID_Low = msg_ID<<5;
	mcp2515_write(MCP_TXB0SIDH, ID_High);
	mcp2515_write(MCP_TXB0SIDL, ID_Low);
	// Writing the Message Length to the MCP Tx DLC
	int length = msg.Msg_length;
	mcp2515_write(MCP_TXB0DLC, length);
	//Writing the Data to the MCP Tx
	for(int i=0; i<=length; i++){
		mcp2515_write (MCP_TXB0D0+i, msg.Data[i]);
	}
	printf("Transmitting the Data\n");
	printf("ID: %d,\t Length:%d,\t Data[0]:%d,\t Data[1]:%d,\t Data[2]:%d \n",msg.Msg_ID,msg.Msg_length,msg.Data[0],msg.Data[1],msg.Data[2]);	
	//Requesting to send the data
	mcp2515_request_to_send(MCP_RTS_TX0);
}
/* 
Description: The function is used to receive the CAN messages
Input: Nothing
Output: Returns the CAN Message structure received by reading the Buffers
*/
CAN_Msg can_receive(){
	CAN_Msg msg;
	//Reading the Message ID from the MCP Rx
	int msg_ID_High = mcp2515_read(MCP_RXB0SIDH);
	int msg_ID_Low = mcp2515_read(MCP_RXB0SIDL);
	msg.Msg_ID= (msg_ID_High<<3)| (msg_ID_Low>>5);
	//Reading the Message Length from the MCP Rx
	int length = mcp2515_read(MCP_RXB0DLC);
	msg.Msg_length=length;
	//Reading the Data from the MCP Rx
	for(int i=0; i<=length; i++){
		msg.Data[i]=mcp2515_read(MCP_RXB0D0+i);
	}
	mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	return msg;
}