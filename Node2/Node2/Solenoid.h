/*
 * Solenoid.h
 *
 * Created: 10.11.2021 10:10:26
 *  Author: harishnv
 */ 


#ifndef SOLENOID_H_
#define SOLENOID_H_

// The pin is connected to Pin41
#define Solenoid_PIN PIO_PC9

void solenoid_init();
void solenoid_controller(int value);


#endif /* SOLENOID_H_ */