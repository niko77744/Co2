#include "App_Task.h"

/* 起始任务配置 */
#define START_STACK_DEPTH   128
#define START_TASK_PRIORITY 5
TaskHandle_t start_task_handle;
void App_Task_Start(void* pvParameters);

/* 通信任务配置 */
#define COMMUNICATION_TASK_STACK_SIZE   128
#define COMMUNICATION_TASK_PRIORITY 3
TaskHandle_t communication_handle;
void App_Task_Communication(void* pvParameters);

/* UI任务配置 */
#define UI_TASK_STACK_SIZE   128
#define UI_TASK_PRIORITY 3
TaskHandle_t ui_handle;
void App_Task_UI(void* pvParameters);

/* 软件定时器扫描按键 */
uint8_t timerid_Key = 0;
TimerHandle_t timer_scan10ms;
void timer_scan10ms_cbk(TimerHandle_t xTimer) {
    uint8_t key_code = Inf_Key_Scan(0);
    if (key_code == 1) Select_flag = 1;
    else if (key_code == 2) Select_flag = 3;
    else if (key_code == 3)Select_flag = 4;
}

void App_Task_Init(void) {
    /* 创建软件定时器 */
    timer_scan10ms = xTimerCreate((char*)"ScanKey10ms", 10, pdTRUE, (void*)&timerid_Key, timer_scan10ms_cbk);
    xTimerStart(timer_scan10ms, portMAX_DELAY);
    /* 创建起始任务 */
    xTaskCreate(
        (TaskFunction_t)App_Task_Start,
        (char*)"Start_Task",
        (configSTACK_DEPTH_TYPE)START_STACK_DEPTH,
        (void*)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (TaskHandle_t*)&start_task_handle);
    /* 启动调度器 */
    vTaskStartScheduler();
}


void App_Task_Start(void* pvParameters) {
    /* 通信任务配置 */
    xTaskCreate(
        (TaskFunction_t)App_Task_Communication,
        (char*)"App_Task_Communication",
        (configSTACK_DEPTH_TYPE)COMMUNICATION_TASK_STACK_SIZE,
        (void*)NULL,
        (UBaseType_t)COMMUNICATION_TASK_PRIORITY,
        (TaskHandle_t*)&communication_handle);

    /* UI任务配置 */
    xTaskCreate(
        (TaskFunction_t)App_Task_UI,
        (char*)"App_Task_ui",
        (configSTACK_DEPTH_TYPE)UI_TASK_STACK_SIZE,
        (void*)NULL,
        (UBaseType_t)UI_TASK_PRIORITY,
        (TaskHandle_t*)&ui_handle);

    /* 删除自己 */
    vTaskDelete(NULL);
}

void App_Task_Communication(void* pvParameters) {
    TickType_t last_tick = xTaskGetTickCount();
    while (1) {
        // 通过RS485获取 温度、湿度、Co2浓度
        App_RS485_GetDiffValue(GET_TEMP_CMD, IS_VOICE_TEMP);
        App_RS485_GetDiffValue(GET_CO2_CMD, IS_VOICE_CO2);
        App_RS485_GetDiffValue(GET_HUM_CMD, IS_VOICE_HUM);
        // App_Tcp_RecvAndSend();
        vTaskDelayUntil(&last_tick, 1000); // 多少个Tick 1个tick默认1ms
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
