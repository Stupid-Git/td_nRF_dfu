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
    public partial class FormRTR500BLE : Form
    {
        List<String> m_theRTR500BLEComPortList;
        String m_appHexFileName = "";

        public FormRTR500BLE()
        {
            InitializeComponent();

            m_theRTR500BLEComPortList = new List<String>();

            m_appHexFileName = appCmn_Config.GetSavedFilename();
            tbFilename.Text = m_appHexFileName;
        }

        private void radioButton_Application_CheckedChanged(object sender, EventArgs e)
        {

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

            DfuProgram.dfuUpdate_RTR500BLE_bin(m_theRTR500BLEComPortList[0], "dfu_temp.bin", "dfu_temp.dat");


        }

        private void btSerialPortScan_Click(object sender, EventArgs e)
        {
            //appCommon.serialPortSearch();
            //appCommon.serialPortSearch_1();

            m_theRTR500BLEComPortList = appCmn_SerialPorts.getRTR500UsbSerialPortList();

            foreach (var S in m_theRTR500BLEComPortList)
            {
                Console.WriteLine(" ------> {0}\n", S);
            }

        }

        private void btnTest1_Click(object sender, EventArgs e)
        {

        }

        private void btn_test_Dfu_transport_Click(object sender, EventArgs e)
        {

        }
    }
}
