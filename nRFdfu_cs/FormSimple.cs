using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace nRFdfu_cs
{
    public partial class FormSimple : Form
    {

        List<String> m_theRTR500BLEComPortList;
        String m_appHexFileName = "";

        public FormSimple()
        {
            InitializeComponent();

            InitializeBackgroundWorker();

            m_theRTR500BLEComPortList = new List<String>();

            m_appHexFileName = appCmn_Config.GetSavedFilename();
            tbFilename.Text = m_appHexFileName;
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {

            //openFileDialog1.InitialDirectory = "c:\\";  
            openFileDialog1.InitialDirectory = appCmn_Config.GetSavedFilename();

            openFileDialog1.Title = "Open Hex Image or ZIP File";
            //openFileDialog1.Filter = "CP files (*.cp)|*.cp|All files (*.*)|*.*|txt files (*.txt)|*.txt";  
            //openFileDialog1.Filter = "テキスト ファイル (*.txt)|*.txt|すべてのファイル (*.*)|*.*";
            openFileDialog1.Filter = "HEX files (*.hex)|*.hex| Zip files (*.zip)|*.zip";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.Multiselect = false; //true;  

            /*
            if ( openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK )  
            {  
                array<String> lines = System.IO.File.ReadAllLines(openFileDialog1.FileName);  
                foreach (String line in lines) {  
                    //?????  
                    System.Diagnostics.Debug.WriteLine("",line);  
                }  
            }
            */
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (openFileDialog1.FileName != "")
                {
                    //tbFilename.Text = openFileDialog1.FileName;
                    appCmn_Config.SaveFilename(openFileDialog1.FileName);
                    tbFilename.Text = appCmn_Config.GetSavedFilename();

                }
                foreach (String file in openFileDialog1.FileNames)
                {
                    System.Diagnostics.Debug.WriteLine("", file);
                    Console.WriteLine(">> {0}", file);
                    /*
                    array<String> lines = System.IO.File.ReadAllLines(file);  
                    foreach (String line in lines) 
                    {  
                        System.Diagnostics.Debug.WriteLine("",line);  
                        Console.WriteLine(">> {0}",line);  
                    }
                    */
                }
            }

            //appCmn_Config.vvvReadAllSettings();
        }

        class bgWorker_2_argument
        {
            public String portName;
            public String binFileName;
            public String datFileName;
        }

        private void btnProgram_Click(object sender, EventArgs e)
        {
            //if (m_theRTR500BLEComPortList.Count == 0)
            //{
            m_theRTR500BLEComPortList = appCmn_SerialPorts.getRTR500UsbSerialPortList();
            if (m_theRTR500BLEComPortList.Count == 0)
            {
                MessageBox.Show("RTR-500BLE が接続していないようです。", "", MessageBoxButtons.OK);
                return;
            }
            //}
            if (m_theRTR500BLEComPortList.Count > 1)
            {
                MessageBox.Show("接続しているRTR-500BLE を一つだけにしてください。", "", MessageBoxButtons.OK);
                return;
            }


            // ... continue
            m_appHexFileName = tbFilename.Text;
            DfuProgram.hex2bin(m_appHexFileName, "dfu_temp.bin", "dfu_temp.dat");

#if false // OLD direct way
            DfuProgram.dfuUpdate_RTR500BLE_bin(m_theRTR500BLEComPortList[0], "dfu_temp.bin", "dfu_temp.dat");
#else
            bgWorker_2_argument arg = new bgWorker_2_argument();
            arg.portName = m_theRTR500BLEComPortList[0];
            arg.binFileName = "dfu_temp.bin";
            arg.datFileName = "dfu_temp.dat";

            this.btnProgram.Enabled = false;

            backgroundWorker1.RunWorkerAsync(arg);
#endif

        }

        private void exampleCancelButton_Click(object sender, EventArgs e)
        {
            // Cancel the asynchronous operation.
            this.backgroundWorker1.CancelAsync();
        }




        class bgWorker_3_argument
        {
            public String Address;
            public int Baud_rate;
            public String portName;
            public String binFileName;
            public String datFileName;
        }


        private void button1_Click(object sender, EventArgs e)
        {

            int r = 0;
            bool bSts;
            DfuMain dfuMain = new DfuMain();


            string[] workerResult = new string[2];
            workerResult[0] = "info";
            workerResult[1] = "calling arg(workerResult); in dfuUpdate_RTR500BLE_bin_2";
           // r = arg(42, workerResult);


#if false // OLD direct way
            DfuProgram.dfuUpdate_RTR500BLE_bin(m_theRTR500BLEComPortList[0], "dfu_temp.bin", "dfu_temp.dat");
            //bSts = dfuMain.update_Me_bin("112233445566", 1000000, "COM3", "test.bin", "test.dat");
            bSts = dfuMain.update_Me_bin("EE0DFCA3E988", 1000000, "COM3", "test.bin", "test.dat");
            if (bSts)
                r = 0;
            else
                r = -1;
#else
            //bSts = dfuMain.update_Me_bin("EE0DFCA3E988", 1000000, "COM3", "test.bin", "test.dat");
            bgWorker_3_argument arg3 = new bgWorker_3_argument();
            arg3.Address = "EE0DFCA3E988";
            arg3.Baud_rate = 1000000;
            arg3.portName = "COM3";
            arg3.binFileName = "test.bin";
            arg3.datFileName = "test.dat";

            this.btnProgram.Enabled = false;

            backgroundWorker1.RunWorkerAsync(arg3);
#endif
            

            //return (r);
        }
        //=====================================================================
        //=====================================================================
        //=====================================================================
        //=====================================================================
        //=====================================================================
        
        private void btnTest1_Click(object sender, EventArgs e)
        {
            
            long t2;
            long t1;
            /**/
            t1 = DateTime.UtcNow.Ticks;
            Console.WriteLine("t1 = {0}", t1);
            t2 = t1;
            while (t2 == t1)
                t2 = DateTime.UtcNow.Ticks;

            Console.WriteLine("t2 = {0}", t2);
            Console.WriteLine("t2 - t1 = {0}", t2 - t1);
            /**/
            /*
              t1 = 635924151670437162
              t2 = 635924151670447163
              t2 - t1 = 10001
             */


            double dt2;
            double dt1;

            dt1 = my_time.time();
            Console.WriteLine("dt1 = {0}", dt1);
            dt2 = dt1;
            while (dt2 == dt1)
                dt2 = my_time.time();

            Console.WriteLine("dt2 = {0}", dt2);
            Console.WriteLine("dt2 - t1 = {0}", dt2 - dt1);
            /*
              dt1 = 63592416378.1095
              dt2 = 63592416378.1105
              dt2 - t1 = 0.00099945068359375
             */


        }

        private void btSerialPortScan_Click(object sender, EventArgs e)
        {
            m_theRTR500BLEComPortList = appCmn_SerialPorts.getRTR500UsbSerialPortList();

            foreach (var S in m_theRTR500BLEComPortList)
            {
                Console.WriteLine(" ------> {0}\n", S);
            }
        }

        //=====================================================================
        //=====================================================================
        //=====================================================================
        //=====================================================================
        //=====================================================================



        public delegate void objectProcessor_del(object obj);

        class Objectprocessor_2
        {
            BackgroundWorker worker;

            public Objectprocessor_2(object sender, DoWorkEventArgs e)
            {
                worker = sender as BackgroundWorker;
            }

            public int processObject(int percentProgress, object userState)
            {
                worker.ReportProgress(percentProgress, userState);
                return (0);
            }
            /*public int processObject(int i, object obj)
            {
                worker.ReportProgress(42, obj);
                return (0);
            }*/
        }


        //=====================================================================
        //=====================================================================
        //===== backgroundWorker ==============================================
        //=====================================================================
        //=====================================================================
        private System.ComponentModel.BackgroundWorker backgroundWorker1;

        // Set up the BackgroundWorker object by attaching event handlers. 
        private void InitializeBackgroundWorker()
        {
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();

            this.backgroundWorker1.WorkerReportsProgress = true;
            this.backgroundWorker1.WorkerSupportsCancellation = true;

            backgroundWorker1.DoWork += new DoWorkEventHandler(backgroundWorker1_DoWork);
            backgroundWorker1.RunWorkerCompleted += new RunWorkerCompletedEventHandler(backgroundWorker1_RunWorkerCompleted);
            backgroundWorker1.ProgressChanged += new ProgressChangedEventHandler(backgroundWorker1_ProgressChanged);
        }


        public int localProcessObject(int percentProgress, object userState)
        {
            backgroundWorker1.ReportProgress(percentProgress, userState);
            return (0);
        }

        // This event handler is where the actual, potentially time-consuming work is done.
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            int rtn;
            // Get the BackgroundWorker that raised this event.
            BackgroundWorker worker = sender as BackgroundWorker;

            // Assign the result of the computation
            // to the Result property of the DoWorkEventArgs
            // object. This is will be available to the 
            // RunWorkerCompleted eventhandler.

            if (e.Argument.GetType() == typeof(bgWorker_2_argument))
            {
                Console.WriteLine("########### bgWorker_2_argument");
                bgWorker_2_argument arg = e.Argument as bgWorker_2_argument;

                Console.WriteLine("arg.portName    = {0}", arg.portName);
                Console.WriteLine("arg.binFileName = {0}", arg.binFileName);
                Console.WriteLine("arg.datFileName = {0}", arg.datFileName);

                //int r = DfuProgram.dfuUpdate_RTR500BLE_bin(arg.portName, arg.binFileName, arg.datFileName);

                /*
                Objectprocessor_2 op_2 = new Objectprocessor_2(sender, e);
                rtn = DfuProgram.dfuUpdate_RTR500BLE_bin_2(op_2.processObject, arg.portName, arg.binFileName, arg.datFileName);
                */
                rtn = DfuProgram.dfuUpdate_RTR500BLE_bin_2(localProcessObject, arg.portName, arg.binFileName, arg.datFileName);

                if (rtn == 0)
                {
                    e.Result = "Update complete";// +" r = " + rtn.ToString();
                }
                else
                {
                    e.Result = "Update FAILED" + " r = " + rtn.ToString();
                }

                return; // the backgoround task terminates
            }
            else
            if (e.Argument.GetType() == typeof(bgWorker_3_argument))
            {
                Console.WriteLine("########### bgWorker_3_argument");
                bgWorker_3_argument arg = e.Argument as bgWorker_3_argument;

                Console.WriteLine("arg.Address     = {0}", arg.Address);
                Console.WriteLine("arg.Baud_rate   = {0}", arg.Baud_rate);
                Console.WriteLine("arg.portName    = {0}", arg.portName);
                Console.WriteLine("arg.binFileName = {0}", arg.binFileName);
                Console.WriteLine("arg.datFileName = {0}", arg.datFileName);

                //int r = DfuProgram.dfuUpdate_RTR500BLE_bin(arg.portName, arg.binFileName, arg.datFileName);

                /*
                Objectprocessor_2 op_2 = new Objectprocessor_2(sender, e);
                rtn = DfuProgram.dfuUpdate_RTR500BLE_bin_2(op_2.processObject, arg.portName, arg.binFileName, arg.datFileName);
                */
                timer1.Start();
                rtn = DfuProgram.dfuUpdate_Me_bin_2(localProcessObject, arg.Address, arg.Baud_rate, arg.portName, arg.binFileName, arg.datFileName);

                if (rtn == 0)
                {
                    e.Result = "Update complete";// +" r = " + rtn.ToString();
                }
                else
                {
                    e.Result = "Update FAILED" + " r = " + rtn.ToString();
                }

                return; // the backgoround task terminates
            }
            else /// testing stuff
            {
                if (e.Argument.GetType() == typeof(int))
                    Console.WriteLine("########### INT");
                if (e.Argument.GetType() == typeof(String))
                    Console.WriteLine("########### STRING");

                System.Threading.Thread.Sleep(2000);

                if (worker.CancellationPending)
                {
                    e.Cancel = true;
                    e.Result = "Task Was Cancelled";
                }

                string[] workerResult = new string[2];
                workerResult[0] = "String1";
                workerResult[1] = "String2";
                worker.ReportProgress(42, workerResult);
                
                e.Result = "This is an object";                
            }
        }

        // This event handler deals with the results of the background operation.
        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            // First, handle the case where an exception was thrown.
            if (e.Error != null)
            {
                MessageBox.Show(e.Error.Message);
            }
            else if (e.Cancelled)
            {
                // Next, handle the case where the user canceled 
                // the operation.
                // Note that due to a race condition in 
                // the DoWork event handler, the Cancelled
                // flag may not have been set, even though
                // CancelAsync was called.
                resultLabel.Text = "Canceled";
            }
            else
            {
                // Finally, handle the case where the operation 
                // succeeded.
                resultLabel.Text = e.Result.ToString();
            }

            btnProgram.Enabled = true;
        }

        bool enableUpdate;
        String richText = "";
        // This event handler updates the progress bar.
        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {   
            string[] workerResult = (string[])e.UserState;

            //Console.WriteLine("##########----------------------------------------");
            //Console.WriteLine("##########  " + workerResult[0]);
            //Console.WriteLine("##########  " + workerResult[1]);
            //MessageBox.Show(workerResult[0]);

            //Console.WriteLine(">>  " + workerResult[0] + ": " + workerResult[1] + " <<");
            //richTextBox1.AppendText(workerResult[0] + ": " + workerResult[1] + "\r\n");
            richText += workerResult[0] + ": " + workerResult[1] + "\r\n";
            /*
            if (enableUpdate == true)
            {
                enableUpdate = false;
                richTextBox1.AppendText( richText );
                richTextBox1.ScrollToCaret();
                richText = "";
            }
            */
            
            if (workerResult[0] == "update_progress")
            {
                resultLabel.Text = "Update_progress: " + workerResult[1];
                this.progressBar1.Value = e.ProgressPercentage;
            }
            /*
            if(workerResult[0] == "status")
            {
                resultLabel.Text = "Status: " + workerResult[1];
            }
            
            if(workerResult[0] == "error")
            {
                resultLabel.Text = "Error: " + workerResult[1];
            }
            
            if (workerResult[0] == "debug")
            {
                resultLabel.Text = "Debug: " + workerResult[1];
            }
            
            if(workerResult[0] == "info")
            {
                resultLabel.Text = "Info: " + workerResult[1];
            }
            */
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            enableUpdate = true;
            timer1.Start();

            if (enableUpdate == true)
            {
                enableUpdate = false;
                if (richText.Length != 0)
                {
                    richTextBox1.AppendText(richText);
                    richTextBox1.ScrollToCaret();
                    richText = "";
                }
            }
            

            //richTextBox1.ScrollToCaret();
        }




    }



}
