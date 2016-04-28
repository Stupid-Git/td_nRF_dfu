using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace nRFdfu_cs
{
    static class dfu_util
    {

        /*
        # Nordic libraries
        from nordicsemi.exceptions import NordicSemiException
        */


        /*
        # TODO: Create query function that maps query-result strings with functions
        def query_func(question, default=False):
            """
            Ask a string question
            No input defaults to "no" which results in False
            """
            valid = {"yes": True, "y": True, "no": False, "n": False}
            if default is True:
                prompt = " [Y/n]"
            else:
                prompt = " [y/N]"

            while True:
                print "%s %s" % (question, prompt)
                choice = raw_input().lower()
                if choice == '':
                    return default
                elif choice in valid:
                    return valid[choice]
                else:
                    print "Please respond with y/n"

        */

        /*
        def convert_uint16_to_array(value):
            """
            Converts a int to an array of 2 bytes (little endian)

            :param int value: int value to convert to list
            :return list[int]: list with 2 bytes
            """
            byte0 = value & 0xFF
            byte1 = (value >> 8) & 0xFF
            return [byte0, byte1]
        */

        /*
        def convert_uint32_to_array(value):
            """
            Converts a int to an array of 4 bytes (little endian)

            :param int value: int value to convert to list
            :return list[int]: list with 4 bytes
            """
            byte0 = value & 0xFF
            byte1 = (value >> 8) & 0xFF
            byte2 = (value >> 16) & 0xFF
            byte3 = (value >> 24) & 0xFF
            return [byte0, byte1, byte2, byte3]
        */

        //---------------------------------------------------------------------
        static public Byte[] slip_parts_to_four_bytes(UInt32 seq, UInt32 dicp, UInt32 rel, UInt32 pkt_type, UInt32 pkt_len)
        {
            Byte[] ints = new Byte[4];
            ints[0] = 0; ints[1] = 0; ints[2] = 0; ints[3] = 0;

            ints[0] = (Byte)((seq) | (((seq + 1) % 8) << 3) | (dicp << 6) | (rel << 7));
            ints[1] = (Byte)(pkt_type | ((pkt_len & 0x000F) << 4));
            ints[2] = (Byte)((pkt_len & 0x0FF0) >> 4);
            ints[3] = (Byte)((~(ints[0] + ints[1] + ints[2] + ints[3]) + 1) & 0xFF);

            return (ints);
            /*TODO
             * ints = [0,0,0,0]
             * ints[0] = (seq) | (((seq+1)%8) << 3) | (dicp << 6) | (rel << 7)
             * ints[1] = pkt_type | ((pkt_len & 0x000F) << 4)
             * ints[2] = (pkt_len & 0x0FF0) >> 4
             * ints[3] = (~(sum(ints[0:3]))+1) & 0xFF
             * 
             * return ''.join(chr(b) for b in ints)
             * */

        /*        
        def slip_parts_to_four_bytes(seq, dip, rp, pkt_type, pkt_len):
        """
        Creates a SLIP header.

        For a description of the SLIP header go to:
        http://developer.nordicsemi.com/nRF51_SDK/doc/7.2.0/s110/html/a00093.html

        :param int seq: Packet sequence number
        :param int dip: Data integrity check
        :param int rp: Reliable packet
        :param pkt_type: Payload packet
        :param pkt_len: Packet length
        :return: str with SLIP header
        """
        ints = [0, 0, 0, 0]
        ints[0] = seq | (((seq + 1) % 8) << 3) | (dip << 6) | (rp << 7)
        ints[1] = pkt_type | ((pkt_len & 0x000F) << 4)
        ints[2] = (pkt_len & 0x0FF0) >> 4
        ints[3] = (~(sum(ints[0:3])) + 1) & 0xFF

        return ''.join(chr(b) for b in ints)
        */
        }



        //---------------------------------------------------------------------
        static public Byte[] int32_to_bytes(UInt32 u32)
        {
            Byte[] data = new Byte[4];
            data[0] = (Byte)((u32 >> 0) & 0x000000FF);
            data[1] = (Byte)((u32 >> 8) & 0x000000FF);
            data[2] = (Byte)((u32 >> 16) & 0x000000FF);
            data[3] = (Byte)((u32 >> 24) & 0x000000FF);
            return (data);
        }

        /*
        def int32_to_bytes(value):
            """
            Converts a int to a str with 4 bytes

            :param value: int value to convert
            :return: str with 4 bytes
            """
            ints = [0, 0, 0, 0]
            ints[0] = (value & 0x000000FF)
            ints[1] = (value & 0x0000FF00) >> 8
            ints[2] = (value & 0x00FF0000) >> 16
            ints[3] = (value & 0xFF000000) >> 24
            return ''.join(chr(b) for b in ints)
        */

        //---------------------------------------------------------------------
        static public Byte[] int16_to_bytes(UInt16 u16)
        {
            Byte[] data = new Byte[2];
            data[0] = (Byte)((u16 >> 0) & 0x00FF);
            data[1] = (Byte)((u16 >> 8) & 0x00FF);
            return (data);
        /*
        def int16_to_bytes(value):
            """
            Converts a int to a str with 4 bytes

            :param value: int value to convert
            :return: str with 4 bytes
            """

            ints = [0, 0]
            ints[0] = (value & 0x00FF)
            ints[1] = (value & 0xFF00) >> 8
            return ''.join(chr(b) for b in ints)
        */
        }

        
        //---------------------------------------------------------------------
        public static Byte[] slip_encode_esc_chars(Byte[] data_in)
        {
            // Replace 0xCO  with 0xDBDC and 0xDB with 0xDBDD
            int i;
            Byte _char;
            List<Byte> result = new List<byte>();
            result.Clear();

            for (i = 0; i < data_in.Length; i++)
            {
                _char = data_in[i];
                if (_char == 0xC0)
                {
                    result.Add(0xDB);
                    result.Add(0xDC);
                }
                else
                    if (_char == 0xDB)
                    {
                        result.Add(0xDB);
                        result.Add(0xDD);
                    }
                    else
                    {
                        result.Add(_char);
                    }
            }
            return (result.ToArray());
            /*
            def slip_encode_esc_chars(data_in):
                """Encode esc characters in a SLIP package.

                Replace 0xCO  with 0xDBDC and 0xDB with 0xDBDD.

                 :type str data_in: str to encode
                 :return: str with encoded packet
                """
                result = []
                data = []
                for i in data_in:
                    data.append(ord(i))

                while len(data):
                    char = data.pop(0)
                    if char == 0xC0:
                        result.extend([0xDB, 0xDC])
                    elif char == 0xDB:
                        result.extend([0xDB, 0xDD])
                    else:
                        result.append(char)
                return ''.join(chr(i) for i in result)
            */

        }

        //@staticmethod
        public static Byte[] slip_decode_esc_chars(Byte[] data)
        {
            //"""Replace 0xDBDC with 0xCO and 0xDBDD with 0xDB"""
            Byte _char;
            Byte _char2;
            Byte[] _buf;
            int _len;

            Byte[] result;
            Byte _char_C0;
            Byte _char_DB;
            int pos;

            _char_C0 = 0xC0;
            _char_DB = 0xDB;

            _buf = new Byte[data.Length];
            _len = 0;

            pos = 0;
            int dlen = data.Length;
            while (pos < dlen)
            {
                _char = data[pos++]; //data.pop(0);
                if (_char == 0xDB)
                {
                    _char2 = data[pos++]; //data.pop(0);
                    if (_char2 == 0xDC)
                        _buf[_len++] = _char_C0;
                    else
                        if (_char2 == 0xDD)
                            _buf[_len++] = _char_DB;
                        else
                        {
                            //raise Exception('Char 0xDB NOT followed by 0xDC or 0xDD');
                            Console.Write("buf32_decode_esc_chars: Exception: Char 0xDB NOT followed by 0xDC or 0xDD \n");
                            return (null);
                        }
                }
                else
                {
                    _buf[_len++] = _char;
                }
            }

            result = new Byte[_len];
            for (pos = 0; pos < _len; pos++)
            {
                result[pos] = _buf[pos];
            }

            return result;
            /*
            def slip_decode_esc_chars(data):
                """Decode esc characters in a SLIP package.

                Replaces 0xDBDC with 0xCO and 0xDBDD with 0xDB.

                :return: str decoded data
                :type str data: data to decode
                """
                result = []
                while len(data):
                    char = data.pop(0)
                    if char == 0xDB:
                        char2 = data.pop(0)
                        if char2 == 0xDC:
                            result.append(0xC0)
                        elif char2 == 0xDD:
                            result.append(0xDB)
                        else:
                            raise NordicSemiException('Char 0xDB NOT followed by 0xDC or 0xDD')
                    else:
                        result.append(char)
                return result
            */
        }
    }















    static class my_dfu_util
    {

        public static void buf32_print(String s, Byte[] P)
        {
            UInt32 i;
            //int  j;
            //Console.Write("%s\n", s);
            Console.Write("{0}\n", s);

            for (i = 0; i < P.Length; i++)
            {
                Console.Write("{0:x02} ", P[i]);
                if ((i % 16) == 15)
                    Console.Write("\n");
            }
            Console.Write("\n");
        }

        public static void buf32_printShort(String s, Byte[] P)
        {
            UInt32 i;
            UInt32 LO, HI;
            //int  j;

            LO = 32;
            HI = 32;
            if (P.Length > 32)
            {
                //printf("P->length = %d\n", P->length); 
                HI = (UInt32)( P.Length - 32 );
                //printf("    HI = %d\n", HI);
                HI = HI / 16;
                //printf("    HI = %d\n", HI);
                HI = HI * 16;
                //printf("    HI = %d\n", HI);
                if ((P.Length - HI) > 32)
                    HI = HI + 16;
            }

            Console.Write("{0}\n", s);

            for (i = 0; i < P.Length; i++)
            {
                if ((i < LO) || (i >= HI))
                {
                    Console.Write("{0:x02} ", P[i]);
                    if ((i % 16) == 15)
                        Console.Write("\n");
                }
                else
                    if (i == LO)
                    {
                        //Console.Write("------------------------------------------------\n");
                        Console.Write(" ...\n");
                    }
            }
            Console.Write("\n");
        }   
    }


}



