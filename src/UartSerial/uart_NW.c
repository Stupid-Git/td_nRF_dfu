#include "uart_xx.h"


#include "stdio.h"
#include "tchar.h"


int32_t uart_NW_Open(Uart_NW_t* U)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;

    //uint32_t r;    
    COMMTIMEOUTS Timeouts;
    DWORD dwLastError;
    DCB dcb;
    
    dwLastError = 0;
    uart_NW_Close( pU ); //In case we are already open

    //Call CreateFile to open the comms port
    pU->hComm = CreateFile(U->szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(pU->hComm == INVALID_HANDLE_VALUE)
    {
        dwLastError = GetLastError();
        //ATLTRACE(_T("sc__Open, Failed to open the comms port, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        return(dwLastError);
    }

   
    //-------------------------------------------------------------------------
    // IOCTL_SERIAL_SET_QUEUE_SIZE: Set queue size
    /*TODO

    TODO*/

    //-------------------------------------------------------------------------
    //---- IOCTL_SERIAL_GET_TIMEOUTS: Retrieve timeouts ----
    if (!GetCommTimeouts(pU->hComm, &Timeouts))
    {
        dwLastError =  GetLastError();
        //ATLTRACE(_T("GetCommTimeouts, Failed in call to GetCommTimeouts, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        return(dwLastError);
    }

    //---- IOCTL_SERIAL_SET_TIMEOUTS: Set timeouts ----
    memset(&Timeouts, 0, sizeof(Timeouts));
    Timeouts.ReadIntervalTimeout = 0;
    Timeouts.ReadTotalTimeoutMultiplier = 0;
    Timeouts.ReadTotalTimeoutConstant = 1000;
    Timeouts.WriteTotalTimeoutMultiplier = 0;
    Timeouts.WriteTotalTimeoutConstant = 0;

    if ( !SetCommTimeouts(pU->hComm, &Timeouts) )
    {
        dwLastError =  GetLastError();
        //ATLTRACE(_T("SetCommTimeouts, Failed in call to GetCommTimeouts, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        return(dwLastError);
    }

    //-----------------------------------------------------------------------------
    // IOCTL_SERIAL_SET_WAIT_MASK: Set current event mask
    /*TODO

    TODO*/

    //-----------------------------------------------------------------------------
    FillMemory(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(pU->hComm, &dcb))     // get current DCB
    {
        dwLastError =  GetLastError();
        //ATLTRACE(_T("GetCommState, Failed in call to GetCommState, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        return(dwLastError);
    }
    // IOCTL_SERIAL_GET_BAUD_RATE: Retrieve Baud Rate
    //   Baud Rate=9600
    // IOCTL_SERIAL_GET_LINE_CONTROL: Retrieve line control
    //   WordLength=8
    //   StopBits=1 stop bit
    //   Parity=No parity       
    // IOCTL_SERIAL_GET_CHARS: Retrieve special characters
    //   EofChar=0x0
    //   ErrorChar=0x0
    //   BreakChar=0x0
    //   EventChar=0x0
    //   XonChar=0x0
    //   XoffChar=0x0
    // IOCTL_SERIAL_GET_HANDFLOW: Retrieve handshake information
    //   ControlHandShake=0
    //   FlowReplace=0
    //   XonLimit=0
    //   XoffLimit=0

    //-------------------------------------------------------------------------
                               // DWORD DCBlength;      /* sizeof(DCB)                     */

    dcb.BaudRate = CBR_38400;  // DWORD BaudRate;       /* Baudrate at which running       */

    dcb.ByteSize = 8;          //BYTE ByteSize;        /* Number of bits/byte, 4-8        */
    dcb.Parity   = NOPARITY;   //BYTE Parity;          /* 0-4=None,Odd,Even,Mark,Space    */
    dcb.StopBits = ONESTOPBIT; //BYTE StopBits;        /* 0,1,2 = 1, 1.5, 2               */

    dcb.EofChar   = 0x00;    //char EofChar;         /* End of Input character          */
    dcb.ErrorChar = 0x00;    //char ErrorChar;       /* Error replacement char          */
  //dcb.BreakChar = 0x00;
    dcb.EvtChar   = 0x00;    //char EvtChar;         /* Received Event character        */
    dcb.XonChar   = 0x00;    //char XonChar;         /* Tx and Rx X-ON character        */
    dcb.XoffChar  = 0x00;    //char XoffChar;        /* Tx and Rx X-OFF character       */

    //---------------------------------
    //dcb.ControlHandShake = 8;
    //dcb.FlowReplace      = 128;
    //dcb.XonLimit         = 0;
    //dcb.XoffLimit        = 0;
    dcb.XonLim         = 0; //WORD XonLim;          /* Transmit X-ON threshold         */
    dcb.XoffLim        = 0; //WORD XoffLim;         /* Transmit X-OFF threshold        */

    //case CtsDtrFlowControl:    
    /*
    dcb.fOutxCtsFlow = TRUE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
  //dcb.ControlHandShake = 10;
  //dcb.FlowReplace      = 128;
    */
    /**/
    dcb.fOutxCtsFlow = TRUE;
    dcb.fOutxDsrFlow = FALSE;
  //dcb.fDtrControl = DTR_CONTROL_HANDSHAKE; // ->dcb.ControlHandShake = 10;
  dcb.fDtrControl = DTR_CONTROL_ENABLE;    // ->dcb.ControlHandShake = 9;
  //  dcb.fDtrControl = DTR_CONTROL_DISABLE;   // ->dcb.ControlHandShake = ?;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
  //dcb.ControlHandShake = 9;
  //dcb.FlowReplace      = 128;
    /**/

    
    //case CtsRtsFlowControl:
    dcb.fOutxCtsFlow = TRUE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;

        
    dcb.fBinary = FALSE;  //DWORD fBinary: 1;     /* Binary Mode (skip EOF check)    */
    dcb.fParity = TRUE;  //DWORD fParity: 1;     /* Enable parity checking          */
                            //DWORD fOutxCtsFlow:1; /* CTS handshaking on output       */
                            //DWORD fOutxDsrFlow:1; /* DSR handshaking on output       */
                            //DWORD fDtrControl:2;  /* DTR Flow control                */
    dcb.fDsrSensitivity = FALSE;   //DWORD fDsrSensitivity:1; /* DSR Sensitivity              */
    dcb.fTXContinueOnXoff = FALSE; //DWORD fTXContinueOnXoff: 1; /* Continue TX when Xoff sent */
                            //DWORD fOutX: 1;       /* Enable output X-ON/X-OFF        */
                            //DWORD fInX: 1;        /* Enable input X-ON/X-OFF         */
    dcb.fErrorChar    = FALSE; //DWORD fErrorChar: 1;  /* Enable Err Replacement          */
    dcb.fNull         = FALSE; //DWORD fNull: 1;       /* Enable Null stripping           */
   
  //dcb.fRtsControl   = RTS_CONTROL_DISABLE;   //DWORD fRtsControl:2;  /* Rts Flow control                */
  //8,64  dcb.fRtsControl   = RTS_CONTROL_ENABLE;    //DWORD fRtsControl:2;  /* Rts Flow control                */
  dcb.fRtsControl   = RTS_CONTROL_HANDSHAKE; //DWORD fRtsControl:2;  /* Rts Flow control                */
    
    dcb.fAbortOnError = FALSE; //DWORD fAbortOnError:1; /* Abort all reads and writes on Error */
    //DWORD fDummy2:17;     /* Reserved                        */
    //WORD wReserved;       /* Not currently used              */
    //WORD wReserved1;      /* Fill for now.                   */


    // Set new state.
    if (!SetCommState(pU->hComm, &dcb))
    {
        dwLastError =  GetLastError();
        //ATLTRACE(_T("SetCommState, Failed in call to GetCommState, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        return(dwLastError);
        // Error in SetCommState. Possibly a problem with the communications 
        // port handle or a problem with the DCB structure itself.
    }

    //https://msdn.microsoft.com/en-us/library/windows/hardware/ff546574%28v=vs.85%29.aspx
    //IOCTL_SERIAL_SET_HANDFLOW

    //-------------------------------------------------------------------------
    // IOCTL_SERIAL_PURGE: Purge requests
    //   Purge mask=TXABORT: Read requests, RXABORT: Receive buffer, TXCLEAR: Write requests, RXCLEAR: Write buffer
    /*TODO

    TODO*/

    
    if (!EscapeCommFunction(pU->hComm, SETRTS/*dwFunc*/))
    {
        dwLastError = GetLastError();
        //ATLTRACE(_T("sc__Escape, Failed in call to EscapeCommFunction, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        return(dwLastError);
    }

    return(dwLastError);
    return(r);
}

bool uart_NW_IsOpen(Uart_NW_t* U)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;
    
    return( (pU->hComm != INVALID_HANDLE_VALUE) );
}

    
int32_t uart_NW_Close(Uart_NW_t* U)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;
    if( uart_NW_IsOpen(pU) == true)
    {
        //Close down the comms port
        CloseHandle(pU->hComm);
        pU->hComm = INVALID_HANDLE_VALUE;
    }
    return(0);
}


uint32_t uart_NW_Write_A(Uart_NW_t* U, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;
    DWORD dwLastError;
    DWORD dwBytesWritten;
    
    if( uart_NW_IsOpen(pU) == false)
        return(-1);

    dwLastError = 0;
    dwBytesWritten = 0;
    if (!WriteFile(pU->hComm, p_data, dwNumberOfBytesToWrite, &dwBytesWritten, NULL))
    {
        dwLastError = GetLastError();
        printf("uart_NW_Write_A: ERROR: dwLastError = %d, 0x%08x\n", dwLastError, dwLastError);
        dwBytesWritten = 0;
    }

    *p_dwBytesWritten = dwBytesWritten;
    return(dwLastError);
}

uint32_t uart_NW_Read_A(Uart_NW_t* U, uint8_t *p_data, uint32_t dwNumberOfBytesToRead, uint32_t *p_dwBytesRead)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;
    DWORD dwLastError;
    DWORD dwBytesRead;
    //Validate our parameters

    if( uart_NW_IsOpen(pU) == false)
        return(-1);

    dwLastError = 0;
    dwBytesRead = 0;
    if (!ReadFile(pU->hComm, p_data, dwNumberOfBytesToRead, &dwBytesRead, NULL))
    {
        dwLastError = GetLastError();
        printf("serdev_Read: ERROR: dwLastError = %d, 0x%08x\n", dwLastError, dwLastError);
        dwBytesRead = 0;
    }

    *p_dwBytesRead = dwBytesRead;
    return dwLastError;
}

/*
Value	Meaning
MS_CTS_ON  0x0010    The CTS (clear-to-send) signal is on.
MS_DSR_ON  0x0020    The DSR (data-set-ready) signal is on.
MS_RING_ON 0x0040 	 The ring indicator signal is on.
MS_RLSD_ON 0x0080    The RLSD (receive-line-signal-detect) signal is on.
*/

uint32_t uart_NW_GetCommModemStatus(Uart_NW_t* U, uint32_t *p_dwModemStatus)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;
    DWORD dwLastError;
    DWORD dwModemStatus;
    //Validate our parameters

    if( uart_NW_IsOpen(pU) == false)
        return(-1);

    dwLastError = 0;
    dwModemStatus = 0;

    if (!GetCommModemStatus(pU->hComm, &dwModemStatus) )
    {
        dwLastError =  GetLastError();
        //ATLTRACE(_T("sc__GetModemStatus, Failed in call to GetCommModemStatus, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        dwModemStatus = 0;
    }

    *p_dwModemStatus = dwModemStatus;
    return(dwLastError);
}
uint32_t uart_NW_IsCTS_ON(Uart_NW_t* U)
{
    Uart_NW_t* pU = (Uart_NW_t*)U;
    uint32_t r;
    uint32_t dwModemStatus;

    r = uart_NW_GetCommModemStatus(pU, &dwModemStatus);
    if(r != 0)
        return(-1);

    if( (dwModemStatus & MS_CTS_ON) != 0 )
        return(1);
    else
        return(0);

    return(0);
}



int32_t uart_NW_Write(Uart_NW_t* U, uint8_t *packet, uint16_t offset, uint16_t size)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;

    return(r);
}

int32_t uart_NW_Read(Uart_NW_t* U, uint8_t *packet, uint16_t offset, uint16_t size)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;

    return(r);
}




static int32_t dummyHandler(Uart_NW_t* pU, uint8_t *packet, uint16_t size)
{
    return(0);
}

int32_t uart_NW_register_pkt_handler_0x01(Uart_NW_t* U, uart_NW_handler handler)
{
    Uart_NW_t* pU = (Uart_NW_t*)U;
    pU->pkt_handler_0x01 = handler;
    return(0);
}

int32_t uart_NW_register_pkt_handler_T2  (Uart_NW_t* U, uart_NW_handler handler)
{
    Uart_NW_t* pU = (Uart_NW_t*)U;
    pU->pkt_handler_T2 = handler;
    return(0);
}

int32_t uart_NW_register_pkt_handler_SLIP(Uart_NW_t* U, uart_NW_handler handler)
{
    Uart_NW_t* pU = (Uart_NW_t*)U;
    pU->pkt_handler_SLIP = handler;
    return(0);
}

int32_t uart_NW_Init(Uart_NW_t* U, void *config)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;

    return(r);
}
int32_t uart_NW_Init_A(Uart_NW_t* U, char *portName, uint32_t baudRate)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;

    return(r);
}

int32_t uart_NW_Init_N(Uart_NW_t* U, int portNum, uint32_t baudRate)
{
    int r = 0;
    Uart_NW_t* pU = (Uart_NW_t*)U;

    //Form the string version of the port number
    _stprintf_s(pU->szPort, sizeof(pU->szPort)/sizeof(TCHAR), _T("\\\\.\\COM%d"), portNum);
    pU->baudRate = baudRate;

    pU->hComm = INVALID_HANDLE_VALUE;
    pU->pkt_handler_0x01 = dummyHandler;

    return(r);
}

