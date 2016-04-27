using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;

using Nordicsemi; //DisconnectReason

namespace nRFdfu_cs
{
#if false
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
#endif // false
    
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




    class DfuTransportMe : DfuTransportBle//_new
    {
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


        const int DEFAULT_BAUD_RATE = 38400;
        const bool DEFAULT_FLOW_CONTROL = false;
        const double DEFAULT_SERIAL_PORT_TIMEOUT = 1.0F;//  # Timeout time on serial port read
        const double ACK_PACKET_TIMEOUT = 1.0F;//  # Timeout time for for ACK packet received before reporting timeout through event system
        const double SEND_INIT_PACKET_WAIT_TIME = 1.0F;//  # Time to wait before communicating with bootloader after init packet is sent
        const double SEND_START_DFU_WAIT_TIME = 10.0F;//  # Time to wait before communicating with bootloader after start DFU packet is sent
        const int DFU_PACKET_MAX_SIZE = 512;//  # The DFU packet max size
        */


        //MasterEmulator master;
        String emulator_id;

        DfuMaster dfu_master;

        //TODO last_error = DfuErrorCodeBle.SUCCESS;
        bool waiting_for_notification;
        Byte last_sent_opcode = DfuOpcodesBle.INVALID_OPCODE;
        bool received_response;
        int last_error;

        //=====================================================================
        public DfuTransportMe(String peer_device_address, Int32 baud_rate, String emulator_id = "") //, own_address=None, bond_info=None):
            : base() //super(DfuTransportMe, this).__init__()
        {

            dfu_master = new DfuMaster(peer_device_address, baud_rate);//, own_address, bond_info);

            this.emulator_id = emulator_id;
            dfu_master.set_response_callback(this.response_callback);
            dfu_master.set_notification_callback(this.notification_callback);
            dfu_master.set_disconnected_callback(this.disconnected_callback);

            last_error = DfuErrorCodeBle.SUCCESS;
            waiting_for_notification = false;
            last_sent_opcode = DfuOpcodesBle.INVALID_OPCODE;
            received_response = false;
        }


        public override bool get_received_response()
        {
            return this.received_response;
        }

        public override void clear_received_response()
        {
            this.received_response = false;
        }

        public override bool is_waiting_for_notification()
        {
            return this.waiting_for_notification;
        }

        public override void set_waiting_for_notification()
        {
            this.waiting_for_notification = true;
        }

        public override int get_last_error()
        {
            return this.last_error;
        }

        void response_callback(/*DfuOpcodesBle*/ Byte opcode, int error_code)
        {
            String  log_message;
            logger.debug(String.Format("Response received for Request Op Code = {0}, error_code = {1}", opcode, error_code));

            //String status_text = "TODO DfuErrorCodeBle.error_code_lookup(error_code)";
            String status_text = DfuErrorCodeBle.error_code_lookup(error_code);

            if (opcode == DfuOpcodesBle.START_DFU)
                log_message = String.Format("Response for 'Start DFU' received - Status: {0}", status_text);
            else if (opcode == DfuOpcodesBle.INITIALIZE_DFU)
                log_message = String.Format("Response for 'Initialize DFU Params' received - Status: {0}", status_text);
            else if (opcode == DfuOpcodesBle.RECEIVE_FIRMWARE_IMAGE)
                log_message = String.Format("Response for 'Receive FW Data' received - Status: {0}", status_text);
            else if (opcode == DfuOpcodesBle.VALIDATE_FIRMWARE_IMAGE)
                log_message = String.Format("Response for 'Validate' received - Status: {0}", status_text);
            else
                log_message = "Response for Unknown command received.";

            logger.debug(log_message);

            if (error_code != DfuErrorCodeBle.SUCCESS)
            {
                this._send_event(DfuEvent.ERROR_EVENT, 0, false, /*log_message=*/log_message);
                this.last_error = error_code;
            }

            if (this.last_sent_opcode == opcode)
                this.received_response = true;
            else
                logger.debug(String.Format("response_callback opcode mismatch TX-{0} RX-{1}", this.last_sent_opcode, opcode));

            /* TODO
            logging.debug("Response received for Request Op Code = {0}".format(opcode))

            status_text = DfuErrorCodeBle.error_code_lookup(error_code)

            if opcode == DfuOpcodesBle.START_DFU:
                log_message = "Response for 'Start DFU' received - Status: {0}".format(status_text)
            elif opcode == DfuOpcodesBle.INITIALIZE_DFU:
                log_message = "Response for 'Initialize DFU Params' received - Status: {0}".format(status_text)
            elif opcode == DfuOpcodesBle.RECEIVE_FIRMWARE_IMAGE:
                log_message = "Response for 'Receive FW Data' received - Status: {0}".format(status_text)
            elif opcode == DfuOpcodesBle.VALIDATE_FIRMWARE_IMAGE:
                log_message = "Response for 'Validate' received - Status: {0}".format(status_text)
            else:
                log_message = "Response for Unknown command received."

            logging.debug(log_message)

            if error_code != DfuErrorCodeBle.SUCCESS:
                this._send_event(DfuEvent.ERROR_EVENT, log_message=log_message)
                this.last_error = error_code

            if this.last_sent_opcode == opcode:
                this.received_response = true
            */
        }

        void notification_callback()
        {
            if (!this.waiting_for_notification)
            {
                String log_message = "Packet receipt notification received when it is not expected";
                this._send_event(DfuEvent.ERROR_EVENT, 0, false, /*log_message=*/log_message);
                //pass
            }
            else
            {
                logger.debug("Packet receipt notification received.");
                //pass
            }

            this.waiting_for_notification = false;
            /* TODO
            if not this.waiting_for_notification:
                log_message = "Packet receipt notification received when it is not expected"
                this._send_event(DfuEvent.ERROR_EVENT, log_message=log_message)
                pass
            else:
                logging.debug("Packet receipt notification received.")
                pass

            this.waiting_for_notification = false
            */
        }

        void disconnected_callback(DisconnectReason reason) //v_Fn_discon
        {
            logger.debug(String.Format("Device disconnected, reason: {0}", reason));
            /*
            logging.debug("Device disconnected, reason: {0}".format(reason))
            */
        }

        public override void send_packet_data(byte[] data)
        {
            Console.WriteLine("TODO send_packet_data(byte[] data)");

            this.dfu_master.send_packet_data(data);
            /*
            packet = struct.unpack('B'*len(data), data)
            this.dfu_master.send_packet_data(packet)
            */
        }

        public override void send_control_data(Byte opcode, byte[] data)
        {
            Console.WriteLine("dfuTransport_Me: send_control_data(Byte opcode, byte[] data)");

            Byte [] packet = new Byte[1 + data.Length];
            packet[0] = opcode;
            for(int i = 0; i< data.Length; i++)
                packet[1+i] = data[i];
            this.last_sent_opcode = opcode;
            this.dfu_master.send_control_data(packet);

            /*
            packet_data = struct.unpack('B'*len(data), data)
            packet = [opcode]
            packet.extend(packet_data)
            this.last_sent_opcode = opcode
            this.dfu_master.send_control_data(packet)
            */
        }

        //=====================================================================
        public override void open()
        {
            //REF this.dfuSerial.dfuSer_Init_and_open();
            /*REF 
            //super(DfuTransportSerial, this).open()
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

            if (this.is_open())
            {
                return;
            }

            /*TODO*/
            String log_message = "Connecting...";
            
            this._send_event(DfuEvent.PROGRESS_EVENT, /*progress=*/0, /*done=*/ false, /*log_message=*/log_message);

            if( !this.dfu_master.scan_and_connect( /*emulator_filter=*/ this.emulator_id) )
            {
                log_message = "Failed to find or connect to device.";
                this._send_event(DfuEvent.PROGRESS_EVENT, /*progress=*/0,  /*done=*/ true, /*log_message=*/log_message);
                this._send_event(DfuEvent.ERROR_EVENT, /*progress=*/0,  /*done=*/ true, /*log_message=*/log_message);
                this.last_error = DfuErrorCodeBle.OPERATION_FAILED;
            }

            bool pipes_opened = this.dfu_master.open_pipes();

            if(pipes_opened == false)
            {
                log_message = "Device does not have the required DFU service";
                this._send_event(DfuEvent.PROGRESS_EVENT,  /*progress=*/0,  /*done=*/ true, /*log_message=*/log_message);
                this._send_event(DfuEvent.ERROR_EVENT,   /*progress=*/0,  /*done=*/ true, /*log_message=*/log_message);
                this.last_error = DfuErrorCodeBle.INVALID_STATE;
            }
        }

        //=====================================================================
        public override bool is_open()
        {
            return this.dfu_master.connected;
            //REF //super(DfuTransportSerial, this).is_open();            
            //REF return(this.dfuSerial.dfuSer_is_open() );
            //REF //return(this.serial_port.IsOpen);
        }

        //=====================================================================
        public override void close()
        {
            //# wait a second to be able to receive the disconnect event from peer device.
            System.Threading.Thread.Sleep(1000);
            //# Disconnect from peer device if not done already and clean up.
            this.dfu_master.disconnect();
            /*
            //# wait a second to be able to receive the disconnect event from peer device.
            time.sleep(1)
            //# Disconnect from peer device if not done already and clean up.
            this.dfu_master.disconnect();
            */
            //REF super(DfuTransportSerial, this).close()
            //REF this.dfuSerial.dfuSer_DeInit_close(); //this.serial_port.Close();
        }

    }


}



