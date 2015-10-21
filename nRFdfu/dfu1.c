
#include "stdint.h"
#include "stdio.h"
#include "../stdbool.h"

#include "dfu_transport.h"

#include "dfu.h"

/*
# Python standard library
import os
import tempfile
import shutil
import logging
from time import time, sleep
from datetime import datetime, timedelta

# Nordic libraries
from nordicsemi.exceptions import *
from nordicsemi.dfu.package import Package
from nordicsemi.dfu.dfu_transport import DfuEvent
from nordicsemi.dfu.model import HexType
from nordicsemi.dfu.manifest import SoftdeviceBootloaderFirmware

logger = logging.getLogger(__name__)
*/

//class HexType:
#define  HexType_SOFTDEVICE   1
#define  HexType_BOOTLOADER   2
#define  HexType_SD_BL        3
#define  HexType_APPLICATION  4



Dfu_t gx_Dfu; 
Dfu_t *Dfu(char* zip_file_path, DfuTransport_t *dfu_transport)
{
    Dfu_t *P; 
    /*
    Initializes the dfu upgrade, unpacks zip and registers callbacks.

    @param zip_file_path: Path to the zip file with the firmware to upgrade
    @type zip_file_path: str
    @param dfu_transport: Transport backend to use to upgrade
    @type dfu_transport: nordicsemi.dfu.dfu_transport.DfuTransport
    @return
    */
    P = &gx_Dfu;

    P->zip_file_path = zip_file_path;
    P->ready_to_send = true;
    P->response_opcode_received = 0;//None;

    P->temp_dir = "";//tempfile.mkdtemp(prefix="nrf_dfu_");
    P->unpacked_zip_path = "";//os.path.join(self.temp_dir, 'unpacked_zip');
    P->manifest = 0;//Package.unpack_package(self.zip_file_path, self.unpacked_zip_path);

    if (dfu_transport)
        P->dfu_transport = dfu_transport;

    P->dfu_transport->register_events_callback(P->dfu_transport, DfuEvent_TIMEOUT_EVENT, P->timeout_event_handler);
    P->dfu_transport->register_events_callback(P->dfu_transport, DfuEvent_ERROR_EVENT, P->error_event_handler);

    return(P);
}

/*
class Dfu(object):
    """ Class to handle upload of a new hex image to the device. """

    def __init__(self, zip_file_path, dfu_transport):
        """
        Initializes the dfu upgrade, unpacks zip and registers callbacks.

        @param zip_file_path: Path to the zip file with the firmware to upgrade
        @type zip_file_path: str
        @param dfu_transport: Transport backend to use to upgrade
        @type dfu_transport: nordicsemi.dfu.dfu_transport.DfuTransport
        @return
        """
        self.zip_file_path = zip_file_path
        self.ready_to_send = True
        self.response_opcode_received = None

        self.temp_dir = tempfile.mkdtemp(prefix="nrf_dfu_")
        self.unpacked_zip_path = os.path.join(self.temp_dir, 'unpacked_zip')
        self.manifest = Package.unpack_package(self.zip_file_path, self.unpacked_zip_path)

        if dfu_transport:
            self.dfu_transport = dfu_transport

        self.dfu_transport.register_events_callback(DfuEvent.TIMEOUT_EVENT, self.timeout_event_handler)
        self.dfu_transport.register_events_callback(DfuEvent.ERROR_EVENT, self.error_event_handler)
*/

/*
    def __del__(self):
        """
        Destructor removes the temporary directory for the unpacked zip
        :return:
        """
        shutil.rmtree(self.temp_dir)
*/

void error_event_handler(Dfu_t *P, char *log_message)
{
    /*
    Event handler for errors, closes the transport backend.
    :param str log_message: The log message for the error.
    :return:
    */
    if( P->dfu_transport->is_open(P->dfu_transport) )
        P->dfu_transport->close(P->dfu_transport);

    /*TODO
    logger.error(log_message)
    */
}
/*
    def error_event_handler(self, log_message=""):
        """
        Event handler for errors, closes the transport backend.
        :param str log_message: The log message for the error.
        :return:
        """
        if self.dfu_transport.is_open():
            self.dfu_transport.close()

        logger.error(log_message)

*/

void  timeout_event_handler(Dfu_t *P, char *log_message)
{
    /*
    Event handler for timeouts, closes the transport backend.
    :param log_message: The log message for the timeout.
    :return:
    */
    if( P->dfu_transport->is_open(P->dfu_transport) )
        P->dfu_transport->close(P->dfu_transport);

    /*TODO
    logger.error(log_message)
    */
}
/*
    def timeout_event_handler(self, log_message):
        """
        Event handler for timeouts, closes the transport backend.
        :param log_message: The log message for the timeout.
        :return:
        """
        if self.dfu_transport.is_open():
            self.dfu_transport.close()

        logger.error(log_message)

*/

//#include <memory.h>
//#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
buf6_t* _read_file(char * file_path)
{
    buf6_t *file_content;
    uint8_t *pd;
	struct  _stat32 s;
	FILE*         fd;
	size_t bytes;
    long l;
    long rest;

	fd = fopen(file_path, "rb");
	if (fd == 0)
	{
		printf("Input file %s does not exist\n", file_path);
		goto open_failed;
	}

	//if (fstat (fd, &s) != 0)
    if (_fstat32(fd->_file, &s) != 0)
	{
		printf("Could not stat input file %s\n", file_path);
		goto stat_failed;
	}

    printf("debug: file size = %d, 0x%x\n",s.st_size, s.st_size);
	l = s.st_size;

    if ((file_content = (buf6_t*) malloc(4 + l)) == NULL)
	{
		printf("Could not allocate memory for reading file %s\n", file_path);
		goto malloc_failed;
	}

    pd = file_content->data;

	rest = l;
	while (rest > 0)
	{
		bytes = fread(pd + (l - rest), 1, rest, fd);
		if (bytes < 0)
		{
			printf("Could not read file %s\n", file_path);
			goto read_failed;
		}
		else if (bytes == 0)
		{
			break;	//end of file
		}
		rest -= bytes;
	}

    file_content->len = l;
	
	// No special error treatment necessary, we need to unmap and close
	// the file anyway.

	fclose(fd);
	
	return file_content;
	
	// Clean up on error.
  read_failed:
    free(file_content);
  
  malloc_failed:
  stat_failed:
    fclose(fd);
  
  open_failed:

	return(0);// NULL;
}

/*
    @staticmethod
    def _read_file(file_path):
        """
        Reads a file and returns the content as a string.

        :param str file_path: The path to the file to read.
        :return str: Content of the file.
        """
        buffer_size = 4096

        file_content = ""

        with open(file_path, 'rb') as binary_file:
            while True:
                data = binary_file.read(buffer_size)

                if data:
                    file_content += data
                else:
                    break

        return file_content

*/

#include "windows.h" //Sleep


int32_t _wait_while_opening_transport(Dfu_t *P)
{
    //int r=0;
    float timeout = 10.0;
    bool timed_out;
    //start_time = datetime.now();
    
    while ( P->dfu_transport->is_open(P->dfu_transport) == false)
    {
        //timed_out = datetime.now() - start_time > timedelta(0, timeout)
        if(timeout <= 0.0)
            timed_out = true;
        
        if( timed_out == true)
        {
            //log_message = "Failed to open transport backend"
            //raise NordicSemiException(log_message)
            printf("Failed to open transport backend\n");
            return(1);
        }
        Sleep(100); //sleep(0.1)
        timeout -= 0.1; //karel
    }
    return(0);
}
/*
    def _wait_while_opening_transport(self):
        timeout = 10
        start_time = datetime.now()

        while not self.dfu_transport.is_open():
            timed_out = datetime.now() - start_time > timedelta(0, timeout)

            if timed_out:
                log_message = "Failed to open transport backend"
                raise NordicSemiException(log_message)

            sleep(0.1)


*/
char* os_path_join( char* d1, char* d2)
{
    char *path;
    int len;
    int len1;
    int len2;

    len1 = strlen(d1);
    len2 = strlen(d2);
    len = len1 + len2;

    path = (char*)malloc( len );

    path[0] = 0;

    //copy
    //copy

    return(path);
}

int32_t _dfu_send_image(Dfu_t *pD, int program_mode, class_Firmware_t *firmware_manifest)
{
    int32_t r;
    int  softdevice_size;
    int  bootloader_size;
    int  application_size;
    int firmware_size;
    char *bin_file_path ;
    buf6_t *firmware;
    char *dat_file_path;
    buf6_t *init_packet;

    /*
    Does DFU for one image. Reads the firmware image and init file.
    Opens the transport backend, calls setup, send and finalize and closes the backend again.
    @param program_mode: What type of firmware the DFU is
    @type program_mode: nordicsemi.dfu.model.HexType
    @param firmware_manifest: The manifest for the firmware image
    @type firmware_manifest: nordicsemi.dfu.manifest.Firmware
    @return:
    */

    r = 0;
    //if(firmware_manifest == 0)//is None:
    //    raise MissingArgumentException("firmware_manifest must be provided.")

    if( pD->dfu_transport->is_open(pD->dfu_transport) )
    {
        printf("IllegalStateException(Transport is already open.)\n"); //raise IllegalStateException("Transport is already open.")
        return(42);
    }

    r = pD->dfu_transport->open(pD->dfu_transport);
    if(r != 0) {return(r);}

    r = _wait_while_opening_transport(pD);
    if(r != 0) {return(r);}

    softdevice_size = 0;
    bootloader_size = 0;
    application_size = 0;

    bin_file_path = os_path_join(pD->unpacked_zip_path, firmware_manifest->bin_file); //"TODO";//os.path.join(self.unpacked_zip_path, firmware_manifest.bin_file);
    firmware = _read_file(bin_file_path);
    if( firmware == 0) {return(-1);}

    dat_file_path = os_path_join(pD->unpacked_zip_path, firmware_manifest->dat_file); //"TODO"; //os.path.join(self.unpacked_zip_path, firmware_manifest.dat_file);    
    init_packet = _read_file(dat_file_path);
    if( init_packet == 0) {return(-1);}

    if(program_mode == HexType_SD_BL)
    {
        //if( not isinstance(firmware_manifest, SoftdeviceBootloaderFirmware) )
        //{
        //    raise NordicSemiException("Wrong type of manifest")
        //}
        softdevice_size = firmware_manifest->sd_size;
        bootloader_size = firmware_manifest->bl_size;
        firmware_size = firmware->len;

        if( softdevice_size + bootloader_size != firmware_size )
        {
            /*TODO raise NordicSemiException(
                "Size of bootloader ({} bytes) and softdevice ({} bytes)"
                " is not equal to firmware provided ({} bytes)".format(
                bootloader_size, softdevice_size, firmware_size));
                */
            return(42);
        }
    }
    else
    {
        if(program_mode == HexType_SOFTDEVICE)
        {
            softdevice_size = firmware->len;
        }
        else
        {
            if(program_mode == HexType_BOOTLOADER)
            {
                bootloader_size = firmware->len;
            }
            else
            {
                if(program_mode == HexType_APPLICATION)
                {
                    application_size = firmware->len;
                }
            }
        }
    }

    //TODO start_time = time();
    /*
    logger.info("Starting DFU upgrade of type %s, SoftDevice size: %s, bootloader size: %s, application size: %s",
    program_mode,
    softdevice_size,
    bootloader_size,
    application_size)

    logger.info("Sending DFU start packet, afterwards we wait for the flash on "
    "target to be initialized before continuing.")
    */
    pD->dfu_transport->send_start_dfu(pD->dfu_transport, program_mode, softdevice_size, bootloader_size,
        application_size);

    //logger.info("Sending DFU init packet");
    pD->dfu_transport->send_init_packet(pD->dfu_transport, init_packet);

    //logger.info("Sending firmware file");
    pD->dfu_transport->send_firmware(pD->dfu_transport, firmware);

    pD->dfu_transport->send_validate_firmware(pD->dfu_transport);

    pD->dfu_transport->send_activate_firmware(pD->dfu_transport);

    //TODO end_time = time();
    //logger.info("DFU upgrade took {0}s".format(end_time - start_time));

    pD->dfu_transport->close(pD->dfu_transport);

    return(r);
}
/*
def _dfu_send_image(self, program_mode, firmware_manifest):
        """
        Does DFU for one image. Reads the firmware image and init file.
        Opens the transport backend, calls setup, send and finalize and closes the backend again.
        @param program_mode: What type of firmware the DFU is
        @type program_mode: nordicsemi.dfu.model.HexType
        @param firmware_manifest: The manifest for the firmware image
        @type firmware_manifest: nordicsemi.dfu.manifest.Firmware
        @return:
        """

        if firmware_manifest is None:
            raise MissingArgumentException("firmware_manifest must be provided.")

        if self.dfu_transport.is_open():
            raise IllegalStateException("Transport is already open.")

        self.dfu_transport.open()
        self._wait_while_opening_transport()

        softdevice_size = 0
        bootloader_size = 0
        application_size = 0

        bin_file_path = os.path.join(self.unpacked_zip_path, firmware_manifest.bin_file)
        firmware = self._read_file(bin_file_path)

        dat_file_path = os.path.join(self.unpacked_zip_path, firmware_manifest.dat_file)
        init_packet = self._read_file(dat_file_path)

        if program_mode == HexType.SD_BL:
            if not isinstance(firmware_manifest, SoftdeviceBootloaderFirmware):
                raise NordicSemiException("Wrong type of manifest")
            softdevice_size = firmware_manifest.sd_size
            bootloader_size = firmware_manifest.bl_size
            firmware_size = len(firmware)
            if softdevice_size + bootloader_size != firmware_size:
                raise NordicSemiException(
                    "Size of bootloader ({} bytes) and softdevice ({} bytes)"
                    " is not equal to firmware provided ({} bytes)".format(
                    bootloader_size, softdevice_size, firmware_size))

        elif program_mode == HexType.SOFTDEVICE:
            softdevice_size = len(firmware)

        elif program_mode == HexType.BOOTLOADER:
            bootloader_size = len(firmware)

        elif program_mode == HexType.APPLICATION:
            application_size = len(firmware)

        start_time = time()
        logger.info("Starting DFU upgrade of type %s, SoftDevice size: %s, bootloader size: %s, application size: %s",
                    program_mode,
                    softdevice_size,
                    bootloader_size,
                    application_size)

        logger.info("Sending DFU start packet, afterwards we wait for the flash on "
                    "target to be initialized before continuing.")
        self.dfu_transport.send_start_dfu(program_mode, softdevice_size, bootloader_size,
                                          application_size)

        logger.info("Sending DFU init packet")
        self.dfu_transport.send_init_packet(init_packet)

        logger.info("Sending firmware file")
        self.dfu_transport.send_firmware(firmware)

        self.dfu_transport.send_validate_firmware()

        self.dfu_transport.send_activate_firmware()

        end_time = time()
        logger.info("DFU upgrade took {0}s".format(end_time - start_time))

        self.dfu_transport.close()

*/


int32_t dfu_send_images(Dfu_t *pD)
{
    int32_t r;
    /*
    Does DFU for all firmware images in the stored manifest.
    */
    r = 0;

    if(pD->manifest->softdevice_bootloader != 0 )
        _dfu_send_image(pD, HexType_SD_BL, pD->manifest->softdevice_bootloader);

    if(pD->manifest->softdevice != 0)
        _dfu_send_image(pD,HexType_SOFTDEVICE, pD->manifest->softdevice);
    
    if(pD->manifest->bootloader != 0)
        _dfu_send_image(pD,HexType_BOOTLOADER, pD->manifest->bootloader);

    if(pD->manifest->application != 0)
        _dfu_send_image(pD,HexType_APPLICATION, pD->manifest->application);
    
    return(r);
}
/*
    def dfu_send_images(self):
        """
        Does DFU for all firmware images in the stored manifest.
        :return:
        """
        if self.manifest.softdevice_bootloader:
            self._dfu_send_image(HexType.SD_BL, self.manifest.softdevice_bootloader)

        if self.manifest.softdevice:
            self._dfu_send_image(HexType.SOFTDEVICE, self.manifest.softdevice)

        if self.manifest.bootloader:
            self._dfu_send_image(HexType.BOOTLOADER, self.manifest.bootloader)

        if self.manifest.application:
            self._dfu_send_image(HexType.APPLICATION, self.manifest.application)

*/

