#ifndef __MODBUS_MASTER_
#define __MODBUS_MASTER_

#include "main.h"
#include "usart.h"
#include <string.h>


void Inf_RS485_Init(void);
void Inf_RS485_SendCmd(uint8_t* data, uint8_t size);
int16_t Inf_RS485_GetValue(void);


#endif // !__MODBUS_MASTER_
