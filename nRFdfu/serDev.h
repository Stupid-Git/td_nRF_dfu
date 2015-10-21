#ifndef __SERDEV_H
#define __SERDEV_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <windows.h>
#include <tchar.h>

#include "stdint.h"
#include "../stdbool.h"

//#include "../maikon1/serC.h"

struct _SerDev
{
    char* portName;

//    sc_t *pserC;

    HANDLE hComm;

};

typedef struct _SerDev serdev_t;

uint32_t serdev_Open_A (serdev_t *uart, char *portName, uint32_t baudRate);

uint32_t serdev_Open_n (serdev_t *uart, int portNum, uint32_t baudRate);
uint32_t serdev_OpenNew_n (serdev_t *uart, int portNum, uint32_t baudRate);

uint32_t serdev_Write  (serdev_t *uart, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten);
uint32_t serdev_Read   (serdev_t *uart, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead);
uint32_t serdev_Close  (serdev_t *uart);

int serdev_IsCTS_ON(serdev_t *uart);

#ifdef __cplusplus
}
#endif

#endif // __SERDEV_H


