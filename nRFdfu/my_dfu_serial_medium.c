

#include "my_dfu_serial_medium.h"


/*
typedef struct serial_medium_s
{
bool   (*is_open)(void);
int    (*open)(void *transport_config);
int    (*close)(void *transport_config);
int    (*send_packet)(uint8_t packet_type, uint8_t *packet, int size);
void   (*register_packet_handler)(void (*handler)(uint8_t packet_type, uint8_t *packet, uint16_t size));
const char * (*get_transport_name)(void);
// custom extension for UART transport implementations
int    (*set_baudrate)(uint32_t baudrate);
// support async transport layers, e.g. IRQ driven without buffers
int    (*can_send_packet_now)(uint8_t packet_type);

}  serial_medium_t;
*/

static bool null_is_open(void)
{
    return false;//true;
}
static int null_open(void *transport_config)
{
    return 0;
}


static int null_close(void *transport_config)
{
    return 0;
}

static int null_can_send_packet_now(uint8_t packet_type)
{
    return 0;
}

static int null_send_packet(uint8_t packet_type, uint8_t * packet, int size)
{
    return -1;
}

static void null_register_packet_handler(void (*handler)(uint8_t packet_type, uint8_t *packet, uint16_t size))
{
    //log_info("registering packet handler");
    //packet_handler = handler;
}

static const char * null_get_transport_name(void){
    return "NULL";
}

static void dummy_handler(uint8_t packet_type, uint8_t *packet, uint16_t size)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int32_t null_Init_N(void* U/*Uart_NW_t* U*/, int portNum, uint32_t baudRate)
{
    return(0);
}
static int32_t  null_Init_A (void* U/*Uart_NW_t* U*/, char* portName, uint32_t baudRate)
{
    return(0);
}

static     int32_t null_Init   (void* U/*Uart_NW_t* U*/, void* config)
{
    return(0);
}
static     int32_t  null_Open   (void* U/*Uart_NW_t* U*/)
{
    return(0);
}
static     bool    null_IsOpen (void* U/*Uart_NW_t* U*/)
{
    return false;//true;
}
static     int32_t  null_Close  (void* U/*Uart_NW_t* U*/)
{
    return(0);
}



static uint32_t null_Read_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    *p_dwBytesRead = dwNumberOfBytesToRead;
    return(0);
}

static uint32_t null_Write_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    *p_dwBytesWritten = dwNumberOfBytesToWrite;
    return(0);
}

static uint32_t null_Read_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    *p_dwBytesRead = dwNumberOfBytesToRead;
    return(0);
}

static uint32_t null_Write_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    *p_dwBytesWritten = dwNumberOfBytesToWrite;
    return(0);
}


static serial_medium_t* serial_medium_null;
#include "stdlib.h" //malloc

//========== NULL ==========
// get Null singleton
serial_medium_t* serial_medium_Null_instance(void)
{
    if (!serial_medium_null) {
        serial_medium_null = (serial_medium_t*) malloc( sizeof(serial_medium_t));
        serial_medium_null->is_open                       = null_is_open;
        serial_medium_null->open                          = null_open;
        serial_medium_null->close                         = null_close;
        serial_medium_null->send_packet                   = null_send_packet;
        serial_medium_null->register_packet_handler       = null_register_packet_handler;
        serial_medium_null->get_transport_name            = null_get_transport_name;
        serial_medium_null->set_baudrate                  = NULL;
        serial_medium_null->can_send_packet_now           = null_can_send_packet_now;

        serial_medium_null->_Init                         = null_Init;
        serial_medium_null->_Init_A                       = null_Init_A;
        serial_medium_null->_Init_N                       = null_Init_N;

        serial_medium_null->_IsOpen                       = null_IsOpen;
        serial_medium_null->_Open                         = null_Open;
        serial_medium_null->_Close                        = null_Close;
        serial_medium_null->_Read_A_SLIP                  = null_Read_A_SLIP;
        serial_medium_null->_Write_A_SLIP                 = null_Write_A_SLIP;
        serial_medium_null->_Read_A_RAW                   = null_Read_A_RAW;
        serial_medium_null->_Write_A_RAW                  = null_Write_A_RAW;

    }
    return serial_medium_null;
}




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "../src/UartSerial/uart_xx.h"

static Uart_NW_t m_theOneAndOnly_Uart_NW;

Uart_NW_t* __GET_uart_NW(void)
{
    Uart_NW_t* uart = &m_theOneAndOnly_Uart_NW; //TODO
    return( uart );
}

static int32_t NW_Init_N(void* U/*Uart_NW_t* U*/, int portNum, uint32_t baudRate)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Init_N( uart, portNum, baudRate );
    return(r);
}
static int32_t  NW_Init_A (void* U/*Uart_NW_t* U*/, char* portName, uint32_t baudRate)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Init_A( uart, portName, baudRate );
    return(r);
}

static     int32_t NW_Init   (void* U/*Uart_NW_t* U*/, void* config)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Init( uart, config );
    return(r);
}
static  int32_t  NW_Open(void* U/*Uart_NW_t* U*/)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Open( uart );
    return(r);
}
static  bool  NW_IsOpen (void* U/*Uart_NW_t* U*/)
{
    bool r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_IsOpen( uart );
    return(r);
}
static     int32_t  NW_Close  (void* U/*Uart_NW_t* U*/)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Close( uart );
    return(r);
}


static uint32_t NW_Read_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Read_A( uart, p_data, dwNumberOfBytesToRead, p_dwBytesRead);
    return(r);
}

static uint32_t NW_Write_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Write_A( uart, p_data, dwNumberOfBytesToWrite, p_dwBytesWritten);
    return(r);
}

static uint32_t NW_Read_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Read_A( uart, p_data, dwNumberOfBytesToRead, p_dwBytesRead);
    return(r);
}

static uint32_t NW_Write_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_NW();

    r = uart_NW_Write_A( uart, p_data, dwNumberOfBytesToWrite, p_dwBytesWritten);
    return(r);
}

static const char * NW_get_transport_name(void){
    return "NW";
}

static serial_medium_t* serial_medium_NW;

//========== NULL ==========
// get Null singleton
serial_medium_t* serial_medium_NW_instance(void)
{
    Uart_NW_t* uart;

    if (!serial_medium_NW)
    {
        uart = __GET_uart_NW();
        uart->hComm = INVALID_HANDLE_VALUE;

        serial_medium_NW = (serial_medium_t*) malloc( sizeof(serial_medium_t));
        serial_medium_NW->is_open                       = null_is_open;
        serial_medium_NW->open                          = null_open;
        serial_medium_NW->close                         = null_close;
        serial_medium_NW->send_packet                   = null_send_packet;
        serial_medium_NW->register_packet_handler       = null_register_packet_handler;
        serial_medium_NW->get_transport_name            = NW_get_transport_name;
        serial_medium_NW->set_baudrate                  = NULL;
        serial_medium_NW->can_send_packet_now           = null_can_send_packet_now;

        serial_medium_NW->_Init                         = NW_Init;
        serial_medium_NW->_Init_A                       = NW_Init_A;
        serial_medium_NW->_Init_N                       = NW_Init_N;  // NW_Init_N

        serial_medium_NW->_IsOpen                       = NW_IsOpen;
        serial_medium_NW->_Open                         = NW_Open;    // NW_Open
        serial_medium_NW->_Close                        = NW_Close;   // NW_Close
        serial_medium_NW->_Read_A_SLIP                  = NW_Read_A_SLIP;
        serial_medium_NW->_Write_A_SLIP                 = NW_Write_A_SLIP;
        serial_medium_NW->_Read_A_RAW                   = NW_Read_A_RAW;
        serial_medium_NW->_Write_A_RAW                  = NW_Write_A_RAW;

    }
    return serial_medium_NW;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "../src/UartSerial/uart_xx.h"

static Uart_NW_t m_theOneAndOnly_Uart_NWasUW;

Uart_NW_t* __GET_uart_UW(void)
{
    Uart_NW_t* uart = &m_theOneAndOnly_Uart_NWasUW; //TODO
    return( uart );
}

// same as NW
static int32_t UW_Init_N(void* U/*Uart_NW_t* U*/, int portNum, uint32_t baudRate)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Init_N( uart, portNum, baudRate );
    return(r);
}
// same as NW
static int32_t  UW_Init_A (void* U/*Uart_NW_t* U*/, char* portName, uint32_t baudRate)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Init_A( uart, portName, baudRate );
    return(r);
}
// same as NW
static     int32_t UW_Init   (void* U/*Uart_NW_t* U*/, void* config)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Init( uart, config );
    return(r);
}
// same as NW
static  int32_t  UW_Open(void* U/*Uart_NW_t* U*/)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Open( uart );
    return(r);
}
// same as NW
static  bool  UW_IsOpen (void* U/*Uart_NW_t* U*/)
{
    bool r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_IsOpen( uart );
    return(r);
}
// same as NW
static     int32_t  UW_Close  (void* U/*Uart_NW_t* U*/)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Close( uart );
    return(r);
}


static uint32_t UW_Read_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Read_A( uart, p_data, dwNumberOfBytesToRead, p_dwBytesRead);
    return(r);
}

static uint32_t UW_Write_A_SLIP(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    // This is a raw write function
    r = uart_NW_Write_A( uart, p_data, dwNumberOfBytesToWrite, p_dwBytesWritten);
    return(r);
}

static uint32_t UW_Read_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToRead,  uint32_t *p_dwBytesRead)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    r = uart_NW_Read_A( uart, p_data, dwNumberOfBytesToRead, p_dwBytesRead);
    return(r);
}

static uint32_t UW_Write_A_RAW(void* U/*Uart_NW_t* U*/, uint8_t *p_data, uint32_t dwNumberOfBytesToWrite, uint32_t *p_dwBytesWritten)
{
    uint32_t r;
    Uart_NW_t* uart = 0;
    uart = __GET_uart_UW();

    // This is a raw write function
    r = uart_NW_Write_A( uart, p_data, dwNumberOfBytesToWrite, p_dwBytesWritten);
    return(r);
}

static const char * UW_get_transport_name(void){
    return "UW";
}

static serial_medium_t* serial_medium_UW;

//========== NULL ==========
// get Null singleton
serial_medium_t* serial_medium_UW_instance(void)
{
    Uart_NW_t* uart;

    if (!serial_medium_UW)
    {
        uart = __GET_uart_UW();
        uart->hComm = INVALID_HANDLE_VALUE;

        serial_medium_UW = (serial_medium_t*) malloc( sizeof(serial_medium_t));
        serial_medium_UW->is_open                       = null_is_open;
        serial_medium_UW->open                          = null_open;
        serial_medium_UW->close                         = null_close;
        serial_medium_UW->send_packet                   = null_send_packet;
        serial_medium_UW->register_packet_handler       = null_register_packet_handler;
        serial_medium_UW->get_transport_name            = UW_get_transport_name;
        serial_medium_UW->set_baudrate                  = NULL;
        serial_medium_UW->can_send_packet_now           = null_can_send_packet_now;

        serial_medium_UW->_Init                         = UW_Init;
        serial_medium_UW->_Init_A                       = UW_Init_A;
        serial_medium_UW->_Init_N                       = UW_Init_N;

        serial_medium_UW->_IsOpen                       = UW_IsOpen;
        serial_medium_UW->_Open                         = UW_Open;
        serial_medium_UW->_Close                        = UW_Close;
        serial_medium_UW->_Read_A_SLIP                  = UW_Read_A_SLIP;
        serial_medium_UW->_Write_A_SLIP                 = UW_Write_A_SLIP;
        serial_medium_UW->_Read_A_RAW                   = UW_Read_A_RAW;
        serial_medium_UW->_Write_A_RAW                  = UW_Write_A_RAW;

    }
    return serial_medium_UW;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

static uint8_t sData[4096];
static uint8_t rData[4096];


void operation_ban()
{}
void operation_lift_ban()
{}

bool serialPortOpen()
{
    return(false);
}
void serialPort1_D_Close()
{
}
int32_t serialPort1_D_Write(uint8_t* sData, int x, int y)
{
    return(0);
}
void serialPort1_D_DiscardInBuffer()
{}
void serialPort1_D_DiscardOutBuffer()
{}

uint8_t serialPort1_D_ReadByte()
{
return 0;
}

int serialPort1_D_BytesToRead()
{
    return 0;
}

int serialPort1_D_Read(uint8_t *buffer, int offset, int len)
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////

int BitConverter_D_ToUInt16(uint8_t * buffer, int pos)
{
    return(42);
}

void Application_D_DoEvents()
{
}


bool  button1_D_Enabled = true;

void richTextBox1_D_Clear() {}

void richTextBox1_D_AppendText( char * S) {}

void delay(int x) {}

void Buffer_D_BlockCopy(uint8_t *p, int a,uint8_t *pq, int aq, int qqa){}

void button1_D_PerformClick(){}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// 通信コマンド

uint32_t CMD_USB_TEST = 0xe0;									// ＵＳＢ通信試験

uint32_t CMD_nRF_MODE = 0xe6;									// ｎＲＦ５１ブートモード設定
uint32_t CMD_nRF_SLIPWRAP = 0xe7;								// ｎＲＦ５１ＳＬＩＰブロックラップ

uint32_t CMD_CFM_ELASE = 0xe8;									// ＣＰＵフラッシュメモリ消去
uint32_t CMD_CFM_PROGRAM = 0xe9;								// ＣＰＵフラッシュメモリプログラム
uint32_t CMD_CFM_COMPARE = 0xea;								// ＣＰＵフラッシュメモリ比較

uint32_t CMD_SFM_ELASE = 0xeb;									// シリアルフラッシュメモリ消去
uint32_t CMD_SFM_WRITE = 0xec;									// シリアルフラッシュメモリプログラム
uint32_t CMD_SFM_READ = 0xed;									// シリアルフラッシュメモリ比較

uint32_t CMD_CPU_RESET = 0xee;									// ＣＰＵリセット
uint32_t CMD_ID = 0xef;										// パーツＩＤとシリアル番号を送信

// 送信

void sendCommand()
{
    uint16_t i, len, sum;

    serialPort1_D_DiscardInBuffer();
    serialPort1_D_DiscardOutBuffer();

    len = BitConverter_D_ToUInt16(sData, 3);

    for (sum = 0, i = 0; i < len + 5; i++) sum += sData[i];

    sData[len + 5] = (byte)sum;
    sData[len + 6] = (byte)(sum / 256);

    serialPort1_D_Write(sData, 0, len + 7);
}

// 受信
bool recvCommand()
{
    int i, n;

    uint16_t len, sum;


    for (i = 0; i < 5; i++)
    {
        //try
        //{
            rData[i] = (byte)serialPort1_D_ReadByte();
        //}
        //catch (TimeoutException)
        //{
        //    return false;
        //}

        Application_D_DoEvents();
    }

    if (rData[0] != 0x01)
    {
        return false;
    }

    len = (uint16_t)(BitConverter_D_ToUInt16(rData, 3) + 7);

    if (len > 4096) return false;

    i = 0;

    //try
    //{
        while (serialPort1_D_BytesToRead() < (len - 5))         // 予定バイト数が受信されてからＲｅａｄすると早い
        {
            Application_D_DoEvents();
        }

        while (i < len - 5)
        {
            n = serialPort1_D_Read(rData, 5 + i, len - 5 - i);
            if (n > 0) i += (uint16_t)n;

            Application_D_DoEvents();
        }
    //}
    //catch (TimeoutException)
    //{
    //    return false;
    //}

    sum = 0;

    for (i = 0; i < len - 2; i++) sum += rData[i];

    if (sum != BitConverter_D_ToUInt16(rData, len - 2)) return false;

    return true;
}

// ＣＲＣ３２


uint32_t crc32 = 0;

void crc32_add(uint32_t val8)
{
    uint32_t i, poly;
    uint32_t entry;
    uint32_t crc_in;
    uint32_t crc_out;

    crc_in = crc32;

    poly = 0xEDB88320;
    entry = (crc_in ^ ((uint32_t)val8)) & 0xFF;
    for (i = 0; i < 8; i++)
    {
        if ((entry & 0x01) == 0x01)
            entry = (entry >> 1) ^ poly;
        else
            entry >>= 1;
    }
    crc_out = ((crc_in >> 8) & 0x00FFFFFF) ^ entry;
    crc32 = crc_out;
    return;
}

/*
void cOM1シリアルポートToolStripMenuItem_Click(object sender, EventArgs e)
{
    cOM1シリアルポートToolStripMenuItem.Checked = true;
    USB仮想COMToolStripMenuItem.Checked = false;
    toolStripStatusLabel1.Text = "COM1 Port";
    serial_port = 0;
}

void uSBToolStripMenuItem_Click(object sender, EventArgs e)
{
    cOM1シリアルポートToolStripMenuItem.Checked = false;
    USB仮想COMToolStripMenuItem.Checked = true;
    toolStripStatusLabel1.Text = "USB Port";
    serial_port = 1;
}

void USB仮想COMToolStripMenuItem_Click(object sender, EventArgs e)
{
    cOM1シリアルポートToolStripMenuItem.Checked = false;
    USB仮想COMToolStripMenuItem.Checked = true;
    toolStripStatusLabel1.Text = "USB Port";
    serial_port = 1;
}

void button1_Click(object sender, EventArgs e)
{
    if (cancel == 0)
    {
        cancel = 1;
        button1_D_Enabled = false;
    }
}

void エコーバック通信EToolStripMenuItem_Click(object sender, EventArgs e)
{
    uint16_t len, sum;

    uint32_t i;

    int err;


    // 操作禁止
    operation_ban();
    button1_D_Enabled = true;

    richTextBox1_D_Clear();

    if (serialPortOpen() == false)
    {
        richTextBox1_D_AppendText("ポートオープン失敗\r\n");
        goto error_proc;
    }

    // ブレーク信号送出
    sData[0] = 0x00;
    serialPort1_D_Write(sData, 0, 1);
    delay(50);

    for (i = 0; i < 1024; i++) sData[5 + i] = (uint8_t)i;

    sData[0] = 0x01;
    sData[1] = (uint8_t)CMD_USB_TEST;
    sData[2] = 0x00;

    sData[3] = (uint8_t)0xf9;
    sData[4] = (uint8_t)0x03;

    sData[3] = (uint8_t)0x00;
    sData[4] = (uint8_t)0x04;

    len = BitConverter.ToUInt16(sData, 3);

    for (sum = 0, i = 0; i < (len + 5); i++) sum += sData[i];

    sData[len + 5] = (byte)sum;
    sData[len + 6] = (byte)(sum / 256);

    err = 0;
    cancel = 0;

    for (i = 0; i < 1000000; i++)
    {
        if (cancel != 0) break;

        rData[2] = 0x00;

        serialPort1_D_Write(sData, 0, len + 7);

        if ((recvCommand() == false) || (rData[2] != 0x06))
        {
            err++;

            serialPort1_D_Close();
            delay(600);

            if (serialPortOpen() == false) goto error_proc;

            serialPort1_D_DiscardInBuffer();
            serialPort1_D_DiscardOutBuffer();

            // ブレーク信号送出
            sData[0] = 0x00;
            serialPort1_D_Write(sData, 0, 1);
            delay(50);

            sData[0] = 0x01;
        }

        richTextBox1_D_AppendText(String.Format("送信回数 : {0:d}  エラー回数 : {1:d}\r\n", i, err));
        Application_D_DoEvents();
    }

error_proc: ;
    richTextBox1_D_AppendText("終了\r\n");
    serialPort1_D_Close();

    // 操作禁止解除
    operation_lift_ban();
    button1_D_PerformClick();
}
*/

    uint8_t password_user[8] = { (uint8_t)'C', (uint8_t)'r', (uint8_t)'p', (uint8_t)'t', (uint8_t)'0', (uint8_t)'8', (uint8_t)'0', (uint8_t)'1' };   // ユーザーアプリケーション　ＵＳＢコマンドのパスワード（８バイト）

void BootnRFIn_DFU_MODE()//ｎＲＦ５１ブートモード設定BToolStripMenuItem_Click(object sender, EventArgs e)
{
    // 操作禁止
    operation_ban();
    button1_D_Enabled = true;

    richTextBox1_D_Clear();

    if (serialPortOpen() == false)
    {
        richTextBox1_D_AppendText("ポートオープン失敗\r\n");
        goto error_proc;
    }

    // ブレーク信号送出
    sData[0] = 0x00;
    serialPort1_D_Write(sData, 0, 1);
    delay(50);

    sData[0] = 0x01;
    sData[1] = (uint8_t)CMD_nRF_MODE;
    sData[2] = 0x00; //DFU_MODE sub command 0

    sData[3] = (uint8_t)0x08;
    sData[4] = (uint8_t)0x00;

    Buffer_D_BlockCopy(password_user, 0, sData, 5, 8);

    sendCommand();
    if (recvCommand() == false)
    {
        richTextBox1_D_AppendText("受信異常\r\n");
    }

error_proc: ;
    richTextBox1_D_AppendText("終了\r\n");
    serialPort1_D_Close();

    // 操作禁止解除
    operation_lift_ban();
    button1_D_PerformClick();
}

void BootnRFIn_NORMAL_MODE()//ｎＲＦ５１ブートモード解除ToolStripMenuItem_Click(object sender, EventArgs e)
{
    // 操作禁止
    operation_ban();
    button1_D_Enabled = true;

    richTextBox1_D_Clear();

    if (serialPortOpen() == false)
    {
        richTextBox1_D_AppendText("ポートオープン失敗\r\n");
        goto error_proc;
    }

    // ブレーク信号送出
    sData[0] = 0x00;
    serialPort1_D_Write(sData, 0, 1);
    delay(50);

    sData[0] = 0x01;
    sData[1] = (uint8_t)CMD_nRF_MODE;
    sData[2] = 0x01; //NORMAL_MODE sub command 1

    sData[3] = (uint8_t)0x08;
    sData[4] = (uint8_t)0x00;

    Buffer_D_BlockCopy(password_user, 0, sData, 5, 8);

    sendCommand();
    if (recvCommand() == false)
    {
        richTextBox1_D_AppendText("受信異常\r\n");
    }

error_proc: ;
    richTextBox1_D_AppendText("終了\r\n");
    serialPort1_D_Close();

    // 操作禁止解除
    operation_lift_ban();
    button1_D_PerformClick();
}

void SLIP()//ｎＲＦ５１ＳＬＩＰブロックラップToolStripMenuItem_Click(object sender, EventArgs e)
{
    // 操作禁止
    operation_ban();
    button1_D_Enabled = true;

    richTextBox1_D_Clear();

    if (serialPortOpen() == false)
    {
        richTextBox1_D_AppendText("ポートオープン失敗\r\n");
        goto error_proc;
    }

    // ブレーク信号送出
    sData[0] = 0x00;
    serialPort1_D_Write(sData, 0, 1);
    delay(50);

    sData[0] = 0x01;
    sData[1] = (uint8_t)CMD_nRF_SLIPWRAP;
    sData[2] = 0x00;
#if 0//false
    sData[3] = (uint8_t)0x10;
    sData[4] = (uint8_t)0x00;
    //
    sData[5] = (uint8_t)0xc0;            
    sData[6] = (uint8_t)0xc1;
    sData[7] = (uint8_t)0x4e;
    sData[8] = (uint8_t)0x00;
    sData[9] = (uint8_t)0xf1;
    sData[10] = (uint8_t)0x02;
    sData[11] = (uint8_t)0x00;
    sData[12] = (uint8_t)0x00;
    sData[13] = (uint8_t)0x00;
    sData[14] = (uint8_t)0xe8;
    sData[15] = (uint8_t)0x43;
    sData[16] = (uint8_t)0x00;
    sData[17] = (uint8_t)0x00;
    sData[18] = (uint8_t)0xc1;
    sData[19] = (uint8_t)0xa7;
    sData[20] = (uint8_t)0xc0;
#else
    /*
    sData[3] = (uint8_t)0x04;
    sData[4] = (uint8_t)0x00;
    //
    sData[5] = (uint8_t)0xc0;
    sData[6] = (uint8_t)0xc0;
    sData[7] = (uint8_t)0xc0;
    sData[8] = (uint8_t)0xc0;
    */
    /* INIT*/
    sData[3] = (uint8_t)((32 - 5) + 1);
    sData[4] = (uint8_t)0x00;
    //
    sData[ 5] = (uint8_t)0xc0;

    sData[ 6] = (uint8_t)0xd1;
    sData[ 7] = (uint8_t)0x4e;
    sData[ 8] = (uint8_t)0x01;
    sData[ 9] = (uint8_t)0xe0;

    sData[10] = (uint8_t)0x03;            
    sData[11] = (uint8_t)0x0;
    sData[12] = (uint8_t)0x0;
    sData[13] = (uint8_t)0x0;

    sData[14] = (uint8_t)0x04;            
    sData[15] = (uint8_t)0x0;
    sData[16] = (uint8_t)0x0;
    sData[17] = (uint8_t)0x0;

    sData[18] = (uint8_t)0x0;
    sData[19] = (uint8_t)0x0;
    sData[20] = (uint8_t)0x0;
    sData[21] = (uint8_t)0x0;

    sData[22] = (uint8_t)0x0;
    sData[23] = (uint8_t)0x0;
    sData[24] = (uint8_t)0x0;
    sData[25] = (uint8_t)0x0;

    sData[26] = (uint8_t)0xf0;
    sData[27] = (uint8_t)0x51;
    sData[28] = (uint8_t)0x00;
    sData[29] = (uint8_t)0x00;

    sData[30] = (uint8_t)0xd8;
    sData[31] = (uint8_t)0x59;

    sData[32] = (uint8_t)0xc0;

    /*

    ////////////////////////////////////////
    sData[3] = (uint8_t)((32 - 5) + 1);
    sData[4] = (uint8_t)0x00;
    //
    sData[5] = (uint8_t)0xc0;

    sData[6] = (uint8_t)0xda;
    sData[7] = (uint8_t)0x4e;
    sData[8] = (uint8_t)0x01;
    sData[9] = (uint8_t)0xd7;

    sData[10] = (uint8_t)0x01;
    sData[11] = (uint8_t)0x0;
    sData[12] = (uint8_t)0x0;
    sData[13] = (uint8_t)0x0;

    sData[14] = (uint8_t)0x0;
    sData[15] = (uint8_t)0x0;
    sData[16] = (uint8_t)0x0;
    sData[17] = (uint8_t)0x0;

    sData[18] = (uint8_t)0x0;
    sData[19] = (uint8_t)0x0;
    sData[20] = (uint8_t)0x0;
    sData[21] = (uint8_t)0x0;

    sData[22] = (uint8_t)0x01;
    sData[23] = (uint8_t)0x0;
    sData[24] = (uint8_t)0xfe;
    sData[25] = (uint8_t)0xff;

    sData[26] = (uint8_t)0x9e;
    sData[27] = (uint8_t)0x18;
    sData[28] = (uint8_t)0x00;
    sData[29] = (uint8_t)0x00;

    sData[30] = (uint8_t)0x71;
    sData[31] = (uint8_t)0xbd;

    sData[32] = (uint8_t)0xc0;
    */

#endif //false



    sendCommand();
    if (recvCommand() == false)
    {
        richTextBox1_D_AppendText("受信異常\r\n");
    }

error_proc: ;
    richTextBox1_D_AppendText("終了\r\n");
    serialPort1_D_Close();

    // 操作禁止解除
    operation_lift_ban();
    button1_D_PerformClick();
}




















