/*
 * Can_Driver.c
 *
 * Created: 13.10.2021 14:15:32
 *  Author: harishnv
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPI.h"
#include "mcp2515.h"

//BaudRate generated is 250Kbps and Tq = (PS1+1)+(PS2+1)+(propagation_delay+1)+(Syncronisation_delay)
#define CAN_F_CPU 16000000
#define BAUD_RATE 250000
#define TQ 16
#define PS1 6
#define propagation_delay 5
#define PS2 1

/* 
Description: Reading the values from MCP2515
Input: Address from where it has to be read through SPI
Output: The value read from the SPI commands
*/
uint8_t mcp2515_read(uint8_t address)
{
	uint8_t result;
	//Select the slave
	slave_select();
	spi_write(MCP_READ);
	spi_write(address);
	result = spi_read();
	//De select the slave after reading
	slave_deselect();
	return result;
}
/* 
Description: Writing the values to MCP2515
Input: Data and address where it has to be written through SPI
Output: Nothing
*/
void mcp2515_write(uint8_t address, uint8_t data)
{
	//Select the slave
	slave_select();
	spi_write(MCP_WRITE);
	spi_write(address);
	spi_write(data);
	//De select the slave after writing
	slave_deselect();
	_delay_ms(1);
}
/* 
Description: Reset the MCP2515 during init
Input: Nothing
Output: Nothing
*/
void mcp2515_reset()
{
	slave_select();
	spi_write(MCP_RESET);
	slave_deselect();
	_delay_ms(1);
}
/* 
Description:Request to send from the address
Input: Address
Output: Nothing
*/
void mcp2515_request_to_send(int address)
{
	slave_select();
	spi_write(address);
	slave_deselect();
	_delay_ms(1);
}
/* 
Description: Modfiying the bit 
Input: Nothing
Output: Nothing
*/
void mcp2515_bit_modify(uint8_t address,uint8_t mask, uint8_t data)
{
	slave_select();
	spi_write(MCP_BITMOD);
	spi_write(address);
	spi_write(mask);
	spi_write(data);
	slave_deselect();
	_delay_ms(1);
}
/* 
Description: Initialization function for the MCP2515 where the CNF registers are configured and setting the Baud rate at 250Kbps
Input: Nothing
Output: Nothing
*/
uint8_t mcp2515_init()
{
	uint8_t value;
	uint8_t BRP = ((CAN_F_CPU/(2 * TQ * BAUD_RATE)))-1;
	// Initialize SPI
	spi_init (); 
	// Send reset -command
	mcp2515_reset (); 
	// Self -test
	value = mcp2515_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is NOT in configuration mode after reset!\n");
		return 1;
	}	
	// Configuring CNF registers with 250kbps
	mcp2515_write(MCP_CNF1,SJW2|BRP);
	mcp2515_write(MCP_CNF2,BTLMODE|PS1<<3|propagation_delay);
	mcp2515_write(MCP_CNF3,PS2);
	
	// Settiing the MCP2515 to Normal mode for sending the data to other nodes
	mcp2515_write(MCP_CANCTRL,MODE_NORMAL);
	value = mcp2515_read(MCP_CANSTAT);
	//Checking if the mode is in LoopBack mode or not
	if ((value & MODE_MASK) == MODE_LOOPBACK) {
		printf("MCP2515 is in LoopBack configuration mode after %d reset!\n",(value & MODE_MASK));
	}
	return 0;
}

