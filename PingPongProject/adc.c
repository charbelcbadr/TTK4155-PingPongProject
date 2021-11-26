/*
 * CFile1.c
 *
 * Created: 19.09.2021 14:54:40
 *  Author: charbelb
 */ 


#define F_CPU 4.9152e6
#define ADC_BASE 0x1400
#define ADC_CLOCK 2.45e6
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

// Declaring the storage allocation for ADC to 0x1400
volatile char* adc_ext= (char *) ADC_BASE;

/*
Description: Initialsing the clock for ADC
Input: Nothing
Output: Nothing
*/
void initialize_clock_adc() {
	//Initialising the Port D pin as output
 	DDRD |= (1<<PD5);
	//Generating the timer register and setting Compare Output Mode for channel A
 	TCCR1A |= (1<<COM1A0);
	//No prescaler and using CTC waveform to generate the clock
 	TCCR1B |= (1<<WGM12)|(1<<CS10);
	//The flag is set after the counter is matched with the value in OCR1A 
 	OCR1A = 0b00000000;
}

/*
Description: Reading the ADC values
Input: Reads the value of the channel specified
Output: Retruns the value of the channel
*/
 uint8_t read_adc(uint8_t channelNum) {
	int16_t ADC_wait = ((9*4*2)/2450000);	
 	adc_ext[0]=0;
	_delay_us(ADC_wait);
	int i=0;
	uint8_t toReturn;
	
	while(i<=channelNum)
	{
		toReturn=adc_ext[0];
		i++;
	}
	return toReturn;
 }