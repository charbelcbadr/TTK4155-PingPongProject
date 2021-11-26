/*
 * EEPROM_driver.h
 *
 * Created: 15.11.2021 13:53:08
 *  Author: harishnv
 */ 


#ifndef EEPROM_DRIVER_H_
#define EEPROM_DRIVER_H_


void EEPROM_write( int Address, int Data);
int EEPROM_read(int Address);

#endif /* EEPROM_DRIVER_H_ */