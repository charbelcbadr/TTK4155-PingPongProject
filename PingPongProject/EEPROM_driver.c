/*
 * EEPROM_driver.c
 *
 * Created: 15.11.2021 13:52:51
 *  Author: harishnv
 */ 
#include <avr/io.h>
#include "EEPROM_driver.h"

/*
Description: Writing the Data through the Address into the EEPROM
Input: Nothing
Output: Nothing
(the reference for the source code is from Atmega162) 
*/
void EEPROM_write( int Address, int Data)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address and data registers */
	EEAR = Address;
	EEDR = Data;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}
/*
Description: Reading the Data through the Address from the EEPROM
Input: Address from where it has to be read from
Output: Returns the data
(the reference for the source code is from Atmega162) 
*/
int EEPROM_read(int Address)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address register */
	EEAR = Address;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}