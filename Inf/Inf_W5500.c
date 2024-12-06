#include "Inf_W5500.h"

#define SN 0 /* socket 的编号 */
#define OK 1
#define FAIL 0

uint8_t mac[6] = { 0x00,0x08,0xdc,0x11,0x11,0x11 };
uint8_t ip[4] = { 192, 168, 23, 104 };
uint8_t sub[4] = { 255, 255, 255, 0 };
uint8_t gw[4] = { 192, 168, 23, 1 };
uint8_t sn = 0;
uint8_t serverIP[4] = { 192, 168, 23, 101 };
uint16_t serverPort = 8080;
/* 客户端 服务端是否开启成功 */
uint8_t is_client_success_flag = 0;
uint8_t is_server_success_flag = 0;

// 接收数据的缓存区
uint8_t rBuff[2048] = { 0 };
int8_t listenStatus;
uint16_t rDataLength;
uint8_t clientIp[4];
uint16_t clientPort;

void Inf_ETH_Reset(void)
{
    /* 3. 置为低电平500us */
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    /* 4. 复位高电平 */
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
}

void Inf_ETH_SetMac(void)
{
    printf("开始设置mac地址\n");
    setSHAR(mac);
    HAL_Delay(1);

    printf("设置mac地址完成\n");
}
void Inf_ETH_SetIP(void)
{
    // 1. 设置ip地址
    printf("开始设置ip地址\n");
    setSIPR(ip);
    HAL_Delay(1);
    printf("设置ip地址完成\n");
    // 2. 设置子网掩码
    printf("开始设置子网掩码\n");
    setSUBR(sub);
    HAL_Delay(1);
    printf("设置子网掩码完成\n");
    // 3. 设置网关
    printf("开始设置网关\n");
    setGAR(gw);
    HAL_Delay(1);
    printf("设置网关完成\n");
}

/**
 * 启动TCP客户端的功能函数
 * 本函数尝试启动并连接到指定的服务器
 * 它首先检查当前socket的状态，然后根据状态执行相应的操作
 * - 如果socket已关闭，则尝试打开socket
 * - 如果socket已初始化，则尝试连接到服务器
 * - 如果socket处于关闭等待状态，则重置连接状态并关闭socket
 * - 如果socket已建立连接，则设置成功标志并返回成功
 *
 * @return uint8_t 返回函数执行结果，OK表示成功，FAIL表示失败
 */
uint8_t Inf_W5500_StartTcpClient(void) {
    // 获取当前socket的状态
    uint8_t status = getSn_SR(SN);

    // 根据socket的不同状态执行相应的操作
    if (status == SOCK_CLOSED) { /* 表示socket已经关闭了 */
        // 尝试打开socket，并设置为TCP模式
        uint8_t r = socket(SN, Sn_MR_TCP, 8080, SF_TCP_NODELAY);
        if (r == SN) {
            // printf("socket 0 打开成功\r\n");
        }
        else {
            // printf("socket 0 打开失败\r\n");
        }
    }
    else if (status == SOCK_INIT) { /* 表示socket的已经打开, 并处于tcp模式 */
        // printf("进入...\r\n");
        // 尝试连接到指定的服务器
        int8_t r = connect(SN, serverIP, serverPort);
        if (r == SOCK_OK) {
            printf("客户端连接服务器%d.%d.%d.%d:%d\r\n", serverIP[0], serverIP[1], serverIP[2], serverIP[3], serverPort);
        }
        else {
            // printf("客户端连接服务器失败\r\n");
        }
        // printf("退出...\r\n");
    }
    else if (status == SOCK_CLOSE_WAIT) {
        // 当连接断开时，重置成功标志并关闭socket
        is_client_success_flag = 0;
        printf("失去与服务端的连接\r\n");
        close(SN);
    }
    else if (status == SOCK_ESTABLISHED) { /* 如果已经建立连接 */
        // 设置成功标志并返回成功
        is_client_success_flag = 1;
        return OK;
    }
    // 如果上述条件都不满足，表示操作失败
    return FAIL;
}


/**
 * 启动W5500 TCP服务器
 * 该函数负责初始化socket，监听客户端请求，并处理客户端连接
 */
void Inf_W5500_StartTcpServer(void) {
    // 1、socket0中断使能
    setSIMR(0);
    // 2、获取socket0的状态
    uint8_t socketStatus = getSn_SR(0);
    switch (socketStatus) {
        // 表示socket 0 已经打开，处于TCP工作状态，可以监听或连接
    case SOCK_INIT:
        // 使用socket 0 监听来自客户端的请求 
        listenStatus = listen(0);
        if (listenStatus == SOCK_OK) {
            printf("socket 0 监听成功...\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKINIT) {
            printf("socket 0 没有初始化...\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKCLOSED) {
            printf("socket 0 已经关闭...\r\n");
        }
        else {
            printf("未知错误...\r\n");
            is_server_success_flag = 0;
        }
        break;
        // 表示连接已经建立，可以接受数据或发送数据
    case SOCK_ESTABLISHED:
        // 获取客户端ip和端口
        getSn_DIPR(0, clientIp);
        clientPort = getSn_DPORT(0);
        printf("客户端：%d.%d.%d.%d:%d 成功建立连接...\r\n",
            clientIp[0],
            clientIp[1],
            clientIp[2],
            clientIp[3],
            clientPort);
        is_server_success_flag = 1;
        break;
        // socket处于关闭状态，资源已经释放，在这里我们可以重新打开socket
    case SOCK_CLOSED:
        if (socket(0, Sn_MR_TCP, 8080, SF_TCP_NODELAY) == 0) {
            printf("socket 0 打开成功!...\r\n");
        }
        else {
            printf("socket 0 打开失败！...\r\n");

        }
        is_server_success_flag = 0;
        break;
    case SOCK_CLOSE_WAIT:
        is_server_success_flag = 0;
        printf("失去与客户端的连接\r\n");
        close(SN);
        break;
    default:
        break;
    }
}

/**
 * @brief 发送TCP数据到远程服务器
 *
 * 该函数首先检查待发送数据的长度，如果长度为0，则直接返回，不进行任何操作。
 * 接着，函数通过调用`getSn_SR(SN)`来获取当前网络连接的状态。
 * 如果当前状态为`SOCK_ESTABLISHED`，即TCP连接已经建立，则函数会打印提示信息并调用`send`函数来发送数据。
 * 若当前状态不是`SOCK_ESTABLISHED`，则函数会打印提示信息，表明没有连接到TCP服务器，因此无法发送数据。
 *
 * @param data 要发送的数据数组
 * @param dataLen 数据数组的长度
 */
void Inf_W5500_SendTcpData(uint8_t data[], uint16_t dataLen) {
    // 检查数据长度，如果为0则直接返回
    if (dataLen == 0) return;

    // 获取当前网络连接的状态
    uint8_t status = getSn_SR(SN);

    // 判断当前状态是否为TCP连接已经建立
    if (status == SOCK_ESTABLISHED) {
        // 已建立连接，发送数据
        printf("已经建立连接，发送数据....\r\n");
        send(SN, data, dataLen);
    }
    else {
        // 未连接到TCP服务器，无法发送数据
        printf("没有连接到TCP服务端，无法发送数据...\r\n");
    }
}


/**
 * Inf_W5500_RecvAndSend函数用于处理W5500以太网芯片的接收和发送数据过程
 * 该函数主要负责检查接收缓冲区是否有数据，如果有数据则进行处理
 * 处理过程包括：检查数据内容，根据接收到的指令调用相应的函数
 * 并打印相关信息
 */
void Inf_W5500_RecvAndSend(void) {

    // 等待接收数据 Sn_IR(RECV) 中断无论何时，只要接收到对方数据，此位置为1
    if ((getSn_IR(0) & Sn_IR_RECV) == 1) {
        // 在等待链接期间，socket状态 != SOCK_ESTABLISHED，则表示socket发生了意外
        // ，就关闭了socket 0，并退出函数
        if (getSn_SR(0) != SOCK_ESTABLISHED) {
            printf("socket 0发送意外，将重新打开 socket 0...\r\n");
            close(0);
            return;
        }
    }

    // 把Sn_IR_RECV 置1，清除接收中断位，用于下次接收
    setSn_IR(0, getSn_IR(0) & Sn_IR_RECV);

    // 获取就收数据的长度
    rDataLength = getSn_RX_RSR(0);

    if (rDataLength > 0) {
        memset(rBuff, 0, sizeof((char*)rBuff));
        // 接受数据
        recv(0, rBuff, rDataLength);
        printf("接收到的数据长度是：%d,内容是：0x%x 0x%x 0x%x\r\n", rDataLength, rBuff[0], rBuff[1], rBuff[2]);
        rDataLength = 0;
        if ((rBuff[0] == 0xAA) && (rBuff[1] == 0xBB)) {
            if (rBuff[2] == 0x00) {
                /* 调用获取湿度 */
                // App_RS485_GetDiffValue(0, -1);
                printf("以太网获取湿度...\r\n");
            }
            else if (rBuff[2] == 0x01) {
                /* 调用获取温度 */
                // App_RS485_GetDiffValue(1, -1);
                printf("以太网获取温度...\r\n");
            }
            else if (rBuff[2] == 0x02) {
                /* 调用二氧化碳浓度 */
                // App_RS485_GetDiffValue(2, -1);
                printf("以太网获取二氧化碳...\r\n");
            }
            else if (rBuff[2] == 0x03) {
                /* 全部调用 */
            }
        }
        // // 将接受到的数据返回给发送方
        // send(0, rBuff, rDataLength);
    }
}


void Inf_W5500_Init(void)
{
    /* 1. 初始化spi */
    MX_SPI2_Init();
    /* 2. 调用用户注册函数 */
    user_register_function();
    /* 3. 重置W5500 */
    Inf_ETH_Reset();
    /* 4. 设置MAC地址 */
    Inf_ETH_SetMac();
    /* 5. 设置IP信息 */
    Inf_ETH_SetIP();

    HAL_Delay(2000);
}
