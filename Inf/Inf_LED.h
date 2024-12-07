#ifndef __INF_LED_H__
#define __INF_LED_H__

#include "gpio.h"

typedef struct {
    GPIO_TypeDef* GPIO;
    uint16_t GPIO_Pin;
    GPIO_PinState ActiveLevel;	/* �����ƽ*/
}LED_GPIO_T;

typedef struct {
    uint16_t FalshTime;  //��˸ʱ�� ��λms
    enum {
        On,
        Off,
        Flash,
    }status;
}LED_STATUS_T;

void Inf_LED_Init(void);
void Inf_LED_On(void);
void Inf_LED_Off(void);
void Inf_LED_Toggle(void);
void Inf_LED_Control(uint8_t mode, uint16_t flashtime_ms);

#endif /* __INF_LED_H__ */
