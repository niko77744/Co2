#ifndef __APP_COM_H__
#define __APP_COM_H__

#include "Inf_TWASRPRO.h"
#include "Inf_W5500.h"
#include "modbusMaster.h"

/* Ҫ���͸�TCPServer������ǰ׺ */
#define PREFIX_RH "RH"
#define PREFIX_T "T"  
#define PREFIX_CO2 "CO2"
/* Ҫ����getTCPServer�����ݵ�λ */
#define SUFFIX_RH "%"
#define SUFFIX_T "DC"
#define SUFFIX_CO2 "ppm"



void App_Com_Init(void);
void App_RS485_GetDiffValue(uint8_t kind, int8_t voice);
void App_ASRPRO_Send(uint8_t* data, uint8_t size);
void App_Tcp_Send(uint8_t* data, uint16_t size);
void App_Tcp_RecvAndSend(void);

#endif /* __APP_COM_H__ */
