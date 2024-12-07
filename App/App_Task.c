#include "App_Task.h"

/* ��ʼ�������� */
#define START_STACK_DEPTH   128
#define START_TASK_PRIORITY 5
TaskHandle_t start_task_handle;
void App_Task_Start(void* pvParameters);

/* ͨ���������� */
#define COMMUNICATION_TASK_STACK_SIZE   128
#define COMMUNICATION_TASK_PRIORITY 3
TaskHandle_t communication_handle;
void App_Task_Communication(void* pvParameters);

/* UI�������� */
#define UI_TASK_STACK_SIZE   128
#define UI_TASK_PRIORITY 3
TaskHandle_t ui_handle;
void App_Task_UI(void* pvParameters);

/* �����ʱ��ɨ�谴�� */
uint8_t timerid_Key = 0;
TimerHandle_t timer_scan10ms;
void timer_scan10ms_cbk(TimerHandle_t xTimer) {
    uint8_t key_code = Inf_Key_Scan(0);
    if (key_code == 1) Select_flag = 1;
    else if (key_code == 2) Select_flag = 3;
    else if (key_code == 3)Select_flag = 4;
}

void App_Task_Init(void) {
    /* ���������ʱ�� */
    timer_scan10ms = xTimerCreate((char*)"ScanKey10ms", 10, pdTRUE, (void*)&timerid_Key, timer_scan10ms_cbk);
    xTimerStart(timer_scan10ms, portMAX_DELAY);
    /* ������ʼ���� */
    xTaskCreate(
        (TaskFunction_t)App_Task_Start,
        (char*)"Start_Task",
        (configSTACK_DEPTH_TYPE)START_STACK_DEPTH,
        (void*)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (TaskHandle_t*)&start_task_handle);
    /* ���������� */
    vTaskStartScheduler();
}


void App_Task_Start(void* pvParameters) {
    /* ͨ���������� */
    xTaskCreate(
        (TaskFunction_t)App_Task_Communication,
        (char*)"App_Task_Communication",
        (configSTACK_DEPTH_TYPE)COMMUNICATION_TASK_STACK_SIZE,
        (void*)NULL,
        (UBaseType_t)COMMUNICATION_TASK_PRIORITY,
        (TaskHandle_t*)&communication_handle);

    /* UI�������� */
    xTaskCreate(
        (TaskFunction_t)App_Task_UI,
        (char*)"App_Task_ui",
        (configSTACK_DEPTH_TYPE)UI_TASK_STACK_SIZE,
        (void*)NULL,
        (UBaseType_t)UI_TASK_PRIORITY,
        (TaskHandle_t*)&ui_handle);

    /* ɾ���Լ� */
    vTaskDelete(NULL);
}

void App_Task_Communication(void* pvParameters) {
    TickType_t last_tick = xTaskGetTickCount();
    while (1) {
        // ͨ��RS485��ȡ �¶ȡ�ʪ�ȡ�Co2Ũ��
        App_RS485_GetDiffValue(GET_TEMP_CMD, IS_VOICE_TEMP);
        App_RS485_GetDiffValue(GET_CO2_CMD, IS_VOICE_CO2);
        App_RS485_GetDiffValue(GET_HUM_CMD, IS_VOICE_HUM);
        // App_Tcp_RecvAndSend();
        vTaskDelayUntil(&last_tick, 1000); // ���ٸ�Tick 1��tickĬ��1ms
    }
}

void App_Task_UI(void* pvParameters) {
    TickType_t last_tick = xTaskGetTickCount();
    while (1) {
        App_Menu_UI();
        Inf_LED_Control((uint8_t)Flash, 2000);
        vTaskDelayUntil(&last_tick, 15);
    }
}
