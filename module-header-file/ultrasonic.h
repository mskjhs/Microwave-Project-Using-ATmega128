/*
 * ultrasonic.h
 *
 * Created: 2021-09-13 오전 10:52:50
 *  Author: kccistc
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define TRUE 1
#define FALSE 0

#define US_TCN TCNT1     //현재 Timer Counter 1번을 쓴다.
#define TRIG PG4         // Trigger PIN 
#define TRIG_DDR  DDRG	 // 방향 설정 출력
#define TRIG_PORT PORTG
// echo pin HW interrupt가 지원되는 포트를 잡아서 설정
// HW external INT4을 잡아서 쓴다. PE4이 INT4를 지원하는 포트
#define ECHO PE4
#define ECHO_DDR DDRE
#define ECHO_PIN PINE

void ultrasonic_init(void);
void ultrasonic_trigger(void);
#endif /* ULTRASONIC_H_ */