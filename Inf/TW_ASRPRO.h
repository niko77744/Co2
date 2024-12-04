#ifndef __TW_ASRPRO_H__
#define __TW_ASRPRO_H__

#include "usart.h"
#include "main.h"

void TW_ASRPRO_Init(void);
void TW_ASRPRO_SendData(uint8_t* data, uint8_t size);


#endif /* __TW_ASRPRO_H__ */
