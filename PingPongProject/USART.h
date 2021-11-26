/*
 * IncFile1.h
 *
 * Created: 15.09.2021 15:46:14
 *  Author: charbelb
 */ 


#ifndef USART_H_
#define USART_H_

int USART_Transmit(char data, FILE *stream);
int USART_Recieve(FILE *stream);
void USART_Init (unsigned int ubrr);

#endif /* USART_H_ */