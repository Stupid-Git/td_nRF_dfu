
#include "stdint.h"
#include "stdio.h"
#include "../stdbool.h"

#include "dfu_transport.h"


#define DEFAULT_SERIAL_PORT_TIMEOUT  1.0  //# Timeout time on serial port read
#define ACK_PACKET_TIMEOUT           1.0  //# Timeout time for for ACK packet received before reporting timeout through event system
#define SEND_INIT_PACKET_WAIT_TIME   1.0  //# Time to wait before communicating with bootloader after init packet is sent
#define SEND_START_DFU_WAIT_TIME     10.0 //# Time to wait before communicating with bootloader after start DFU packet is sent
#define DFU_PACKET_MAX_SIZE          512  //# The DFU packet max size


//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################

int32_t serT_register_events_callback(DfuTransport_t *P, int event_type, xxx_event_handler_del callback)
{
    /*
    Register a callback.

    :param DfuEvent callback:
    :return: None
    """
    if event_type not in self.callbacks:
    self.callbacks[event_type] = []

    self.callbacks[event_type].append(callback)
    */
    return(0); //   pass
}





void init_Dfu_Serial(DfuTrans_Serial_t *gp);

DfuTrans_Serial_t gx_DfuTrans_Serial;

DfuTransport_t * DfuTransportSerial(char *com_port, int baud_rate, bool flow_control)
{
    DfuTrans_Serial_t *pTS;
    
    float timeout;
    timeout = DEFAULT_SERIAL_PORT_TIMEOUT;
    
    pTS = &gx_DfuTrans_Serial;
    
    init_Dfu_Serial(pTS);

    pTS->com_port = com_port;
    pTS->baud_rate = baud_rate;
    if (flow_control == true)
        pTS->flow_control = 1;
    else
        pTS->flow_control = 0;
    
    pTS->timeout = timeout;

    pTS->serial_port = 0;

    return(0);
}


/*
class DfuTransportSerial(DfuTransport):
    def __init__(self, com_port, baud_rate=38400, flow_control=False, timeout=DEFAULT_SERIAL_PORT_TIMEOUT):
        super(DfuTransportSerial, self).__init__()
        self.com_port = com_port
        self.baud_rate = baud_rate
        self.flow_control = 1 if flow_control else 0
        self.timeout = timeout
        self.serial_port = None
        """:type: serial.Serial """
*/

int32_t serT_open(DfuTransport_t *pT)
{
    return(0);
}
/*
    def open(self):
        super(DfuTransportSerial, self).open()

        try:
            self.serial_port = Serial(port=self.com_port, baudrate=self.baud_rate, rtscts=self.flow_control, timeout=self.timeout)
        except Exception, e:
            raise NordicSemiException("Serial port could not be opened on {0}. Reason: {1}".format(self.com_port, e.message))
*/


int32_t serT_close(DfuTransport_t *pT)
{
    return(0);
}
/*
    def close(self):
        super(DfuTransportSerial, self).close()
        self.serial_port.close()
*/

bool serT_is_open(DfuTransport_t *pT)
{
    DfuTrans_Serial_t *P;

    P = (DfuTrans_Serial_t*)pT;
    //super(DfuTransportSerial, self).is_open()
    if( P->serial_port == 0)
        return false;

    return( P->serial_port->isOpen(&P->spInfo));
}
/*
    def is_open(self):
        super(DfuTransportSerial, self).is_open()

        if self.serial_port is None:
            return False

        return self.serial_port.isOpen()
*/

int32_t serT_send_validate_firmware(DfuTransport_t *P)
{
    /*
    Send request to device to verify that firmware has been correctly transferred.

    This call will block until validation is sent and validation is complete.
    If timeout or errors occurs exception is thrown.

    :return bool: True if firmware validated successfully.
    */
    return(0); //   pass
}
/*
    def send_validate_firmware(self):
        super(DfuTransportSerial, self).send_validate_firmware()
        return True
*/

int32_t serT_send_init_packet(DfuTransport_t *P, buf6_t *init_packet)
{
    /*
    Send init_packet to device.

    This call will block until init_packet is sent and transfer of packet is complete.
    If timeout or errors occurs exception is thrown.

    :param str init_packet: Init packet as a str.
    :return:
    */
    return(0); //   pass
}

/*
    def send_init_packet(self, init_packet):
        super(DfuTransportSerial, self).send_init_packet(init_packet)

        frame = int32_to_bytes(DFU_INIT_PACKET)
        frame += init_packet
        frame += int16_to_bytes(0x0000)  # Padding required

        packet = HciPacket(frame)
        self.send_packet(packet)
        time.sleep(SEND_INIT_PACKET_WAIT_TIME)
*/


int32_t serT_send_start_dfu(DfuTransport_t *P, int program_mode, int softdevice_size, int bootloader_size, int app_size)
{
    /*
        frame = int32_to_bytes(DFU_START_PACKET)
        frame += int32_to_bytes(mode)
        frame += DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size)

        packet = HciPacket(frame)
        self.send_packet(packet)
        time.sleep(SEND_START_DFU_WAIT_TIME)
    */
    return(0); //   pass
}


/*
    def send_start_dfu(self, mode, softdevice_size=None, bootloader_size=None, app_size=None):
        super(DfuTransportSerial, self).send_start_dfu(mode, softdevice_size, bootloader_size, app_size)

        frame = int32_to_bytes(DFU_START_PACKET)
        frame += int32_to_bytes(mode)
        frame += DfuTransport.create_image_size_packet(softdevice_size, bootloader_size, app_size)

        packet = HciPacket(frame)
        self.send_packet(packet)
        time.sleep(SEND_START_DFU_WAIT_TIME)
*/


int32_t serT_send_activate_firmware(DfuTransport_t *P)
{
    /*
    Send command to device to activate new firmware and restart the device.
    The device will start up with the new firmware.

    Raises an nRFException if anything fails.

    :return:
    */
    return(0); //   pass
}
/*
    def send_activate_firmware(self):
        super(DfuTransportSerial, self).send_activate_firmware()
*/


int32_t serT_send_firmware(DfuTransport_t *P, buf6_t *firmware)
{
    /*
    Start sending firmware to device.

    This call will block until transfer of firmware is complete.
    If timeout or errors occurs exception is thrown.

    :param str firmware:
    :return:
    */
    return(0); //   pass
}

/*
    def send_firmware(self, firmware):
        super(DfuTransportSerial, self).send_firmware(firmware)

        def progress_percentage(part, whole):
            return int(100 * float(part)/float(whole))

        frames = []
        self._send_event(DfuEvent.PROGRESS_EVENT, progress=0, done=False, log_message="")

        for i in range(0, len(firmware), DFU_PACKET_MAX_SIZE):
            data_packet = HciPacket(int32_to_bytes(DFU_DATA_PACKET) + firmware[i:i + DFU_PACKET_MAX_SIZE])
            frames.append(data_packet)

        frames_count = len(frames)

        # Send firmware packets
        for count, pkt in enumerate(frames):
            self.send_packet(pkt)
            self._send_event(DfuEvent.PROGRESS_EVENT,
                             log_message="",
                             progress=progress_percentage(count, frames_count),
                             done=False)

        # Send data stop packet
        frame = int32_to_bytes(DFU_STOP_DATA_PACKET)
        packet = HciPacket(frame)
        self.send_packet(packet)

        self._send_event(DfuEvent.PROGRESS_EVENT, progress=100, done=False, log_message="")
*/


static int send_packet(/*TODOserdev_t *uart,*/ buf6_t *pkt)
{
    return(42);
}

/*
    def send_packet(self, pkt):
        attempts = 0
        last_ack = None
        packet_sent = False

        logger.debug("PC -> target: {0}".format(pkt))

        while not packet_sent:
            self.serial_port.write(pkt.data)
            attempts += 1
            ack = self.get_ack_nr()

            if last_ack is None:
                break

            if ack == (last_ack + 1) % 8:
                last_ack = ack
                packet_sent = True

                if attempts > 3:
                    raise Exception("Three failed tx attempts encountered on packet {0}".format(pkt.sequence_number))
*/


int get_ack_nr(void) {return(42);} //TODO
/*
    def get_ack_nr(self):
        def is_timeout(start_time, timeout_sec):
            return not (datetime.now() - start_time <= timedelta(0, timeout_sec))

        uart_buffer = ''
        start = datetime.now()

        while uart_buffer.count('\xC0') < 2:
            # Disregard first of the two C0
            temp = self.serial_port.read(6)

            if temp:
                uart_buffer += temp

            if is_timeout(start, ACK_PACKET_TIMEOUT):
                # reset HciPacket numbering back to 0
                HciPacket.sequence_number = 0
                self._send_event(DfuEvent.TIMEOUT_EVENT,
                                 log_message="Timed out waiting for acknowledgement from device.")

                # quit loop
                break

                # read until you get a new C0
                # RESUME_WORK

        if len(uart_buffer) < 2:
            raise NordicSemiException("No data received on serial port. Not able to proceed.")

        logger.debug("PC <- target: {0}".format(binascii.hexlify(uart_buffer)))
        data = self.decode_esc_chars(uart_buffer)

        # Remove 0xC0 at start and beginning
        data = data[1:-1]

        # Extract ACK number from header
        return (data[0] >> 3) & 0x07

    @staticmethod
    def decode_esc_chars(data):
        """Replace 0xDBDC with 0xCO and 0xDBDD with 0xDB"""
        result = []

        data = bytearray(data)

        while len(data):
            char = data.pop(0)

            if char == 0xDB:
                char2 = data.pop(0)

                if char2 == 0xDC:
                    result.append(0xC0)
                elif char2 == 0xDD:
                    result.append(0xDB)
                else:
                    raise Exception('Char 0xDB NOT followed by 0xDC or 0xDD')
            else:
                result.append(char)

        return result

DATA_INTEGRITY_CHECK_PRESENT = 1
RELIABLE_PACKET = 1
HCI_PACKET_TYPE = 14

DFU_INIT_PACKET = 1
DFU_START_PACKET = 3
DFU_DATA_PACKET = 4
DFU_STOP_DATA_PACKET = 5

DFU_UPDATE_MODE_NONE = 0
DFU_UPDATE_MODE_SD = 1
DFU_UPDATE_MODE_BL = 2
DFU_UPDATE_MODE_APP = 4


class HciPacket(object):
    """Class representing a single HCI packet"""

    sequence_number = 0

    def __init__(self, data=''):
        HciPacket.sequence_number = (HciPacket.sequence_number + 1) % 8
        self.temp_data = ''
        self.temp_data += slip_parts_to_four_bytes(HciPacket.sequence_number,
                                                   DATA_INTEGRITY_CHECK_PRESENT,
                                                   RELIABLE_PACKET,
                                                   HCI_PACKET_TYPE,
                                                   len(data))
        self.temp_data += data
        # Add escape characters
        crc = crc16.calc_crc16(self.temp_data, crc=0xffff)

        self.temp_data += chr(crc & 0xFF)
        self.temp_data += chr((crc & 0xFF00) >> 8)

        self.temp_data = slip_encode_esc_chars(self.temp_data)

        self.data = chr(0xc0)
        self.data += self.temp_data
        self.data += chr(0xc0)

    def __str__(self):
        return binascii.hexlify(self.data)
*/

void init_Dfu_Serial(DfuTrans_Serial_t *gp)
{
    gp->DfuTransport.is_open = serT_is_open;
    gp->DfuTransport.open = serT_open;
    gp->DfuTransport.close = serT_close;

    gp->DfuTransport.send_start_dfu = serT_send_start_dfu;
    gp->DfuTransport.send_init_packet = serT_send_init_packet;
    gp->DfuTransport.send_firmware = serT_send_firmware;
    gp->DfuTransport.send_activate_firmware = serT_send_activate_firmware;
    gp->DfuTransport.send_validate_firmware = serT_send_validate_firmware;


    gp->DfuTransport.register_events_callback = serT_register_events_callback;
}

