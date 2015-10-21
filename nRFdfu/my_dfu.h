
#ifndef __DFU_H
#define __DFU_H

#ifdef __cplusplus
extern "C"
{
#endif



struct CL_DfuEvent
{
#define DfuEvent_PROGRESS_EVENT 1
#define DfuEvent_TIMEOUT_EVENT  2
#define DfuEvent_ERROR_EVENT    3
    int i;
};


typedef struct class_InitPacketData
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
    int packet_version;      // init packet version
    int compression_type;    // init packet compression type
    int device_type;         // device type
    int device_revision;     // device revision
    int application_version; // application version
    int softdevice_req[2];     // softdevice requirements
    char* firmware_hash;     // firmware hash
    int firmware_crc16;      // firmware CRC-16 calculated value
} class_InitPacketData_t;


typedef struct class_Firmware_s
{
    char * bin_file;
    char * dat_file;
    class_InitPacketData_t * init_packet_data;

    int sd_size; //BL + SD
    int bl_size; //BL + SD

} class_Firmware_t;


typedef struct manifest_s
{
    class_Firmware_t  *softdevice_bootloader;
    class_Firmware_t  *softdevice;
    class_Firmware_t  *bootloader;
    class_Firmware_t  *application;

    int app_size;

} manifest_t;

typedef struct CL_Dfu
{
    char *zip_file_path;
    bool ready_to_send;
    int response_opcode_received;
    char *temp_dir;
    char *unpacked_zip_path;
    manifest_t *manifest;

    DfuTransport_t *dfu_transport;

    xxx_event_handler_del error_event_handler;
    xxx_event_handler_del timeout_event_handler;

} Dfu_t;

Dfu_t *Dfu(char* zip_file_path, DfuTransport_t *dfu_transport);

int32_t dfu_send_images(Dfu_t *pD);


#ifdef __cplusplus
}
#endif

#endif // __DFU_H



