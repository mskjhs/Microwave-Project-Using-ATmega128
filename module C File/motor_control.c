/*
 * motor_control.c
 *
 * Created: 2021-09-14 오후 6:21:09
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>

void pwm_init()
{
	DDRE |= (1 << PE2) | (1 << PE3) | (1 << PE5);
	//DDRE |= 0b00100101
	//모드5  고속 PWM timer/counter 3번
	TCCR3A |= (1 <<WGM30);
	TCCR3B |= (1 <<WGM32);
	
	//비반전모드 TOP :0xff 비교일치값 : OCR3C : PE5
	TCCR3A |= (1 << COM3C1);
	
	//분주비 64   16MHZ / 64 ==> 250kHZ
	//256 / 250000  ==> 1.02ms
	//127 / 250000  ==> 0.5ms
	TCCR3B |=  (1 << CS31) | (1 << CS30);    //분주비 64
	
	OCR3C = 0;
}
void stop_fan()
{
	PORTE |= 0b00001100;
	OCR3C = 0;
}
void forward_direction()
{
	PORTE &= 0b11110111;
	PORTE |= 0b00000100;
	OCR3C = 70;
}
void backward_direction()
{
	PORTE &= 0b111110111;
	PORTE |= 0b00001000;
	OCR3C = 70;
}