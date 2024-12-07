#ifndef __INF_TWASRPRO_H__
#define __INF_TWASRPRO_H__

#include "usart.h"
#include "main.h"

void TW_ASRPRO_Init(void);
void TW_ASRPRO_SendData(uint8_t* data, uint8_t size);

#endif /* __INF_TWASRPRO_H__ */
