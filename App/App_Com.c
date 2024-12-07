/*
* @Description: communication ͨ��ģ��
* @Author: niko77744
*/
#include "App_Com.h"


void App_Com_Init(void) {
    Inf_W5500_Init();
    printf("��ʼ����̫���ɹ�...\r\n");
    while (Inf_W5500_StartTcpClient() == 0) {
        HAL_Delay(1000);
    }
    printf("�����ͻ��˳ɹ�....\r\n");
}

uint8_t div_flag = 0;           /* �����Ƿ�Ҫ����10 */
uint8_t kind_oled = 10;         /* ��ʾ��ͬ���ݵĿ�ͷ�͵�λ */

void App_RS485_GetDiffValue(uint8_t kind, int8_t voice) {
    /* Ҫ���͸�TCPSERVER��ǰ׺�ͺ�׺ */
    const char* prefix;
    const char* suffix;

    /* ��ʾ����С����� */
    printf("detection\n");

    /* ׼�����͵����� */
    uint8_t send_data[8] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x01 };

    /* ���ݲ�ͬ��������ò�ͬ�Ĵ����� */
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

    /* ���������Щ485������ */
    Inf_RS485_SendCmd(send_data, 8);

    /* �ж��Ƿ������ж� */
    if (usart2_flag == 1) {
        kind_oled = kind;
        usart2_flag = 0;

        /* ���ú�����ȡֵ */
        int16_t value = Inf_RS485_GetValue();

        /* ��ȡ�����ݵ���CRCУ����� */
        if (value == -100) {
            printf("err crc\n");
            return;
        }

        float res_value = 0;

        /* ʹ��ȫ�ֱ����ж��Ƿ���Ҫ����10 */
        if (div_flag) {
            res_value = value / 10.0;
        }
        else {
            res_value = (float)value;
        }

        /* ���ݴ��� */
        char value_str[10];

        /* ͨ����̫���������� */
        uint8_t sendETH_data[20];
        snprintf((char*)sendETH_data, sizeof(sendETH_data), "%s:%.1f%s", prefix, res_value, suffix);

        sprintf(value_str, "%.1f", res_value); // ��������ת��Ϊ����λС�����ַ���
        printf("result = %s\n", value_str);
    }
}

