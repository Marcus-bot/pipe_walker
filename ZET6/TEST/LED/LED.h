#ifndef __LED_H
#define __LED_H
#include<sys.h>
#define LED PCout(13)
#define LED1 PGout(9)
#define LED2 PGout(10)
#define LED3 PGout(11)
#define LED4 PGout(12)
void LED_init(void);
void LED_ctl(void);
#endif
