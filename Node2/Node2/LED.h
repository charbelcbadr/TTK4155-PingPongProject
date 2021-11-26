/*
 * LED.h
 *
 * Created: 18.11.2021 22:48:30
 *  Author: harishnv
 */ 


#ifndef LED_H_
#define LED_H_

#define LED_1 PIO_PA19
#define LED_2 PIO_PA20

void init_LED();
void Switch_only_LED1();
void Switch_only_LED2();
#endif /* LED_H_ */