
#include "stdint.h"
#include "stdio.h"
#include "../src/stdbool.h"

#include "../src/myutils.h"

#include "W_slip.h"
#include "my_dfu_transport.h"


#define DEFAULT_SERIAL_PORT_TIMEOUT  1.0  //# Timeout time on serial port read
#define ACK_PACKET_TIMEOUT           1.0  //# Timeout time for for ACK packet received before reporting timeout through event system
#define SEND_INIT_PACKET_WAIT_TIME   1.0  //# Time to wait before communicating with bootloader after init packet is sent
#define SEND_START_DFU_WAIT_TIME     10.0 //# Time to wait before communicating with bootloader after start DFU packet is sent
#define DFU_PACKET_MAX_SIZE          512  //# The DFU packet max size


static DfuTrans_Serial_t m_TheOneAndOnlyInstance_DfuTrans_Serial;

void init_Dfu_Serial(DfuTrans_Serial_t *gp);


//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t serT_register_events_callback(DfuTransport_t *P, int event_type, xxx_event_handler_del callback)
{
    /*
    Register a callback.

    :param DfuEvent callback:
    :return: None
    """
    if event_type not in self.callbacks:
    self.callbacks[event_type] = []

    self.callbacks[event_type].append(callback)
    */
    return(0); //   pass
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DfuTransport_t* DfuTransportSerial(serial_medium_t* medium, char *com_port, int baud_rate, bool flow_control)
{
    DfuTrans_Serial_t *pTS;
    
    float timeout;
    timeout = DEFAULT_SERIAL_PORT_TIMEOUT;
    
    printf("DfuTransportSerial()\n");

    pTS = &m_TheOneAndOnlyInstance_DfuTrans_Serial;
    
    init_Dfu_Serial(pTS);

    pTS->com_port = com_port;
    pTS->baud_rate = baud_rate;
    if (flow_control == true)
        pTS->flow_control = 1;
    else
        pTS->flow_control = 0;
    
    pTS->timeout = timeout;

    pTS->serial_medium = medium;

    return(&pTS->DfuTransport); //???
}


/*
class DfuTransportSerial(DfuTransport):
    def __init__(self, com_port, baud_rate=38400, flow_control=False, timeout=DEFAULT_SERIAL_PORT_TIMEOUT):
        super(DfuTransportSerial, self).__init__()
        self.com_port = com_port
        self.baud_rate = baud_rate
        self.flow_control = 1 if flow_control else 0
        self.timeout = timeout
        self.serial_port = None
        """:type: serial.Serial """
*/

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t OLD_serT_open(DfuTransport_t *pT)
{
    int32_t r;

    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)pT;

    printf("DfuTransportSerial->open()\n");

    r = self->serial_medium->open(0);

    return(r);
}

#include "../src/UartSerial/uart_xx.h"
int32_t serT_open(DfuTransport_t *pT)
{
    int32_t r;

    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)pT;

    printf("DfuTransportSerial->open()\n");

    //int32_t uartRet;
    //Uart_NW_t  uart0;
    //Uart_NW_t* p_uart;
  //int32_t  Controller_com_port_num = 56;   //NW board serial_medium_NW_instance    COM56 nRF PROTO
    int32_t  Controller_com_port_num = 61;   //UW board serial_medium_UW_instance    COM61 RTR500BLE
    uint32_t m_Controller_baud_rate = 500000; //38400;
    
    //p_uart = &uart0;
    //p_uart->hComm = INVALID_HANDLE_VALUE;//(HANDLE)0xFFFFFFFF;

    //r = self->serial_medium->uart_NW_Init_N( p_uart, Controller_com_port_num, m_Controller_baud_rate);//, rtscts=Controller_flow_control, timeout=1);
    r = self->serial_medium->_Init_N( 0/* p_uart*/, Controller_com_port_num, m_Controller_baud_rate);//, rtscts=Controller_flow_control, timeout=1);

    //r = self->serial_medium->uart_NW_Open( p_uart );
    r = self->serial_medium->_Open( 0/*p_uart*/ );
    if( r != 0 )
    {
        //printf("uart_NW_Open could not be opened on %s\n", gx_Controller_com_port);
        return(-1); //??
    }


    //OLD r = self->serial_medium->open(0);

    return(r);
}

/*
    def open(self):
        super(DfuTransportSerial, self).open()

        try:
            self.serial_port = Serial(port=self.com_port, baudrate=self.baud_rate, rtscts=self.flow_control, timeout=self.timeout)
        except Exception, e:
            raise NordicSemiException("Serial port could not be opened on {0}. Reason: {1}".format(self.com_port, e.message))
*/


int32_t serT_close(DfuTransport_t *pT)
{
    int32_t r;

    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)pT;

    printf("DfuTransportSerial->close()\n");

    //r = self->serial_medium->close(0);
    r = self->serial_medium->_Close(0);

    return(r);
}
/*
    def close(self):
        super(DfuTransportSerial, self).close()
        self.serial_port.close()
*/

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool serT_is_open(DfuTransport_t *pT)
{
    bool bRet;

    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)pT;

    printf("DfuTransportSerial->is_open()\n");
 
  //bRet = self->serial_medium->is_open();
    bRet = self->serial_medium->_IsOpen(0);

    return( bRet );

    /*
    //super(DfuTransportSerial, self).is_open()
    if( pSelf->serial_port == 0)
        return false;

    return( pSelf->serial_port->isOpen(&pSelf->spInfo));
    */
}
/*
    def is_open(self):
        super(DfuTransportSerial, self).is_open()

        if self.serial_port is None:
            return False

        return self.serial_port.isOpen()
*/



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static buf32_t* buf32_decode_esc_chars(buf32_t * data)
{

    //'''Replace 0xDBDC with 0xCO and 0xDBDD with 0xDB'''
    uint32_t _len;
    uint8_t _char;
    uint8_t _char2;
    static uint8_t resultbuf[1000];
    static buf32_t result32;
    buf32_t *result;
    uint8_t _char_C0;
    uint8_t _char_DB;
    uint32_t pos;

    _char_C0 = 0xC0;
    _char_DB = 0xDB;

    result32.buffer = resultbuf;
    result = &result32;
    buf32_zero(result);

    pos = 0;
    _len = data->length;
    while( pos < _len)
    {
        _char = data->buffer[pos++]; //data.pop(0);
        if(_char == 0xDB)
        {
            _char2 = data->buffer[pos++]; //data.pop(0);
            if(_char2 == 0xDC)
                buf32_cpy(result, &_char_C0, 1);  //result.append(0xC0);
            else
                if(_char2 == 0xDD)
                    buf32_cpy(result, &_char_DB, 1);  //result.append(0xDB);
                else
                {
                    //raise Exception('Char 0xDB NOT followed by 0xDC or 0xDD');
                    printf("buf32_decode_esc_chars: Exception: Char 0xDB NOT followed by 0xDC or 0xDD \n");
                    return(0);
                }
        }
        else
        {
            buf32_cpy(result, &_char, 1);  //result.append(_char);
        }
    }
    return result;
    /*
    def decode_esc_chars(data):
        '''Replace 0xDBDC with 0xCO and 0xDBDD with 0xDB'''
        result = []
        while len(data):
            char = data.pop(0)
            if char == 0xDB:
                char2 = data.pop(0)
                if char2 == 0xDC:
                    result.append(0xC0)
                elif char2 == 0xDD:
                    result.append(0xDB)
                else:
                    raise Exception('Char 0xDB NOT followed by 0xDC or 0xDD')
            else:
                result.append(char)
        return result
    */ 
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int  L_is_timeout(double start_time, double timeout_sec)
{
    //return not (datetime.now() - start_time <= timedelta(0, timeout_sec))
    int rtn;

    rtn = 42;
/*TODO
    if( (datetime.now() - start_time) <= timedelta(0, timeout_sec) ) 
        rtn = 0;
    else
        rtn = 1;
*/
    return(rtn);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int get_ack_nr(DfuTransport_t *P)
{
    DfuTrans_Serial_t* self;
    self = (DfuTrans_Serial_t*)P;

    int       toCount;
    uint32_t  r_serDev;

    uint8_t   uart_buffer_buffer[600];
    buf32_t   uart_buffer;

    uint8_t   temp_buf[600];
    buf32_t   temp;

    buf32_t*  data;

    //start = datetime.now()

    temp.buffer = temp_buf;

    uart_buffer.buffer = uart_buffer_buffer;
    buf32_zero(&uart_buffer);

    toCount = 0;
    while(buf32_count(&uart_buffer, 0xC0) < 2)
    {
        buf32_zero(&temp);
        //Disregard first of the two C0
      //r_serDev = serdev_Read                 (uart, temp->buffer, 1, &temp.len); //temp = uart.read(1);
      //r_serDev = uart_NW_Read_A              (uart, temp.buffer, 1, &temp.length); //temp = uart.read(1);
        r_serDev = self->serial_medium->_Read_A_SLIP(0,    temp.buffer, 1, &temp.length); //temp = uart.read(1);
        if( r_serDev == 0)
        {
            if(temp.length != 0)
            {
                toCount = 0;
                buf32_join(&uart_buffer, &temp); //uart_buffer.append(ord(temp));
            }
            else
            {
                toCount++;
                if(toCount == 10)
                {
                    printf("Controller_get_ack_nr: timeout\n");
                    return(0xFF);
                }
            }
        /*
        if( L_is_timeout(start, 5))
        {
            // reset HciPacket numbering back to 0
            HciPacket.sequence_number = 0;

            // call timeout callback from parent layer
            self.timeout_callback();

            // quit loop
            break;
        }
        */
        }
        else
        {
            printf("Controller_get_ack_nr: serdev_Read: ERROR = %d, 0x%08x\n", r_serDev, r_serDev);
            return(0xFF);
            //break;
        }
    }

    //read until you get a new C0
    //RESUME_WORK
    buf32_print("\nController_get_ack_nr", &uart_buffer);
    data = buf32_decode_esc_chars(&uart_buffer);

    /*
    // Remove 0xC0 at start and beginning
    data = data[1:-1];
    // print "non-slip data ", [data]
    return (data[0] >> 3) & 0x07;
    */

    // print "non-slip data ", [data]
    return( (data->buffer[1] >> 3) & 0x07);

/*TODO
    //uart_buffer = []
    //start = datetime.now()
    while(uart_buffer.count(0xC0) < 2)
    {
        //Disregard first of the two C0
        temp = uart.read(1);
        if( temp)
            uart_buffer.append(ord(temp));

        if( L_is_timeout(start, 5))
        {
            // reset HciPacket numbering back to 0
            HciPacket.sequence_number = 0;

            // call timeout callback from parent layer
            self.timeout_callback();

            // quit loop
            break
        }
    }
    //read until you get a new C0
    //RESUME_WORK
    data = decode_esc_chars(uart_buffer);

    // Remove 0xC0 at start and beginning
    data = data[1:-1];

    // print "non-slip data ", [data]
    return (data[0] >> 3) & 0x07;
*/
}
/*
    def get_ack_nr(self):
        def is_timeout(start_time, timeout_sec):
            return not (datetime.now() - start_time <= timedelta(0, timeout_sec))

        uart_buffer = ''
        start = datetime.now()

        while uart_buffer.count('\xC0') < 2:
            # Disregard first of the two C0
            temp = self.serial_port.read(6)

            if temp:
                uart_buffer += temp

            if is_timeout(start, ACK_PACKET_TIMEOUT):
                # reset HciPacket numbering back to 0
                HciPacket.sequence_number = 0
                self._send_event(DfuEvent.TIMEOUT_EVENT,
                                 log_message="Timed out waiting for acknowledgement from device.")

                # quit loop
                break

                # read until you get a new C0
                # RESUME_WORK

        if len(uart_buffer) < 2:
            raise NordicSemiException("No data received on serial port. Not able to proceed.")

        logger.debug("PC <- target: {0}".format(binascii.hexlify(uart_buffer)))
        data = self.decode_esc_chars(uart_buffer)

        # Remove 0xC0 at start and beginning
        data = data[1:-1]

        # Extract ACK number from header
        return (data[0] >> 3) & 0x07
*/


static int  last_ack = 42;//None==42

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int send_packet_NW(DfuTransport_t *P, buf32_t *pkt)
{
    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)P;
   
    //logger.debug("PC -> target: {0}".format(pkt))
    
    uint32_t r_serDev;
    uint32_t dwBytesWritten;

    int  attempts = 0;
    int  ack = 0;//None
    //int  last_ack = 0;//None
    bool packet_sent = false;

    while( packet_sent == false)
////////
    {
        //r_serDev =                   serdev_Write(uart, pkt->data,   pkt->len,    &dwBytesWritten);
        //r_serDev =                uart_NW_Write_A(uart, pkt->buffer, pkt->length, &dwBytesWritten);
        r_serDev = self->serial_medium->_Write_A_SLIP   (0,    pkt->buffer, pkt->length, &dwBytesWritten);
        //r_serDev = self->serial_medium->send_packet(0,  pkt->buffer, pkt->length);
        if( r_serDev != 0 )
        {
            printf("Controller_send_packet: serdev_Write: ERROR = %d, 0x%08x\n", r_serDev, r_serDev);
            return(-1);
        }
        if( pkt->length != dwBytesWritten )
        {
            printf("Controller_send_packet: serdev_Write: pkt->len[%d] != dwBytesWritten[%d]\n", pkt->length, dwBytesWritten);
            return(-1);
        }

        attempts += 1;
        ack = get_ack_nr(P);
        if( ack == 0xFF)
        {
            printf("Controller_send_packet: Controller_get_ack_nr: ERROR\n");
            return(-1);
        }


        //orig if( last_ack == 0)//is None:
        //orig     break;
        if(last_ack == 42) //None
        {
            printf("################################################################\n");
            printf("#####  FIRST ACK ###############################################\n");
            printf("################################################################\n");
            last_ack = ack;
            break;
        }

        if( ack == (last_ack + 1) % 8 )
        {
            last_ack = ack;
            packet_sent = true;

            if (attempts > 3)
            {
                //raise Exception("Three failed tx attempts encountered on packet {0}".format(pkt.sequence_number))
                //raise Exception("Three failed tx attempts encountered on packet {0}".format(seq))
                printf("Three failed tx attempts encountered on packet %d\n", -1 );//.format(seq))
                return(-1);
            }
        }
        else
        {
            printf("################################################################\n");
            printf("#####  RETRY     ###############################################\n");
            printf("################################################################\n");

        }
    }

    
    printf("DfuTransportSerial->send_packet_NW(), packet length is %d\n", pkt->length );
//    self->serial_port->write(pkt);
//    self->serial_port->close();
//    self->serial_port->isOpen();
//    self->serial_port->read(6);

    return(0);
}
/*
    def send_packet(self, pkt):
        attempts = 0
        last_ack = None
        packet_sent = False

        logger.debug("PC -> target: {0}".format(pkt))

        while not packet_sent:
            self.serial_port.write(pkt.data)
            attempts += 1            ack = self.get_ack_nr()

            if last_ack is None:
                break

            if ack == (last_ack + 1) % 8:
                last_ack = ack
                packet_sent = True

                if attempts > 3:
                    raise Exception("Three failed tx attempts encountered on packet {0}".format(pkt.sequence_number))
*/

///////////////////////////////////////////////////////////////////////////////

static uint32_t CMD_nRF_MODE = 0xe6;									// ｎＲＦ５１ブートモード設定
static uint32_t CMD_nRF_SLIPWRAP = 0xe7;								// ｎＲＦ５１ＳＬＩＰブロックラップ

static uint8_t password_user[8] = { (uint8_t)'C', (uint8_t)'r', (uint8_t)'p', (uint8_t)'t', (uint8_t)'0', (uint8_t)'8', (uint8_t)'0', (uint8_t)'1' };   // ユーザーアプリケーション　ＵＳＢコマンドのパスワード（８バイト）

static void delay(uint32_t value)
{
    //Sleep( value);
    float time_in_seconds = ((float)value)/1000.0;
    time_sleep(time_in_seconds);
}

static uint16_t BitConverter_D_ToUInt16(uint8_t *buffer, uint16_t position)
{
    uint16_t value;
    value = 0;
    value += buffer[position + 0];
    value += ((uint16_t)(buffer[position + 1])) << 8;
    return(value);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t sendCommand(DfuTrans_Serial_t *self, uint8_t *sData, uint16_t length);
int32_t sendCommand(DfuTrans_Serial_t *self, uint8_t *sData, uint16_t length)
{    
    uint16_t i, len, sum;
    uint32_t dwBytesWritten;
    int32_t  r;
    uint32_t ur;

    //serialPort1_D_DiscardInBuffer();
    //serialPort1_D_DiscardOutBuffer();

    len = BitConverter_D_ToUInt16(sData, 3);

    for (sum = 0, i = 0; i < len + 5; i++) sum += sData[i];

    sData[len + 5] = (byte)sum;
    sData[len + 6] = (byte)(sum / 256);

    r = 0;
    //serialPort1_D_Write(sData, 0, len + 7);
    ur = self->serial_medium->_Write_A_RAW( 0, sData, len + 7, &dwBytesWritten);
    if(ur != 0)
        r = -1;
    if(len + 7 != dwBytesWritten)
        r = -1;

    return(r);
}

bool recvCommand(DfuTrans_Serial_t *self, uint8_t *rData, uint32_t *rLength);
bool recvCommand(DfuTrans_Serial_t *self, uint8_t *rData, uint32_t *rLength)
{
    int32_t   r;
    uint32_t  ur;
    uint32_t  dwBytesRead;
    int       i;//, n;
    uint16_t  len, sum;


    for (i = 0; i < 5; i++)
    {
        if( (i==1) &&(rData[0] != 0x01) )
        {
            printf("recvCommand: flushing non 0x01 char = 0x%02x\n", rData[0]);
            i = 0;
        }
        ur = self->serial_medium->_Read_A_RAW(0, &rData[i], 1, &dwBytesRead); //  rData[i] = (byte)serialPort1_D_ReadByte();
        if( ur != 0)
        {
            continue;
        }
    }

    if (rData[0] != 0x01)
    {
        return false;
    }

    len = (uint16_t)(BitConverter_D_ToUInt16(rData, 3) + 7);

    if (len > 4096) return false;

    i = 0;
    //while (serialPort1_D_BytesToRead() < (len - 5))         // 予定バイト数が受信されてからＲｅａｄすると早い
    //{
    //    Application_D_DoEvents();
    //}
    while (i < len - 5)
    {
        //n = serialPort1_D_Read(rData, 5 + i, len - 5 - i);
        //if (n > 0) i += (uint16_t)n;
        ur = self->serial_medium->_Read_A_RAW(0, &rData[5 + i], len - 5 - i, &dwBytesRead);
        if( ur != 0)
        {
            continue;
        }
        if (dwBytesRead > 0) i += (uint16_t)dwBytesRead;
        //Application_D_DoEvents();
    }

    sum = 0;
    for (i = 0; i < len - 2; i++) sum += rData[i];

    if (sum != BitConverter_D_ToUInt16(rData, len - 2)) return false;

    return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static  uint8_t   m_SLIP_rData[4096];
static  uint32_t  m_SLIP_rLength;



static int get_ack_nr_UW_SLIP(DfuTransport_t *P)
{
    int ack_val;
    buf32_t   uart_buffer;
    buf32_t*  data;

    uart_buffer.buffer = &m_SLIP_rData[5];
    uart_buffer.length = m_SLIP_rLength;

    uint32_t  r_serDev;
    uint32_t  posC0_0;
    uint32_t  posC0_1;


    printf("\r\n");
    if( buf32_count(&uart_buffer, 0xC0) < 2)
    {
        printf("Need two 0xC0 bytes\r\n");
        return(0xff);
    }

    posC0_0 = 0;
    while(uart_buffer.buffer[posC0_0] != 0xC0)
    {
        posC0_0++;
    }

    posC0_1 = posC0_0 + 1;
    while(uart_buffer.buffer[posC0_1] != 0xC0)
    {
        posC0_1++;
    }

    uart_buffer.buffer = &uart_buffer.buffer[posC0_0];
    uart_buffer.length = posC0_1 - posC0_0;



    buf32_print("get_ack_nr_UW_SLIP", &uart_buffer);
    data = buf32_decode_esc_chars(&uart_buffer);
    buf32_print("get_ack_nr_UW_SLIP --Non SLIP data ---", data);

    /*
    // Remove 0xC0 at start and beginning
    data = data[1:-1];
    // print "non-slip data ", [data]
    return (data[0] >> 3) & 0x07;
    */

    // print "non-slip data ", [data]
    ack_val = (data->buffer[1] >> 3) & 0x07;
    printf("ack_val = 0x%1x, %d\n\n", ack_val, ack_val);

    return( ack_val );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint32_t send_packet_UW_SLIP(DfuTrans_Serial_t *self, uint8_t* buffer, uint32_t length, uint32_t* p_dwBytesWritten);
uint32_t send_packet_UW_SLIP(DfuTrans_Serial_t *self, uint8_t* buffer, uint32_t length, uint32_t* p_dwBytesWritten)
{
    uint32_t i;
    uint32_t dwBytesWritten;
    uint32_t r;
    uint8_t sData[4096];
    int reloop;

    uint32_t  SLIP_rLength;

    //DfuTrans_Serial_t *self;
    //self = (DfuTrans_Serial_t*)P;

    reloop = 2; // 2 means can re-loop one more time
    while( reloop != 0 )
    {
        // ブレーク信号送出
        sData[0] = 0x00;
        r = self->serial_medium->_Write_A_RAW(0, sData, 1, &dwBytesWritten);
        delay(50);

        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_SLIPWRAP;
        sData[2] = 0x00;

        sData[3] = (uint8_t)( (length & 0x000000ff) >> 0 );
        sData[4] = (uint8_t)( (length & 0x0000ff00) >> 8 );

        memcpy(&sData[5], buffer, length);

        memset(m_SLIP_rData, 0, sizeof(m_SLIP_rData));
        m_SLIP_rLength = 0;

        sendCommand(self, sData, 5 + length);
        SLIP_rLength = 0;
        if (recvCommand(self, m_SLIP_rData, &SLIP_rLength) == false) //SLIP_rLength UNUSED
        {
            printf("send_packet_UW_SLIP: 受信異常\r\n");
            *p_dwBytesWritten = 0;
            r = -1;
        }
        else
        {
            if( m_SLIP_rData[2] == 0x15 )
            {
                r = -1;
                reloop--;
            }
            else
            {
                printf("send_packet_UW_SLIP: OK\r\n");
                m_SLIP_rLength = BitConverter_D_ToUInt16( m_SLIP_rData, 3);
                *p_dwBytesWritten = length;
                r = 0;
                reloop = 0;
            }
        }
    }

    //logger.debug("PC -> target: {0}".format(pkt))
    
    return(r);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int send_packet_UW(DfuTransport_t *P, buf32_t *pkt)
{
    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)P;
   
    //logger.debug("PC -> target: {0}".format(pkt))
    
    uint32_t r_serDev;
    uint32_t dwBytesWritten;

    int  attempts = 0;
    int  ack = 0;//None
    bool packet_sent = false;

    while( packet_sent == false)
    {
        //r_serDev = self->serial_medium->_Write_A_SLIP   (0,    pkt->buffer, pkt->length, &dwBytesWritten);
        r_serDev = send_packet_UW_SLIP(self, pkt->buffer, pkt->length, &dwBytesWritten);
        if( r_serDev != 0 )
        {
            printf("Controller_send_packet: serdev_Write: ERROR = %d, 0x%08x\n", r_serDev, r_serDev);
            return(-1);
        }
        if( pkt->length != dwBytesWritten )
        {
            printf("Controller_send_packet: serdev_Write: pkt->len[%d] != dwBytesWritten[%d]\n", pkt->length, dwBytesWritten);
            return(-1);
        }

        attempts += 1;
        //ack = get_ack_nr(P);
        ack = get_ack_nr_UW_SLIP(P);
        if( ack == 0xFF)
        {
            printf("Controller_send_packet: Controller_get_ack_nr: ERROR\n");
            return(-1);
        }


        //orig if( last_ack == 0)//is None:
        //orig     break;
        if(last_ack == 42) //None
        {
            printf("UW ################################################################\n");
            printf("UW #####  FIRST ACK ###############################################\n");
            printf("UW ################################################################\n");
            last_ack = ack;
            break;
        }

        if( ack == (last_ack + 1) % 8 )
        {
            last_ack = ack;
            packet_sent = true;

            if (attempts > 3)
            {
                //raise Exception("Three failed tx attempts encountered on packet {0}".format(pkt.sequence_number))
                //raise Exception("Three failed tx attempts encountered on packet {0}".format(seq))
                printf("Three failed tx attempts encountered on packet %d\n", -1 );//.format(seq))
                return(-1);
            }
        }
        else
        {
            printf("UW ################################################################\n");
            printf("UW #####  RETRY     ###############################################\n");
            printf("UW ################################################################\n");

        }

    }

    
    printf("DfuTransportSerial->send_packet_UW(), packet length is %d\n", pkt->length );
//    self->serial_port->write(pkt);
//    self->serial_port->close();
//    self->serial_port->isOpen();
//    self->serial_port->read(6);

    return(0);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int send_recv_SET_DEVICE_DFU_MODE_ON(DfuTransport_t *P)
{
    uint32_t dwBytesWritten;
    uint32_t rLength;
    uint32_t r;
    uint8_t sData[128];
    uint8_t rData[128];
    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)P;

    const char* TransName;

    TransName = self->serial_medium->get_transport_name();
    if( (TransName[0] == 'N') && (TransName[1] == 'W'))
    {
        printf("\n");
        printf("\n");
        printf("You better have done the Button 4 Reset by now\n");
        printf("\n");
        printf("\n");
    }
    if( (TransName[0] == 'U') && (TransName[1] == 'W'))
    {        
        //===== STEP 1 - Mode selecton =====
        // ブレーク信号送出
        sData[0] = 0x00;
        r = self->serial_medium->_Write_A_RAW(0, sData, 1, &dwBytesWritten);
        delay(50);

        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;  // 0xE6
        sData[2] = 0x01;                   // 0x01 (0ｘ00:開放 0ｘ01:確保) DFU_MODE sub command 1
        sData[3] = (uint8_t)0x08;          // 0x08 len LSB
        sData[4] = (uint8_t)0x00;          // 0x00 len MSB

        memcpy(&sData[5], password_user, 8);

        sendCommand(self, sData, 5 + 8);
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: 受信異常\r\n");
        }
        else
        {
            if( rData[2] != 0x06)
                printf("send_recv_SET_DEVICE_DFU_MODE_ON: DID NOT GET ACK\r\n");
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: OK\r\n");
        }


        //delay(42);

/*---------------------------------------------------*/
/*
        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;  // 0xE6
        sData[2] = 0x02;                   // 0x02  //DFU_MODE sub command 2 - RESET, BOOT CONTROL
        sData[3] = (uint8_t)0x02;          // 0x02 len LSB //ERROR-was-0x08
        sData[4] = (uint8_t)0x00;          // 0x00 len MSB

        sData[5] = (uint8_t)0x01;          // 0x01 RESET HIGH  
        sData[6] = (uint8_t)0x00;          // 0x00 BOOT  LOW

        sendCommand(self, sData, 5 + 2); //was 5 + 8
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : 受信異常\r\n");
        }
        else
        {
            if( rData[2] != 0x06)
                printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : DID NOT GET ACK\r\n");
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : OK\r\n");
        }

        delay(1000);

        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;  // 0xE6
        sData[2] = 0x02;                   // 0x02  //DFU_MODE sub command 2 - RESET, BOOT CONTROL
        sData[3] = (uint8_t)0x02;          // 0x02 len LSB //ERROR-was-0x08
        sData[4] = (uint8_t)0x00;          // 0x00 len MSB

        sData[5] = (uint8_t)0x01;          // 0x01 RESET HIGH  
        sData[6] = (uint8_t)0x01;          // 0x01 BOOT  HIGH

        sendCommand(self, sData, 5 + 2); //was 5 + 8
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : 受信異常\r\n");
        }
        else
        {
            if( rData[2] != 0x06)
                printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : DID NOT GET ACK\r\n");
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : OK\r\n");
        }

        delay(1000);
*/
/*---------------------------------------------------*/

        //===== STEP 2 - RESET-LOW, BOOT-LOW =====
        // ブレーク信号送出
        //sData[0] = 0x00;
        //r = self->serial_medium->_Write_A_RAW(0, sData, 1, &dwBytesWritten);
        //delay(50);

        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;  // 0xE6
        sData[2] = 0x02;                   // 0x02  //DFU_MODE sub command 2 - RESET, BOOT CONTROL
        sData[3] = (uint8_t)0x02;          // 0x02 len LSB //ERROR-was-0x08
        sData[4] = (uint8_t)0x00;          // 0x00 len MSB

        sData[5] = (uint8_t)0x00;          // 0x00 RESET LOW  
        sData[6] = (uint8_t)0x00;          // 0x00 BOOT  LOW

        sendCommand(self, sData, 5 + 2); //was 5 + 8
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : 受信異常\r\n");
        }
        else
        {
            if( rData[2] != 0x06)
                printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : DID NOT GET ACK\r\n");
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : OK\r\n");
        }

        delay(1000);

        //===== STEP 3 - RESET-HIGH, BOOT-LOW =====
        // ブレーク信号送出
        //sData[0] = 0x00;
        //r = self->serial_medium->_Write_A_RAW(0, sData, 1, &dwBytesWritten);
        //delay(50);

        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;  // 0xE6
        sData[2] = 0x02;                   // 0x02  //DFU_MODE sub command 2 - RESET, BOOT CONTROL
        sData[3] = (uint8_t)0x02;          // 0x02 len LSB //ERROR-was-0x08
        sData[4] = (uint8_t)0x00;          // 0x00 len MSB

        sData[5] = (uint8_t)0x01;          // 0x00 RESET HIGH
        sData[6] = (uint8_t)0x00;          // 0x00 BOOT  LOW

        sendCommand(self, sData, 5 + 2); //was 5 + 8
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-LOW : 受信異常\r\n");
        }
        else
        {
            if( rData[2] != 0x06)
                printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-LOW : DID NOT GET ACK\r\n");
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-LOW : OK\r\n");
        }


        delay(1000);

        //===== STEP 3 - RESET-HIGH, BOOT-HIGH =====
        // ブレーク信号送出
        //sData[0] = 0x00;
        //r = self->serial_medium->_Write_A_RAW(0, sData, 1, &dwBytesWritten);
        //delay(50);

        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;  // 0xE6
        sData[2] = 0x02;                   // 0x02  //DFU_MODE sub command 2 - RESET, BOOT CONTROL
        sData[3] = (uint8_t)0x02;          // 0x02 len LSB //ERROR-was-0x08
        sData[4] = (uint8_t)0x00;          // 0x00 len MSB

        sData[5] = (uint8_t)0x01;          // 0x00 RESET HIGH  
        sData[6] = (uint8_t)0x01;          // 0x00 BOOT  HIGH

        sendCommand(self, sData, 5 + 2); //was 5 + 8
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-HIGH : 受信異常\r\n");
        }
        else
        {
            if( rData[2] != 0x06)
                printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-HIGH : DID NOT GET ACK\r\n");
            printf("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-HIGH : OK\r\n");
        }


    }
    //logger.debug("PC -> target: {0}".format(pkt))
    
    return(0);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int send_recv_SET_DEVICE_DFU_MODE_OFF(DfuTransport_t *P)
{
    uint32_t dwBytesWritten;
    uint32_t rLength;
    uint32_t r;
    uint8_t sData[128];
    uint8_t rData[128];
    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)P;

    const char* TransName;

    TransName = self->serial_medium->get_transport_name();
    if( (TransName[0] == 'N') && (TransName[1] == 'W'))
    {
        printf("\n");
        printf("\n");
        printf("Nothing to do now, but check if your Nordic Device has been updated\n");
        printf("\n");
        printf("\n");
    }
    if( (TransName[0] == 'U') && (TransName[1] == 'W'))
    {        

        // ブレーク信号送出
        sData[0] = 0x00;
        r = self->serial_medium->_Write_A_RAW(0, sData, 1, &dwBytesWritten);
        delay(50);

        //===== STEP 3 - RESET-LOW, BOOT-HIGH =====
        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;
        sData[2] = 0x02; //DFU_MODE sub command 2 - RESET, BOOT CONTROL
        sData[3] = (uint8_t)0x08;
        sData[4] = (uint8_t)0x00;
        sData[5] = (uint8_t)0x00; //RESET
        sData[6] = (uint8_t)0x01; //BOOT

        sendCommand(self, sData, 5 + 8);
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : 受信異常\r\n");
        }
        else
        {
            if( rData[2] != 0x06)
                printf("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : DID NOT GET ACK\r\n");
            printf("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : OK\r\n");
        }
        delay(1000);


        //===== STEP 3 - RESET-HIGH, BOOT-HIGH =====
        // ブレーク信号送出
        //sData[0] = 0x00;
        //r = self->serial_medium->_Write_A_RAW(0, sData, 1, &dwBytesWritten);
        //delay(50);

        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;
        sData[2] = 0x02; //DFU_MODE sub command 2 - RESET, BOOT CONTROL
        sData[3] = (uint8_t)0x08;
        sData[4] = (uint8_t)0x00;
        sData[5] = (uint8_t)0x01; //RESET
        sData[6] = (uint8_t)0x01; //BOOT

        sendCommand(self, sData, 5 + 8);
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : 受信異常\r\n");
        }
        else
        {
            if( rData[2] != 0x06)
                printf("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : DID NOT GET ACK\r\n");
            printf("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : OK\r\n");
        }
        delay(1000);


        sData[0] = 0x01;
        sData[1] = (uint8_t)CMD_nRF_MODE;
        sData[2] = 0x00; //NORMAL_MODE sub command 0

        sData[3] = (uint8_t)0x08;
        sData[4] = (uint8_t)0x00;

        memcpy(&sData[5], password_user, 8);

        sendCommand(self, sData, 5 + 8);
        if (recvCommand(self, rData, &rLength) == false)
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_OFF: 受信異常\r\n");
        }
        else
        {
            printf("send_recv_SET_DEVICE_DFU_MODE_OFF: OK\r\n");
        }




    }
    //logger.debug("PC -> target: {0}".format(pkt))
        return(0);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t serT_SET_DEVICE_DFU_MODE_ON(DfuTransport_t *P)
{
    int32_t  r;
    r = send_recv_SET_DEVICE_DFU_MODE_ON(P);
    return(r);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t serT_SET_DEVICE_DFU_MODE_OFF(DfuTransport_t *P)
{
    int32_t  r;
    r = send_recv_SET_DEVICE_DFU_MODE_OFF(P);
    return(r);
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int send_packet(DfuTransport_t *P, buf32_t *pkt)
{
    int32_t r;
    const char* TransName;
        DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)P;

    TransName = self->serial_medium->get_transport_name();
    if( (TransName[0] == 'N') && (TransName[1] == 'W'))
    {
        r = send_packet_NW(P, pkt);
    }
    if( (TransName[0] == 'U') && (TransName[1] == 'W'))
    {        
        r = send_packet_UW(P, pkt);
    }
    
    return(r);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t serT_send_validate_firmware(DfuTransport_t *P)
{
    /*
    Send request to device to verify that firmware has been correctly transferred.

    This call will block until validation is sent and validation is complete.
    If timeout or errors occurs exception is thrown.

    :return bool: True if firmware validated successfully.
    */


    printf("DfuTransportSerial->send_validate_firmware()\n");

    return(0); //   pass
}
/*
    def send_validate_firmware(self):
        super(DfuTransportSerial, self).send_validate_firmware()
        return True
*/


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t serT_send_init_packet(DfuTransport_t *P, buf32_t *init_packet)
{
    int32_t r;
    buf32_t *pFrame;
    buf32_t *pPacket;

    printf("DfuTransportSerial->send_init_packet()\n");
    printf("    init_packet length = %d\n", init_packet->length );
    
    uint16_t hardware_version    = *(uint16_t*)&init_packet->buffer[0]; //0x0000 //0xFFFF;
    uint16_t hardware_revision   = *(uint16_t*)&init_packet->buffer[2]; //0x0000; //0xFFFF;
    uint32_t application_version = *(uint32_t*)&init_packet->buffer[4]; //0x00000000; //0xFFFFFFFF;
    uint16_t softdevice_len      = *(uint16_t*)&init_packet->buffer[8]; //0x0001;
    uint16_t softdevice_array    = *(uint16_t*)&init_packet->buffer[10]; //0xFFFE;
    uint16_t image_crc           = *(uint16_t*)&init_packet->buffer[12];
    uint16_t pad16               = *(uint16_t*)&init_packet->buffer[14];

    printf("     hardware_version    = 0x%04x\n", hardware_version);
    printf("     hardware_revision   = 0x%04x\n", hardware_revision);
    printf("     application_version = 0x%08x\n", application_version);
    printf("     softdevice_len      = 0x%04x\n", softdevice_len);
    printf("     softdevice_array    = 0x%04x\n", softdevice_array);
    printf("     image_crc           = 0x%04x\n", image_crc);
    printf("     pad16               = 0x%04x\n", pad16);
    printf("\n");

    pFrame = malloc_frame_DFU_INIT_PACKET( init_packet );
    pPacket = malloc_HciPacket( pFrame );
    
    r = send_packet(P, pPacket);
    buf32_Destroy(&pFrame);
    buf32_Destroy(&pPacket);

    time_sleep(SEND_INIT_PACKET_WAIT_TIME);

    /*
    Send init_packet to device.

    This call will block until init_packet is sent and transfer of packet is complete.
    If timeout or errors occurs exception is thrown.

    :param str init_packet: Init packet as a str.
    :return:
    */

    return(r); //   pass
}
/*
    def send_init_packet(self, init_packet):
        super(DfuTransportSerial, self).send_init_packet(init_packet)

        frame = int32_to_bytes(DFU_INIT_PACKET)
        frame += init_packet
        frame += int16_to_bytes(0x0000)  # Padding required

        packet = HciPacket(frame)
        self.send_packet(packet)
        time.sleep(SEND_INIT_PACKET_WAIT_TIME)
*/


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t serT_send_start_dfu(DfuTransport_t *P, int program_mode, int softdevice_size, int bootloader_size, int app_size)
{
    int32_t r;
    buf32_t *pFrame;
    buf32_t *pPacket;

    last_ack = 42;

    printf("DfuTransportSerial->send_start_dfu()\n");
    printf("    hex_type        = %d\n", program_mode);
    printf("    softdevice_size = %d\n", softdevice_size);
    printf("    bootloader_size = %d\n", bootloader_size);
    printf("    app_size        = %d\n", app_size);
    printf("\n");

    pFrame = malloc_frame_DFU_START_PACKET( program_mode, softdevice_size, bootloader_size, app_size);
    malloc_HciPacket_Set_sequence_number( 0 ); //HCI_PROBLEM
    pPacket = malloc_HciPacket( pFrame );
    r = send_packet(P, pPacket);

    buf32_Destroy(&pFrame);
    buf32_Destroy(&pPacket);

    time_sleep(SEND_START_DFU_WAIT_TIME);
    /*
        frame = int32_to_bytes(DFU_START_PACKET)
        frame += int32_to_bytes(mode)
        frame += DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size)

        packet = HciPacket(frame)
        self.send_packet(packet)
        time.sleep(SEND_START_DFU_WAIT_TIME)
    */

    return(r); //   pass
}
/*
    def send_start_dfu(self, mode, softdevice_size=None, bootloader_size=None, app_size=None):
        super(DfuTransportSerial, self).send_start_dfu(mode, softdevice_size, bootloader_size, app_size)

        frame = int32_to_bytes(DFU_START_PACKET)
        frame += int32_to_bytes(mode)
        frame += DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size)

        packet = HciPacket(frame)
        self.send_packet(packet)
        time.sleep(SEND_START_DFU_WAIT_TIME)
*/


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t serT_send_activate_firmware(DfuTransport_t *P)
{
    /*
    Send command to device to activate new firmware and restart the device.
    The device will start up with the new firmware.

    Raises an nRFException if anything fails.

    :return:
    */


    printf("DfuTransportSerial->send_activate_firmware()\n");

    return(0); //   pass
}
/*
    def send_activate_firmware(self):
        super(DfuTransportSerial, self).send_activate_firmware()
*/


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int progress_percentage(int part, int whole)
{
    return( (int)( 100.0 * ((float)part)/((float)whole) ) );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int32_t serT_send_firmware(DfuTransport_t *P, buf32_t *firmware)
{
    int32_t r;
    DfuTrans_Serial_t *self;
    self = (DfuTrans_Serial_t*)P;

    /*
    Start sending firmware to device.

    This call will block until transfer of firmware is complete.
    If timeout or errors occurs exception is thrown.

    :param str firmware:
    :return:
    */

    printf("DfuTransportSerial->send_firmware()\n");
    printf("    Firmware length is %d bytes\n", firmware->length);
    printf("    will be sent in %d blocks of size 512\n", (firmware->length / 512) + 1);
    printf("\n");


    //_send_event(DfuEvent.PROGRESS_EVENT, progress=0, done=False, log_message="")
    uint32_t i;
    uint32_t count;
    buf32_t* pFrame;
    buf32_t* pPacket;
    uint32_t length;

    for( i=0; i <firmware->length; i += DFU_PACKET_MAX_SIZE)
    {
        length = DFU_PACKET_MAX_SIZE;
        if( length > (firmware->length - i) )
            length = (firmware->length - i);

        pFrame = malloc_frame_DFU_DATA_PACKET( &firmware->buffer[i], length);
        pPacket = malloc_HciPacket( pFrame );

        r = send_packet(P, pPacket);
        
        buf32_Destroy(&pFrame);
        buf32_Destroy(&pPacket);
        //data_packet = HciPacket(int32_to_bytes(DFU_DATA_PACKET) + firmware[i:i + DFU_PACKET_MAX_SIZE])
        //frames.append(data_packet)
        if( r != 0 )
            return( r );
    }

    //frames_count = frames->length;

    // Send firmware packets
    //for( count = 0; count < frames->length; count++)
    //{
    //    send_packet(pkt)
    //    //_send_event(DfuEvent.PROGRESS_EVENT,log_message="", progress=progress_percentage(count, frames_count), done=False);
    //}
    time_sleep(1.0); //karel

    // Send data stop packet
    pFrame = malloc_frame_DFU_STOP_DATA_PACKET();
    pPacket = malloc_HciPacket( pFrame );

    r = send_packet(P, pPacket);

    buf32_Destroy(&pFrame);
    buf32_Destroy(&pPacket);

    //frame = int32_to_bytes(DFU_STOP_DATA_PACKET)
    //packet = HciPacket(frame);
    //send_packet(packet);

    //_send_event(DfuEvent.PROGRESS_EVENT, progress=100, done=False, log_message="")

    return(r); //   pass
}

/*
    def send_firmware(self, firmware):
        super(DfuTransportSerial, self).send_firmware(firmware)

        def progress_percentage(part, whole):
            return int(100 * float(part)/float(whole))

        frames = []
        self._send_event(DfuEvent.PROGRESS_EVENT, progress=0, done=False, log_message="")

        for i in range(0, len(firmware), DFU_PACKET_MAX_SIZE):
            data_packet = HciPacket(int32_to_bytes(DFU_DATA_PACKET) + firmware[i:i + DFU_PACKET_MAX_SIZE])
            frames.append(data_packet)

        frames_count = len(frames)

        # Send firmware packets
        for count, pkt in enumerate(frames):
            self.send_packet(pkt)
            self._send_event(DfuEvent.PROGRESS_EVENT,
                             log_message="",
                             progress=progress_percentage(count, frames_count),
                             done=False)

        # Send data stop packet
        frame = int32_to_bytes(DFU_STOP_DATA_PACKET)
        packet = HciPacket(frame)
        self.send_packet(packet)

        self._send_event(DfuEvent.PROGRESS_EVENT, progress=100, done=False, log_message="")
*/



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/*
@staticmethod
    def decode_esc_chars(data):
        """Replace 0xDBDC with 0xCO and 0xDBDD with 0xDB"""
        result = []

        data = bytearray(data)

        while len(data):
            char = data.pop(0)

            if char == 0xDB:
                char2 = data.pop(0)

                if char2 == 0xDC:
                    result.append(0xC0)
                elif char2 == 0xDD:
                    result.append(0xDB)
                else:
                    raise Exception('Char 0xDB NOT followed by 0xDC or 0xDD')
            else:
                result.append(char)

        return result

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

DATA_INTEGRITY_CHECK_PRESENT = 1
RELIABLE_PACKET = 1
HCI_PACKET_TYPE = 14

DFU_INIT_PACKET = 1
DFU_START_PACKET = 3
DFU_DATA_PACKET = 4
DFU_STOP_DATA_PACKET = 5

DFU_UPDATE_MODE_NONE = 0
DFU_UPDATE_MODE_SD = 1
DFU_UPDATE_MODE_BL = 2
DFU_UPDATE_MODE_APP = 4


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class HciPacket(object):
    """Class representing a single HCI packet"""

    sequence_number = 0

    def __init__(self, data=''):
        HciPacket.sequence_number = (HciPacket.sequence_number + 1) % 8
        self.temp_data = ''
        self.temp_data += slip_parts_to_four_bytes(HciPacket.sequence_number,
                                                   DATA_INTEGRITY_CHECK_PRESENT,
                                                   RELIABLE_PACKET,
                                                   HCI_PACKET_TYPE,
                                                   len(data))
        self.temp_data += data
        # Add escape characters
        crc = crc16.calc_crc16(self.temp_data, crc=0xffff)

        self.temp_data += chr(crc & 0xFF)
        self.temp_data += chr((crc & 0xFF00) >> 8)

        self.temp_data = slip_encode_esc_chars(self.temp_data)

        self.data = chr(0xc0)
        self.data += self.temp_data
        self.data += chr(0xc0)

    def __str__(self):
        return binascii.hexlify(self.data)
*/

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void init_Dfu_Serial(DfuTrans_Serial_t *gp)
{
    
    printf("DfuTransportSerial->init_Dfu_Serial()\n");

    gp->DfuTransport.is_open                  = serT_is_open;
    gp->DfuTransport.open                     = serT_open;
    gp->DfuTransport.close                    = serT_close;

    gp->DfuTransport.send_start_dfu           = serT_send_start_dfu;
    gp->DfuTransport.send_init_packet         = serT_send_init_packet;
    gp->DfuTransport.send_firmware            = serT_send_firmware;
    gp->DfuTransport.send_activate_firmware   = serT_send_activate_firmware;
    gp->DfuTransport.send_validate_firmware   = serT_send_validate_firmware;


    gp->DfuTransport.register_events_callback = serT_register_events_callback;

    gp->DfuTransport.SET_DEVICE_DFU_MODE_ON   = serT_SET_DEVICE_DFU_MODE_ON;
    gp->DfuTransport.SET_DEVICE_DFU_MODE_OFF  = serT_SET_DEVICE_DFU_MODE_OFF;


    
}

