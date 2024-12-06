#include "Inf_TWASRPRO.h"

void TW_ASRPRO_Init(void) {
    MX_USART3_UART_Init();
}

void TW_ASRPRO_SendData(uint8_t* data, uint8_t size) {
    HAL_UART_Transmit(&huart3, data, size, HAL_MAX_DELAY);
}
