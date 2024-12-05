#include "Inf_W5500.h"

uint8_t mac[6] = { 0x00,0x08,0xdc,0x11,0x11,0x11 };
uint8_t ip[4] = { 192, 168, 23, 104 };
uint8_t sub[4] = { 255, 255, 255, 0 };
uint8_t gw[4] = { 192, 168, 23, 1 };
uint8_t sn = 0;
uint8_t serverIP[4] = { 192, 168, 23, 101 };
uint16_t serverPort = 8080;

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

void Inf_ETH_StartTCP_Client(void)
{


    while (1)
    {
        /* 1. 读取socket的状态 */
        uint8_t sn_sr = getSn_SR(sn);
        /* 2. 判断socket的状态 */
        if (sn_sr == SOCK_CLOSED)
        {
            // 没有人使用
            int8_t socket_r = socket(sn, Sn_MR_TCP, 8080, 0);
            if (socket_r == sn)
            {
                printf("创建socket成功");
            }
            else
            {
                printf("创建socket失败");
            }
        }
        else if (sn_sr == SOCK_INIT)
        {
            // 创建socket完成 需要连接服务端
            uint8_t connect_r = connect(sn, serverIP, serverPort);
            if (connect_r == SOCK_OK)
            {
                printf("连接服务端成功");
            }
            else
            {
                close(sn);
                printf("连接服务端失败");
            }
        }
        else if (sn_sr == SOCK_ESTABLISHED)
        {
            printf("进入到es状态");
            send(sn, "hello i'm tcp client", 20);
            break;
        }
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
    /* 6. 创建客户端 连接服务端 */
    Inf_ETH_StartTCP_Client();
}
