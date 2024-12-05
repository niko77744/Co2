#include "Inf_Key.h"

uint32_t key_value = 0;
void Inf_Key_Init(void) {
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, &key_value, 1);
}
