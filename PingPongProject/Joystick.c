/*
 * CFile1.c
 *
 * Created: 21.09.2021 16:45:19
 *  Author: charbelb
 */ 

#define F_CPU 4.9152e6
#define BAUD 9600
#define MYUBRR  F_CPU/16/BAUD-1
//No operation where the values in this range is considered as deadzone
#define NOP 5

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "Joystick.h"
#include "adc.h"
#include "USART.h"
#include "OLED.h"
#include "Menu.h"
#include "CAN.h"
#include "Replay.h"
#include "System_StartUp.h"


//Handling the system
int state;
extern int current_HighScore[3];
//Global variables for the Joystick values to be read during Initialization
int intial_X,intial_Y;
extern int present_score;
int default_mode=0;
int default_mode_end=0;
/* 
Description: Reads the values from the ADC from the Multifunction board
Input: Nothing
Output: Returns the values of Joystick and Sliders from the Multifunction board
*/
Joystick read_out_adc(){
	Joystick toReturn;
	toReturn.Joystick_Y=read_adc(0);
	toReturn.Joystick_X=read_adc(1);
	toReturn.Slider_R=read_adc(2);
	toReturn.Slider_L=read_adc(3);
	return toReturn;
}
/* 
Description: The function is called during the Initialization where the offset values are read from the Multifunction board to make it to zero
Input: Nothing
Output: Nothing
*/
void Joystick_init(){
		intial_X=read_adc(1);
		intial_Y=read_adc(0);
}
/* 
Description: Applying the transform function to recieve the values in specified range
Input: Nothing
Output: Returns the values of Joystick and Sliders after the transformation
*/
Joystick apply_transform (Joystick initial){
	 Joystick result;
	 result.Joystick_X=(initial.Joystick_X-intial_X);
	 result.Joystick_Y=(initial.Joystick_Y-intial_Y);
	 result.Slider_L=(int)((initial.Slider_L-127.5)*200/255.0);
	 result.Slider_R=(int)((initial.Slider_R-127.5)*200/255.0);
	 return result;
}
/* 
Description: Function is used to determine the direction from the JoyStick
Input: Current values from the Joystick 
Output: Returns the direction of the JoyStick
*/
char* Direction_Joystick(Joystick current){
	if (current.Joystick_X>NOP && current.Joystick_X>=current.Joystick_Y)
		return "RIGHT";
	else if (current.Joystick_Y>NOP && current.Joystick_X<current.Joystick_Y)
		return "UP";
	else if (current.Joystick_Y<-NOP && current.Joystick_X>=current.Joystick_Y)
		return "DOWN";
	else if (current.Joystick_X<-NOP && current.Joystick_X<current.Joystick_Y)
		return "LEFT";
	else
		return "NEUTRAL";
	}
/* 
Description: Function is used to determine the JoyStick values 
Input: Current values from the Joystick 
Output: Returns the values of the JoyStick after transformation
*/
Joystick Joystick_values(Joystick current){
	current = read_out_adc();
	current = apply_transform(current);
	if(current.Joystick_X >= 0)
	{
		current.Joystick_X = ((current.Joystick_X) * 100.0/(255.0-intial_X));
	}
	else
	{
		current.Joystick_X = (((current.Joystick_X * 100)/intial_X));
	}
	if(current.Joystick_Y >= 0)
	{
		current.Joystick_Y = ((current.Joystick_Y) * 100.0/(255.0-intial_Y));
	}
	else 
	{
		current.Joystick_Y = (((current.Joystick_Y * 100)/intial_Y));
	}
	
	return current;	
}
/* 
Description: Function is used to provide the information of the Joystick
Input: Current values from the Joystick 
Output: Returns the values of the JoyStick after transformation
*/
 char* Joystick_info(Joystick current){
	current=Joystick_values(current);
	return Direction_Joystick(current);	
}
/* 
Description: Reads the SLider and Joystick buttons from Multifunction Board
Input: The head contains the data selected from the list in Menu
Output: Nothing
*/
void Button(Node* head){
	//Reading the value of the button
	int read_JoyStick_button_status = (PINB & 1);
	int read_Slider_Right_Button_status = (PINB & 2);
	//Checks if the button is pressed
	if(read_JoyStick_button_status == 0)
	{
		//When Start is pressed, the game starts
		if(head->data == "Start")
			{
				reset_oled();
				state = 1;
				if(default_mode_end == 1)
				{
					default_mode=0;
					easy_mode();
				}
				if(default_mode == 0)
				{
					
					easy_mode();
				}
				reset_system_node_2();
				OLED_timer(4,3,50);
				reset_oled();
				oled_goto_line(3);
				oled_goto_column(20);
 				oled_print("GAME STARTED",LARGE);
 				send_input_singals(); 
			}
		//When calibrated is pressed, the joystick is calibrated	
		if(head->data == "Calibrate")
			{
				Joystick_init();
				reset_system_node_2();
				reset_oled();
				oled_goto_line(3);
				oled_goto_column(30);
 				oled_print("CALIBRATED",MEDIUM);
				_delay_ms(2000);
				reset_oled();
				displayMenu();	
			}
		//When Replay is pressed, the current user how they played game will be replayed by retrieving the values from SRAM
		if(head->data == "Replay")
		{
			reset_system_node_2();
			if(default_mode == 2)
			{
				difficult_mode();
			}
			else
			{
				easy_mode();
			}
			reset_oled();
			oled_goto_line(3);
			oled_goto_column(10);
			oled_print("REPLAY STARTED",LARGE);
			replay_retrieve();
			reset_oled();
			displayMenu();	
			replay_status=1;
		}
		//When Highscore is pressed, Displays the Highscore in OLED 
		if(head->data == "Highscore")
		{	
			reset_oled();
			oled_print("HIGHSCORES",LARGE);
			read_HighScore();
			char* temp=NULL;
			for(int i=0;i<3;i++)
			{	
				int current_value =current_HighScore[i];
				temp = convertIntegerToChar(current_value);
				oled_goto_line(i+2);
				oled_goto_column(35);
				oled_print(temp,LARGE);	
			}
			_delay_ms(3000);
			reset_oled();
			displayMenu();		
		}
		//When Mode is pressed, Displays the modes to be chosen
		if(head->data == "Mode")
		{
			reset_oled();
			_delay_ms(2000);
			Sub_displayMenu();
		}
		//When Exit is pressed, exits from the game
		if(head->data == "Contrast")
		{
			reset_oled();
			Sub_displayMenu_contrast();
		}
		//When Easy mode is pressed, the CAN message is sent to the node2 to make the game simpler
		if(head->data == "EASY")
		{	
			easy_mode();
			default_mode=1;
			reset_oled();
			displayMenu();
		}
		//When Difficult mode is pressed, the CAN message is sent to the node2 to make the game Tough
		if(head->data == "DIFFICULT")
		{
			difficult_mode();
			default_mode = 2;
			reset_oled();
			displayMenu();		
		}
		//When LOW contrast is pressed, the brightness is low
		if(head->data == "LOW")
		{
			contrast(50);
			reset_oled();
			displayMenu();
		}
		//When LOW contrast is pressed, the brightness is medium
		if(head->data == "MEDIUM")
		{
			contrast(150);
			reset_oled();
			displayMenu();
		}
		//When BRIGHT contrast is pressed, the brightness is high
		if(head->data == "BRIGHT")
		{
			contrast(255);
			reset_oled();
			displayMenu();
		}
	}
	//Checks if the Right Slider button is pressed and displays the main menu
	if(read_Slider_Right_Button_status == 2)
	{
		reset_oled();
		head=displayMenu();	
	}
	
}
/* 
Description: Sends the CAN message to the other nodes when the game is started
Input: Nothing
Output: Nothing
*/
void send_input_singals(){
		CAN_Msg message;
		Joystick current;
		//If the State is 1, then the game has started otherwise stops sending the transmitting messages
		while(state== 1)
		{			
			Joystick value= Joystick_values(current);
			//Configuring the message ID, Length and Data to be sent
			message.Msg_ID = 1;
			message.Msg_length = 3;
			if(value.Joystick_X<-5)
			{
				message.Data[0] = value.Joystick_X+201;
			}
			//If the data is NOP region, the data is sent as 0
			else if(5>value.Joystick_X && value.Joystick_X>-5)
			{
				message.Data[0] = 0;
			}
			else
			{
				message.Data[0] = value.Joystick_X;
			}
			if(value.Joystick_Y<-5)
			{
				message.Data[1] = value.Joystick_Y+201;
			}
			else if(5>value.Joystick_Y && value.Joystick_Y>-5)
			{
		 		message.Data[1] = 0;
		 	 }
			else
			{
				message.Data[1] = value.Joystick_Y;
			}
		//Manipulating the Right slider values to minimize the issues of negative numbers
		if(value.Slider_R>0)
		{
			message.Data[1] = value.Slider_R;
		}
		else
		{
			message.Data[1] = value.Slider_R+201;
		}
		//If the read_Slider Left button is pressed, Solenoid pin is activated by sending a data of  10
		if((PINB & 4)==4)
		{
			message.Data[2]=10;
		}
		else
		{
			message.Data[2]=0;
		}
		//Sending the CAN message to other nodes
		can_transmit(message);
		//Updates the score until game is ended
 		update_present_score();
		//Storing the CAN messages of the current user to replay it later
		replay_store(message);
		 _delay_ms(250);
	}
}

/*
Description: Depending on the user chosen mode, a CAN message is sent to assign the mode. Function for easy mode
Input: Nothing
Output: Nothing
*/
void easy_mode()
{
	CAN_Msg mode_msg;
	mode_msg.Msg_ID = 9;
	mode_msg.Msg_length = 1;
	mode_msg.Data[0] = 1;
	can_transmit(mode_msg);
	_delay_ms(250);
}

/*
Description: Depending on the user chosen mode, a CAN message is sent to assign the mode. Function for difficult mode
Input: Nothing
Output: Nothing
*/
void difficult_mode()
{
	CAN_Msg mode_msg;
	mode_msg.Msg_ID = 9;
	mode_msg.Msg_length = 1;
	mode_msg.Data[0] = 2;
	can_transmit(mode_msg);	
	_delay_ms(250);
}