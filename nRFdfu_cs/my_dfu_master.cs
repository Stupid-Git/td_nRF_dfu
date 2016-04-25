using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Nordicsemi;

namespace nRFdfu_cs
{
    class DfuMaster : MasterEmulator
    {
/*
# Nordic libraries
from Nordicsemi import BtUuid, PipeType, PipeStore
from master_emulator import MasterEmulator
from nordicsemi.dfu import dfu_transport_ble
from nordicsemi.dfu.dfu_transport_ble import DfuOpcodesBle

logging = logging.getLogger(__name__)
*/
        int num_of_send_tries = 1;
        
        //# Helper functions
        byte [] create_byte_array(int size, byte value=0x55)
        {
            //""" Create a IronPython byte array with initial value. """
            return null; //TODO System.Array[System.Byte]([value]*size);
        }




        public delegate void v_Fn_v();

        v_Fn_v response_callback;
        v_Fn_v notification_callback;
        v_Fn_v disconnected_callback;
        
        BtUuid dfu_service_uuid;
        BtUuid dfu_packet_characteristic_uuid;
        BtUuid dfu_control_characteristic_uuid;
    //# BtUuid cccd_descriptor_uuid;


        int pipe_dfu_packet = 0;
        int pipe_dfu_control_point = 0;
        int pipe_dfu_control_point_notify = 0;

        public DfuMaster(String peer_device_address, Int32 baud_rate)
            : base(peer_device_address, baud_rate)//, own_address=None, bond_info=None)
        {
            /**/
            //super(DfuMaster, this).__init__(peer_device_address, baud_rate, own_address, bond_info)

            this.dfu_service_uuid = new BtUuid(DfuTransportBle.UUID_DFU_SERVICE);
            this.dfu_packet_characteristic_uuid = new BtUuid(DfuTransportBle.UUID_DFU_PACKET_CHARACTERISTIC);
            this.dfu_control_characteristic_uuid = new BtUuid(DfuTransportBle.UUID_DFU_CONTROL_STATE_CHARACTERISTIC);
            //# this.cccd_descriptor_uuid = DfuTransportBle.UUID_CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR;

            // this.dfu_service_uuid = new BtUuid(dfu_transport_ble.UUID_DFU_SERVICE);
            // this.dfu_packet_characteristic_uuid = new BtUuid(dfu_transport_ble.UUID_DFU_PACKET_CHARACTERISTIC);
            // this.dfu_control_characteristic_uuid = new BtUuid(dfu_transport_ble.UUID_DFU_CONTROL_STATE_CHARACTERISTIC);
            // //# this.cccd_descriptor_uuid = dfu_transport_ble.UUID_CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR;

            this.response_callback = null;
            this.notification_callback = null;
            this.disconnected_callback = null;
        
            
            this.pipe_dfu_packet = 0;
            this.pipe_dfu_control_point = 0;
            this.pipe_dfu_control_point_notify = 0;        
        }
        

        public void set_notification_callback(v_Fn_v callback_function )
        {
            this.notification_callback = callback_function;
        }

        public void set_response_callback(v_Fn_v callback_function)
        {
            this.response_callback = callback_function;
        }

        public void set_disconnected_callback(v_Fn_v callback_function)
        {
            this.disconnected_callback = callback_function;
        }

        void setup_service()
        {
        /*
        """ Set up DFU service database. """
        # Add DFU Service
        this.master.SetupAddService(this.dfu_service_uuid, PipeStore.Remote)

        # Add DFU characteristics
        this.master.SetupAddCharacteristicDefinition(this.dfu_packet_characteristic_uuid, 2, create_byte_array(2))
        this.pipe_dfu_packet = this.master.SetupAssignPipe(PipeType.Transmit)

        this.master.SetupAddCharacteristicDefinition(this.dfu_control_characteristic_uuid, 2, create_byte_array(2))
        this.pipe_dfu_control_point = this.master.SetupAssignPipe(PipeType.TransmitWithAck)
        this.pipe_dfu_control_point_notify = this.master.SetupAssignPipe(PipeType.Receive)
        */
        }

        public bool open_pipes()
        {
        /*
        try:
            this.master.OpenRemotePipe(this.pipe_dfu_control_point_notify)
        except Exception:
            logging.error("Failed to open pipes.")
            return false

        return true
        */

            try
            {
                this.master.OpenRemotePipe(this.pipe_dfu_control_point_notify);
            }
            catch( Exception ex)
            {
                //logging.error("Failed to open pipes.");
                logger.error("Failed to open pipes.");
                return false;
            }
            return true;
        }

        void send_packet_data(byte [] data)
        {
            /* this.send_data(this.pipe_dfu_packet, System.Array[System.Byte](data), num_of_send_tries); */
        }

        void send_control_data(byte [] data)
        {
            /* this.send_data(this.pipe_dfu_control_point, System.Array[System.Byte](data), num_of_send_tries)   */
            send_data(pipe_dfu_control_point, data, num_of_send_tries);
        }

        void data_received_handler(Object sender, Nordicsemi.PipeDataEventArgs data_event)
        {
        /*
        if data_event.PipeNumber == this.pipe_dfu_control_point_notify:
            op_code = int(data_event.PipeData[0])

            if op_code == DfuOpcodesBle.RESPONSE:
                request_op_code = int(data_event.PipeData[1])
                response_value = int(data_event.PipeData[2])
                this.response_callback(request_op_code, response_value)

            if op_code == DfuOpcodesBle.PKT_RCPT_NOTIF:
                logging.debug("Number of bytes LSB = {0}".format(data_event.PipeData[1]))
                logging.debug("Number of bytes MSB = {0}".format(data_event.PipeData[2]))
                this.notification_callback()

        else:
            logging.debug("Received data on unexpected pipe {0}".format(e.PipeNumber))
        */
        }

        //void meEv_OnDisconnected(object sender, ValueEventArgs<DisconnectReason> arguments)
        void disconnected_handler(Object sender, ValueEventArgs<DisconnectReason> e)
        {
        /*
        # Set disconnect_event_expected to true to avoid logging "Error: Unexpected disconnection
        # event!" when disconnecting for buttonless DFU.
        this.disconnect_event_expected = true
        super(DfuMaster, this).disconnected_handler(sender, e)
        this.disconnected_callback(e.Value)
        */
        }
    }
}
