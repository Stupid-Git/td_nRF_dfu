using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


using System.Collections;


//Added by karel
using System.Xml;
using System.Xml.Serialization;
using System.Configuration; //.Configuration;
//using System.Configuration.Assemblies; //.Configuration;
using System.Management;

using testAppConfig;

namespace nRFdfu_cs
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }





        //=====================================================================
        //=====================================================================
        private void dfu_Form_Load(System.Object sender, System.EventArgs e)
        {
            tbFilename.Text = GetSavedFilename();
        }

        //=====================================================================
        //=====================================================================
        //http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
        private void updata()
        {
  /*TODO*/
            //OLD String name = ConfigurationSettings.AppSettings["name"];
            //OLD String firstName = ConfigurationSettings.AppSettings["firstName"];
            String name = ConfigurationManager.AppSettings["name"];
            String firstName = ConfigurationManager.AppSettings["firstName"];
            Console.WriteLine("{0} - {1}", firstName, name);

            System.Configuration.Configuration config =
                ConfigurationManager.OpenExeConfiguration(
                ConfigurationUserLevel.None);

            config.AppSettings.Settings.Remove("firstName");
            config.AppSettings.Settings.Add("firstName", "New firstName");

            config.AppSettings.Settings.Remove("filename");
            config.AppSettings.Settings.Add("filename", "C:/abc.hex");

            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection("appSettings");

            //OLD firstName = ConfigurationSettings.AppSettings["firstName"];
            firstName = ConfigurationManager.AppSettings["firstName"];
            Console.WriteLine("{0} - {1}", firstName, name);
/*TODO */
        }

        void mydata()
        {
/*TODO
            MySection mySection = (MySection)ConfigurationManager.GetSection("MySection");

            Console.WriteLine("--- MySection ---");
            Console.WriteLine("{0} ; {1}", mySection.Name, mySection.FirstName);
            Console.WriteLine("");
TODO */
        }

        void testMySection()
        {
/*TODO*/
                        MySection section = (MySection)ConfigurationManager.GetSection("MySection");

                        Console.WriteLine("{0} ; {1}", section.Name, section.FirstName);

                        section.Name += " plusN";
                        section.FirstName += " plusn";
/*TODO */
        }

        void myAppSettingsTest()
        {
/*TODO*/
#if true
            String greeting = ConfigurationManager.AppSettings["greeting"];
            Console.WriteLine(greeting);

            String name = /*Configuration.*/ConfigurationManager.AppSettings["name"];
            String firstName = /*Configuration.*/ConfigurationManager.AppSettings["firstname"];
            Console.WriteLine("My name's {0} {1}", firstName, name);

            String name1 = /*Configuration.*/ConfigurationManager.AppSettings[0];

            foreach (String aValue in ConfigurationManager.AppSettings)
            {
                Console.WriteLine("User: {0} - {1}",
                    aValue, ConfigurationManager.AppSettings[aValue]);
            }

            updata();

            mydata();

            //ConfigurationManager.AppSettings["firstname"] = "NewFirstName";
            testMySection();
#endif // false
/*TODO */
        }

        private void btnTest1_Click(object sender, EventArgs e)
        {
            
            myAppSettingsTest();
            

            /*
            String fname1 = GetSavedFilename();
            Console.WriteLine("[1] filename1 = {0}", fname1);

            if(fname1 == "")
                SaveFilename("c:\\abc.def");
            else
                SaveFilename( fname1 );

            String fname2 = GetSavedFilename();
            Console.WriteLine("[2] filename2 = {0}", fname2);
            */

            appCmn_SerialPorts.serialPortSearch();
            //appCommon.serialPortSearch_1();
        }


        static void vvvReadAllSettings()
        {
            try
            {
                var appSettings = ConfigurationManager.AppSettings;
                if (appSettings.Count == 0)
                {
                    Console.WriteLine("AppSettings is empty.");
                }
                else
                {
                    foreach (var key in appSettings.AllKeys)
                    {
                        Console.WriteLine("Key: {0} Value: {1}", key, appSettings[key]);
                    }
                }
            }
            catch (ConfigurationErrorsException) { Console.WriteLine("Error reading app settings"); }
        }
        String GetSavedFilename()
        {
            vvvReadAllSettings();

            System.Collections.Specialized.NameValueCollection appSettings;

            appSettings = ConfigurationManager.AppSettings;
            String filename = appSettings["filename"];
            if (filename == null)
                filename = "";
            return (filename);
        }

        void SaveFilename(String new_filename)
        {
            string key = "filename";
            string value = new_filename;
            try
            {
                var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                var settings = configFile.AppSettings.Settings;
                if (settings[key] == null)
                {
                    settings.Add(key, value);
                }
                else
                {
                    settings[key].Value = value;
                }
                configFile.Save(ConfigurationSaveMode.Modified);
                ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);
            }
            catch (ConfigurationErrorsException) { Console.WriteLine("Error writing app settings"); }

/*TODO
            System.Configuration.Configuration config =
                ConfigurationManager.OpenExeConfiguration(
                ConfigurationUserLevel.None);

            config.AppSettings.Settings.Remove("filename");
            config.AppSettings.Settings.Add("filename", new_filename);

            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection("appSettings");

            //String  filename = ConfigurationSettings.AppSettings["filename"];
            //if( filename == nullptr )
            //    filename = "";
TODO*/
        }
        /**/



        //http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
        //=====================================================================
        //=====================================================================

        private void btn_test_Dfu_transport_Click(object sender, EventArgs e)
        {
            // Check
            //   https://github.com/NordicSemiconductor/pc-nrfutil/blob/master/nordicsemi/__main__.py

            // I basically copied this one 'cos it goes from HEX to Program and is shorter
            // and easier to understand than the 'current' version above
            //   https://github.com/tnesheim/GradOS_FinalProject/tree/master/with_riot/examples/hello-world/sdk7.2/examples/dfu/hci_dfu_send_hex
            //   https://github.com/tnesheim/GradOS_FinalProject/blob/3c97958d79eb87d5f870c98dd7a63354a04e882d/with_riot/examples/hello-world/sdk7.2/examples/dfu/hci_dfu_send_hex/hci_dfu_send_hex.py

            // IntelHex Util form
            //   https://github.com/martin-helmich/libcintelhex

//TODO C#            Controller_upload_firmware_1(); // see hci_dfu_send_hex_1.c
            //Controller_upload_firmware(); // see hci_dfu_send_hex.c

            //test_Dfu_transport();
        }


        //=====================================================================
        // http://stackoverflow.com/questions/15576616/how-do-i-read-multiple-files-after-click-opened-in-openfiledialog
        private void btnBrowse_Click(object sender, EventArgs e)
        {
            //openFileDialog1.InitialDirectory = "c:\\";  
            openFileDialog1.InitialDirectory = GetSavedFilename();

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
                    SaveFilename(openFileDialog1.FileName);
                    tbFilename.Text = GetSavedFilename();

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
        }


        //---------------------------------------------------------------------
        void radioButton_Softdevice_CheckedChanged(System.Object sender, System.EventArgs e)
        {
            if (radioButton_Softdevice.Checked)
            {
                radioButton_Application.Checked = false;
                radioButton_Bootloader.Checked = false;
            }
        }
        void radioButton_Application_CheckedChanged(System.Object sender, System.EventArgs e)
        {
            if (radioButton_Application.Checked)
            {
                radioButton_Softdevice.Checked = false;
                radioButton_Bootloader.Checked = false;
            }
        }
        void radioButton_Bootloader_CheckedChanged(System.Object sender, System.EventArgs e)
        {
            if (radioButton_Bootloader.Checked)
            {
                radioButton_Softdevice.Checked = false;
                radioButton_Application.Checked = false;
            }
        }

        //---------------------------------------------------------------------
        void comboBox_Comport_SelectedIndexChanged(System.Object sender, System.EventArgs e)
        {
        }
        void comboBox_baudrate_SelectedIndexChanged(System.Object sender, System.EventArgs e)
        {
        }
        void checkBox_flowcontrol_CheckedChanged(System.Object sender, System.EventArgs e)
        {
        }
        /*
        char* fileName;
        uint32_t Hexfile_type;
        char* ComportName;
        uint32_t baudRate;
        uint32_t flowControl;
        */
        //REF http://www.codeproject.com/Articles/442784/Best-gotchas-of-Cplusplus-CLI#marshal_context

        private void btnProgram_Click(object sender, EventArgs e)
        {
            /*
            int32_t r;

            fileName = (char*)Marshal.StringToHGlobalAnsi(tbFilename.Text).ToPointer();
            ComportName = (char*)Marshal.StringToHGlobalAnsi(comboBox_Comport.Items[0].ToString()).ToPointer();

            baudRate = Convert.ToInt32(comboBox_baudrate.Items[0].ToString());

            flowControl = checkBox_flowcontrol.Checked ? 1 : 0;

            if (radioButton_Softdevice.Checked) Hexfile_type = 1; //TODO
            if (radioButton_Application.Checked) Hexfile_type = 2; //TODO
            if (radioButton_Bootloader.Checked) Hexfile_type = 3; //TODO

            r = my_dfu_OverSerial_program(fileName, Hexfile_type, ComportName, baudRate, flowControl);

            Marshal.FreeHGlobal(IntPtr(fileName));
            Marshal.FreeHGlobal(IntPtr(ComportName));
            */

            DfuProgram dfuProgram = new DfuProgram();

            int r;
            String fileName = "jhhjh.hex";
            int Hexfile_type = 42;
            String ComportName = "COM666";
            UInt32 baudRate = 500000;
            int flowControl = 1;

            r = dfuProgram.dfuProgram_RTR500BLE(fileName, Hexfile_type, ComportName, baudRate, flowControl);



        }
        //=====================================================================
        //=====================================================================

        //void tbFilename_TextChanged(System.Object  sender, System.EventArgs  e) {   }
        //void label1_Click(System.Object  sender, System.EventArgs  e) {    }


    }
}
