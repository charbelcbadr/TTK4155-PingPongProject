/*
 * SPI.h
 *
 * Created: 12.10.2021 20:11:15
 *  Author: harishnv
 */ 


#ifndef SPI_H_
#define SPI_H_

#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7

void spi_init();
uint8_t spi_read();
void spi_write( int data);
void slave_select();
void slave_deselect();

#endif /* SPI_H_ */