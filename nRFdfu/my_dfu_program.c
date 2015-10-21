
#include <stdio.h>

#include "my_dfu_program.h"


#include "W_ihex_LoadToMem.h"
#include <stdio.h>



int32_t test_1_save_bin_to_file( char* filename, uint8_t* pBin, uint32_t len)
{
    int r;

    uint32_t i;

    FILE*   fd;
    size_t  bytes;

    fd = fopen(filename, "wb");
	if (fd == 0)
	{
		return(-1);
	}

    for( i = 0 ; i < len ; i++)
    {
        bytes = fwrite( pBin + i, 1, 1, fd);
    }

	fclose(fd);
	
    return(0);
}



// .dat file
//00 00 00 00 00 00 00 00 01 00 FE FF 9E 18
// DFU Init Packet
int32_t test_1_save_DFUInit_data( char* filename, uint16_t crcValue )
{
    int r;

    // the .dat file contains 14 bytes
    //00 00 00 00 00 00 00 00 01 00 FE FF 9E 18

    //00 00                                      ==  0x0000      hardware_version
    //      00 00                                ==  0x0000      hardware_revision
    //            00 00 00 00                    ==  0x00000000  application_version - 
    //                       01 00               ==  0x0001      softdevice_len   - don't know where this definition comes from
    //                              FE FF        ==  0xFFFE      softdevice_array - don't know where this definition comes from
    //                                    9E 18  ==  0x189E      CRC calculated from binary file

    uint16_t hardware_version    = 0x0000;      //0xFFFF;
    uint16_t hardware_revision   = 0x0000;      //0xFFFF;
    uint32_t application_version = 0x00000000;  //0xFFFFFFFF;
    uint16_t softdevice_len      = 0x0001;
    uint16_t softdevice_array    = 0xFFFE;
    uint16_t image_crc           = crcValue;


    /* REF Python Code
    init_packet = int32_to_bytes(DFU_INIT_PACKET);
    init_packet += int16_to_bytes(hardware_version);
    init_packet += int16_to_bytes(hardware_revision);
    init_packet += int32_to_bytes(application_version);
    init_packet += int16_to_bytes(softdevice_len);
    init_packet += int16_to_bytes(softdevice_array);
    init_packet += int16_to_bytes(image_crc);
    init_packet += int16_to_bytes(0x0000);
    */

    /* REF My Current C Code
    buf32_zero(&init_packet);
    //memcpy( &init_packet.data[0],int32_to_bytes(DFU_INIT_PACKET), 4); init_packet.len += 4;

    buf32_cpy(&init_packet, int32_to_bytes(DFU_INIT_PACKET), 4);
    buf32_cpy(&init_packet, int16_to_bytes(hardware_version), 2);
    buf32_cpy(&init_packet, int16_to_bytes(hardware_revision), 2);
    buf32_cpy(&init_packet, int32_to_bytes(application_version), 4);
    buf32_cpy(&init_packet, int16_to_bytes(softdevice_len), 2);
    buf32_cpy(&init_packet, int16_to_bytes(softdevice_array), 2);
    buf32_cpy(&init_packet, int16_to_bytes(image_crc), 2);
    buf32_cpy(&init_packet, int16_to_bytes(0x0000), 2);
    */
    
    FILE*   fd;
    size_t  bytes;

    fd = fopen(filename, "wb");
	if (fd == 0)
	{
		return(-1);
	}

    bytes = fwrite( &hardware_version,    2, 1, fd); //uint16_t  hardware_version    = 0x0000;      //0xFFFF;
    bytes = fwrite( &hardware_revision,   2, 1, fd); //uint16_t  hardware_revision   = 0x0000;      //0xFFFF;
    bytes = fwrite( &application_version, 4, 1, fd); //uint32_t  application_version = 0x00000000;  //0xFFFFFFFF;
    bytes = fwrite( &softdevice_len,      2, 1, fd); //uint16_t  softdevice_len      = 0x0001;
    bytes = fwrite( &softdevice_array,    2, 1, fd); //uint16_t  softdevice_array    = 0xFFFE;
    bytes = fwrite( &image_crc,           2, 1, fd); //uint16_t  image_crc           = crcValue;

	fclose(fd);
	
    return(0);
}
 
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

int32_t test_1_print_DFUInit_json( char* filename_json, char* filename_bin, char* filename_dat, uint16_t crcValue )
{
    int r;
    FILE *fd;

    uint16_t hardware_version    = 0x0000;      //0xFFFF;
    uint16_t hardware_revision   = 0x0000;      //0xFFFF;
    uint32_t application_version = 0x00000000;  //0xFFFFFFFF;
    uint16_t softdevice_len      = 0x0001;
    uint16_t softdevice_array    = 0xFFFE;
    uint16_t image_crc           = crcValue;

    fd = stdout;

    fprintf( fd, "{\n");
    fprintf( fd, "    \"manifest\": {\n");
    fprintf( fd, "        \"application\": {\n");
    fprintf( fd, "            \"bin_file\": \"%s\",\n", filename_bin);
    fprintf( fd, "            \"dat_file\": \"%s\",\n", filename_dat);
    fprintf( fd, "            \"init_packet_data\": {\n");
    fprintf( fd, "                \"application_version\": %d,\n", application_version) ; // --> uint32_t application_version = 0x00000000;
    fprintf( fd, "                \"device_revision\": %d,\n", hardware_revision);        // --> uint16_t hardware_revision   = 0x0000; 
    fprintf( fd, "                \"device_type\": %d,\n", hardware_version);             // --> uint16_t hardware_version    = 0x0000;
    fprintf( fd, "                \"firmware_crc16\": %d,\n", image_crc);                 // --> uint16_t image_crc           = 0x189e; --> decimal 6302
    fprintf( fd, "                \"softdevice_req\": [\n");                              // --> uint16_t softdevice_len      = 0x0001; --> one entry in the array
    fprintf( fd, "                    %d\n", softdevice_array);                           // --> uint16_t softdevice_array    = 0xFFFE;
    fprintf( fd, "                ]\n");
    fprintf( fd, "            }\n");
    fprintf( fd, "        }\n");
    fprintf( fd, "    }\n");
    fprintf( fd, "}\n");

    return(0);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define GXIMAGE_BUFFER_CAPACITY 0x8000    

static uint8_t           sm_gxImage_buffer[GXIMAGE_BUFFER_CAPACITY];
static W_ihexMemImage_t  sm_gxImage0;

//-----------------------------------------------------------------------------
int32_t test_1( char* filename )
{
    int r;
    W_ihexMemImage_t* pI;

    pI = &sm_gxImage0;
    pI->gx_bin = sm_gxImage_buffer;
    pI->gx_binCapacity = sizeof(sm_gxImage_buffer); //GXIMAGE_BUFFER_CAPACITY

    r = W_ihex_LoadToMem( pI, filename );
    if( r != 0)
    {
        return(r);
    }

    printf("=== test_1 ===\n");
    printf("   Binary length = 0x%04x, %5d\n", pI->gx_len, pI->gx_len );
    printf("   Binary   CRC  = 0x%04x, %5d\n", pI->gx_crc, pI->gx_crc );
    printf("\n");

    r = test_1_save_bin_to_file("dfu_test_bin.bin", pI->gx_bin, pI->gx_len );
    if( r != 0)
    {
        return(r);
    }

    r = test_1_save_DFUInit_data("dfu_test_bin.dat", pI->gx_crc );
    if( r != 0)
    {
        return(r);
    }

    r = test_1_print_DFUInit_json("manifest.json", "dfu_test_bin.bin", "dfu_test_bin.dat", pI->gx_crc );
    if( r != 0)
    {
        return(r);
    }

    return(0);
}

extern "C" int dfu_main(void);

//-----------------------------------------------------------------------------
int32_t my_dfu_OverSerial_program( char* fileName, uint32_t Hexfile_type, char* ComportName, uint32_t baudRate, uint32_t flowControl )
{

    printf("fileName     = %s\n", fileName);
    printf("Hexfile_type = %d\n", Hexfile_type);
    printf("ComportName  = %s\n", ComportName);
    printf("baudRate     = %d\n", baudRate);
    printf("flowControl  = %d\n", flowControl);

    test_1(fileName);

    dfu_main();

    return(0);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
