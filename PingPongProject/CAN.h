/*
 * CAN.h
 *
 * Created: 13.10.2021 11:14:06
 *  Author: harishnv
 */ 


#ifndef CAN_H_
#define CAN_H_

//Structure for configuring a CAN message
typedef struct {
	uint16_t Msg_ID;
 	uint8_t Msg_length;
 	uint8_t Data[8];
	}CAN_Msg;

CAN_Msg can_receive();
void can_transmit(CAN_Msg msg);
CAN_Msg can_receive();
void CAN_init();

#endif /* CAN_H_ */