/*
 * IncFile1.h
 *
 * Created: 21.09.2021 16:45:40
 *  Author: charbelb
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "Menu.h"
//Structure for the values of JoyStick and Silder to store
typedef struct 
{
	int Joystick_X;
	int Joystick_Y;
	int Slider_L;
	int Slider_R;
}Joystick;
//Structure for the Joystick and slider buttons
typedef struct
{
	int Joystick_Button;
	int Slider_L_Button;
	int Slider_R_Button;
}Button_tst;

//Global flag for handling the mode
int default_mode;
int replay_status;

Joystick read_out_adc();
Joystick apply_transform(Joystick initial);
char* Direction_Joystick(Joystick current);
char* Joystick_info(Joystick current);
void Button(Node* head);
void Joystick_init();
Joystick Joystick_values(Joystick current);
void send_input_singals();
void difficult_mode();
void easy_mode();

#endif /* JOYSTICK_H_ */