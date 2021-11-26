/*
 * LED.c
 *
 * Created: 18.11.2021 22:48:13
 *  Author: harishnv
 */ 
#include "sam.h"
#include "LED.h"
/* 
Description: Intialisation of LEDS
Input: Nothing
Output: Nothing
*/
void init_LED()
{
    //enable LEDs as output
    PIOA->PIO_PER |=(LED_1|LED_2);
    PIOA->PIO_OER |=(LED_1|LED_2);
}
/* 
Description: Function for Switch on LED1 and switch off LED2
Input: Nothing
Output: Nothing
*/
void Switch_only_LED1()
{
    PIOA->PIO_SODR =LED_1;
    PIOA->PIO_CODR =LED_2;
}
/* 
Description: Function for Switch on LED2 and switch off LED1
Input: Nothing
Output: Nothing
*/
void Switch_only_LED2()
{
    PIOA->PIO_CODR =LED_1;
    PIOA->PIO_SODR =LED_2;
}