#include "Inf_W5500.h"

#define SN 0 /* socket �ı�� */
#define OK 1
#define FAIL 0

uint8_t mac[6] = { 0x00,0x08,0xdc,0x11,0x11,0x11 };
uint8_t ip[4] = { 192, 168, 23, 104 };
uint8_t sub[4] = { 255, 255, 255, 0 };
uint8_t gw[4] = { 192, 168, 23, 1 };
uint8_t sn = 0;
uint8_t serverIP[4] = { 192, 168, 23, 101 };
uint16_t serverPort = 8080;
/* �ͻ��� ������Ƿ����ɹ� */
uint8_t is_client_success_flag = 0;
uint8_t is_server_success_flag = 0;

// �������ݵĻ�����
uint8_t rBuff[2048] = { 0 };
int8_t listenStatus;
uint16_t rDataLength;
uint8_t clientIp[4];
uint16_t clientPort;

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

/**
 * ����TCP�ͻ��˵Ĺ��ܺ���
 * �������������������ӵ�ָ���ķ�����
 * �����ȼ�鵱ǰsocket��״̬��Ȼ�����״ִ̬����Ӧ�Ĳ���
 * - ���socket�ѹرգ����Դ�socket
 * - ���socket�ѳ�ʼ�����������ӵ�������
 * - ���socket���ڹرյȴ�״̬������������״̬���ر�socket
 * - ���socket�ѽ������ӣ������óɹ���־�����سɹ�
 *
 * @return uint8_t ���غ���ִ�н����OK��ʾ�ɹ���FAIL��ʾʧ��
 */
uint8_t Inf_W5500_StartTcpClient(void) {
    // ��ȡ��ǰsocket��״̬
    uint8_t status = getSn_SR(SN);

    // ����socket�Ĳ�ͬ״ִ̬����Ӧ�Ĳ���
    if (status == SOCK_CLOSED) { /* ��ʾsocket�Ѿ��ر��� */
        // ���Դ�socket��������ΪTCPģʽ
        uint8_t r = socket(SN, Sn_MR_TCP, 8080, SF_TCP_NODELAY);
        if (r == SN) {
            // printf("socket 0 �򿪳ɹ�\r\n");
        }
        else {
            // printf("socket 0 ��ʧ��\r\n");
        }
    }
    else if (status == SOCK_INIT) { /* ��ʾsocket���Ѿ���, ������tcpģʽ */
        // printf("����...\r\n");
        // �������ӵ�ָ���ķ�����
        int8_t r = connect(SN, serverIP, serverPort);
        if (r == SOCK_OK) {
            printf("�ͻ������ӷ�����%d.%d.%d.%d:%d\r\n", serverIP[0], serverIP[1], serverIP[2], serverIP[3], serverPort);
        }
        else {
            // printf("�ͻ������ӷ�����ʧ��\r\n");
        }
        // printf("�˳�...\r\n");
    }
    else if (status == SOCK_CLOSE_WAIT) {
        // �����ӶϿ�ʱ�����óɹ���־���ر�socket
        is_client_success_flag = 0;
        printf("ʧȥ�����˵�����\r\n");
        close(SN);
    }
    else if (status == SOCK_ESTABLISHED) { /* ����Ѿ��������� */
        // ���óɹ���־�����سɹ�
        is_client_success_flag = 1;
        return OK;
    }
    // ������������������㣬��ʾ����ʧ��
    return FAIL;
}


/**
 * ����W5500 TCP������
 * �ú��������ʼ��socket�������ͻ������󣬲�����ͻ�������
 */
void Inf_W5500_StartTcpServer(void) {
    // 1��socket0�ж�ʹ��
    setSIMR(0);
    // 2����ȡsocket0��״̬
    uint8_t socketStatus = getSn_SR(0);
    switch (socketStatus) {
        // ��ʾsocket 0 �Ѿ��򿪣�����TCP����״̬�����Լ���������
    case SOCK_INIT:
        // ʹ��socket 0 �������Կͻ��˵����� 
        listenStatus = listen(0);
        if (listenStatus == SOCK_OK) {
            printf("socket 0 �����ɹ�...\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKINIT) {
            printf("socket 0 û�г�ʼ��...\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKCLOSED) {
            printf("socket 0 �Ѿ��ر�...\r\n");
        }
        else {
            printf("δ֪����...\r\n");
            is_server_success_flag = 0;
        }
        break;
        // ��ʾ�����Ѿ����������Խ������ݻ�������
    case SOCK_ESTABLISHED:
        // ��ȡ�ͻ���ip�Ͷ˿�
        getSn_DIPR(0, clientIp);
        clientPort = getSn_DPORT(0);
        printf("�ͻ��ˣ�%d.%d.%d.%d:%d �ɹ���������...\r\n",
            clientIp[0],
            clientIp[1],
            clientIp[2],
            clientIp[3],
            clientPort);
        is_server_success_flag = 1;
        break;
        // socket���ڹر�״̬����Դ�Ѿ��ͷţ����������ǿ������´�socket
    case SOCK_CLOSED:
        if (socket(0, Sn_MR_TCP, 8080, SF_TCP_NODELAY) == 0) {
            printf("socket 0 �򿪳ɹ�!...\r\n");
        }
        else {
            printf("socket 0 ��ʧ�ܣ�...\r\n");

        }
        is_server_success_flag = 0;
        break;
    case SOCK_CLOSE_WAIT:
        is_server_success_flag = 0;
        printf("ʧȥ��ͻ��˵�����\r\n");
        close(SN);
        break;
    default:
        break;
    }
}

/**
 * @brief ����TCP���ݵ�Զ�̷�����
 *
 * �ú������ȼ����������ݵĳ��ȣ��������Ϊ0����ֱ�ӷ��أ��������κβ�����
 * ���ţ�����ͨ������`getSn_SR(SN)`����ȡ��ǰ�������ӵ�״̬��
 * �����ǰ״̬Ϊ`SOCK_ESTABLISHED`����TCP�����Ѿ��������������ӡ��ʾ��Ϣ������`send`�������������ݡ�
 * ����ǰ״̬����`SOCK_ESTABLISHED`���������ӡ��ʾ��Ϣ������û�����ӵ�TCP������������޷��������ݡ�
 *
 * @param data Ҫ���͵���������
 * @param dataLen ��������ĳ���
 */
void Inf_W5500_SendTcpData(uint8_t data[], uint16_t dataLen) {
    // ������ݳ��ȣ����Ϊ0��ֱ�ӷ���
    if (dataLen == 0) return;

    // ��ȡ��ǰ�������ӵ�״̬
    uint8_t status = getSn_SR(SN);

    // �жϵ�ǰ״̬�Ƿ�ΪTCP�����Ѿ�����
    if (status == SOCK_ESTABLISHED) {
        // �ѽ������ӣ���������
        printf("�Ѿ��������ӣ���������....\r\n");
        send(SN, data, dataLen);
    }
    else {
        // δ���ӵ�TCP���������޷���������
        printf("û�����ӵ�TCP����ˣ��޷���������...\r\n");
    }
}


/**
 * Inf_W5500_RecvAndSend�������ڴ���W5500��̫��оƬ�Ľ��պͷ������ݹ���
 * �ú�����Ҫ��������ջ������Ƿ������ݣ��������������д���
 * ������̰���������������ݣ����ݽ��յ���ָ�������Ӧ�ĺ���
 * ����ӡ�����Ϣ
 */
void Inf_W5500_RecvAndSend(void) {

    // �ȴ��������� Sn_IR(RECV) �ж����ۺ�ʱ��ֻҪ���յ��Է����ݣ���λ��Ϊ1
    if ((getSn_IR(0) & Sn_IR_RECV) == 1) {
        // �ڵȴ������ڼ䣬socket״̬ != SOCK_ESTABLISHED�����ʾsocket����������
        // ���͹ر���socket 0�����˳�����
        if (getSn_SR(0) != SOCK_ESTABLISHED) {
            printf("socket 0�������⣬�����´� socket 0...\r\n");
            close(0);
            return;
        }
    }

    // ��Sn_IR_RECV ��1����������ж�λ�������´ν���
    setSn_IR(0, getSn_IR(0) & Sn_IR_RECV);

    // ��ȡ�������ݵĳ���
    rDataLength = getSn_RX_RSR(0);

    if (rDataLength > 0) {
        memset(rBuff, 0, sizeof((char*)rBuff));
        // ��������
        recv(0, rBuff, rDataLength);
        printf("���յ������ݳ����ǣ�%d,�����ǣ�0x%x 0x%x 0x%x\r\n", rDataLength, rBuff[0], rBuff[1], rBuff[2]);
        rDataLength = 0;
        if ((rBuff[0] == 0xAA) && (rBuff[1] == 0xBB)) {
            if (rBuff[2] == 0x00) {
                /* ���û�ȡʪ�� */
                // App_RS485_GetDiffValue(0, -1);
                printf("��̫����ȡʪ��...\r\n");
            }
            else if (rBuff[2] == 0x01) {
                /* ���û�ȡ�¶� */
                // App_RS485_GetDiffValue(1, -1);
                printf("��̫����ȡ�¶�...\r\n");
            }
            else if (rBuff[2] == 0x02) {
                /* ���ö�����̼Ũ�� */
                // App_RS485_GetDiffValue(2, -1);
                printf("��̫����ȡ������̼...\r\n");
            }
            else if (rBuff[2] == 0x03) {
                /* ȫ������ */
            }
        }
        // // �����ܵ������ݷ��ظ����ͷ�
        // send(0, rBuff, rDataLength);
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
}
