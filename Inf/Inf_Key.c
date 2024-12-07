#include "Inf_Key.h"

uint32_t key_value = 0;
const uint8_t key_offset = 100;
void Inf_Key_Init(void) {
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, &key_value, 1);
}

#define KEY0  Inf_Key_GetAction(Key0)//读取按键0
#define KEY1  Inf_Key_GetAction(Key1)//读取按键1
#define KEY2  Inf_Key_GetAction(Key2)//读取按键2 

uint8_t Inf_Key_GetAction(uint8_t key_code) {
    if (key_code == Key2) {
        if (key_value <= 2800 && key_value >= 2700) return 1;
        else return 0;
    }
    if (key_code == Key1) {
        if (key_value <= 2100 && key_value >= 2000) return 1;
        else return 0;
    }
    if (key_code == Key0) {
        if (key_value <= 100) return 1;
        else return 0;
    }
    return 2;
}

uint8_t Inf_Key_Scan(uint8_t mode) {
    static uint8_t key_up = 1;//按键按松开标志
    static uint8_t key_time = 0;
    if (mode)key_up = 1;  //支持连按		  
    if (key_up && (KEY0 == 1 || KEY1 == 1 || KEY2 == 1) && (key_time++ >= 2)) {
        key_up = 0;
        key_time = 0;
        if (KEY0 == 1)return KEY0_PRES;
        else if (KEY1 == 1)return KEY1_PRES;
        else if (KEY2 == 1)return KEY2_PRES;
    }
    else if (KEY0 == 0 && KEY1 == 0 && KEY2 == 0) key_up = 1;
    return 0;// 无按键按下
}
