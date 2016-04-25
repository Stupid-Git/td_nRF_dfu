using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;

namespace nRFdfu_cs
{
    public abstract class DfuSerial
    {
        public abstract int dfuSer_Init_and_open();
        public abstract int dfuSer_DeInit_close();
        public abstract bool dfuSer_is_open();
        public abstract int dfuSer_send_packet(Byte[] pkt);
    }


    public class DfuSerial_UW : DfuSerial
    {
        public String portName = "";
        SerialPort serialPort1 = null;


        public override int dfuSer_Init_and_open()
        {
            int r = 0;

            if (serialPort1 == null)
            {
                serialPort1 = new SerialPort(this.portName, 500000, Parity.None, 8, StopBits.One);
            }
            serialPort1.Open();
            r = this.send_recv_SET_DEVICE_DFU_MODE_ON();
            return (r);
        }

        public override int dfuSer_DeInit_close()
        {
            int r = 0;
         // r = this.send_recv_SET_DEVICE_DFU_MODE_OFF();
            r = this.send_recv_SET_DEVICE_DFU_MODE_OFF_NoReset();
            serialPort1.Close();
            return (r);
        }

        public override bool dfuSer_is_open()
        {
            if (serialPort1 == null)
                return (false);
            return (serialPort1.IsOpen);
            //return (false);
        }
        public override int dfuSer_send_packet(Byte[] pkt)
        {
            int r = 0;
//#if true
//            Console.WriteLine("Testing <dfuSer_send_packet> Len = {0}", pkt.Length);
//#else
            r = send_packet_UW(pkt);
//#endif
            return (r);
        }

        const Byte CMD_nRF_MODE = 0xe6;     // ｎＲＦ５１ブートモード設定
        const Byte CMD_nRF_SLIPWRAP = 0xe7; // ｎＲＦ５１ＳＬＩＰブロックラップ

        Byte[] password_user = new Byte[8] { (Byte)'C', (Byte)'r', (Byte)'p', (Byte)'t', (Byte)'0', (Byte)'8', (Byte)'0', (Byte)'1' };   // ユーザーアプリケーション　ＵＳＢコマンドのパスワード（８バイト）


        void delay(UInt32 value)
        {
            //Sleep( value);
            double time_in_seconds = ((double)value) / 1000.0;
            my_time.sleep(time_in_seconds);
        }

        UInt16 BitConverter_D_ToUInt16(Byte[] buffer, UInt16 position)
        {
            UInt16 value;
            value = 0;
            value += buffer[position + 0];
            value += (UInt16)(((UInt16)(buffer[position + 1])) << 8);
            return (value);
        }

        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        int sendCommand(Byte[] sData, UInt16 length)
        {
            UInt16 i, len, sum;
            int r;

            serialPort1.DiscardInBuffer();
            serialPort1.DiscardOutBuffer();

            len = BitConverter_D_ToUInt16(sData, 3);

            for (sum = 0, i = 0; i < len + 5; i++) sum += sData[i];

            sData[len + 5] = (byte)sum;
            sData[len + 6] = (byte)(sum / 256);

            r = 0;

            serialPort1.Write(sData, 0, len + 7);
            /*
            ur = _Write_A_RAW(0, sData, len + 7, &dwBytesWritten);
            
            if (ur != 0)
                r = -1;
            if (len + 7 != dwBytesWritten)
                r = -1;
            */
            return (r);
        }

        bool recvCommand(Byte[] rData, ref UInt32 rLength)
        {
            int i;
            UInt16 len, sum;


            for (i = 0; i < 5; i++)
            {
                if ((i == 1) && (rData[0] != 0x01))
                {
                    //Console.Write("recvCommand: flushing non 0x01 char = 0x%02x\n", rData[0]);
                    Console.Write("recvCommand: flushing non 0x01 char = 0x{0:x02}\n", rData[0]);
                    i = 0;
                }
                // ur = _Read_A_RAW(0, &rData[i], 1, &dwBytesRead); //  
                rData[i] = (byte)serialPort1.ReadByte();
                //if (ur != 0)
                //{
                //    continue;
                //}
            }

            if (rData[0] != 0x01)
            {
                return false;
            }

            len = (UInt16)(BitConverter_D_ToUInt16(rData, 3) + 7);

            if (len > 4096) return false;

            i = 0;
            //while (serialPort1_D_BytesToRead() < (len - 5))         // 予定バイト数が受信されてからＲｅａｄすると早い
            //{
            //    Application_D_DoEvents();
            //}
            while (i < len - 5)
            {
                int n = serialPort1.Read(rData, 5 + i, len - 5 - i);
                if (n > 0) i += (UInt16)n;
                //ur = _Read_A_RAW(0, &rData[5 + i], len - 5 - i, &dwBytesRead);
                //if (ur != 0)
                //{
                //    continue;
                //}
                //if (dwBytesRead > 0) i += (UInt16)dwBytesRead;
                //Application_D_DoEvents();
            }

            sum = 0;
            for (i = 0; i < len - 2; i++) sum += rData[i];

            if (sum != BitConverter_D_ToUInt16(rData, (UInt16)(len - 2)))
                return false;

            return true;
        }


        //-----------------------------------------------------------------------------
        int send_recv_SET_DEVICE_DFU_MODE_ON()
        {
           // UInt32 dwBytesWritten;
            UInt32 rLength = 0;
            //UInt32 r;
            Byte []  sData = new Byte[128];
            Byte []  rData = new Byte[128];
            
            //===== STEP 1 - Mode selecton =====
            // ブレーク信号送出
            // ブレーク信号送出
            sData[0] = 0x00;
            serialPort1.Write(sData, 0, 1); //            r = self.serial_medium._Write_A_RAW(0, sData, 1, &dwBytesWritten);
            delay(50);

            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;  // 0xE6
            sData[2] = 0x01;                   // 0x01 (0ｘ00:開放 0ｘ01:確保) DFU_MODE sub command 1
            sData[3] = (Byte)0x08;          // 0x08 len LSB
            sData[4] = (Byte)0x00;          // 0x00 len MSB

            Buffer.BlockCopy(password_user, 0, sData, 5, 8); //   memcpy(&sData[5], password_user, 8);

            sendCommand(sData, 5 + 8);
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: 受信異常\r\n");
            }
            else
            {
                if( rData[2] != 0x06)
                    Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: DID NOT GET ACK\r\n");
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: OK\r\n");
            }


            //===== STEP 2 - RESET-LOW, BOOT-LOW =====
            // ブレーク信号送出
            //sData[0] = 0x00;
            //r = self.serial_medium._Write_A_RAW(0, sData, 1, &dwBytesWritten);
            //delay(50);
            
            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;  // 0xE6
            sData[2] = 0x02;                   // 0x02  //DFU_MODE sub command 2 - RESET, BOOT CONTROL
            sData[3] = (Byte)0x02;          // 0x02 len LSB //ERROR-was-0x08
            sData[4] = (Byte)0x00;          // 0x00 len MSB

            sData[5] = (Byte)0x00;          // 0x00 RESET LOW  
            sData[6] = (Byte)0x00;          // 0x00 BOOT  LOW

            sendCommand(sData, 5 + 2); //was 5 + 8
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : 受信異常\r\n");
            }
            else
            {
                if( rData[2] != 0x06)
                    Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : DID NOT GET ACK\r\n");
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-LOW, BOOT-LOW : OK\r\n");
            }

            delay(1000);

            //===== STEP 3 - RESET-HIGH, BOOT-LOW =====
            // ブレーク信号送出
            //sData[0] = 0x00;
            //r = self.serial_medium._Write_A_RAW(0, sData, 1, &dwBytesWritten);
            //delay(50);

            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;  // 0xE6
            sData[2] = 0x02;                   // 0x02  //DFU_MODE sub command 2 - RESET, BOOT CONTROL
            sData[3] = (Byte)0x02;          // 0x02 len LSB //ERROR-was-0x08
            sData[4] = (Byte)0x00;          // 0x00 len MSB

            sData[5] = (Byte)0x01;          // 0x00 RESET HIGH
            sData[6] = (Byte)0x00;          // 0x00 BOOT  LOW

            sendCommand(sData, 5 + 2); //was 5 + 8
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-LOW : 受信異常\r\n");
            }
            else
            {
                if( rData[2] != 0x06)
                    Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-LOW : DID NOT GET ACK\r\n");
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-LOW : OK\r\n");
            }


            delay(1000);

            //===== STEP 3 - RESET-HIGH, BOOT-HIGH =====
            // ブレーク信号送出
            //sData[0] = 0x00;
            //r = self.serial_medium._Write_A_RAW(0, sData, 1, &dwBytesWritten);
            //delay(50);

            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;  // 0xE6
            sData[2] = 0x02;                   // 0x02  //DFU_MODE sub command 2 - RESET, BOOT CONTROL
            sData[3] = (Byte)0x02;          // 0x02 len LSB //ERROR-was-0x08
            sData[4] = (Byte)0x00;          // 0x00 len MSB

            sData[5] = (Byte)0x01;          // 0x00 RESET HIGH  
            sData[6] = (Byte)0x01;          // 0x00 BOOT  HIGH

            sendCommand(sData, 5 + 2); //was 5 + 8
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-HIGH : 受信異常\r\n");
            }
            else
            {
                if( rData[2] != 0x06)
                    Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-HIGH : DID NOT GET ACK\r\n");
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_ON: RESET-HIGH, BOOT-HIGH : OK\r\n");
            }
            
            //logger.debug("PC . target: {0}".format(pkt))
    
            return(0);
        }


        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        int send_recv_SET_DEVICE_DFU_MODE_OFF_NoReset()
        {
            //UInt32 dwBytesWritten;
            UInt32 rLength = 0;
            //UInt32 r;
            Byte[] sData = new Byte[128];
            Byte[] rData = new Byte[128];


            // ブレーク信号送出
            sData[0] = 0x00;
            //r = _Write_A_RAW(0, sData, 1, &dwBytesWritten);
            serialPort1.Write(sData, 0, 1);
            delay(50);
            /*
            //===== STEP 3 - RESET-LOW, BOOT-HIGH =====
            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;
            sData[2] = 0x02; //DFU_MODE sub command 2 - RESET, BOOT CONTROL
            sData[3] = (Byte)0x08;
            sData[4] = (Byte)0x00;
            sData[5] = (Byte)0x00; //RESET
            sData[6] = (Byte)0x01; //BOOT

            sendCommand(sData, 5 + 8);
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : 受信異常\r\n");
            }
            else
            {
                if (rData[2] != 0x06)
                    Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : DID NOT GET ACK\r\n");
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : OK\r\n");
            }
            delay(1000);


            //===== STEP 3 - RESET-HIGH, BOOT-HIGH =====
            // ブレーク信号送出
            //sData[0] = 0x00;
            //r = self.serial_medium._Write_A_RAW(0, sData, 1, &dwBytesWritten);
            //delay(50);

            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;
            sData[2] = 0x02; //DFU_MODE sub command 2 - RESET, BOOT CONTROL
            sData[3] = (Byte)0x08;
            sData[4] = (Byte)0x00;
            sData[5] = (Byte)0x01; //RESET
            sData[6] = (Byte)0x01; //BOOT

            sendCommand(sData, 5 + 8);
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : 受信異常\r\n");
            }
            else
            {
                if (rData[2] != 0x06)
                    Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : DID NOT GET ACK\r\n");
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : OK\r\n");
            }
            delay(1000);
            */

            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;
            sData[2] = 0x00; //NORMAL_MODE sub command 0

            sData[3] = (Byte)0x08;
            sData[4] = (Byte)0x00;

            Buffer.BlockCopy(password_user, 0, sData, 5, 8); //   memcpy(&sData[5], password_user, 8);

            sendCommand(sData, 5 + 8);
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: 受信異常\r\n");
            }
            else
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: OK\r\n");
            }

            //logger.debug("PC . target: {0}".format(pkt))
            return (0);
        }

        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        int send_recv_SET_DEVICE_DFU_MODE_OFF()
        {
            //UInt32 dwBytesWritten;
            UInt32 rLength = 0;
            //UInt32 r;
            Byte[] sData = new Byte[128];
            Byte[] rData = new Byte[128];


            // ブレーク信号送出
            sData[0] = 0x00;
            //r = _Write_A_RAW(0, sData, 1, &dwBytesWritten);
            serialPort1.Write(sData, 0, 1);
            delay(50);

            //===== STEP 3 - RESET-LOW, BOOT-HIGH =====
            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;
            sData[2] = 0x02; //DFU_MODE sub command 2 - RESET, BOOT CONTROL
            sData[3] = (Byte)0x08;
            sData[4] = (Byte)0x00;
            sData[5] = (Byte)0x00; //RESET
            sData[6] = (Byte)0x01; //BOOT

            sendCommand(sData, 5 + 8);
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : 受信異常\r\n");
            }
            else
            {
                if (rData[2] != 0x06)
                    Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : DID NOT GET ACK\r\n");
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-LOW, BOOT-HIGH : OK\r\n");
            }
            delay(1000);


            //===== STEP 3 - RESET-HIGH, BOOT-HIGH =====
            // ブレーク信号送出
            //sData[0] = 0x00;
            //r = self.serial_medium._Write_A_RAW(0, sData, 1, &dwBytesWritten);
            //delay(50);

            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;
            sData[2] = 0x02; //DFU_MODE sub command 2 - RESET, BOOT CONTROL
            sData[3] = (Byte)0x08;
            sData[4] = (Byte)0x00;
            sData[5] = (Byte)0x01; //RESET
            sData[6] = (Byte)0x01; //BOOT

            sendCommand(sData, 5 + 8);
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : 受信異常\r\n");
            }
            else
            {
                if (rData[2] != 0x06)
                    Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : DID NOT GET ACK\r\n");
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: RESET-HIGH, BOOT-HIGH : OK\r\n");
            }
            delay(1000);


            sData[0] = 0x01;
            sData[1] = (Byte)CMD_nRF_MODE;
            sData[2] = 0x00; //NORMAL_MODE sub command 0

            sData[3] = (Byte)0x08;
            sData[4] = (Byte)0x00;

            Buffer.BlockCopy(password_user, 0, sData, 5, 8); //   memcpy(&sData[5], password_user, 8);

            sendCommand(sData, 5 + 8);
            if (recvCommand(rData, ref rLength) == false)
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: 受信異常\r\n");
            }
            else
            {
                Console.Write("send_recv_SET_DEVICE_DFU_MODE_OFF: OK\r\n");
            }

            //logger.debug("PC . target: {0}".format(pkt))
            return (0);
        }


        //-------------------
        int countNoOf(List<Byte> L, Byte b0)
        {
            int i;
            int cnt = 0;
            for (i = 0; i < L.Count(); i++)
            {
                if (L[i] == b0)
                    cnt++;
            }
            return (cnt);
        }

        Byte[] m_SLIP_rData = new Byte[4096]; //USED
        UInt32 m_SLIP_rLength;
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        int get_ack_nr_UW_SLIP()
        {
            int ack_val;
            List<Byte> uart_buffer = new List<Byte>();

            for (int i = 0; i < m_SLIP_rLength; i++)
                uart_buffer.Add(m_SLIP_rData[5 + i]);

            UInt32 posC0_0;
            UInt32 posC0_1;

            Console.Write("\r\n");
            if (countNoOf(uart_buffer, 0xC0) < 2)
            {
                Console.Write("Need two 0xC0 bytes\r\n");
                return (0xff);
            }

            posC0_0 = 0;
            while (uart_buffer[(int)posC0_0] != 0xC0)
            {
                posC0_0++;
            }

            posC0_1 = posC0_0 + 1;
            while (uart_buffer[(int)posC0_1] != 0xC0)
            {
                posC0_1++;
            }

            List<Byte> slip_buf = new List<byte>();
            slip_buf.AddRange(uart_buffer.GetRange((int)posC0_0, (int)(posC0_1 - posC0_0) + 1)); // +1 to get length
            //uart_buffer.buffer = &uart_buffer.buffer[posC0_0];
            //uart_buffer.length = posC0_1 - posC0_0;

            //my_dfu_util.buf32_print("get_ack_nr_UW_SLIP", slip_buf.ToArray());
            Byte[] data = my_dfu_util.decode_esc_chars(slip_buf.ToArray());
            //my_dfu_util.buf32_print("get_ack_nr_UW_SLIP --Non SLIP data ---", data);

            /*
             * // Remove 0xC0 at start and beginning
             * data = data[1:-1];
             * / print "non-slip data ", [data]
             * 
             * return (data[0] >> 3) & 0x07;
             * */

            // print "non-slip data ", [data]
            ack_val = (data[1] >> 3) & 0x07;
            //Console.Write("ack_val = 0x%1x, %d\n\n", ack_val, ack_val);
            //Console.Write("ack_val = 0x{0:x1}, {1}\n\n", ack_val, ack_val);
            return (ack_val);
        }

        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        UInt32 send_packet_UW_SLIP(Byte[] buffer, UInt32 length, ref UInt32 p_dwBytesWritten)
        {
            int r = -1;
            Byte[] sData = new Byte[4096];
            int reloop;

            UInt32 SLIP_rLength;

            reloop = 2; // 2 means can re-loop one more time
            while (reloop != 0)
            {
                // ブレーク信号送出
                sData[0] = 0x00;
                serialPort1.Write(sData, 0, 1);
                //r = _Write_A_RAW(0, sData, 1, &dwBytesWritten);
                delay(50);

                sData[0] = 0x01;
                sData[1] = (Byte)CMD_nRF_SLIPWRAP;
                sData[2] = 0x00;

                sData[3] = (Byte)((length & 0x000000ff) >> 0);
                sData[4] = (Byte)((length & 0x0000ff00) >> 8);
                
                Buffer.BlockCopy(buffer, 0, sData, 5, (int)length);                                

                
                m_SLIP_rLength = 0;

                sendCommand(sData, (UInt16)(5 + length) );
                SLIP_rLength = 0;
                if (recvCommand(m_SLIP_rData, ref SLIP_rLength) == false)  //SLIP_rLength unused
                {
                    Console.Write("send_packet_UW_SLIP: 受信異常\r\n");
                    p_dwBytesWritten = 0;
                    r = -1;
                }
                else
                {
                    if (m_SLIP_rData[2] == 0x15)
                    {
                        r = -1;
                        reloop--;
                    }
                    else
                    {
                        //Console.Write("send_packet_UW_SLIP: OK\r\n");
                        m_SLIP_rLength = BitConverter_D_ToUInt16(m_SLIP_rData, 3);
                        p_dwBytesWritten = length;
                        r = 0;
                        reloop = 0;
                    }
                }
            }

            //logger.debug("PC . target: {0}".format(pkt))
            //logger.debug(String.Format("PC . target: {0}", pkt);

            return ((UInt32)(r));
        }




        //-----------------------------------------------------------------------------
        int last_ack = 42;//None==42
        int send_packet_UW(Byte[] pkt)
        {
            UInt32 r_serDev;
            UInt32 dwBytesWritten = 0;

            int attempts = 0;
            int ack = 0;//None
            bool packet_sent = false;

            while (packet_sent == false)
            {
                //r_serDev = self.serial_medium._Write_A_SLIP   (0,    pkt.buffer, pkt.length, &dwBytesWritten);
                r_serDev = send_packet_UW_SLIP(pkt, (UInt32)(pkt.Length), ref dwBytesWritten);
                if (r_serDev != 0)
                {
                    Console.Write("Controller_send_packet: serdev_Write: ERROR = {0}, 0x{1:x8}\n", r_serDev, r_serDev);
                    return (-1);
                }
                if (pkt.Length != dwBytesWritten)
                {
                    Console.Write("Controller_send_packet: serdev_Write: pkt.len[{0}] != dwBytesWritten[{1}]\n", pkt.Length, dwBytesWritten);
                    return (-1);
                }

                attempts += 1;
                //ack = get_ack_nr(P);
                ack = get_ack_nr_UW_SLIP();
                if (ack == 0xFF)
                {
                    Console.Write("Controller_send_packet: Controller_get_ack_nr: ERROR\n");
                    return (-1);
                }


                //orig if( last_ack == 0)//is None:
                //orig     break;
                if (last_ack == 42) //None
                {
                    Console.Write("UW ################################################################\n");
                    Console.Write("UW #####  FIRST ACK ###############################################\n");
                    Console.Write("UW ################################################################\n");
                    last_ack = ack;
                    break;
                }

                if (ack == (last_ack + 1) % 8)
                {
                    last_ack = ack;
                    packet_sent = true;

                    if (attempts > 3)
                    {
                        //raise Exception("Three failed tx attempts encountered on packet {0}".format(pkt.sequence_number))
                        //raise Exception("Three failed tx attempts encountered on packet {0}".format(seq))
                        Console.Write("Three failed tx attempts encountered on packet {0}\n", -1);//.format(seq))
                        return (-1);
                    }
                }
                else
                {
                    Console.Write("UW ################################################################\n");
                    Console.Write("UW #####  RETRY     ###############################################\n");
                    Console.Write("UW ################################################################\n");

                }

            }


            //Console.Write("DfuTransportSerial.send_packet_UW(), packet length is {0}\n", pkt.Length);
            //logger.debug( String.Format("DfuTransportSerial.send_packet_UW(), packet length is {0}\n", pkt.Length) );

            return (0);
        }
    }


    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    public class DfuSerial_NW : DfuSerial
    {


        public override int dfuSer_Init_and_open()
        {
            int r = 0;
            return (r);
        }

        public override int dfuSer_DeInit_close()
        {
            int r = 0;
            return (r);
        }

        public override bool dfuSer_is_open()
        {
            return (false);
        }

        public override int dfuSer_send_packet(Byte[] pkt)
        {
            int r = 0;
//TODO            r = send_packet_NW(pkt);
            return (r);
        }


        int send_recv_SET_DEVICE_DFU_MODE_ON()
        {
            return (42);
        }


        int send_recv_SET_DEVICE_DFU_MODE_OFF()
        {
            Console.Write("\n");
            Console.Write("\n");
            Console.Write("Nothing to do now, but check if your Nordic Device has been updated\n");
            Console.Write("\n");
            Console.Write("\n");

            return (42);
        }
    }

  
}
