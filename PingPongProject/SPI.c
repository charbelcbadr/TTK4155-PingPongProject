/*
 * SPI.c
 *
 * Created: 12.10.2021 20:10:47
 *  Author: harishnv
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPI.h"

/* 
Description: Initialisation of SPI
Input: Nothing
Output: Nothing
*/
void spi_init()
{
	//Set MOSI, SCK, SS to the port B as input
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);	
	//Set MISO to the port B as output
	DDRB &= ~(1<<MISO);	
	//Writing to the PORT B of SS
	PORTB |= (1<<SS);	
	//Setting the SPI control register by enabling the SPI and dividing the Fosc clock by 16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
/* 
Description: Reading the SPI
Input: Nothing
Output: Nothing
*/
uint8_t spi_read()
{
	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));	
	return SPDR;
}
/* 
Description: Writing the data through SPI
Input: Nothing
Output: Nothing
*/
void spi_write( int data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));	
}
/* 
Description: Selecting the Slave
Input: Nothing
Output: Nothing
*/
void slave_select()
{
	PORTB &=~(1<<SS);
}
/* 
Description: De selecting the Slave
Input: Nothing
Output: Nothing
*/
void slave_deselect()
{
	PORTB |=(1<<SS);
}