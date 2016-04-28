using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO; //new BinaryReader File.Open 

namespace nRFdfu_cs
{
    public class InitPacketData
    {
        /*
        The InitPacketData data model.
            :param int packet_version: init packet version
            :param int compression_type: init packet compression type
            :param int device_type:  device type
            :param int device_revision: device revision
            :param int application_version:  application version
            :param list softdevice_req: softdevice requirements
            :param str firmware_hash: firmware hash
            :param int firmware_crc16: firmware CRC-16 calculated value
            :return: InitPacketData
            """
        */
        public int packet_version;      // init packet version
        public int compression_type;    // init packet compression type
        public int device_type;         // device type
        public int device_revision;     // device revision
        public int application_version; // application version
        public int[] softdevice_req;//[2];     // softdevice requirements
        public String/* char* */ firmware_hash;     // firmware hash
        public int firmware_crc16;      // firmware CRC-16 calculated value

        public InitPacketData()
        {
            softdevice_req = new int[2]; //??

        }
    }



    public class Firmware
    {

        public String bin_file;//=None,
        public String dat_file;//=None,
        public InitPacketData init_packet_data;
        public int sd_size; //BL + SD
        public int bl_size; //BL + SD

        /*
         * The firmware datamodel
         * 
         * :param str bin_file: Firmware binary file
         * :param str dat_file: Firmware .dat file (init packet for Nordic DFU)
         * :param dict init_packet_data:  Initial packet data
         * :return:
         */

        public Firmware(String bin_file = null, String dat_file = null, InitPacketData init_packet_data = null)
        {
            this.dat_file = dat_file;
            this.bin_file = bin_file;

            //TODO if(init_packet_data != null)
            //TODO     this.init_packet_data = InitPacketData(**init_packet_data);
        }
    }

    public class Manifest
    {

        public Firmware softdevice_bootloader;
        public Firmware softdevice;
        public Firmware bootloader;
        public Firmware application;

        public int app_size;
    }

    //import abc
    //import logging

    //# Nordic Semiconductor imports
    //from nordicsemi.dfu.util import int32_to_bytes

    //logger = logging.getLogger(__name__)

/* model.c   public class HexType
    {
        public static int  SOFTDEVICE = 1;
        public static int  BOOTLOADER = 2;
        public static int  SD_BL = 3;
        public static int  APPLICATION = 4;
    }*/

    
    public class FakeManifest
    {
        /*
        --manifest.json-- file
        {
            "manifest": {
                "application": {
                    "bin_file": "nrf51422_xxac_s110.bin",
                    "dat_file": "nrf51422_xxac_s110.dat",
                    "init_packet_data": {
                        "application_version": 0,             --> uint32_t application_version = 0x00000000;
                        "device_revision": 0,                 --> uint16_t hardware_revision   = 0x0000; 
                        "device_type": 0,                     --> uint16_t hardware_version    = 0x0000;
                        "firmware_crc16": 6302,               --> uint16_t image_crc           = 0x189e; --> decimal 6302
                        "softdevice_req": [                   --> uint16_t softdevice_len      = 0x0001; --> one entry in the array
                            65534                             --> uint16_t softdevice_array    = 0xFFFE;
                        ]
                    }
                }
            }
        }
        */

        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        //
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        static public Manifest myFakeManifest_fromFiles(String binFileName, String datFileName)
        {
            Manifest pm = new Manifest();

            pm = myFakeManifest();

            pm.application.bin_file = binFileName; // "dfu_temp.bin";  //"nrf51422_xxac_s110.bin";
            pm.application.dat_file = datFileName; // "dfu_temp.dat"; //"nrf51422_xxac_s110.dat";


            return(pm);
        }

        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        //
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        //Manifest FakeManifest;
        static public Manifest myFakeManifest()
        {
            Manifest pm = new Manifest();

            //
            pm.app_size = 0;

            /*
            --manifest.json-- file
            {
                "manifest": {
                    "application": {
                    }
                }
            }
            */

            pm.bootloader = null;  //  NO Bootloader <- "manifest": { "application": 
            pm.softdevice = null;  //  NO Softdevice <- "manifest": { "application": 
            pm.softdevice_bootloader = null;  //  NO Softdevice_Bootloader <- "manifest": { "application": 

            pm.application = new Firmware();// &FakeApplication;
            //

            //
            pm.application.bin_file = "dfu_temp.bin";  //"nrf51422_xxac_s110.bin";
            pm.application.dat_file = "dfu_temp.dat"; //"nrf51422_xxac_s110.dat";
            pm.application.init_packet_data = new InitPacketData();
            pm.application.bl_size = 0;
            pm.application.sd_size = 0;
            //

            //
            //        "init_packet_data": {

            //            "application_version": 0,             -> uint32_t application_version = 0x00000000;
            pm.application.init_packet_data.application_version = 0x00000000;

            //            "device_revision": 0,                 --> uint16_t hardware_revision   = 0x0000; 
            pm.application.init_packet_data.device_revision = 0x0000;

            //            "device_type": 0,                     --> uint16_t hardware_version    = 0x0000;
            pm.application.init_packet_data.device_type = 0x0000;

            //            "firmware_crc16": 6302,               --> uint16_t image_crc           = 0x189e; --> decimal 6302
            pm.application.init_packet_data.firmware_crc16 = 0x189e;
            //            "softdevice_req": [                   --> uint16_t softdevice_len      = 0x0001; --> one entry in the array
            //                65534                             --> uint16_t softdevice_array    = 0xFFFE;
            pm.application.init_packet_data.softdevice_req[0] = 0xFFFE;
            pm.application.init_packet_data.softdevice_req[1] = 0;
            //            ]
            //        }
            pm.application.init_packet_data.compression_type = 0;
            pm.application.init_packet_data.firmware_hash = "";
            pm.application.init_packet_data.packet_version = 0;

            //

            return (pm);
        }
    }

    class Dfu //my_dfu
    {
        String os_path_join(String d1, String d2)
        {
            String path;
            //int i1;
            //int i2;
            //int len;
            //int len1;
            //int len2;

            //len1 = d1.Length;//strlen(d1);
            //len2 = d2.Length;//strlen(d2);
            //len = len1 + len2;

            //path = new String(...);//(char*)malloc( len + 1 );
            //path[0] = 0;

            //copy
            //for( i1=0; i1<len1; i1++)
            //    path[i1] = d1[i1];
            //copy
            //for( i2=0; i2<len2; i2++)
            //    path[i1+i2] = d2[i2];    
            //path[i1+i2] = 0;

            path = d1 + d2;

            return (path);
        }

        //import os
        //import tempfile
        //import shutil
        //import logging
        //from time import time, sleep
        //from datetime import datetime, timedelta

        //# Nordic libraries
        //from nordicsemi.exceptions import *
        //from nordicsemi.dfu.package import Package
        //from nordicsemi.dfu.dfu_transport import DfuEvent
        //from nordicsemi.dfu.model import HexType
        //from nordicsemi.dfu.manifest import SoftdeviceBootloaderFirmware

        //logger = logging.getLogger(__name__)


        DfuTransport dfu_transport;
        String zip_file_path;
        bool ready_to_send;
        int response_opcode_received;

        String temp_dir;
        String unpacked_zip_path;

        Manifest manifest;

        //---------------------------------------------------------------------
        // This overload is the standard python style overload
        public Dfu(String zip_file_path, DfuTransport dfu_transport) //__init__(this, zip_file_path, dfu_transport):
        {
            /*
             * Initializes the dfu upgrade, unpacks zip and registers callbacks.
             * 
             * @param zip_file_path: Path to the zip file with the firmware to upgrade
             * @type zip_file_path: str
             * @param dfu_transport: Transport backend to use to upgrade
             * @type dfu_transport: nordicsemi.dfu.dfu_transport.DfuTransport
             * @return
             */

            this.zip_file_path = zip_file_path;
            this.ready_to_send = true;
            this.response_opcode_received = 0;//None;
#if false
            this.temp_dir = "TODO"; //tempfile.mkdtemp(prefix="nrf_dfu_");
            this.unpacked_zip_path = "TODO"; //os.path.join(this.temp_dir, 'unpacked_zip');
            this.manifest = null;//"TODO"; //Package.unpack_package(this.zip_file_path, this.unpacked_zip_path);
#else
            this.temp_dir = "./";
            this.unpacked_zip_path = "./";
            this.manifest = FakeManifest.myFakeManifest();//"TODO"; //Package.unpack_package(this.zip_file_path, this.unpacked_zip_path);           
#endif

            if (dfu_transport != null)
                this.dfu_transport = dfu_transport;

            this.dfu_transport.register_events_callback(DfuEvent.TIMEOUT_EVENT, this.timeout_event_handler);
            this.dfu_transport.register_events_callback(DfuEvent.ERROR_EVENT, this.error_event_handler);
        }

        //---------------------------------------------------------------------
        // This overload takes the ".bin" and ".dat" files rather than the "package"
        // This is only for programming the application, for which a fake manifest
        // is created
        public Dfu(String binFileName, String datFileName, DfuTransport dfu_transport) //__init__(this, zip_file_path, dfu_transport):
        {
            /*
             * Initializes the dfu upgrade, unpacks zip and registers callbacks.
             * 
             * @param zip_file_path: Path to the zip file with the firmware to upgrade
             * @type zip_file_path: str
             * @param dfu_transport: Transport backend to use to upgrade
             * @type dfu_transport: nordicsemi.dfu.dfu_transport.DfuTransport
             * @return
             */

            this.zip_file_path = "Unused zip_file_path";
            this.ready_to_send = true;
            this.response_opcode_received = 0;//None;
#if false
            this.temp_dir = "TODO"; //tempfile.mkdtemp(prefix="nrf_dfu_");
            this.unpacked_zip_path = "TODO"; //os.path.join(this.temp_dir, 'unpacked_zip');
            this.manifest = null;//"TODO"; //Package.unpack_package(this.zip_file_path, this.unpacked_zip_path);
#else
            this.temp_dir = "./";
            this.unpacked_zip_path = "./";
            //this.manifest = FakeManifest.myFakeManifest();//"TODO"; //Package.unpack_package(this.zip_file_path, this.unpacked_zip_path);           
            this.manifest = FakeManifest.myFakeManifest_fromFiles(binFileName, datFileName);//"TODO"; //Package.unpack_package(this.zip_file_path, this.unpacked_zip_path);           
#endif

            if (dfu_transport != null)
                this.dfu_transport = dfu_transport;

            this.dfu_transport.register_events_callback(DfuEvent.TIMEOUT_EVENT, this.timeout_event_handler);
            this.dfu_transport.register_events_callback(DfuEvent.ERROR_EVENT, this.error_event_handler);
        }
        //void ~Dfu() // __del__(this):
        //{
        /*
         * Destructor removes the temporary directory for the unpacked zip
         * :return:
         */
        //shutil.rmtree(this.temp_dir)
        //}

        public void error_event_handler(int progress, bool done, String log_message)
        {
            /*
             * Event handler for errors, closes the transport backend.
             * :param str log_message: The log message for the error.
             * :return:
             */
            if (this.dfu_transport.is_open())
                this.dfu_transport.close();

            logger.error(log_message);
        }

        public void timeout_event_handler(int progress, bool done, String log_message)
        {
            /*
             * Event handler for timeouts, closes the transport backend.
             * :param log_message: The log message for the timeout.
             * :return:
             */
            if (this.dfu_transport.is_open())
                this.dfu_transport.close();

            logger.error(log_message);
        }

        //@staticmethod
        static Byte[] _read_file(String file_path)
        {
            /*
             * Reads a file and returns the content as a string.
             * 
             * :param str file_path: The path to the file to read.
             * :return str: Content of the file.
             */
            Byte[] file_content = null;// = "";

            // 1.
            using (BinaryReader b = new BinaryReader(File.Open(file_path, FileMode.Open)))
            {
                // 2.
                // Position and length variables.
                int pos = 0;
                // 2A.
                // Use BaseStream.
                int length = (int)b.BaseStream.Length;
                file_content = new Byte[length];
                while (pos < length)
                {
                    file_content[pos] = b.ReadByte();  // SLOW !!! but 
                    pos++;
                    /*
                    // 3.
                    // Read integer.
                    int v = b.ReadInt32();
                    Console.WriteLine(v);

                    // 4.
                    // Advance our position variable.
                    pos += sizeof(int);
                    */
                }
            }
            return (file_content);
        }

        //void _wait_while_opening_transport()
        int _wait_while_opening_transport()
        {
            /*TODO
            double timeout = 10;
            start_time = datetime.now();

            while( false == this.dfu_transport.is_open())
            {
                timed_out = datetime.now() - start_time > timedelta(0, timeout);

                if(timed_out)
                {
                    log_message = "Failed to open transport backend";
                    raise NordicSemiException(log_message);
                }

                sleep(0.1);
            }
            TODO*/

            double timeout = 10.0F;
            bool timed_out = false;
            //start_time = datetime.now();

            while (false == this.dfu_transport.is_open())
            {
                //timed_out = datetime.now() - start_time > timedelta(0, timeout)
                if (timeout <= 0.0)
                    timed_out = true;

                if (timed_out == true)
                {
                    //log_message = "Failed to open transport backend"
                    //raise NordicSemiException(log_message)
                    Console.Write("Failed to open transport backend\n");
                    return (1);
                }
                System.Threading.Thread.Sleep(100); //sleep(0.1)
                timeout -= 0.1F; //karel
            }
            return (0);
        }

        void _dfu_send_image(int program_mode, Firmware firmware_manifest)
        {
            /*
             * Does DFU for one image. Reads the firmware image and init file.
             * Opens the transport backend, calls setup, send and finalize and closes the backend again.
             * @param program_mode: What type of firmware the DFU is
             * @type program_mode: nordicsemi.dfu.model.HexType
             * @param firmware_manifest: The manifest for the firmware image
             * @type firmware_manifest: nordicsemi.dfu.manifest.Firmware
             * @return:
             */

            
            if( firmware_manifest == null )
            {
                //raise MissingArgumentException("firmware_manifest must be provided.");
                Console.WriteLine("firmware_manifest must be provided.");
                return;
            }

            if( this.dfu_transport.is_open() )
            {
                //raise IllegalStateException("Transport is already open.");
                Console.WriteLine("Transport is already open.");
                return;
            }


            this.dfu_transport.open(); //TODO this initialisation (e.g. put RTR500BLE into program mode [SET_DEVICE_DFU_MODE_ON]) should be done here
            //_wait_while_opening_transport();
            int rtn = this._wait_while_opening_transport(); if (rtn != 0) { return; }

            int softdevice_size = 0;
            int bootloader_size = 0;
            int application_size = 0;

            String bin_file_path = os_path_join(this.unpacked_zip_path, firmware_manifest.bin_file);
            Byte[] firmware = _read_file(bin_file_path);

            String dat_file_path = os_path_join(this.unpacked_zip_path, firmware_manifest.dat_file);
            Byte[] init_packet = _read_file(dat_file_path);

            if (program_mode == HexType.SD_BL)
            {
                //if( not isinstance(firmware_manifest, SoftdeviceBootloaderFirmware))
                //    raise NordicSemiException("Wrong type of manifest")
                softdevice_size = firmware_manifest.sd_size;
                bootloader_size = firmware_manifest.bl_size;
                int firmware_size = firmware.Length;
                if (softdevice_size + bootloader_size != firmware_size)
                {
                    ;
                    /*
                     * raise NordicSemiException(
                     * "Size of bootloader ({} bytes) and softdevice ({} bytes)"
                     * " is not equal to firmware provided ({} bytes)".format(
                     * bootloader_size, softdevice_size, firmware_size))
                     * */
                }
            }
            else
            {
                if (program_mode == HexType.SOFTDEVICE)
                {
                    softdevice_size = firmware.Length;
                }
                else
                    if (program_mode == HexType.BOOTLOADER)
                    {
                        bootloader_size = firmware.Length;
                    }
                    else
                        if (program_mode == HexType.APPLICATION)
                        {
                            application_size = firmware.Length;
                        }
            }

            double start_time = my_time.time();
           
            logger.info(String.Format("Starting DFU upgrade of type {0}, SoftDevice size: {1}, bootloader size: {2}, application size: {3}",
                          program_mode, softdevice_size, bootloader_size, application_size) );
            logger.info("Sending DFU start packet, afterwards we wait for the flash on target to be initialized before continuing.");

            /* debugging */          
            this.dfu_transport.send_start_dfu(program_mode, softdevice_size, bootloader_size, application_size);

            logger.info("Sending DFU init packet");
            this.dfu_transport.send_init_packet(init_packet);

            logger.info("Sending firmware file");
            this.dfu_transport.send_firmware(firmware);

            this.dfu_transport.send_validate_firmware();

            this.dfu_transport.send_activate_firmware();

            double end_time = my_time.time();
            logger.info(String.Format("DFU upgrade took {0}s", (end_time - start_time)) );
            /* debugging */
            this.dfu_transport.close();  //TODO this DEinitialisation (e.g. put RTR500BLE into Normal mode [SET_DEVICE_DFU_MODE_OFF]) should be done here
        }

        public void dfu_send_images()
        {
            /*
             * Does DFU for all firmware images in the stored manifest.
             * :return:
             */
            if (this.manifest.softdevice_bootloader != null)
                this._dfu_send_image(HexType.SD_BL, this.manifest.softdevice_bootloader);

            if (this.manifest.softdevice != null)
                this._dfu_send_image(HexType.SOFTDEVICE, this.manifest.softdevice);

            if (this.manifest.bootloader != null)
                this._dfu_send_image(HexType.BOOTLOADER, this.manifest.bootloader);

            if (this.manifest.application != null)
                this._dfu_send_image(HexType.APPLICATION, this.manifest.application);

        }
    }

}