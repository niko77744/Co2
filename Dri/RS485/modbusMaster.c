#include "modbusMaster.h"


const unsigned int crc_table[256] = {
    0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
    0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
    0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
    0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
    0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
    0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
    0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
    0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
    0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
    0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
    0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
    0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
    0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
    0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
    0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
    0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
    0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
    0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
    0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
    0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
    0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
    0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
    0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
    0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
    0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
    0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
    0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
    0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
    0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
    0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
    0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
    0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040,
};



/**
 * ����CRC��ѭ������У�飩ֵ�ĺ���
 * ʹ��Ԥ�����crc_table��δ�ڴ˴�����ж��壩������CRCֵ
 *
 * @param inData ָ���������ݵ�ָ�룬��������Ϊuint8_t
 * @param size �������ݵĳ��ȣ���λΪ�ֽ�
 * @return ���ؼ���õ���CRCֵ����������Ϊuint16_t
 */
uint16_t calculate_crc(uint8_t* inData, uint8_t size) {
    // ��ʼ��CRCֵΪ0xFFFF
    uint16_t crc = 0xFFFF;

    // �����������ݵ�ÿһ���ֽ�
    while (size--)
    {
        // ���ݵ�ǰCRCֵ�������ֽڣ�ͨ�����ʽ����CRCֵ
        // ����ʹ����λ�ƺ���������ʵ��CRC�㷨
        crc = (crc >> 8) ^ crc_table[(crc ^ *inData++) & 0xff];
    }

    // �������ռ���õ���CRCֵ
    return crc;
}



uint8_t rxBuff[1000];
uint8_t usart2_flag = 0;        /* usart2�жϽ��ձ�־λ RS485 */
uint16_t data_len = 0;          /* usart2���յ����ݳ��� */

/**
 * @brief ��ʼ��RS485ͨ��
 *
 * ������ͨ������HAL�⺯��HAL_UARTEx_ReceiveToIdle_IT����ʼ��RS485ͨ�š�
 * ������UART�Խ������ݣ����ڿ���״̬�´����жϡ��˳�ʼ��������������ͬ��UART�豸��
 * huart2��huart3���ֱ����ڲ�ͬ��ͨ��Ŀ�ġ�����huart2����ʹ��rxBuff��Ϊ���ջ�������
 * �����ý��ճ���Ϊ1000�ֽڡ�����huart3��ʹ��soundBuff��Ϊ���ջ����������ճ���Ϊ20�ֽڡ�
 *
 * ע: �ú����а�����һ����������51��ʼ����ע�ͣ��������һ�������Ƶĵط�����Ϊ����RS485��ʼ����ֱ����ء�
 */
void Inf_RS485_Init(void) {
    // ��ʼ��RS485ͨ�ţ�����UART2�������ݲ����ÿ����ж�
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, rxBuff, 1000);
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, rxBuff, 1000);
}


/**
 * @brief ����RS485����ģʽ
 *
 * ������ͨ������GPIO����������RS485�ķ���ģʽ���ڽ��뷢��ģʽǰ��
 * ������Ҫһ������ʱ��ȷ�����߿��У��������ݳ�ͻ���˴�ʹ����HAL_Delay����
 * �ṩ��ʱ��ȷ��ϵͳ�ڽ��뷢��ģʽǰ���㹻��ʱ��׼����
 *
 * ע�⣺��ע�͵�����ʱ��������Com_RS485_Delay�������������������Խ׶Σ�
 * ���ܿ��ǹ�ʹ�ø�������ʱ���ƣ������շ���ѡ���˹̶���1000������ʱ��
 * ��Щע����ʾ�˿��������п��ܵĿ����͵�����
 */
void Inf_RS485_SendMode(void) {
    // Com_RS485_Delay(1000);
    /**
     * ����TR_Pin����Ϊ�ߵ�ƽ������RS485����ģʽ
     * TR_GPIO_Port��TR_Pin��Ӳ�����þ����������˿���RS485����/����ģʽ��GPIO����
     */
    HAL_GPIO_WritePin(TR_GPIO_Port, TR_Pin, GPIO_PIN_SET);
    // Com_RS485_Delay(1000);
    /**
     * ��ʱ1000���룬ȷ��RS485�������㹻��ʱ��׼�����뷢��״̬
     * ʹ��HAL_Delay�����ṩ��ʱ������һ���򵥵���ʱʵ�ַ�ʽ��������ϵͳʱ��
     */
    HAL_Delay(1000);
}


/**
 * @brief ����RS485����ģʽ
 *
 * ������ͨ������RS485���շ�ת�����ţ���ͨ��ģʽ����Ϊ����ģʽ
 * �ڽ���ģʽ�£��豸���Լ����ͽ�������RS485���������
 *
 * ע�⣺�˺���û����������ͷ���ֵ
 */
void Inf_RS485_RecMode(void) {
    /**
     * @brief ����TR_Pin�����ý���ģʽ
     *
     * ͨ��HAL�⺯��HAL_GPIO_WritePin��TR_Pin����Ϊ�͵�ƽ���Ӷ�����RS485�Ľ���ģʽ
     * ����Ϊ��ȷ���豸���ڽ���״̬�����Խ������������豸������
     */
    HAL_GPIO_WritePin(TR_GPIO_Port, TR_Pin, GPIO_PIN_RESET);
    /**
     * @brief ��ʱ��ȷ������ģʽ���ȶ�
     *
     * ʹ��HAL_Delay������ʱ1000���룬��ȷ��RS485�����ڽ���ģʽ���ȶ�����
     * �����ʱ��Ϊ�˱�����ģʽת�������г������ݽ��մ���
     */
    HAL_Delay(1000);
}


/**
 * @brief ͨ��RS485�ӿڷ�����������
 *
 * ���������𽫸�������������ͨ��RS485�ӿڷ��ͳ�ȥ���ڷ���֮ǰ������������ݵ�CRCУ��ֵ��
 * �������ֵ���ӵ����������С�Ȼ����������RS485Ϊ����ģʽ��ͨ��UART�ӿڷ������ݣ����
 * �ٽ�RS485����Ϊ����ģʽ��׼�����տ��ܵ���Ӧ�����ݡ�
 *
 * @param data ָ��Ҫ���͵����������ָ��
 * @param size Ҫ���͵���������ĳ���
 */
void Inf_RS485_SendCmd(uint8_t* data, uint8_t size) {
    // �������������CRCУ��ֵ��ע��ֻ����ǰ6���ֽڵ�����
    uint16_t crc = calculate_crc(data, 6);

    // ��CRCУ��ֵ�ĵ��ֽں͸��ֽڷֱ���ӵ���������ĵ�6�͵�7�ֽ�λ��
    *(data + 6) = (crc & 0xff);
    *(data + 7) = (crc >> 8);

    // ����RS485�ӿ�Ϊ����ģʽ
    Inf_RS485_SendMode();

    // ͨ��UART�ӿڷ������ݣ����ó�ʱʱ��Ϊ1000����
    HAL_UART_Transmit(&huart2, data, size, 1000);
    // HAL_Delay(100);

    // ��ӡ���ݷ�����ɵ���Ϣ
    printf("��RS485���ݷ������...\r\n");

    // ����RS485�ӿ�Ϊ����ģʽ
    Inf_RS485_RecMode();
}


/**
 * Inf_RS485_GetValue��������ͨ��RS485�ӿڻ�ȡ����ֵ��
 * �ú�������У����յ������ݵ�CRC��Ȼ����ȡ����ֵ����������ջ��档
 *
 * @return int16_t ���ز���ֵ�����CRCУ��ʧ�ܣ��򷵻�-100��
 */
int16_t Inf_RS485_GetValue(void) {
    // 1��У��CRC
    uint16_t current_crc = rxBuff[data_len - 2] | (rxBuff[data_len - 1] << 8);
    uint16_t crc = calculate_crc(rxBuff, 5);

    int16_t temp = 0;

    if (current_crc != crc) {
        printf("crcУ�����...\r\n");
        return -100;
    }

    // 2����ȡ����
    temp = (rxBuff[3] << 8) | rxBuff[4];

    for (uint8_t i = 0; i < 10; i++) {
        printf("0x%x \r\n", rxBuff[i]);
    }

    // ������ջ���ͳ���
    memset(rxBuff, 0, 100);
    data_len = 0;

    // printf("����ֵ = %d\r\n", temp);
    return temp;
}


/**
 * @brief UART�����¼��ص�����
 *
 * �ú�����UART������յ�����ʱ�����ã����ڴ�����յ�������
 * ������ʹ�õ�USARTʵ�������ֲ�ͬ��������Դ����������Ӧ�Ĵ���
 *
 * @param huart UART���������UART�����������Ϣ
 * @param Size ���յ������ݴ�С
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    /* RS485�����ж� */
    if (huart->Instance == USART2) {
        // ��USART2���յ�����ʱִ�����´���
        printf("USART2�жϴ���...\r\n");
        data_len = Size;
        usart2_flag = 1;
        // �������жϷ�ʽ��������
        HAL_UARTEx_ReceiveToIdle_IT(&huart2, rxBuff, 1000);
    }
}

