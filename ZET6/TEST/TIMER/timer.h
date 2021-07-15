#ifndef __TIMER_H
#define __TIMER_H
#include<sys.h>
void myTIMER3_PWM_Init(u16 period,u16 prescaler);
void myTIMER1_PWM_Init(u16 period,u16 prescaler);
void myTIMER5_Init(u16 period,u16 prescaler);
void myTIMER4_Init(u16 period,u16 prescaler);
void TIM5_SET(FunctionalState NewState);
#endif
