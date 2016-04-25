using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

namespace nRFdfu_cs
{
    





    class ks_hexToZip
    {
        /* REF
        static void W()
        {
            // Create 12 integers.
            int[] a = new int[]	{1,4,6,7,11,55, 777,23,266, 44, 82, 93  };
            
            // Use using statement and File.Open.
            using (BinaryWriter b = new BinaryWriter(
                File.Open("file.bin", FileMode.Create)))
            {
                // Use foreach and write all 12 integers.
                foreach (int i in a)
                {
                    b.Write(i);
                }
            }
        }
        */

        /* example fprintf in C#
        static void W1()
        {
            try
            {
                using (StreamWriter sw = new StreamWriter("Book1.csv"))
                {
                    sw.WriteLine(string.Format("{0},{1},{2}", "value of w", "Pa", "Pb"));
                    // ....
                    sw.Flush();
                }
            }
            catch (IOException)
            {
                Console.Write("Coudnt open file\n");
            }
        }
        */



        //=====================================================================
        //=====================================================================
        //=====================================================================
        Int32 test_1_save_bin_to_file(String filename, Byte[] pBin, UInt32 len)
        {
            try
            {
                // Use using statement and File.Open.
                using (BinaryWriter b = new BinaryWriter(File.Open(filename, FileMode.Create)))
                {
                    // Use foreach and write all 12 integers.
                    //foreach (Byte b0 in pBin) -----  this is only good if the 'natural' length of pBin is real length
                    Byte b0;
                    for( UInt32 i = 0; i<len; i++)
                    {
                        b0 = pBin[i];
                        b.Write(b0);
                    }
                }
                return (0);
            }
            catch//()
            {
                return (-1);
            }
        }

        // .dat file
        //00 00 00 00 00 00 00 00 01 00 FE FF 9E 18
        // DFU Init Packet
        Int32 test_1_save_DFUInit_data(String filename, UInt16 crcValue)
        {
            //int r;

            // the .dat file contains 14 bytes
            //00 00 00 00 00 00 00 00 01 00 FE FF 9E 18

            //00 00                                      ==  0x0000      hardware_version
            //      00 00                                ==  0x0000      hardware_revision
            //            00 00 00 00                    ==  0x00000000  application_version - 
            //                       01 00               ==  0x0001      softdevice_len   - don't know where this definition comes from
            //                              FE FF        ==  0xFFFE      softdevice_array - don't know where this definition comes from
            //                                    9E 18  ==  0x189E      CRC calculated from binary file

            UInt16 hardware_version = 0x0000;      //0xFFFF;
            UInt16 hardware_revision = 0x0000;      //0xFFFF;
            UInt32 application_version = 0x00000000;  //0xFFFFFFFF;
            UInt16 softdevice_len = 0x0001;
            UInt16 softdevice_array = 0xFFFE;
            UInt16 image_crc = crcValue;


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
            try
            {
                // Use using statement and File.Open.
                using (BinaryWriter b = new BinaryWriter(File.Open(filename, FileMode.Create)))
                {
                    b.Write(hardware_version);    //bytes = fwrite(&hardware_version, 2, 1, fd); //UInt16  hardware_version    = 0x0000;      //0xFFFF;
                    b.Write(hardware_revision);   //bytes = fwrite(&hardware_revision, 2, 1, fd); //UInt16  hardware_revision   = 0x0000;      //0xFFFF;
                    b.Write(application_version); //bytes = fwrite(&application_version, 4, 1, fd); //UInt32  application_version = 0x00000000;  //0xFFFFFFFF;
                    b.Write(softdevice_len);      //bytes = fwrite(&softdevice_len, 2, 1, fd); //UInt16  softdevice_len      = 0x0001;
                    b.Write(softdevice_array);    //bytes = fwrite(&softdevice_array, 2, 1, fd); //UInt16  softdevice_array    = 0xFFFE;
                    b.Write(image_crc);           //bytes = fwrite(&image_crc, 2, 1, fd); //UInt16  image_crc           = crcValue;
                }
                return (0);
            }
            catch//()
            {
                return (-1);
            }
        }

        /*
        --manifest.json-- file
        {
            "manifest": {
                "application": {
                    "bin_file": "nrf51422_xxac_s110.bin",
                    "dat_file": "nrf51422_xxac_s110.dat",
                    "init_packet_data": {
                        "application_version": 0,             --> UInt32 application_version = 0x00000000;
                        "device_revision": 0,                 --> UInt16 hardware_revision   = 0x0000; 
                        "device_type": 0,                     --> UInt16 hardware_version    = 0x0000;
                        "firmware_crc16": 6302,               --> UInt16 image_crc           = 0x189e; --> decimal 6302
                        "softdevice_req": [                   --> UInt16 softdevice_len      = 0x0001; --> one entry in the array
                            65534                             --> UInt16 softdevice_array    = 0xFFFE;
                        ]
                    }
                }
            }
        }
        */

        Int32 test_1_print_DFUInit_json(String filename_json, String filename_bin, String filename_dat, UInt16 crcValue)
        {
            /*
            int r;
            FILE* fd;

            UInt16 hardware_version = 0x0000;      //0xFFFF;
            UInt16 hardware_revision = 0x0000;      //0xFFFF;
            UInt32 application_version = 0x00000000;  //0xFFFFFFFF;
            UInt16 softdevice_len = 0x0001;
            UInt16 softdevice_array = 0xFFFE;
            UInt16 image_crc = crcValue;

            fd = stdout;

            fprintf(fd, "{\n");
            fprintf(fd, "    \"manifest\": {\n");
            fprintf(fd, "        \"application\": {\n");
            fprintf(fd, "            \"bin_file\": \"%s\",\n", filename_bin);
            fprintf(fd, "            \"dat_file\": \"%s\",\n", filename_dat);
            fprintf(fd, "            \"init_packet_data\": {\n");
            fprintf(fd, "                \"application_version\": %d,\n", application_version); // --> UInt32 application_version = 0x00000000;
            fprintf(fd, "                \"device_revision\": %d,\n", hardware_revision);        // --> UInt16 hardware_revision   = 0x0000; 
            fprintf(fd, "                \"device_type\": %d,\n", hardware_version);             // --> UInt16 hardware_version    = 0x0000;
            fprintf(fd, "                \"firmware_crc16\": %d,\n", image_crc);                 // --> UInt16 image_crc           = 0x189e; --> decimal 6302
            fprintf(fd, "                \"softdevice_req\": [\n");                              // --> UInt16 softdevice_len      = 0x0001; --> one entry in the array
            fprintf(fd, "                    %d\n", softdevice_array);                           // --> UInt16 softdevice_array    = 0xFFFE;
            fprintf(fd, "                ]\n");
            fprintf(fd, "            }\n");
            fprintf(fd, "        }\n");
            fprintf(fd, "    }\n");
            fprintf(fd, "}\n");

            return (0);
            */
            UInt16 hardware_version = 0x0000;      //0xFFFF;
            UInt16 hardware_revision = 0x0000;      //0xFFFF;
            UInt32 application_version = 0x00000000;  //0xFFFFFFFF;
            //UInt16 softdevice_len = 0x0001;
            UInt16 softdevice_array = 0xFFFE;
            UInt16 image_crc = crcValue;

            try
            {
                using (StreamWriter sw = new StreamWriter("dfu_temp.json"))
                {
                //using (StreamWriter sw = new StreamWriter(Console.OpenStandardOutput()) ) // http://stackoverflow.com/questions/3127498/c-sharp-how-to-redirect-stream-to-the-console-out
                //{
                //    sw.AutoFlush = true;
                //    Console.SetOut(sw);

                    sw.Write(string.Format("{0},{1},{2}", "value of w", "Pa", "Pb"));
                    sw.Write(string.Format("{{\n") );
                    sw.Write(string.Format("    \"manifest\": {{\n") );
                    sw.Write(string.Format("        \"application\": {{\n") );
                    sw.Write(string.Format("            \"bin_file\": \"{0}\",\n", filename_bin) );
                    sw.Write(string.Format("            \"dat_file\": \"{0}\",\n", filename_dat) );
                    sw.Write(string.Format("            \"init_packet_data\": {{\n") );
                    sw.Write(string.Format("                \"application_version\": {0},\n", application_version) ); // --> UInt32 application_version = 0x00000000;
                    sw.Write(string.Format("                \"device_revision\": {0},\n", hardware_revision) );       // --> UInt16 hardware_revision   = 0x0000; 
                    sw.Write(string.Format("                \"device_type\": {0},\n", hardware_version) );            // --> UInt16 hardware_version    = 0x0000;
                    sw.Write(string.Format("                \"firmware_crc16\": {0},\n", image_crc) );                // --> UInt16 image_crc           = 0x189e; --> decimal 6302
                    sw.Write(string.Format("                \"softdevice_req\": [\n") );                              // --> UInt16 softdevice_len      = 0x0001; --> one entry in the array
                    sw.Write(string.Format("                    {0}\n", softdevice_array) );                          // --> UInt16 softdevice_array    = 0xFFFE;
                    sw.Write(string.Format("                ]\n") );
                    sw.Write(string.Format("            }}\n") );
                    sw.Write(string.Format("        }}\n") );
                    sw.Write(string.Format("    }}\n") );
                    sw.Write(string.Format("}}\n") );
                    sw.Flush();
                }
            }
            catch (IOException)
            {
                Console.Write("Couldn't open Console.OpenStandardOutput()\n");
            }

            return (0);
        }

        //-----------------------------------------------------------------------------
        //-----------------------------------------------------------------------------
        // #define GXIMAGE_BUFFER_CAPACITY 0x8000    

        // static uint8_t           sm_gxImage_buffer[GXIMAGE_BUFFER_CAPACITY];
        // static W_ihexMemImage_t  sm_gxImage0;

        //-----------------------------------------------------------------------------

        public int test_1(String fileName)
        {
            int r;
           
            W_ihexMemImage_t pI = new W_ihexMemImage_t();

            //pI = &sm_gxImage0;
            //pI.gx_bin = sm_gxImage_buffer;
            //pI.gx_binCapacity = sizeof(sm_gxImage_buffer); //GXIMAGE_BUFFER_CAPACITY

            //pI.gx_binCapacity = 0x8000;  //fake GXIMAGE_BUFFER_CAPACITY

             r = ks_ihex_LoadToMem.W_ihex_LoadToMem(pI, fileName);
            if (r != 0)
            {
                return (r);
            }

            Console.Write("=== test_1 ===\n");
            Console.Write("   Binary length = 0x{0:x04}, {1:d5}\n", pI.image_length, pI.image_length);
            Console.Write("   Binary   CRC  = 0x{0:x04}, {1:d5}\n", pI.image_crc, pI.image_crc);
//          Console.Write("   Binary   CRC  = 0x%04x, %5d\n", pI.image_crc, pI.image_crc);
            Console.Write("\n");

            r = test_1_save_bin_to_file("dfu_temp.bin", pI.image_array, pI.image_length);
            if (r != 0)
            {
                return (r);
            }

            r = test_1_save_DFUInit_data("dfu_temp.dat", pI.image_crc);
            if (r != 0)
            {
                return (r);
            }

            r = test_1_print_DFUInit_json("manifest.json", "dfu_temp.bin", "dfu_temp.dat", pI.image_crc);
            if (r != 0)
            {
                return (r);
            }

            return (0);
        }



        public int hex2binNdat(String hexFileName, String binFileName, String datFileName)
        {
            int r = 0;
        
            W_ihexMemImage_t pI = new W_ihexMemImage_t();

            //pI = &sm_gxImage0;
            //pI.gx_bin = sm_gxImage_buffer;
            //pI.gx_binCapacity = sizeof(sm_gxImage_buffer); //GXIMAGE_BUFFER_CAPACITY

            //pI.gx_binCapacity = 0x8000;  //fake GXIMAGE_BUFFER_CAPACITY

            r = ks_ihex_LoadToMem.W_ihex_LoadToMem(pI, hexFileName);
            if (r != 0)
            {
                return (r);
            }

            Console.Write("=== test_1 ===\n");
            Console.Write("   Binary length = 0x{0:x04}, {1:d5}\n", pI.image_length, pI.image_length);
            Console.Write("   Binary   CRC  = 0x{0:x04}, {1:d5}\n", pI.image_crc, pI.image_crc);
            //          Console.Write("   Binary   CRC  = 0x%04x, %5d\n", pI.image_crc, pI.image_crc);
            Console.Write("\n");

            r = test_1_save_bin_to_file( binFileName, pI.image_array, pI.image_length);
            if (r != 0)
            {
                return (r);
            }

            r = test_1_save_DFUInit_data(datFileName, pI.image_crc);
            if (r != 0)
            {
                return (r);
            }

            r = test_1_print_DFUInit_json("manifest.json", "dfu_temp.bin", "dfu_temp.dat", pI.image_crc);
            if (r != 0)
            {
                return (r);
            }

            return (0);
        }

    }
}
