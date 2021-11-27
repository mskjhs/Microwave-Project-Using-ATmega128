/*
 * 12. N298N_DC_MOTOR_DRIVER_PWM_TIME_CONTROL.c
 *
 * Created: 2021-09-14 오후 12:17:59
 * Author : kccistc
 */ 
/*	
1. connection
	LN298N   ---- ATmega128A
	------        ----------
	ENA			  PE4
	IN1		      PE0
	IN2           PE2

2. motor 제어
   PE0  : 방향 설정 핀
   PE2  : enable 단자
   PE4  : PWM 제어 단자
   IN1(PE0)   IN2(PE2)
		0		   1   :역방향 회전
		1		   0   :정방향 회전
		1          1   :브레이크(정지)
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>   
#include "button.h"
#include "uart0.h"
#include "buzzer.h"
#include "motor_control.h"
#include "I2C_LCD.h"
#include "ds1302.h"
#include "ultrasonic.h"

char sbuff[40];
#define SET 0
#define GO 1
#define PAUSE 2
#define OPEN 3
#define MENU 4

uint16_t mode = 0;
uint16_t on_off = 0;
uint16_t time = 0;
uint16_t counter = 0;
uint16_t number =0;
uint16_t timer =0;
void UART0_transmit(char data);

void dispay(void);
#define DELAY 1000
#define  PRESCALER 1024
volatile int ultrasonic_distance = 0;
volatile int ultrasonic_trigger_timer = 0;
volatile int ultrasonic_distance_real = 0;
char sbuff[40];
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

ISR(INT4_vect)
{
	if (ECHO_PIN & (1<< ECHO)) //라이징 에지
	{
		TCNT1 = 0;
	}
	else     //폴링에지
	{
		ultrasonic_distance = 1000000.0*TCNT1*1024/F_CPU;
		
		//에코핀의 펄스폭을 micro초로 환산
		
	}
}
ISR(TIMER0_OVF_vect) //인터럽트 서비스 루틴 1ms 주기로 인터럽트 발생
{
	TCNT0 = 0;
	static uint16_t secCount = 0;
	ultrasonic_trigger_timer++;   //for 초음파 센서 측정 주기 계산하는 counter
	
	if (++secCount >= 1000)
	{
		secCount = 0;
		if(mode == GO)
		 counter++;
	}
}



int main(void)
{
	pwm_init();
	stdout = &OUTPUT;
	UART0_init();
	button_Init();
	buzzer_init();
	I2C_LCD_init();
	ultrasonic_init();
	mode = MENU;

	TCCR0 |= ((1<<CS02) | (0<<CS01) | (0<<CS00));	//111 => 분주비를 1024로 설정 //100 => 분주비를 64로 설정
	TIMSK |= (1<<TOIE0);	//타이머 오버플로 인터럽트 설정
	sei();

	while (1)
	{

		
		
		switch(mode)
		{	
			case MENU : 
			I2C_LCD_write_string_XY(0,0, "==K-MICROWAVE==        ");
			
			if(getButton1State())
			{
				mode = SET;
				
			}
		
			case SET:
			
			sprintf(sbuff, "Cook-time :%3ds        ", time);
			I2C_LCD_write_string_XY(1,0, sbuff);
			
			if(getButton1State())
			{
				mode = SET;
				time = time + 10;
			}
			
			if(getButton2State())
			{
				mode = SET;
				time = time + 30;
			}
			
			
		
			if(getButton4State())
			{
				mode = GO;
			
				
			}
			break;
			
			case GO:
			measure_distance();
			
		
			forward_direction();
			
			if(counter == time)
			{
				mode = SET;
				counter = 0;
				time =0;
				stop_fan();
				buzzer_on();
				_delay_ms(2000);
				buzzer_off();
			
			}
			if(getButton3State())
			{
				mode = PAUSE;
				
			}
			
			break;
			case PAUSE :
			
			sprintf(sbuff, "Cook-time:%3ds        ", time-counter);
			I2C_LCD_write_string_XY(1,0, sbuff);	
			stop_fan();
			
			if(getButton4State())
			{
				mode = GO;
				
				
			}
			break;
			
			case OPEN:
			measure_distance();
			sprintf(sbuff, "Cook-time:%3ds        ", time-counter);
			I2C_LCD_write_string_XY(1,0, sbuff);
			stop_fan();
			if(getButton1State())
			{
				mode = OPEN;
				time = time + 10;
			}
			
			if(getButton2State())
			{
				mode = OPEN;
				time = time + 30;
			}
			if(getButton3State())
			{
				mode = GO;
				
				
			}
			break;
			
		}
	}
	
 }
void measure_distance(void)
{
	if (ultrasonic_trigger_timer >= 1000)  //1초 ㄱㄷㅁ촏ㅇ
	{
		//sprintf(sbuff,"distance;%3dcm ", ultrasonic_distance/58);
		//I2C_LCD_write_string_XY(0,0,sbuff);
		ultrasonic_trigger();
		ultrasonic_trigger_timer = 0;
		ultrasonic_distance_real =  ultrasonic_distance/58;
		if (ultrasonic_distance_real <= 5)
		{
			PORTA = 0xff;
			mode = GO;
		}
		else
		{
			PORTA =0x00;
			mode = OPEN;
		}
	}
}