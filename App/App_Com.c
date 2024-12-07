/*
* @Description: communication 通信模块
* @Author: niko77744
*/
#include "App_Com.h"


void App_Com_Init(void) {
    Inf_W5500_Init();
    printf("初始化以太网成功...\r\n");
    while (Inf_W5500_StartTcpClient() == 0) {
        HAL_Delay(1000);
    }
    printf("开启客户端成功....\r\n");
}

uint8_t div_flag = 0;           /* 数据是否要除以10 */
uint8_t kind_oled = 10;         /* 显示不同数据的开头和单位 */

void App_RS485_GetDiffValue(uint8_t kind, int8_t voice) {
    /* 要发送给TCPSERVER的前缀和后缀 */
    const char* prefix;
    const char* suffix;

    /* 显示检测中。。。 */
    printf("detection\n");

    /* 准备发送的命令 */
    uint8_t send_data[8] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x01 };

    /* 根据不同的命令调用不同的传感器 */
    if (kind == GET_HUM_CMD) {
        div_flag = 1;
        send_data[3] = 0x00;
        prefix = PREFIX_RH;
        suffix = SUFFIX_RH;
    }
    else if (kind == GET_TEMP_CMD) {
        div_flag = 1;
        send_data[3] = 0x01;
        prefix = PREFIX_T;
        suffix = SUFFIX_T;

    }
    else if (kind == GET_CO2_CMD) {
        div_flag = 0;
        send_data[3] = 0x02;
        prefix = PREFIX_CO2;
        suffix = SUFFIX_CO2;
    }

    /* 发送命令到这些485传感器 */
    Inf_RS485_SendCmd(send_data, 8);

    /* 判断是否进入过中断 */
    if (usart2_flag == 1) {
        kind_oled = kind;
        usart2_flag = 0;

        /* 调用函数获取值 */
        int16_t value = Inf_RS485_GetValue();

        /* 获取到数据但是CRC校验错误 */
        if (value == -100) {
            printf("err crc\n");
            return;
        }

        float res_value = 0;

        /* 使用全局变量判断是否结果要除以10 */
        if (div_flag) {
            res_value = value / 10.0;
        }
        else {
            res_value = (float)value;
        }

        /* 数据处理 */
        char value_str[10];

        /* 通过以太网发送数据 */
        uint8_t sendETH_data[20];
        snprintf((char*)sendETH_data, sizeof(sendETH_data), "%s:%.1f%s", prefix, res_value, suffix);

        sprintf(value_str, "%.1f", res_value); // 将浮点数转换为带两位小数的字符串
        printf("result = %s\n", value_str);
    }
}

