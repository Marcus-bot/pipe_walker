#ifndef __PCA9685_H
#define __PCA9685_H
#include<sys.h>

#define PCA9685_adrr 0x80//??


#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4


#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE


#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9
 
 
#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD
 
 
#define SERVOMIN  90
#define SERVOMAX  700 
#define SERVO000  130 //0¶È
#define SERVO180  520 //180¶È


void WritePCA9685(u8 address,u8 data);
u8 ReadPCA9685(u8 address);
void reset(void);
void setPWMfreq(float freq);
void setPWM(u16 num,u16 on,u16 off);
void PCA9685_Init(u8 freq);
#endif
