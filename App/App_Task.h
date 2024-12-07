#ifndef __APP_TASK_H__
#define __APP_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

#include "gpio.h"
#include "App_UI.h"
#include "Inf_Key.h"
#include "Inf_TWASRPRO.h"

void App_Task_Init(void);

#endif /* __APP_TASK_H__ */
