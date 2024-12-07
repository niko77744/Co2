#ifndef __INF_KEY_H__
#define __INF_KEY_H__

#include "adc.h"
#include <stdlib.h>

extern uint32_t key_value;
typedef enum {
    Key0,
    Key1,
    Key2,
    None
}KeyCode;

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����

void Inf_Key_Init(void);
uint8_t Inf_Key_Scan(uint8_t mode);

#endif /* __INF_KEY_H__ */
