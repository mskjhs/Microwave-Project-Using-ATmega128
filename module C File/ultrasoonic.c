/*
 * ultraSonic.c
 *
 * Created: 2021-04-05 오후 4:02:16
 *  Author: kccistc
 */ 
// PG4 trigger
// PE4(INT4) 외부인터럽트를 이용하여 시간 측정 시작 - 끝 확인

#include "ultrasonic.h"

volatile int ultrasonic_distance;


//초음파 센서 초기화 함수
void ultrasonic_init(void)
{
	 TRIG_DDR |= (1<<TRIG);    //출력(1)으로 설정
	 ECHO_DDR &= ~(1<<ECHO);   //입력(0)으로 설정
	 
	 //외부 인터럽트 트리거 방식을 설정(low/상승/하강에지를 선택하는 레지스터)
	 EICRB |= (0<<ISC41) | (1<<ISC40);  
	 //EICRA : INT 0 ~ 3
	 //EICTB : INT 4 ~ 7
	 // 0 0 : INT의 LOW신호시 일반적인 INT요청
	 // 0 1 : 어ㅓ떠한 신호의 변화에든 INT요청
	 // 1 0 : 하강 에지
	 // 1 1 : 상승 에지
	
	 EIMSK |= (1<<INT4); //외부인터럽트 4번을 요청
	 
	 TCCR1B |= (1 << CS12) | (1<<CS10);  //TCNT1  16비트 타이머 1번 타이머/카운터를 1024로 분주 16000000/1024  ==> 15625HZ 
}

void ultrasonic_trigger(void)
{
	TRIG_PORT &= ~(1<<TRIG);     // LOW 값 출력 
	_delay_us(1);	
	TRIG_PORT |= (1<<TRIG);
	_delay_us(15);
	TRIG_PORT &= ~(1<<TRIG);    //LOW 값 출력
}

