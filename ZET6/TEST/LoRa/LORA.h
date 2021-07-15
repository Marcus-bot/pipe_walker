#ifndef __LORA_H
#define __LORA_H
#include<sys.h>

#define MD0 PAout(1)


u8 LORA_start(void);
u8* LORA_check_cmd(u8 *str);
u8 LORA_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
void got_data(u8* cmd,u8* ack,u16 waittime);
#endif
