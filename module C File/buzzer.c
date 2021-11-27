/*
 * buzzer.c
 *
 * Created: 2021-09-13 오후 4:07:59
 *  Author: kccistc
 */ 

//부져 변수

#include "buzzer.h"
volatile uint8_t buzzer_start_flag = 0;  //button을 눌렀을때 1로 set, buzzer timeout시 0 set
volatile int buzzer_timer = 0;         // 부저 동작시간 200~300ms



void buzzer_init()
{
	BUZZER_DDR |= 1<< BUZZER1;
	
}

void buzzer_on()
{
	buzzer_start_flag = 1;  //button을 눌렀을때 1로 set, buzzer timeout시 0 set
	buzzer_timer = 0;
	
	PORTG |= 1<<BUZZER1;    // Buzzer on
}

void buzzer_off()
{
	buzzer_start_flag = 0;  //button을 눌렀을때 로 set, buzzer timeout시 0 set
	buzzer_timer = 200;
	PORTG &= ~(1 << BUZZER1);  //Buzzer off
}

void buzzer_off_check()
{
	if(buzzer_timer > 200) // 200ms over
	{
		buzzer_off();
	}
}