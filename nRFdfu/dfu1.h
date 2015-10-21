
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


typedef struct _class_Firmware
{
    char * bin_file;
    char * dat_file;
    char * init_packet_data;
    int sd_size;
    int bl_size;

} class_Firmware_t;

typedef struct _manifest
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



