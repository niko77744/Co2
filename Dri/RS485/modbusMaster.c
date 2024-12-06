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
 * 计算CRC（循环冗余校验）值的函数
 * 使用预定义的crc_table（未在此代码段中定义）来计算CRC值
 *
 * @param inData 指向输入数据的指针，数据类型为uint8_t
 * @param size 输入数据的长度，单位为字节
 * @return 返回计算得到的CRC值，数据类型为uint16_t
 */
uint16_t calculate_crc(uint8_t* inData, uint8_t size) {
    // 初始化CRC值为0xFFFF
    uint16_t crc = 0xFFFF;

    // 遍历输入数据的每一个字节
    while (size--)
    {
        // 根据当前CRC值和数据字节，通过查表方式更新CRC值
        // 这里使用了位移和异或操作来实现CRC算法
        crc = (crc >> 8) ^ crc_table[(crc ^ *inData++) & 0xff];
    }

    // 返回最终计算得到的CRC值
    return crc;
}



uint8_t rxBuff[1000];
uint8_t usart2_flag = 0;        /* usart2中断接收标志位 RS485 */
uint16_t data_len = 0;          /* usart2接收的数据长度 */

/**
 * @brief 初始化RS485通信
 *
 * 本函数通过调用HAL库函数HAL_UARTEx_ReceiveToIdle_IT来初始化RS485通信。
 * 它配置UART以接收数据，并在空闲状态下触发中断。此初始化包括了两个不同的UART设备，
 * huart2和huart3，分别用于不同的通信目的。对于huart2，它使用rxBuff作为接收缓冲区，
 * 并配置接收长度为1000字节。对于huart3，使用soundBuff作为接收缓冲区，接收长度为20字节。
 *
 * 注: 该函数中包含了一个关于天问51初始化的注释，这可能是一个待完善的地方，因为它与RS485初始化不直接相关。
 */
void Inf_RS485_Init(void) {
    // 初始化RS485通信，配置UART2接收数据并启用空闲中断
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, rxBuff, 1000);
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, rxBuff, 1000);
}


/**
 * @brief 启用RS485发送模式
 *
 * 本函数通过设置GPIO引脚来启用RS485的发送模式。在进入发送模式前，
 * 可能需要一定的延时来确保总线空闲，避免数据冲突。此处使用了HAL_Delay函数
 * 提供延时，确保系统在进入发送模式前有足够的时间准备。
 *
 * 注意：已注释掉的延时函数调用Com_RS485_Delay表明，在需求分析或测试阶段，
 * 可能考虑过使用更灵活的延时机制，但最终方案选择了固定的1000毫秒延时。
 * 这些注释提示了开发过程中可能的考量和调整。
 */
void Inf_RS485_SendMode(void) {
    // Com_RS485_Delay(1000);
    /**
     * 设置TR_Pin引脚为高电平，启用RS485发送模式
     * TR_GPIO_Port和TR_Pin由硬件配置决定，代表了控制RS485发送/接收模式的GPIO引脚
     */
    HAL_GPIO_WritePin(TR_GPIO_Port, TR_Pin, GPIO_PIN_SET);
    // Com_RS485_Delay(1000);
    /**
     * 延时1000毫秒，确保RS485总线有足够的时间准备进入发送状态
     * 使用HAL_Delay函数提供延时，这是一个简单的延时实现方式，依赖于系统时钟
     */
    HAL_Delay(1000);
}


/**
 * @brief 启用RS485接收模式
 *
 * 本函数通过控制RS485的收发转换引脚，将通信模式设置为接收模式
 * 在接收模式下，设备可以监听和接收来自RS485网络的数据
 *
 * 注意：此函数没有输入参数和返回值
 */
void Inf_RS485_RecMode(void) {
    /**
     * @brief 拉低TR_Pin以启用接收模式
     *
     * 通过HAL库函数HAL_GPIO_WritePin将TR_Pin设置为低电平，从而启用RS485的接收模式
     * 这是为了确保设备处于接收状态，可以接收来自其他设备的数据
     */
    HAL_GPIO_WritePin(TR_GPIO_Port, TR_Pin, GPIO_PIN_RESET);
    /**
     * @brief 延时以确保接收模式的稳定
     *
     * 使用HAL_Delay函数延时1000毫秒，以确保RS485总线在接收模式下稳定工作
     * 这段延时是为了避免在模式转换过程中出现数据接收错误
     */
    HAL_Delay(1000);
}


/**
 * @brief 通过RS485接口发送命令数据
 *
 * 本函数负责将给定的数据数组通过RS485接口发送出去。在发送之前，它会计算数据的CRC校验值，
 * 并将这个值附加到数据数组中。然后，它会设置RS485为发送模式，通过UART接口发送数据，最后
 * 再将RS485设置为接收模式，准备接收可能的响应或数据。
 *
 * @param data 指向要发送的数据数组的指针
 * @param size 要发送的数据数组的长度
 */
void Inf_RS485_SendCmd(uint8_t* data, uint8_t size) {
    // 计算数据数组的CRC校验值，注意只计算前6个字节的数据
    uint16_t crc = calculate_crc(data, 6);

    // 将CRC校验值的低字节和高字节分别添加到数据数组的第6和第7字节位置
    *(data + 6) = (crc & 0xff);
    *(data + 7) = (crc >> 8);

    // 设置RS485接口为发送模式
    Inf_RS485_SendMode();

    // 通过UART接口发送数据，设置超时时间为1000毫秒
    HAL_UART_Transmit(&huart2, data, size, 1000);
    // HAL_Delay(100);

    // 打印数据发送完成的消息
    printf("向RS485数据发送完成...\r\n");

    // 设置RS485接口为接收模式
    Inf_RS485_RecMode();
}


/**
 * Inf_RS485_GetValue函数用于通过RS485接口获取测量值。
 * 该函数首先校验接收到的数据的CRC，然后提取数据值，并清除接收缓存。
 *
 * @return int16_t 返回测量值，如果CRC校验失败，则返回-100。
 */
int16_t Inf_RS485_GetValue(void) {
    // 1、校验CRC
    uint16_t current_crc = rxBuff[data_len - 2] | (rxBuff[data_len - 1] << 8);
    uint16_t crc = calculate_crc(rxBuff, 5);

    int16_t temp = 0;

    if (current_crc != crc) {
        printf("crc校验错误...\r\n");
        return -100;
    }

    // 2、获取数据
    temp = (rxBuff[3] << 8) | rxBuff[4];

    for (uint8_t i = 0; i < 10; i++) {
        printf("0x%x \r\n", rxBuff[i]);
    }

    // 清除接收缓存和长度
    memset(rxBuff, 0, 100);
    data_len = 0;

    // printf("测量值 = %d\r\n", temp);
    return temp;
}


/**
 * @brief UART接收事件回调函数
 *
 * 该函数在UART外设接收到数据时被调用，用于处理接收到的数据
 * 它根据使用的USART实例来区分不同的数据来源，并做出相应的处理
 *
 * @param huart UART句柄，包含UART外设的配置信息
 * @param Size 接收到的数据大小
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    /* RS485触发中断 */
    if (huart->Instance == USART2) {
        // 当USART2接收到数据时执行以下代码
        printf("USART2中断触发...\r\n");
        data_len = Size;
        usart2_flag = 1;
        // 继续以中断方式接收数据
        HAL_UARTEx_ReceiveToIdle_IT(&huart2, rxBuff, 1000);
    }
}

