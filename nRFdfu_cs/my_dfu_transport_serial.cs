using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;

namespace nRFdfu_cs
{

    static class my_time
    {
        public static double time()
        {
            Int64 t = DateTime.UtcNow.Ticks;
            double dt = t / 10000000.0;
            return (dt);
            /*
            long t2;
            long t1;

            t1 = DateTime.UtcNow.Ticks;
            Console.WriteLine("t1 = {0}", t1);
            t2 = t1;
            while (t2 == t1)
                t2 = DateTime.UtcNow.Ticks;

            Console.WriteLine("t2 = {0}", t2);
            Console.WriteLine("t2 - t1 = {0}", t2 - t1);
             * */
            /*
              t1 = 635924151670437162
              t2 = 635924151670447163
              t2 - t1 = 10001
             */
            
        }
        public static void sleep( double timeInSeconds )
        {
            int milliSeconds;
            //milliSeconds = (int)(timeInSeconds * 1000);
            milliSeconds = (int)(timeInSeconds * 1000.0F);
            System.Threading.Thread.Sleep(milliSeconds);

            //time_in_milliseconds = (uint32_t)(time_in_seconds * 1000.0);
            //Sleep(time_in_milliseconds);

        }

    }
    static class datetime
    {
        public static double now()
        {
            return(10.0F);
        }

    }

    
}

namespace nRFdfu_cs
{

//# Python imports
//import time
//from datetime import datetime, timedelta
//import binascii
//import logging

//# Python 3rd party imports
//from serial import Serial

//# Nordic Semiconductor imports
//from nordicsemi.dfu.util import slip_parts_to_four_bytes, slip_encode_esc_chars, int16_to_bytes, int32_to_bytes
//from nordicsemi.dfu import crc16
//from nordicsemi.exceptions import NordicSemiException
//from nordicsemi.dfu.dfu_transport import DfuTransport, DfuEvent




    class DfuTransportSerial : DfuTransport//_new
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


        const int   DEFAULT_BAUD_RATE = 38400;
        const bool  DEFAULT_FLOW_CONTROL = false;
        const double DEFAULT_SERIAL_PORT_TIMEOUT = 1.0F;//  # Timeout time on serial port read
        const double ACK_PACKET_TIMEOUT = 1.0F;//  # Timeout time for for ACK packet received before reporting timeout through event system
        const double SEND_INIT_PACKET_WAIT_TIME = 1.0F;//  # Time to wait before communicating with bootloader after init packet is sent
        const double SEND_START_DFU_WAIT_TIME = 10.0F;//  # Time to wait before communicating with bootloader after start DFU packet is sent
        const int   DFU_PACKET_MAX_SIZE = 512;//  # The DFU packet max size


        SerialPort serial_port;

        //=====================================================================
        public DfuTransportSerial(String com_port, int baud_rate=DEFAULT_BAUD_RATE, bool flow_control=DEFAULT_FLOW_CONTROL, double timeout=DEFAULT_SERIAL_PORT_TIMEOUT)
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
             
            this.serial_port = new SerialPort( com_port, baud_rate, Parity.None, 8, StopBits.One);
        }

        DfuSerial dfuSerial = null;
        public DfuTransportSerial(DfuSerial dfu_serial)
        {
            this.dfuSerial = dfu_serial;
        }

        //=====================================================================
        public override void open()
        {
            this.dfuSerial.dfuSer_Init_and_open();
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
        }

        //=====================================================================
        public override void close()
        {
            //super(DfuTransportSerial, self).close()
            this.dfuSerial.dfuSer_DeInit_close(); //this.serial_port.Close();
        }

        //=====================================================================
        public override bool is_open()
        {
            //super(DfuTransportSerial, self).is_open();            
            return(this.dfuSerial.dfuSer_is_open() );
          //return(this.serial_port.IsOpen);
        }

        //=====================================================================
        public override bool send_validate_firmware()
        {
            //TODO super(DfuTransportSerial, self).send_validate_firmware()
            return true;
        }

        //=====================================================================
        public override void send_init_packet(Byte[] init_packet)
        {
            //super(DfuTransportSerial, self).send_init_packet(init_packet)
            List<Byte> frame = new List<Byte>();

            Console.Write("DfuTransportSerial->send_init_packet()\n");
            Console.Write("    init_packet length = {0}\n", init_packet.Length);

            UInt16 hardware_version = (UInt16)(init_packet[0] + (init_packet[1] <<8)); //0x0000 //0xFFFF;
            UInt16 hardware_revision = (UInt16)(init_packet[2] + (init_packet[3] << 8));// *(uint16_t*)&init_packet->buffer[2]; //0x0000; //0xFFFF;
            UInt32 application_version = (UInt32)(init_packet[4] + (init_packet[5]<<8) + (init_packet[6]<<16) + (init_packet[7]<<24));// *(uint16_t*)&init_packet->buffer[8]; //0x0001;
            UInt16 softdevice_len = (UInt16)(init_packet[8] + (init_packet[9] << 8));// *(uint16_t*)&init_packet->buffer[8]; //0x0001;
            UInt16 softdevice_array = (UInt16)(init_packet[10] + (init_packet[11] << 8));// *(uint16_t*)&init_packet->buffer[10]; //0xFFFE;
            UInt16 image_crc = (UInt16)(init_packet[12] + (init_packet[13] << 8));// *(uint16_t*)&init_packet->buffer[12];
            //UInt16 pad16 = (UInt16)(init_packet[14] + (init_packet[15] << 8));// * (uint16_t*)&init_packet->buffer[14];
            //UInt16 pad16 = (UInt16)(init_packet[14]);// + (init_packet[15] << 8));// * (uint16_t*)&init_packet->buffer[14];

            Console.Write("     hardware_version    = 0x{0:x4}\n", hardware_version);
            Console.Write("     hardware_revision   = 0x{0:x4}\n", hardware_revision);
            Console.Write("     application_version = 0x{0:x8}\n", application_version );
            Console.Write("     softdevice_len      = 0x{0:x4}\n", softdevice_len);
            Console.Write("     softdevice_array    = 0x{0:x4}\n", softdevice_array);
            Console.Write("     image_crc           = 0x{0:x4}\n", image_crc);
            //Console.Write("     pad16               = 0x{0:x4}\n", pad16);
            Console.Write("\n");


            frame.Clear();
            frame.AddRange( my_dfu_util.int32_to_bytes(DFU_INIT_PACKET) );
            frame.AddRange( init_packet );
            frame.AddRange( my_dfu_util.int16_to_bytes(0x0000) ); // # Padding required

            //my_dfu_util.buf32_print("malloc_frame_DFU_INIT_PACKET", frame.ToArray());

            Byte [] packet = HciPacket(frame.ToArray());
            
            this.self_dot_send_packet(packet);

            my_time.sleep(DfuTransportSerial.SEND_INIT_PACKET_WAIT_TIME);
        }


        //=====================================================================
        public override void send_start_dfu(int program_mode, int softdevice_size, int bootloader_size, int app_size)
        {
            Console.Write("DfuTransportSerial->send_start_dfu()\n");
            Console.Write("    hex_type        = {0}\n", program_mode);
            Console.Write("    softdevice_size = {0}\n", softdevice_size);
            Console.Write("    bootloader_size = {0}\n", bootloader_size);
            Console.Write("    app_size        = {0}\n", app_size);
            Console.Write("\n");

            //super(DfuTransportSerial, self).send_start_dfu(mode, softdevice_size, bootloader_size, app_size)
            List<Byte> frame = new List<Byte>();

            frame.Clear();
            frame.AddRange( my_dfu_util.int32_to_bytes(DFU_START_PACKET) );//, 4);
            frame.AddRange( my_dfu_util.int32_to_bytes((UInt32)program_mode) );//, 4);      //hex_type    
            //frame.AddRange( DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size) );
            frame.AddRange( my_dfu_util.int32_to_bytes((UInt32)softdevice_size) );//, 4 );  // size of SoftDevice    
            frame.AddRange( my_dfu_util.int32_to_bytes((UInt32)bootloader_size) );//, 4 );  // size of BootLoader
            frame.AddRange( my_dfu_util.int32_to_bytes((UInt32)app_size) );//, 4 );         // size of Application


            //my_dfu_util.buf32_print("malloc_frame_DFU_START_PACKET", frame.ToArray());

            //malloc_HciPacket_Set_sequence_number( 0 ); //karel added HCI_PROBLEM
            HciPacket_sequence_number = 0;

            Byte [] packet = HciPacket(frame.ToArray());
            
            this.self_dot_send_packet(packet);

            my_time.sleep(DfuTransportSerial.SEND_START_DFU_WAIT_TIME);
        }


        //=====================================================================
        public override void send_activate_firmware()
        {
            //super(DfuTransportSerial, self).send_activate_firmware()
        }


        int progress_percentage(int part, int whole)
        {
            return ((int)(100.0F * (double)(part)/(double)(whole)) );
        }

        //=====================================================================
        public override void send_firmware(Byte[] firmware)
        {
            int idx;
            //super(DfuTransportSerial, self).send_firmware(firmware)

            //List<Byte> lump = new List<Byte>();
            Byte [] lump4 = new Byte[4];
            Byte [] lump;
            List<Byte []> frames = new List<Byte[]>();

            /*TODO*/
            //this._send_event(DfuEvent.PROGRESS_EVENT, /*progress =*/ 0, /*done =*/ false, /*log_message =*/ "");
            this._send_event(DfuEvent.PROGRESS_EVENT, 0, false, "");

            for( idx = 0 ; idx < firmware.Length; idx += DfuTransportSerial.DFU_PACKET_MAX_SIZE)
            {
                int lump_size;
                lump_size = DfuTransportSerial.DFU_PACKET_MAX_SIZE;
                if( lump_size > (firmware.Length - idx) )
                    lump_size = (firmware.Length - idx);

                lump = new Byte[4 + lump_size];

                lump4 = my_dfu_util.int32_to_bytes(DFU_DATA_PACKET);
                int k;
                for( k=0; k<4; k++)
                    lump[k] = lump4[k];

                for( k=0; k<lump_size; k++)
                    lump[4 + k] = firmware[idx + k];

                //my_dfu_util.buf32_printShort("malloc_DFU_DATA_PACKET_frame", lump);

                //Byte [] data_packet = HciPacket(int32_to_bytes(DFU_DATA_PACKET) + firmware[i:i + DfuTransportSerial.DFU_PACKET_MAX_SIZE]);
                Byte [] data_packet = HciPacket( lump);

                frames.Add(data_packet);
            }
            
            int frames_count = frames.Count;
            
            //# Send firmware packets
            //for count, pkt in enumerate(frames):
            int count;
            for (count = 0; count < frames.Count; count++)
            {
                Byte[] pkt = frames[count];


                this.self_dot_send_packet(pkt);
                //this._send_event(DfuEvent.PROGRESS_EVENT, log_message = "", progress = progress_percentage(count, frames_count), done = false);
                this._send_event(DfuEvent.PROGRESS_EVENT, progress_percentage(count, frames_count), false, "");
            }

            //~~~~~~~~~~~~~~~~~~~~~~~~
            my_time.sleep(1.0); //karel added
            //~~~~~~~~~~~~~~~~~~~~~~~~

            //# Send data stop packet
            Byte [] frame = my_dfu_util.int32_to_bytes(DFU_STOP_DATA_PACKET);

            //my_dfu_util.buf32_print("malloc_frame_DFU_STOP_DATA_PACKET", frame);

            Byte []packet = HciPacket(frame);
            this.self_dot_send_packet(packet);

            //this._send_event(DfuEvent.PROGRESS_EVENT, progress=100, done=false, log_message="")
            this._send_event(DfuEvent.PROGRESS_EVENT, 100, false, "");
        }

        //---------------------------------------------------------------------
        void self_dot_send_packet(Byte [] pkt)
        {
            this.dfuSerial.dfuSer_send_packet(pkt);
            /*TODO
            attempts = 0;
            last_ack = None;
            bool packet_sent = false;
            
            logger.debug("PC -> target: {0}".format(pkt));

            while( packet_sent == false )
            {
                this.serial_port.write(pkt.data);
                attempts += 1;
                ack = this.get_ack_nr();

                if (last_ack is None)
                {
                    break;
                }

                if (ack == (last_ack + 1) % 8)
                {
                    last_ack = ack;
                    packet_sent = True;

                    if( attempts > 3)
                    {
                        ;
//TODO                        raise Exception("Three failed tx attempts encountered on packet {0}".format(pkt.sequence_number));
                    }
                }
            }
   TODO*/
        }
        
        //-------------------
        bool is_timeout(double start_time, double timeout_sec)
        {
            //return (~(datetime.now() - start_time <= timeout_sec));//timedelta(0, timeout_sec)) );
            return (true); //TODO
        }
        //-------------------
        int countNoOf( List<Byte> L, Byte b0)
        {
            int i;
            int cnt = 0;
            for( i=0; i< L.Count(); i++)
            {
                if(L[i] == b0)
                    cnt++;
            }
            return(cnt);
        }


        //---------------------------------------------------------------------
        Byte get_ack_nr()
        {
            List<Byte> uart_buffer = new List<Byte>();
            //Byte [] temp = null;
            Byte [] data = null;

            int intByte;

            double start = datetime.now();

            while( countNoOf(uart_buffer, 0xC0 ) < 2 )
            {
                //# Disregard first of the two C0
                if( serial_port.BytesToRead > 0)
                {
                    intByte = serial_port.ReadByte();
                    uart_buffer.Add( (Byte)intByte );
                }
                //temp = this.serial_port.read(6);
                //if( temp != null)
                //    uart_buffer += temp;
                
                if (is_timeout(start, DfuTransportSerial.ACK_PACKET_TIMEOUT))
                {
                    //# reset HciPacket numbering back to 0
                    HciPacket_sequence_number = 0;
//TODO                    this._send_event(DfuEvent.TIMEOUT_EVENT, log_message="Timed out waiting for acknowledgement from device.");
                    this._send_event(DfuEvent.TIMEOUT_EVENT, 0, false, "Timed out waiting for acknowledgement from device.");

                    //# quit loop
                    break;

                    //# read until you get a new C0
                    //# RESUME_WORK
                }
            }

            if (uart_buffer.Count < 2)
            {
//TODO                raise NordicSemiException("No data received on serial port. Not able to proceed.");
            }
            
            //read until you get a new C0
            //RESUME_WORK
            //logger.debug("PC <- target: {0}".format(binascii.hexlify(uart_buffer)))
            //TODO buf32_print("\nController_get_ack_nr", &uart_buffer);
            data = my_dfu_util.decode_esc_chars(uart_buffer.ToArray());

            // print "non-slip data ", [data]
            return( (Byte)( (data[1] >> 3) & 0x07) );
            /*
             * // Remove 0xC0 at start and beginning
             * data = data[1:-1];
             * // print "non-slip data ", [data]
             * return (data[0] >> 3) & 0x07;
             * */
        }


    

//    }
//    static class HciPacket
//    {
        /*
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
        */
        //"""Class representing a single HCI packet"""

        int HciPacket_sequence_number = 0;

        public Byte [] HciPacket(Byte [] data_in )//=''):
        {
            List<Byte> data_out = new List<Byte>();
            HciPacket_sequence_number = (HciPacket_sequence_number + 1) % 8;
/*TODO
            this.temp_data = '';
            
            this.temp_data += slip_parts_to_four_bytes(HciPacket.sequence_number,
                DATA_INTEGRITY_CHECK_PRESENT,
                RELIABLE_PACKET,
                HCI_PACKET_TYPE,
                len(data));

            this.temp_data += data;
            //# Add escape characters
            crc = crc16.calc_crc16(this.temp_data, crc=0xffff);

            this.temp_data += chr(crc & 0xFF);
            this.temp_data += chr((crc & 0xFF00) >> 8);

            this.temp_data = slip_encode_esc_chars(this.temp_data);

            this.data = chr(0xc0);
            this.data += this.temp_data;
            this.data += chr(0xc0);
TODO*/
            List<Byte> temp_data = new List<byte>();

            temp_data.Clear();

            temp_data.AddRange(my_dfu_util.slip_parts_to_four_bytes( (UInt32)(HciPacket_sequence_number),
                                                                     DATA_INTEGRITY_CHECK_PRESENT,
                                                                     RELIABLE_PACKET,
                                                                     HCI_PACKET_TYPE,
                                                                     (UInt32)(data_in.Length)
                                                                   ) );

            temp_data.AddRange( data_in );

            //# Add escape characters
            //UInt16 crc = crc16.calc_crc16(temp_data.ToArray, crc=0xffff);
            UInt16 crc = my_dfu_util.crc16_compute(temp_data.ToArray(), (UInt16)temp_data.Count, crc = 0xffff);
            

            temp_data.Add( (Byte)( (crc>>0) & 0x00FF) );
            temp_data.Add( (Byte)( (crc>>8) & 0x00FF) );

            Byte[] s_data = my_dfu_util.slip_encode_esc_chars(temp_data.ToArray());

            data_out.Clear();
            data_out.Add(0xc0);
            data_out.AddRange(s_data);
            data_out.Add(0xc0);

            return (data_out.ToArray());
        }

        //def __str__(self):
        //    return binascii.hexlify(this.data)
    }


}



