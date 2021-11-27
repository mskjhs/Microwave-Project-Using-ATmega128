/*
 * motor_control.h
 *
 * Created: 2021-09-14 오후 6:21:22
 *  Author: kccistc
 */ 


#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_


void pwm_init();
void stop_fan();
void forward_direction();
void backward_direction();


#endif /* MOTOR_CONTROL_H_ */