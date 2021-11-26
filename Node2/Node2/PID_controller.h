/*
 * PID_controller.h
 *
 * Created: 09.11.2021 19:43:15
 *  Author: harishnv
 */ 


#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_
#include "stdint.h"

//Structure for the PID parameters used for calculating the speed
typedef struct PID_parameters{
 	float Kp;
 	int current_value;
 	int prev_value;
 	int sum_error;
 	int prev_error;
	float Ki;
 	float Kd;
 	int PID_MAX;
 }parameters;

void set_easy_mode();
void set_difficult_mode();
int pid_controller(int encoder_value,int current_position);

#endif /* PID_CONTROLLER_H_ */