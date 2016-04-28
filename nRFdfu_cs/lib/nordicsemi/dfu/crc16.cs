using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace nRFdfu_cs
{
    static class dfu_crc16
    {

        ///////////////////////////////////////////////////////////////////////////////
        public static UInt16 calc_crc16(Byte[] binary_data, UInt16 p_crc)
        {
            UInt16 i;
            UInt16 crc; crc = p_crc; // = (p_crc == null) ? (UInt16)0xffff : p_crc;
            int size;
            size = binary_data.Length;

            for (i = 0; i < size; i++)
            {
                crc = (UInt16)((crc >> 8) | (crc << 8));
                crc ^= binary_data[i];
                crc ^= (UInt16)((crc & 0xff) >> 4);
                crc ^= (UInt16)((crc << 8) << 4);
                crc ^= (UInt16)(((crc & 0xff) << 4) << 1);
                //printf("crc=%04x  ", crc);
            }
            return crc;

            /*
            def calc_crc16(binary_data, crc=0xffff):
                """
                Calculates CRC16 on binary_data

                :param int crc: CRC value to start calculation with
                :param bytearray binary_data: Array with data to run CRC16 calculation on
                :return int: Calculated CRC value of binary_data
                """

                for b in binary_data:
                    crc = (crc >> 8 & 0x00FF) | (crc << 8 & 0xFF00)
                    crc ^= ord(b)
                    crc ^= (crc & 0x00FF) >> 4
                    crc ^= (crc << 8) << 4
                    crc ^= ((crc & 0x00FF) << 4) << 1
                return crc & 0xFFFF
            */
        }


        ///////////////////////////////////////////////////////////////////////////////
        /* OLD
        public static UInt16 crc16_compute(Byte[] p_data, UInt16 size, UInt16 p_crc)
        {
            UInt16 i;
            UInt16 crc; crc = p_crc; // = (p_crc == null) ? (UInt16)0xffff : p_crc;

            for (i = 0; i < size; i++)
            {
                crc = (UInt16)((crc >> 8) | (crc << 8));
                crc ^= p_data[i];
                crc ^= (UInt16)((crc & 0xff) >> 4);
                crc ^= (UInt16)((crc << 8) << 4);
                crc ^= (UInt16)(((crc & 0xff) << 4) << 1);
                //printf("crc=%04x  ", crc);
            }
            return crc;
        }
        */
    }
}

