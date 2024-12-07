#include "Inf_LED.h"


LED_GPIO_T leds = {
    .ActiveLevel = GPIO_PIN_RESET,
    .GPIO = GPIOA,
    .GPIO_Pin = GPIO_PIN_1
};

LED_STATUS_T led_status = {
    .FalshTime = 0,
};


void Inf_LED_Init(void) {
    GPIO_InitTypeDef GPIO_Init_t = { 0 };
    GPIO_Init_t.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_t.Pin = leds.GPIO_Pin;
    GPIO_Init_t.Pull = GPIO_NOPULL;
    GPIO_Init_t.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(leds.GPIO, &GPIO_Init_t);
    Inf_LED_Control((uint8_t)Flash, 2000);
}


void Inf_LED_On(void) {
    HAL_GPIO_WritePin(leds.GPIO, leds.GPIO_Pin, leds.ActiveLevel);
}
void Inf_LED_Off(void) {
    HAL_GPIO_WritePin(leds.GPIO, leds.GPIO_Pin, (GPIO_PinState)(!leds.ActiveLevel));
}
void Inf_LED_Toggle(void) {
    HAL_GPIO_TogglePin(leds.GPIO, leds.GPIO_Pin);
}

void Inf_LED_Control(uint8_t mode, uint16_t flashtime_ms) {
    static uint32_t last_time = 0;
    uint32_t now_time = HAL_GetTick();

    if (now_time - last_time <= led_status.FalshTime) return;
    else last_time = now_time;

    switch (mode) {
    case On:
        Inf_LED_On();
        break;
    case Off:
        Inf_LED_Off();
        break;
    case Flash:
        led_status.FalshTime = flashtime_ms;
        Inf_LED_Toggle();
        break;
    default:
        break;
    }
}


