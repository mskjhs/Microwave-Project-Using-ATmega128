/*
 * extern.h
 *
 * Created: 2021-08-16 오전 9:59:58
 *  Author: sikwon
 */ 


#ifndef EXTERN_H_
#define EXTERN_H_

extern volatile uint8_t rxString[100];
extern volatile uint8_t rxReadyFlag;
extern volatile int rxindex;
extern void UART0_ISR_Receive();


#endif /* EXTERN_H_ */