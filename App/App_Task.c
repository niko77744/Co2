#include "App_Task.h"

extern void Menu_Key_Set(void);
extern int Select_flag;

/* ��ʼ�������� */
#define START_STACK_DEPTH   128
#define START_TASK_PRIORITY 5
TaskHandle_t start_task_handle;
// typedef void (* TaskFunction_t)( void * );
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


void App_Task_Init(void) {
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

    /* ɾ��App_Task_Start�������� */
    vTaskDelete(NULL);
}

void App_Task_Communication(void* pvParameters) {
    TickType_t last_tick = xTaskGetTickCount();
    while (1) {
        TW_ASRPRO_SendData("123", 3);
        vTaskDelayUntil(&last_tick, 5000); // 4��4��Tick����˼ 1��tickĬ��1ms
    }
}

void App_Task_UI(void* pvParameters) {
    TickType_t last_tick = xTaskGetTickCount();
    while (1) {
        Menu_Key_Set();
        uint8_t key_code = Inf_Key_Scan(0);
        if (key_code == 1) {
            Select_flag = 1;
        }
        else if (key_code == 2) {
            Select_flag = 3;
        }
        else if (key_code == 3) {
            Select_flag = 4;
        }
        vTaskDelayUntil(&last_tick, 15);
    }
}
