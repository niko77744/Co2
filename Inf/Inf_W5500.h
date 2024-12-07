#ifndef __INF_W5500_H__
#define __INF_W5500_H__

#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "usart.h"
#include <string.h>
#include "wizchip_conf.h"
#include "socket.h"
#include "httpServer.h"
#include "httpParser.h"
#include "modbusMaster.h"

void Inf_W5500_Init(void);
uint8_t Inf_W5500_StartTcpClient(void);
void Inf_W5500_StartTcpServer(void);
void Inf_W5500_SendTcpData(uint8_t data[], uint16_t dataLen);
void Inf_W5500_RecvAndSend(void);

#endif /* __INF_W5500_H__ */
