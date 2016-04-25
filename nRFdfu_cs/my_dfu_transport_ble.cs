using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace nRFdfu_cs
{
/*    
# Python standard library
from time import sleep
from datetime import datetime, timedelta
import abc
import logging

# Nordic libraries
from nordicsemi.exceptions import NordicSemiException, IllegalStateException
from nordicsemi.dfu.util import int16_to_bytes
from nordicsemi.dfu.dfu_transport import DfuTransport, DfuEvent

logger = logging.getLogger(__name__)
*/

    //# BLE DFU OpCodes :
    enum DfuOpcodesBle
    {
    //""" DFU opcodes used during DFU communication with bootloader
    //
    //    See http://developer.nordicsemi.com/nRF51_SDK/doc/7.2.0/s110/html/a00949.html#gafa9a52a3e6c43ccf00cf680f944d67a3
    //    for further information
    //"""
    INVALID_OPCODE = 0,
    START_DFU = 1,
    INITIALIZE_DFU = 2,
    RECEIVE_FIRMWARE_IMAGE = 3,
    VALIDATE_FIRMWARE_IMAGE = 4,
    ACTIVATE_FIRMWARE_AND_RESET = 5,
    SYSTEM_RESET = 6,
    REQ_PKT_RCPT_NOTIFICATION = 8,
    RESPONSE = 16,
    PKT_RCPT_NOTIF = 17,
    }


    public class DfuErrorCodeBle
    {
        //""" DFU error code used during DFU communication with bootloader
        //
        //    See http://developer.nordicsemi.com/nRF51_SDK/doc/7.2.0/s110/html/a00949.html#gafa9a52a3e6c43ccf00cf680f944d67a3
        //    for further information
        //"""

        public static int SUCCESS = 1;
        public static int INVALID_STATE = 2;
        public static int NOT_SUPPORTED = 3;
        public static int DATA_SIZE_EXCEEDS_LIMIT = 4;
        public static int CRC_ERROR = 5;
        public static int OPERATION_FAILED = 6;

        //@staticmethod
        static void error_code_lookup(DfuErrorCodeBle error_code)
        {
        //"""
        //Returns a description lookup table for error codes received from peer.
        //
        //:param int error_code: Error code to parse
        //:return str: Textual description of the error code
        //"""
        /*
        code_lookup = {DfuErrorCodeBle.SUCCESS: "SUCCESS",
                       DfuErrorCodeBle.INVALID_STATE: "Invalid State",
                       DfuErrorCodeBle.NOT_SUPPORTED: "Not Supported",
                       DfuErrorCodeBle.DATA_SIZE_EXCEEDS_LIMIT: "Data Size Exceeds Limit",
                       DfuErrorCodeBle.CRC_ERROR: "CRC Error",
                       DfuErrorCodeBle.OPERATION_FAILED: "Operation Failed"}

        return code_lookup.get(error_code, "UNKOWN ERROR CODE")
        */
        }
    }



    class DfuTransportBle : DfuTransport
    {
        
        //# Service UUID. For further information, look at the nRF51 SDK documentation V7.2.0:
        //# http://developer.nordicsemi.com/nRF51_SDK/doc/7.2.0/s110/html/a00071.html#ota_spec_number
        public const String UUID_DFU_SERVICE = "000015301212EFDE1523785FEABCD123";
        //# Characteristic UUID
        public const String UUID_DFU_PACKET_CHARACTERISTIC = "000015321212EFDE1523785FEABCD123";
        public const String UUID_DFU_CONTROL_STATE_CHARACTERISTIC = "000015311212EFDE1523785FEABCD123";
        //# Descriptor UUID
        public const UInt16 UUID_CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR = 0x2902;

        //# NOTE:  If packet receipt notification is enabled, a packet receipt
        //#        notification will be received for each 'num_of_packets_between_notif'
        //#        number of packets.
        //#
        //# Configuration tip: Increase this to get lesser notifications from the DFU
        //# Target about packet receipts. Make it 0 to disable the packet receipt
        //# notification

        int NUM_OF_PACKETS_BETWEEN_NOTIF = 10;
        int DATA_PACKET_SIZE = 20;

        void __init__()
        {
        /*
        super(DfuTransportBle, this).__init__()
        */
        }

        public override void open()
        {
        /*
        super(DfuTransportBle, this).open()
        */
        }

        public override bool is_open()
        {
            return (true); //TODO
        /*

        return super(DfuTransportBle, this).is_open()
        */
        }

        public override void close()
        {
        /*
        super(DfuTransportBle, this).close()
        */
        }

        void _wait_for_condition(int condition_function, bool expected_condition_value=true, int timeout=10, string  waiting_for="condition") //TODO params
        {
        /*
        """
        Waits for condition_function to be true
        Will timeout after 60 seconds

        :param function condition_function: The function we are waiting for to return true
        :param str timeout_message: Message that should be logged
        :return:
        """

        start_time = datetime.now()

        while condition_function() != expected_condition_value:
            timeout_message = "Timeout while waiting for {0}.".format(waiting_for)
            timed_out = datetime.now() - start_time > timedelta(0, timeout)
            if timed_out:
                this._send_event(DfuEvent.TIMEOUT_EVENT, log_message=timeout_message)
                raise NordicSemiException(timeout_message)

            if not this.is_open():
                log_message = "Disconnected from device while waiting for {0}.".format(waiting_for)
                raise IllegalStateException(log_message)

            sleep(0.1)

        if this.get_last_error() != DfuErrorCodeBle.SUCCESS:
            error_message = "Error occoured while waiting for {0}. Error response {1}."
            error_code = DfuErrorCodeBle.error_code_lookup(this.get_last_error())
            error_message = error_message.format(waiting_for, error_code)
            this._send_event(DfuEvent.ERROR_EVENT, log_message=error_message)
            raise NordicSemiException(error_message)
        */
        }

//    @abc.abstractmethod
        void send_packet_data(byte [] data)
        {
        /*
        """
        Send data to the packet characteristic

        :param str data: The data to be sent
        :return:
        """
        pass
        */
        }

//    @abc.abstractmethod
        void send_control_data(int opcode, byte [] data) //TODO params
        {
        /*
        """
        Send data to the control characteristic

        :param int opcode: The opcode for the operation command sent to the control characteristic
        :param str data: The data to be sent
        :return:
        """
        pass
        */
        }

//    @abc.abstractmethod
        void get_received_response()
        {
        /*
        """
        Returns True if the transport layer has received a response it expected

        :return: bool
        """
        pass

        void clear_received_response()
        """
        Clears the received response status, sets it to False.

        :return:
        """
        pass
        */
        }

//    @abc.abstractmethod
        void is_waiting_for_notification()
        {
        /*
        """
        Returns True if the transport layer is waiting for a notification

        :return: bool
        """
        pass
        */
        }

        void set_waiting_for_notification()
        {
        /*
        """
        Notifies the transport layer that it should wait for notification

        :return:
        """
        pass
        */
        }

//    @abc.abstractmethod
        void get_last_error()
        {
        /*
        """
        Returns the last error code

        :return: DfuErrorCodeBle
        """
        pass
        */
        }

        void _start_dfu(int program_mode, int image_size_packet) //TODO params
        {
        /*
        logger.debug("Sending 'START DFU' command")
        this.send_control_data(DfuOpcodesBle.START_DFU, chr(program_mode))
        logger.debug("Sending image size")
        this.send_packet_data(image_size_packet)
        this._wait_for_condition(this.get_received_response, waiting_for="response for START DFU")
        this.clear_received_response()
        */
        }

        public override void send_start_dfu(int program_mode, int softdevice_size = 0, int bootloader_size = 0, int app_size = 0)
        {
        /*
        super(DfuTransportBle, this).send_start_dfu(program_mode, softdevice_size, bootloader_size, app_size)
        image_size_packet = DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size)

        this._send_event(DfuEvent.PROGRESS_EVENT, progress=0, log_message="Setting up transfer...")

        try:
            this._start_dfu(program_mode, image_size_packet)
        except IllegalStateException:
            # We got disconnected. Try to send Start DFU again in case of buttonless dfu.
            this.close()
            this.open()

            if not this.is_open():
                raise IllegalStateException("Failed to reopen transport backend.")

            this._start_dfu(program_mode, image_size_packet)
        */
        }

        public override void send_init_packet(byte[] init_packet)
        {
        /*
        super(DfuTransportBle, this).send_init_packet(init_packet)
        init_packet_start = chr(0x00)
        init_packet_end = chr(0x01)

        logger.debug("Sending 'INIT DFU' command")
        this.send_control_data(DfuOpcodesBle.INITIALIZE_DFU, init_packet_start)

        logger.debug("Sending init data")
        for i in range(0, len(init_packet), DATA_PACKET_SIZE):
            data_to_send = init_packet[i:i + DATA_PACKET_SIZE]
            this.send_packet_data(data_to_send)

        logger.debug("Sending 'Init Packet Complete' command")
        this.send_control_data(DfuOpcodesBle.INITIALIZE_DFU, init_packet_end)
        this._wait_for_condition(this.get_received_response, timeout=60, waiting_for="response for INITIALIZE DFU")
        this.clear_received_response()

        if NUM_OF_PACKETS_BETWEEN_NOTIF:
            packet = int16_to_bytes(NUM_OF_PACKETS_BETWEEN_NOTIF)
            logger.debug("Send number of packets before device sends notification")
            this.send_control_data(DfuOpcodesBle.REQ_PKT_RCPT_NOTIFICATION, packet)
        */
        }

        public override void send_firmware(byte[] firmware)
        {
        /*
            void progress_percentage(part, complete):
            """
                Calculate progress percentage
                :param int part: Part value
                :param int complete: Completed value
                :return: int: Percentage complete
                """
            return min(100, (part + DATA_PACKET_SIZE) * 100 / complete)

        super(DfuTransportBle, this).send_firmware(firmware)
        packets_sent = 0
        last_progress_update = -1  # Last packet sequence number when an update was fired to the event system
        bin_size = len(firmware)
        logger.debug("Send 'RECEIVE FIRMWARE IMAGE' command")
        this.send_control_data(DfuOpcodesBle.RECEIVE_FIRMWARE_IMAGE)

        for i in range(0, bin_size, DATA_PACKET_SIZE):
            progress = progress_percentage(i, bin_size)

            if progress != last_progress_update:
                this._send_event(DfuEvent.PROGRESS_EVENT, progress=progress, log_message="Uploading firmware")
                last_progress_update = progress

            this._wait_for_condition(this.is_waiting_for_notification, expected_condition_value=False,
                                     waiting_for="notification from device")

            data_to_send = firmware[i:i + DATA_PACKET_SIZE]

            log_message = "Sending Firmware bytes [{0}, {1}]".format(i, i + len(data_to_send))
            logger.debug(log_message)

            packets_sent += 1

            if NUM_OF_PACKETS_BETWEEN_NOTIF != 0:
                if (packets_sent % NUM_OF_PACKETS_BETWEEN_NOTIF) == 0:
                    this.set_waiting_for_notification()

            this.send_packet_data(data_to_send)

        this._wait_for_condition(this.get_received_response, waiting_for="response for RECEIVE FIRMWARE IMAGE")
        this.clear_received_response()
        */
        }

        public override bool send_validate_firmware()
        {
            return (false);
            /*
            super(DfuTransportBle, this).send_validate_firmware()
            logger.debug("Sending 'VALIDATE FIRMWARE IMAGE' command")
            this.send_control_data(DfuOpcodesBle.VALIDATE_FIRMWARE_IMAGE)
            this._wait_for_condition(this.get_received_response, waiting_for="response for VALIDATE FIRMWARE IMAGE")
            this.clear_received_response()
            logger.info("Firmware validated OK.")
            */
        }

        public override void send_activate_firmware()
        {
        /*
        super(DfuTransportBle, this).send_activate_firmware()
        logger.debug("Sending 'ACTIVATE FIRMWARE AND RESET' command")
        this.send_control_data(DfuOpcodesBle.ACTIVATE_FIRMWARE_AND_RESET)
        */
        }

    }
}
