#include "TW_ASRPRO.h"


void TW_ASRPRO_Init(void) {
    MX_USART2_UART_Init();
}

void TW_ASRPRO_SendData(uint8_t* data, uint8_t size) {
    HAL_UART_Transmit(&huart2, data, size, HAL_MAX_DELAY);
}
