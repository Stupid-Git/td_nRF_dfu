using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using nRFdfu_cs;

namespace nRFdfu_cs
{
    class DfuTransportSerial_rtrSOH : DfuTransport//_new
    {
        
        const int DATA_INTEGRITY_CHECK_PRESENT = 1;
        const int RELIABLE_PACKET = 1;
        const int HCI_PACKET_TYPE = 14;

        const int DFU_INIT_PACKET = 1;
        const int DFU_START_PACKET = 3;
        const int DFU_DATA_PACKET = 4;
        const int DFU_STOP_DATA_PACKET = 5;

        const int DFU_UPDATE_MODE_NONE = 0;
        const int DFU_UPDATE_MODE_SD = 1;
        const int DFU_UPDATE_MODE_BL = 2;
        const int DFU_UPDATE_MODE_APP = 4;

        const float DEFAULT_SERIAL_PORT_TIMEOUT  = 1.0F;  //# Timeout time on serial port read
        const float ACK_PACKET_TIMEOUT           = 1.0F;  //# Timeout time for for ACK packet received before reporting timeout through event system
        const float SEND_INIT_PACKET_WAIT_TIME   = 1.0F;  //# Time to wait before communicating with bootloader after init packet is sent
        const float SEND_START_DFU_WAIT_TIME     = 10.0F; //# Time to wait before communicating with bootloader after start DFU packet is sent
        const int DFU_PACKET_MAX_SIZE            = 512;  //# The DFU packet max size


        //=====================================================================
        public DfuTransportSerial_rtrSOH(String com_port, int baud_rate/*=DEFAULT_BAUD_RATE*/, bool flow_control/*=DEFAULT_FLOW_CONTROL*/, float timeout/*=DEFAULT_SERIAL_PORT_TIMEOUT*/)
        {
            //super(DfuTransportSerial, self).__init__()
            
            /*TODO??
            this.com_port = com_port;
            this.baud_rate = baud_rate;
            if( flow_control == true )
                this.flow_control = 1; 
            else 
                this.flow_control = 0;

            this.timeout = timeout;
            */

            //this.serial_port = None
            //""":type: serial.Serial """
             
            //this.serial_port = new SerialPort( com_port, baud_rate, Parity.None, 8, StopBits.One);
        }
        
        DfuSerial dfuSerial = null;
        public DfuTransportSerial_rtrSOH(DfuSerial dfu_serial)
        {
            this.dfuSerial = dfu_serial;
        }

//static DfuTrans_Serial_t m_TheOneAndOnlyInstance_DfuTrans_Serial;
//void init_Dfu_Serial(DfuTrans_Serial_t *gp);

        //=====================================================================
        public override void open()
        {
            this.dfuSerial.dfu_open();

            //base.open(); // no meaning
            /*
            //super(DfuTransportSerial, self).open()
            try
            {
                //this.serial_port = Serial(port=this.com_port, baudrate=this.baud_rate, rtscts=this.flow_control, timeout=this.timeout)
                this.serial_port.Open();
            }
            catch(Exception e)//        except Exception, e:
            {
                ;//raise NordicSemiException("Serial port could not be opened on {0}. Reason: {1}".format(this.com_port, e.message))
            }
             */
#if false       
            self = (DfuTrans_Serial_t*)pT;
            Console.Write("DfuTransportSerial.open()\n");

            //UInt32  Controller_com_port_num = 56;   //NW board serial_medium_NW_instance
            UInt32  Controller_com_port_num = 61;   //UW board serial_medium_UW_instance
            UInt32  m_Controller_baud_rate = 500000; //38400;
    
            //p_uart = &uart0;
            //p_uart.hComm = INVALID_HANDLE_VALUE;//(HANDLE)0xFFFFFFFF;

            //r = self.serial_medium.uart_NW_Init_N( p_uart, Controller_com_port_num, m_Controller_baud_rate);//, rtscts=Controller_flow_control, timeout=1);
            r = self.serial_medium._Init_N( 0/* p_uart*/, Controller_com_port_num, m_Controller_baud_rate);//, rtscts=Controller_flow_control, timeout=1);

            //r = self.serial_medium.uart_NW_Open( p_uart );
            r = self.serial_medium._Open( 0/*p_uart*/ );
            if( r != 0 )
            {
                //Console.Write("uart_NW_Open could not be opened on %s\n", gx_Controller_com_port);
                return(-1); //??
            }


            //OLD r = self.serial_medium.open(0);

            return(r);
#endif// false
        }



        //=====================================================================
        public override void close() // serT_close(DfuTransport_t *pT)
        {
            this.dfuSerial.dfu_close();
#if false
            UInt32 r;

            DfuTrans_Serial_t *self;
            self = (DfuTrans_Serial_t*)pT;

            Console.Write("DfuTransportSerial.close()\n");

            //r = self.serial_medium.close(0);
            r = self.serial_medium._Close(0);

            return(r);
#endif// false
        }
        /*
         * def close(self):
         * super(DfuTransportSerial, self).close()
         * self.serial_port.close()
         */

        //=====================================================================
        public override bool is_open() //serT_is_open(DfuTransport_t *pT)
        {
            bool bRet;

            DfuTrans_Serial_t *self;
            self = (DfuTrans_Serial_t*)pT;

            Console.Write("DfuTransportSerial.is_open()\n");
 
            //bRet = self.serial_medium.is_open();
            bRet = self.serial_medium._IsOpen(0);

            return( bRet );
            /*
            //super(DfuTransportSerial, self).is_open()
            if( pSelf.serial_port == 0)
                return false;

            return( pSelf.serial_port.isOpen(&pSelf.spInfo));  
            */
        }
        /*
        def is_open(self):
            super(DfuTransportSerial, self).is_open()

            if self.serial_port is None:
                return False

            return self.serial_port.isOpen()
        */



        //=====================================================================
        public override void send_start_dfu(int program_mode, int softdevice_size, int bootloader_size, int app_size) //UInt32 serT_send_start_dfu(DfuTransport_t *P, int program_mode, int softdevice_size, int bootloader_size, int app_size)
        {
            /*
            UInt32 r;
            buf32_t *pFrame;
            buf32_t *pPacket;

            last_ack = 42;

            Console.Write("DfuTransportSerial.send_start_dfu()\n");
            Console.Write("    hex_type        = %d\n", program_mode);
            Console.Write("    softdevice_size = %d\n", softdevice_size);
            Console.Write("    bootloader_size = %d\n", bootloader_size);
            Console.Write("    app_size        = %d\n", app_size);
            Console.Write("\n");

            pFrame = malloc_frame_DFU_START_PACKET( program_mode, softdevice_size, bootloader_size, app_size);
            malloc_HciPacket_Set_sequence_number( 0 ); //HCI_PROBLEM
            pPacket = malloc_HciPacket( pFrame );
            */
            r = self_dot_send_packet(P, pPacket);
            /*
            buf32_Destroy(&pFrame);
            buf32_Destroy(&pPacket);

            time_sleep(SEND_START_DFU_WAIT_TIME);
            */

            /*
             * frame = UInt32o_bytes(DFU_START_PACKET)
             * frame += UInt32o_bytes(mode)
             * frame += DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size)
             * 
             * packet = HciPacket(frame)
             * self.self_dot_send_packet(packet)
             * time.sleep(SEND_START_DFU_WAIT_TIME)
             * */

            return(r); //   pass
        }
        /*
        def send_start_dfu(self, mode, softdevice_size=None, bootloader_size=None, app_size=None):
        super(DfuTransportSerial, self).send_start_dfu(mode, softdevice_size, bootloader_size, app_size)

        frame = UInt32o_bytes(DFU_START_PACKET)
        frame += UInt32o_bytes(mode)
        frame += DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size)

        packet = HciPacket(frame)
        self.self_dot_send_packet(packet)
        time.sleep(SEND_START_DFU_WAIT_TIME)
        */

       
        //=====================================================================
        public override void send_init_packet( Byte [] init_packet) //UInt32 serT_send_init_packet(DfuTransport_t *P, buf32_t *init_packet)
        {
            /*
            UInt32 r;
            buf32_t *pFrame;
            buf32_t *pPacket;

            Console.Write("DfuTransportSerial.send_init_packet()\n");
            Console.Write("    init_packet length = %d\n", init_packet.length );
    
            uint16_t hardware_version    = *(uint16_t*)&init_packet.buffer[0]; //0x0000 //0xFFFF;
            uint16_t hardware_revision   = *(uint16_t*)&init_packet.buffer[2]; //0x0000; //0xFFFF;
            UInt32 application_version = *(UInt32*)&init_packet.buffer[4]; //0x00000000; //0xFFFFFFFF;
            uint16_t softdevice_len      = *(uint16_t*)&init_packet.buffer[8]; //0x0001;
            uint16_t softdevice_array    = *(uint16_t*)&init_packet.buffer[10]; //0xFFFE;
            uint16_t image_crc           = *(uint16_t*)&init_packet.buffer[12];
            uint16_t pad16               = *(uint16_t*)&init_packet.buffer[14];

            Console.Write("     hardware_version    = 0x%04x\n", hardware_version);
            Console.Write("     hardware_revision   = 0x%04x\n", hardware_revision);
            Console.Write("     application_version = 0x%08x\n", application_version);
            Console.Write("     softdevice_len      = 0x%04x\n", softdevice_len);
            Console.Write("     softdevice_array    = 0x%04x\n", softdevice_array);
            Console.Write("     image_crc           = 0x%04x\n", image_crc);
            Console.Write("     pad16               = 0x%04x\n", pad16);
            Console.Write("\n");

            pFrame = malloc_frame_DFU_INIT_PACKET( init_packet );
            pPacket = malloc_HciPacket( pFrame );
            */
            r = self_dot_send_packet(P, pPacket);
            /*
            buf32_Destroy(&pFrame);
            buf32_Destroy(&pPacket);

            time_sleep(SEND_INIT_PACKET_WAIT_TIME);
            */
            //return(r); //   pass
        }
/*
    def send_init_packet(self, init_packet):
        super(DfuTransportSerial, self).send_init_packet(init_packet)

        frame = UInt32o_bytes(DFU_INIT_PACKET)
        frame += init_packet
        frame += int16_to_bytes(0x0000)  # Padding required

        packet = HciPacket(frame)
        self.self_dot_send_packet(packet)
        time.sleep(SEND_INIT_PACKET_WAIT_TIME)
*/

        
        //-----------------------------------------------------------------------------
        static int progress_percentage(int part, int whole)
        {
            return( (int)( 100.0 * ((float)part)/((float)whole) ) );
        }


        //=====================================================================
        public override void send_firmware( Byte[] firmware) //   UInt32 serT_send_firmware(DfuTransport_t *P, buf32_t *firmware)
        {
            /*
            UInt32 r;
            DfuTrans_Serial_t *self;
            self = (DfuTrans_Serial_t*)P;
            */
            /*
             * Start sending firmware to device.
             * 
             * This call will block until transfer of firmware is complete.
             * If timeout or errors occurs exception is thrown.
             * 
             * :param str firmware:
             * :return:
             * */
            /*
            Console.Write("DfuTransportSerial.send_firmware()\n");
            Console.Write("    Firmware length is %d bytes\n", firmware.length);
            Console.Write("    will be sent in %d blocks of size 512\n", (firmware.length / 512) + 1);
            Console.Write("\n");
            */
            /*
            //_send_event(DfuEvent.PROGRESS_EVENT, progress=0, done=False, log_message="")
            UInt32 i;
            UInt32 count;
            buf32_t* pFrame;
            buf32_t* pPacket;
            UInt32 length;

            for( i=0; i <firmware.length; i += DFU_PACKET_MAX_SIZE)
            {
                length = DFU_PACKET_MAX_SIZE;
                if( length > (firmware.length - i) )
                    length = (firmware.length - i);

                pFrame = malloc_frame_DFU_DATA_PACKET( &firmware.buffer[i], length);
                pPacket = malloc_HciPacket( pFrame );
            */
                r = self_dot_send_packet(P, pPacket);
            /*
                buf32_Destroy(&pFrame);
                buf32_Destroy(&pPacket);
                //data_packet = HciPacket(UInt32o_bytes(DFU_DATA_PACKET) + firmware[i:i + DFU_PACKET_MAX_SIZE])
                //frames.append(data_packet)
                if( r != 0 )
                    return( r );
            }
            */
            //frames_count = frames.length;

            // Send firmware packets
            //for( count = 0; count < frames.length; count++)
            //{
            //    self_dot_send_packet(pkt)
            //    //_send_event(DfuEvent.PROGRESS_EVENT,log_message="", progress=progress_percentage(count, frames_count), done=False);
            //}

            /*
            time_sleep(1.0); //karel

            // Send data stop packet
            pFrame = malloc_frame_DFU_STOP_DATA_PACKET();
            pPacket = malloc_HciPacket( pFrame );
            */
            r = self_dot_send_packet(P, pPacket);
            /*
            buf32_Destroy(&pFrame);
            buf32_Destroy(&pPacket);

            //frame = UInt32o_bytes(DFU_STOP_DATA_PACKET)
            //packet = HciPacket(frame);
            //self_dot_send_packet(packet);

            //_send_event(DfuEvent.PROGRESS_EVENT, progress=100, done=False, log_message="")

            return(r); //   pass
            */
        }

        /*
    def send_firmware(self, firmware):
        super(DfuTransportSerial, self).send_firmware(firmware)

        def progress_percentage(part, whole):
            return int(100 * float(part)/float(whole))

        frames = []
        self._send_event(DfuEvent.PROGRESS_EVENT, progress=0, done=False, log_message="")

        for i in range(0, len(firmware), DFU_PACKET_MAX_SIZE):
            data_packet = HciPacket(UInt32o_bytes(DFU_DATA_PACKET) + firmware[i:i + DFU_PACKET_MAX_SIZE])
            frames.append(data_packet)

        frames_count = len(frames)

        # Send firmware packets
        for count, pkt in enumerate(frames):
            self.self_dot_send_packet(pkt)
            self._send_event(DfuEvent.PROGRESS_EVENT,
                             log_message="",
                             progress=progress_percentage(count, frames_count),
                             done=False)

        # Send data stop packet
        frame = UInt32o_bytes(DFU_STOP_DATA_PACKET)
        packet = HciPacket(frame)
        self.self_dot_send_packet(packet)

        self._send_event(DfuEvent.PROGRESS_EVENT, progress=100, done=False, log_message="")
*/

        public override bool send_validate_firmware()
        {
            return (true);
        }

        public override void send_activate_firmware()
        {
            //return (true);
        }

        
        //-----------------------------------------------------------------------------
        int self_dot_send_packet( Byte [] pkt) //self_dot_send_packet(DfuTransport_t *P, buf32_t *pkt)
        {
            int r;
            r = dfuSerial.dfu_send_packet( pkt );
            return(r);
        }
        
/*
    def self_dot_send_packet(self, pkt):
        attempts = 0
        last_ack = None
        packet_sent = False

        logger.debug("PC . target: {0}".format(pkt))

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
















































#if false // JUNK




//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UInt32 serT_register_events_callback(DfuTransport_t *P, int event_type, xxx_event_handler_del callback)
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
    
    Console.Write("DfuTransportSerial()\n");

    pTS = &m_TheOneAndOnlyInstance_DfuTrans_Serial;
    
    init_Dfu_Serial(pTS);

    pTS.com_port = com_port;
    pTS.baud_rate = baud_rate;
    if (flow_control == true)
        pTS.flow_control = 1;
    else
        pTS.flow_control = 0;
    
    pTS.timeout = timeout;

    pTS.serial_medium = medium;

    return(&pTS.DfuTransport); //???
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


/*
    def open(self):
        super(DfuTransportSerial, self).open()

        try:
            self.serial_port = Serial(port=self.com_port, baudrate=self.baud_rate, rtscts=self.flow_control, timeout=self.timeout)
        except Exception, e:
            raise NordicSemiException("Serial port could not be opened on {0}. Reason: {1}".format(self.com_port, e.message))
*/



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
    UInt32  r_serDev;

    Byte   uart_buffer_buffer[600];
    buf32_t   uart_buffer;

    Byte   temp_buf[600];
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
      //r_serDev = serdev_Read                 (uart, temp.buffer, 1, &temp.len); //temp = uart.read(1);
      //r_serDev = uart_NW_Read_A              (uart, temp.buffer, 1, &temp.length); //temp = uart.read(1);
        r_serDev = self.serial_medium._Read_A_SLIP(0,    temp.buffer, 1, &temp.length); //temp = uart.read(1);
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
                    Console.Write("Controller_get_ack_nr: timeout\n");
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
            Console.Write("Controller_get_ack_nr: serdev_Read: ERROR = %d, 0x%08x\n", r_serDev, r_serDev);
            return(0xFF);
            //break;
        }
    }

    //read until you get a new C0
    //RESUME_WORK
    buf32_print("\nController_get_ack_nr", &uart_buffer);
    data = dfu_util.slip_decode_esc_chars(&uart_buffer);

    /*
    // Remove 0xC0 at start and beginning
    data = data[1:-1];
    // print "non-slip data ", [data]
    return (data[0] >> 3) & 0x07;
    */

    // print "non-slip data ", [data]
    return( (data.buffer[1] >> 3) & 0x07);

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
            //# Disregard first of the two C0
            temp = self.serial_port.read(6)

            if temp:
                uart_buffer += temp

            if is_timeout(start, ACK_PACKET_TIMEOUT):
                //# reset HciPacket numbering back to 0
                HciPacket.sequence_number = 0
                self._send_event(DfuEvent.TIMEOUT_EVENT,
                                 log_message="Timed out waiting for acknowledgement from device.")

                //# quit loop
                break

                //# read until you get a new C0
                //# RESUME_WORK

        if len(uart_buffer) < 2:
            raise NordicSemiException("No data received on serial port. Not able to proceed.")

        logger.debug("PC <- target: {0}".format(binascii.hexlify(uart_buffer)))
        data = self.decode_esc_chars(uart_buffer)

        //# Remove 0xC0 at start and beginning
        data = data[1:-1]

        //# Extract ACK number from header
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
   
    //logger.debug("PC . target: {0}".format(pkt))
    
    UInt32 r_serDev;
    UInt32 dwBytesWritten;

    int  attempts = 0;
    int  ack = 0;//None
    //int  last_ack = 0;//None
    bool packet_sent = false;

    while( packet_sent == false)
////////
    {
        //r_serDev =                   serdev_Write(uart, pkt.data,   pkt.len,    &dwBytesWritten);
        //r_serDev =                uart_NW_Write_A(uart, pkt.buffer, pkt.length, &dwBytesWritten);
        r_serDev = self.serial_medium._Write_A_SLIP   (0,    pkt.buffer, pkt.length, &dwBytesWritten);
        //r_serDev = self.serial_medium.send_packet(0,  pkt.buffer, pkt.length);
        if( r_serDev != 0 )
        {
            Console.Write("Controller_send_packet: serdev_Write: ERROR = %d, 0x%08x\n", r_serDev, r_serDev);
            return(-1);
        }
        if( pkt.length != dwBytesWritten )
        {
            Console.Write("Controller_send_packet: serdev_Write: pkt.len[%d] != dwBytesWritten[%d]\n", pkt.length, dwBytesWritten);
            return(-1);
        }

        attempts += 1;
        ack = get_ack_nr(P);
        if( ack == 0xFF)
        {
            Console.Write("Controller_send_packet: Controller_get_ack_nr: ERROR\n");
            return(-1);
        }


        //orig if( last_ack == 0)//is None:
        //orig     break;
        if(last_ack == 42) //None
        {
            Console.Write("################################################################\n");
            Console.Write("#####  FIRST ACK ###############################################\n");
            Console.Write("################################################################\n");
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
                Console.Write("Three failed tx attempts encountered on packet %d\n", -1 );//.format(seq))
                return(-1);
            }
        }
        else
        {
            Console.Write("################################################################\n");
            Console.Write("#####  RETRY     ###############################################\n");
            Console.Write("################################################################\n");

        }
    }

    
    Console.Write("DfuTransportSerial.send_packet_NW(), packet length is %d\n", pkt.length );
//    self.serial_port.write(pkt);
//    self.serial_port.close();
//    self.serial_port.isOpen();
//    self.serial_port.read(6);

    return(0);
}
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UInt32 serT_SET_DEVICE_DFU_MODE_ON(DfuTransport_t *P)
{
    UInt32  r;
    r = send_recv_SET_DEVICE_DFU_MODE_ON(P);
    return(r);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UInt32 serT_SET_DEVICE_DFU_MODE_OFF(DfuTransport_t *P)
{
    UInt32  r;
    r = send_recv_SET_DEVICE_DFU_MODE_OFF(P);
    return(r);
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UInt32 serT_send_validate_firmware(DfuTransport_t *P)
{
    /*
    Send request to device to verify that firmware has been correctly transferred.

    This call will block until validation is sent and validation is complete.
    If timeout or errors occurs exception is thrown.

    :return bool: True if firmware validated successfully.
    */


    Console.Write("DfuTransportSerial.send_validate_firmware()\n");

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
UInt32 serT_send_activate_firmware(DfuTransport_t *P)
{
    /*
    Send command to device to activate new firmware and restart the device.
    The device will start up with the new firmware.

    Raises an nRFException if anything fails.

    :return:
    */


    Console.Write("DfuTransportSerial.send_activate_firmware()\n");

    return(0); //   pass
}
/*
    def send_activate_firmware(self):
        super(DfuTransportSerial, self).send_activate_firmware()
*/


/*
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
       // # Add escape characters
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
    
    Console.Write("DfuTransportSerial.init_Dfu_Serial()\n");

    gp.DfuTransport.is_open                  = serT_is_open;
    gp.DfuTransport.open                     = serT_open;
    gp.DfuTransport.close                    = serT_close;

    gp.DfuTransport.send_start_dfu           = serT_send_start_dfu;
    gp.DfuTransport.send_init_packet         = serT_send_init_packet;
    gp.DfuTransport.send_firmware            = serT_send_firmware;
    gp.DfuTransport.send_activate_firmware   = serT_send_activate_firmware;
    gp.DfuTransport.send_validate_firmware   = serT_send_validate_firmware;


    gp.DfuTransport.register_events_callback = serT_register_events_callback;

    gp.DfuTransport.SET_DEVICE_DFU_MODE_ON   = serT_SET_DEVICE_DFU_MODE_ON;
    gp.DfuTransport.SET_DEVICE_DFU_MODE_OFF  = serT_SET_DEVICE_DFU_MODE_OFF;


    
}

#endif //false // JUNK
        
    }



}