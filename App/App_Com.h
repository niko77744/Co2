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
#define SUFFIX_T "��C"
#define SUFFIX_CO2 "ppm"

void App_RS485_GetDiffValue(uint8_t kind, int8_t voice);

#endif /* __APP_COM_H__ */
