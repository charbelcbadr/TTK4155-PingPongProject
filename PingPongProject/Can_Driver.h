/*
 * Can_Driver.h
 *
 * Created: 13.10.2021 14:15:45
 *  Author: harishnv
 */ 


#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

uint8_t mcp2515_init();
uint8_t mcp2515_read(uint8_t adddress);
void mcp2515_reset();
void mcp2515_bit_modify(uint8_t address,uint8_t mask, uint8_t data);
void mcp2515_write(uint8_t address, uint8_t data);
void mcp2515_request_to_send();

#endif /* CAN_DRIVER_H_ */