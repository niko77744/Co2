#ifndef __MODBUS_MASTER_
#define __MODBUS_MASTER_

#include "main.h"
#include "usart.h"
#include <string.h>

#define GET_HUM_CMD 0           /* ʪ�� */
#define GET_TEMP_CMD 1          /* �¶� */
#define GET_CO2_CMD 2           /* ������̼ */
#define IS_VOICE_HUM 0          /* ����51ʪ�� */
#define IS_VOICE_TEMP 1         /* ����51�¶� */
#define IS_VOICE_CO2 2          /* ����51������̼ */

extern uint8_t usart2_flag;

void Inf_RS485_Init(void);
void Inf_RS485_SendCmd(uint8_t* data, uint8_t size);
int16_t Inf_RS485_GetValue(void);

#endif // !__MODBUS_MASTER_
