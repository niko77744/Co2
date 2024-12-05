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
    /* 3. ��Ϊ�͵�ƽ500us */
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    /* 4. ��λ�ߵ�ƽ */
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
}

void Inf_ETH_SetMac(void)
{
    printf("��ʼ����mac��ַ\n");
    setSHAR(mac);
    HAL_Delay(1);

    printf("����mac��ַ���\n");
}
void Inf_ETH_SetIP(void)
{
    // 1. ����ip��ַ
    printf("��ʼ����ip��ַ\n");
    setSIPR(ip);
    HAL_Delay(1);
    printf("����ip��ַ���\n");
    // 2. ������������
    printf("��ʼ������������\n");
    setSUBR(sub);
    HAL_Delay(1);
    printf("���������������\n");
    // 3. ��������
    printf("��ʼ��������\n");
    setGAR(gw);
    HAL_Delay(1);
    printf("�����������\n");
}

void Inf_ETH_StartTCP_Client(void)
{


    while (1)
    {
        /* 1. ��ȡsocket��״̬ */
        uint8_t sn_sr = getSn_SR(sn);
        /* 2. �ж�socket��״̬ */
        if (sn_sr == SOCK_CLOSED)
        {
            // û����ʹ��
            int8_t socket_r = socket(sn, Sn_MR_TCP, 8080, 0);
            if (socket_r == sn)
            {
                printf("����socket�ɹ�");
            }
            else
            {
                printf("����socketʧ��");
            }
        }
        else if (sn_sr == SOCK_INIT)
        {
            // ����socket��� ��Ҫ���ӷ����
            uint8_t connect_r = connect(sn, serverIP, serverPort);
            if (connect_r == SOCK_OK)
            {
                printf("���ӷ���˳ɹ�");
            }
            else
            {
                close(sn);
                printf("���ӷ����ʧ��");
            }
        }
        else if (sn_sr == SOCK_ESTABLISHED)
        {
            printf("���뵽es״̬");
            send(sn, "hello i'm tcp client", 20);
            break;
        }
    }
}

void Inf_W5500_Init(void)
{
    /* 1. ��ʼ��spi */
    MX_SPI2_Init();
    /* 2. �����û�ע�ắ�� */
    user_register_function();
    /* 3. ����W5500 */
    Inf_ETH_Reset();
    /* 4. ����MAC��ַ */
    Inf_ETH_SetMac();
    /* 5. ����IP��Ϣ */
    Inf_ETH_SetIP();

    HAL_Delay(2000);
    /* 6. �����ͻ��� ���ӷ���� */
    Inf_ETH_StartTCP_Client();
}
