using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace nRFdfu_cs
{
    class DfuProgram
    {

        // ref: my_dfu_OverSerial_program
        public int dfuProgram_RTR500BLE(String fileName, int Hexfile_type, String ComportName, UInt32 baudRate, int flowControl)
        {
            DfuMain dfuMain = new DfuMain();
            int r;
            r = 0;

            Console.Write("fileName     = {0}\n", fileName);
            Console.Write("Hexfile_type = {0}\n", Hexfile_type);
            Console.Write("ComportName  = {0}\n", ComportName);
            Console.Write("baudRate     = {0}\n", baudRate);
            Console.Write("flowControl  = {0}\n", flowControl);

            ks_hexToZip hexToZip = new ks_hexToZip();
            hexToZip.test_1(fileName);

            //dfu_main();
            // dfuMain.serial_RTR500("dummyZipPath", "COM61", 500000, 1); //    COM61 == RTR500BLE
  
            return (r);
        }


        static public int hex2bin( String hexFileName, String binFileName, String datFileName )
        {
            int r = 0;
            ks_hexToZip hexToZip = new ks_hexToZip();

            r = hexToZip.hex2binNdat(hexFileName, binFileName, datFileName);
            return (r);
        }

        static public int dfuUpdate_RTR500BLE_bin(String comPortName, String binFileName, String datFileName)
        {
            int r = 0;
            bool bSts;
            DfuMain dfuMain = new DfuMain();

            bSts = dfuMain.update_RTR500_bin(comPortName, binFileName, datFileName);
            if (bSts)
                r = 0;
            else
                r = -1;

            return (r);
        }


        // Pass Method as Parameter using C#
        // http://stackoverflow.com/questions/2082615/pass-method-as-parameter-using-c-sharp
        // Func<int, object,int> arg

        static public int dfuUpdate_RTR500BLE_bin_2(Func<int, object, int> arg, String comPortName, String binFileName, String datFileName)
        {
            int r = 0;
            bool bSts;
            DfuMain dfuMain = new DfuMain();


            string[] workerResult = new string[2];
            workerResult[0] = "info";
            workerResult[1] = "calling arg(workerResult); in dfuUpdate_RTR500BLE_bin_2";
            r = arg(42, workerResult);

            bSts = dfuMain.update_RTR500_bin_2(arg, comPortName, binFileName, datFileName);
            if (bSts)
                r = 0;
            else
                r = -1;

            return (r);
        }

        static public int dfuUpdate_Me_bin_2(Func<int, object, int> arg,  String Address, Int32 Baud_rate, String comPortName, String binFileName, String datFileName)
        {
            int r = 0;
            bool bSts;
            DfuMain dfuMain = new DfuMain();


            string[] workerResult = new string[2];
            workerResult[0] = "info";
            workerResult[1] = "calling arg(workerResult); in dfuUpdate_RTR500BLE_bin_2";
            r = arg(42, workerResult);

            //public bool update_Me_bin_2(Func<int, object, int> arg, String Address, Int32 Baud_rate, String comPortName, String binFileName, String datFileName)
            bSts = dfuMain.update_Me_bin_2(arg, Address, Baud_rate, comPortName, binFileName, datFileName);

            if (bSts)
                r = 0;
            else
                r = -1;

            return (r);
        }


    }
}
