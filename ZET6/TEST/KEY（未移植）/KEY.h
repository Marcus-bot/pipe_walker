#ifndef __KEY_H
#define __KEY_H
#include<sys.h>
#define KEY0 PCin(5)
#define KEY1 PAin(15)
void KEY_Init(void);
u8 KEY_SCAN(u8 mode);
#endif
