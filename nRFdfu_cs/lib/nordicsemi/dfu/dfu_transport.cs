using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace nRFdfu_cs
{
    //public class DfuEvent
    //{
    //    static public int PROGRESS_EVENT = 1;
    //    static public int TIMEOUT_EVENT = 2;
    //    static public int ERROR_EVENT = 3;
    //}
    public enum DfuEvent
    {
        PROGRESS_EVENT = 1,
        TIMEOUT_EVENT = 2,
        ERROR_EVENT = 3,
    }


    abstract class DfuTransport//_new
    {
        /*
         * This class as an abstract base class inherited from when implementing transports.
         * 
         * The class is generic in nature, the underlying implementation may have missing semantic
         * than this class describes. But the intent is that the implementer shall follow the semantic as
         * best as she can.
         */

        //__metaclass__ = abc.ABCMeta

        //@staticmethod
        public static Byte[] create_image_size_packet(int softdevice_size = 0, int bootloader_size = 0, int app_size = 0)
        {
            /*
             * Creates an image size packet necessary for sending start dfu.
             * 
             * @param softdevice_size: Size of SoftDevice firmware
             * @type softdevice_size: int
             * @param bootloader_size: Size of bootloader firmware
             * @type softdevice_size: int
             * @param app_size: Size of application firmware
             * :return: The image size packet
             * :rtype: str
             */
            //TODO Byte [] softdevice_size_packet = int32_to_bytes(softdevice_size);
            //TODO Byte [] bootloader_size_packet = int32_to_bytes(bootloader_size);
            //TODO Byte [] app_size_packet = int32_to_bytes(app_size);
          //  Byte[] image_size_packet = null; //TODO softdevice_size_packet + bootloader_size_packet + app_size_packet;
          //  return (image_size_packet);

            List<Byte> frame = new List<Byte>();

            frame.Clear();
            //frame.AddRange(my_dfu_util.int32_to_bytes(DFU_START_PACKET));//, 4);
            //frame.AddRange(my_dfu_util.int32_to_bytes((UInt32)program_mode));//, 4);      //hex_type    
            //frame.AddRange( DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size) );
            frame.AddRange(my_dfu_util.int32_to_bytes((UInt32)softdevice_size));//, 4 );  // size of SoftDevice    
            frame.AddRange(my_dfu_util.int32_to_bytes((UInt32)bootloader_size));//, 4 );  // size of BootLoader
            frame.AddRange(my_dfu_util.int32_to_bytes((UInt32)app_size));//, 4 );         // size of Application

            Byte[] image_size_packet = frame.ToArray(); // == softdevice_size_packet + bootloader_size_packet + app_size_packet;
            return (image_size_packet);

        }

        /*
        @abc.abstractmethod
        def __init__(self):
           self.callbacks = {}
        */

        public abstract void open();
        public abstract void close();
        public abstract bool is_open();

        /*
    @abc.abstractmethod
    def send_start_dfu(self, program_mode, softdevice_size=0, bootloader_size=0, app_size=0):
        """
        Send packet to initiate DFU communication. Returns when packet is sent or timeout occurs.

        This call will block until packet is sent.
        If timeout or errors occurs exception is thrown.

        :param nordicsemi.dfu.model.HexType program_mode: Type of firmware to upgrade
        :param int softdevice_size: Size of softdevice firmware
        :param int bootloader_size: Size of bootloader firmware
        :param int app_size: Size of application firmware
        :return:
        """
        pass
        */
        public abstract void send_start_dfu(int program_mode, int softdevice_size = 0, int bootloader_size = 0, int app_size = 0);

        /*
    @abc.abstractmethod
    def send_init_packet(self, init_packet):
        """
        Send init_packet to device.

        This call will block until init_packet is sent and transfer of packet is complete.
        If timeout or errors occurs exception is thrown.

        :param str init_packet: Init packet as a str.
        :return:
        """
        pass
        */
        public abstract void send_init_packet(Byte[] init_packet);

        /*
    @abc.abstractmethod
    def send_firmware(self, firmware):
        """
        Start sending firmware to device.

        This call will block until transfer of firmware is complete.
        If timeout or errors occurs exception is thrown.

        :param str firmware:
        :return:
        """
        pass
        */
        public abstract void send_firmware(Byte[] firmware);

        /*
    @abc.abstractmethod
    def send_validate_firmware(self):
        """
        Send request to device to verify that firmware has been correctly transferred.

        This call will block until validation is sent and validation is complete.
        If timeout or errors occurs exception is thrown.

        :return bool: True if firmware validated successfully.
        """
        pass
        */
        public abstract bool send_validate_firmware();

        /*
    @abc.abstractmethod
    def send_activate_firmware(self):
        """
        Send command to device to activate new firmware and restart the device.
        The device will start up with the new firmware.

        Raises an nRFException if anything fails.

        :return:
        """
        pass
        */
        public abstract void send_activate_firmware();

        /*
    def register_events_callback(self, event_type, callback):
        """
        Register a callback.

        :param DfuEvent callback:
        :return: None
        """
        if event_type not in self.callbacks:
            self.callbacks[event_type] = []

        self.callbacks[event_type].append(callback)
        */

        /*
    def unregister_events_callback(self, callback):
        """
        Unregister a callback.

        :param callback: # TODO: add documentation for callback
        :return: None
        """
        for event_type in self.callbacks.keys():
            if callback in self.callbacks[event_type]:
                self.callbacks[event_type].remove(callback)
        */

        /*
    def _send_event(self, event_type, **kwargs):
        """
        Method for sending events to registered callbacks.

        If callbacks throws exceptions event propagation will stop and this method be part of the track trace.

        :param DfuEvent event_type:
        :param args: Arguments to callback function
        :return:
        """
        if event_type in self.callbacks.keys():
            for callback in self.callbacks[event_type]:
                callback(**kwargs)
        */

        public void _send_event(DfuEvent event_type, int progress, bool done, String log_message)
        {
            /*
            Method for sending events to registered callbacks.

            If callbacks throws exceptions event propagation will stop and this method be part of the track trace.

            :param DfuEvent event_type:
            :param args: Arguments to callback function
            :return:
            */

            if (event_type == DfuEvent.PROGRESS_EVENT)
            {
                logger.update_progress(progress, false, "");
            }
            if (event_type == DfuEvent.ERROR_EVENT)
            {
                logger.error(log_message);
            }
            if (event_type == DfuEvent.TIMEOUT_EVENT)
            {
                logger.error(log_message);
            }
            /* TODO put in dictionary !?!!!
            if( event_type in self.callbacks.keys() )
            {
                foreach( var callback in callbacks[event_type] )
                {
                    callback(**kwargs);
                }
            }
            */
        }


        /*
      //public delegate void update_progress_handler_del(int progress = 0, bool done = false, String log_message = "");
        public delegate void update_progress_handler_del(int progress, bool done, String log_message);
        update_progress_handler_del callback_update_progress = null;

        public delegate void timeout_event_handler_del(int progress, bool done, String log_message);
        timeout_event_handler_del callback_timeout_event = null;

        public delegate void error_event_handler_del(int progress, bool done, String log_message);
        error_event_handler_del callback_error_event = null;
        */

        public delegate void generic_event_handler_del(int progress, bool done, String log_message);
        generic_event_handler_del callback_update_progress = null;
        generic_event_handler_del callback_timeout_event = null;
        generic_event_handler_del callback_error_event = null;

        public void register_events_callback(DfuEvent event_type, generic_event_handler_del callback)
        {
            if (event_type == DfuEvent.PROGRESS_EVENT)
            {
                this.callback_update_progress = callback;
            }
            if (event_type == DfuEvent.ERROR_EVENT)
            {
                this.callback_error_event = callback;
            }
            if (event_type == DfuEvent.TIMEOUT_EVENT)
            {
                this.callback_timeout_event = callback;
            }
        }
    }


}
