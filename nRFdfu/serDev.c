
#include "serDev.h"

#include "stdio.h"

bool serDev__IsOpen(serdev_t *uart)
{
    return( (uart->hComm != INVALID_HANDLE_VALUE) );
}

uint32_t serdev_Open_A(serdev_t *uart, char *portName, uint32_t baudRate)
{
    uint32_t r;
    
    r = 0;//TODO _serdev_Open(uart->pserC, portName, baudRate, NoParity, 8, OneStopBit, CtsRtsFlowControl, FALSE);

    return(r);
}



uint32_t serdev_Open_n(serdev_t *uart, int nPort, uint32_t baudRate)
{
    //uint32_t r;    
    COMMTIMEOUTS Timeouts;
    DWORD dwLastError;
    DCB dcb;
    
    //Form the string version of the port number
    TCHAR szPort[12];
    _stprintf_s(szPort, sizeof(szPort)/sizeof(TCHAR), _T("\\\\.\\COM%d"), nPort);

    dwLastError = 0;
    serdev_Close(uart); //In case we are already open

    //Call CreateFile to open the comms port
    uart->hComm = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(uart->hComm == INVALID_HANDLE_VALUE)
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
    if (!GetCommTimeouts(uart->hComm, &Timeouts))
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

    if ( !SetCommTimeouts(uart->hComm, &Timeouts) )
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
    if (!GetCommState(uart->hComm, &dcb))     // get current DCB
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
    if (!SetCommState(uart->hComm, &dcb))
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

    
    if (!EscapeCommFunction(uart->hComm, SETRTS/*dwFunc*/))
    {
        dwLastError = GetLastError();
        //ATLTRACE(_T("sc__Escape, Failed in call to EscapeCommFunction, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        return(dwLastError);
    }

    return(dwLastError);
}


uint32_t serdev_Close(serdev_t *uart)
{
    if( serDev__IsOpen(uart) == true)
    {
        //Close down the comms port
        CloseHandle(uart->hComm);
        uart->hComm = INVALID_HANDLE_VALUE;
    }
    return(0);
}


uint32_t serdev_Write(serdev_t *uart, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    DWORD dwLastError;
    DWORD dwBytesWritten;
    
    if( serDev__IsOpen(uart) == false)
        return(-1);

    dwLastError = 0;
    dwBytesWritten = 0;
    if (!WriteFile(uart->hComm, p_data, dwNumberOfBytesToWrite, &dwBytesWritten, NULL))
    {
        dwLastError = GetLastError();
        printf("serdev_Write: ERROR: dwLastError = %d, 0x%08x\n", dwLastError, dwLastError);
        dwBytesWritten = 0;
    }

    *p_dwBytesWritten = dwBytesWritten;
    return(dwLastError);
}

uint32_t serdev_Read(serdev_t *uart, uint8_t *p_data, uint32_t dwNumberOfBytesToRead, uint32_t *p_dwBytesRead)
{
    DWORD dwLastError;
    DWORD dwBytesRead;
    //Validate our parameters

    if( serDev__IsOpen(uart) == false)
        return(-1);

    dwLastError = 0;
    dwBytesRead = 0;
    if (!ReadFile(uart->hComm, p_data, dwNumberOfBytesToRead, &dwBytesRead, NULL))
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

uint32_t serdev_GetCommModemStatus(serdev_t *uart, uint32_t *p_dwModemStatus)
{
    DWORD dwLastError;
    DWORD dwModemStatus;
    //Validate our parameters

    if( serDev__IsOpen(uart) == false)
        return(-1);

    dwLastError = 0;
    dwModemStatus = 0;

    if (!GetCommModemStatus(uart->hComm, &dwModemStatus) )
    {
        dwLastError =  GetLastError();
        //ATLTRACE(_T("sc__GetModemStatus, Failed in call to GetCommModemStatus, Error:%u\n"), dwLastError);
        //ThrowSerialException(dwLastError);
        dwModemStatus = 0;
    }

    *p_dwModemStatus = dwModemStatus;
    return(dwLastError);
}

int serdev_IsCTS_ON(serdev_t *uart)
{
    uint32_t r;
    uint32_t dwModemStatus;

    r = serdev_GetCommModemStatus(uart, &dwModemStatus);
    if(r != 0)
        return(-1);

    if( (dwModemStatus & MS_CTS_ON) != 0 )
        return(1);
    else
        return(0);

    return(0);
}