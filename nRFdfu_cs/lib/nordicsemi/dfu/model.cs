using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//from enum import Enum
namespace nRFdfu_cs
{

    //class HexType
    //{
    //    int SOFTDEVICE = 1;
    //    int BOOTLOADER = 2;
    //    int SD_BL = 3;
    //    int APPLICATION = 4;
    //}

    public class HexType
    {
        public static int  SOFTDEVICE = 1;
        public static int  BOOTLOADER = 2;
        public static int  SD_BL = 3;
        public static int  APPLICATION = 4;
    }


    enum FirmwareKeys
    {
        ENCRYPT = 1,
        FIRMWARE_FILENAME = 2,
        BIN_FILENAME = 3,
        DAT_FILENAME = 4,
        INIT_PACKET_DATA = 5,
        SD_SIZE = 6,
        BL_SIZE = 7,
    }
}

