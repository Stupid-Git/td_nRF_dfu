using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace nRFdfu_cs
{
    using System.Configuration;

    using System.Management;

    class appCmn_Config
    {
        
        public void VMain()//string[] args)
        {
            vvvReadAllSettings();
            vvvReadSetting("Setting1");
            vvvReadSetting("NotValid");
            vvvAddUpdateAppSettings("NewSetting", "May 7, 2014");
            vvvAddUpdateAppSettings("Setting1", "May 8, 2014");
            vvvReadAllSettings();
        }

        static public void vvvReadAllSettings()
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


        static public String vvvReadSetting(string key)
        {
            try
            {
                var appSettings = ConfigurationManager.AppSettings;
                string result = appSettings[key] ?? "Not Found"; Console.WriteLine(result);
                return (result);
            }
            catch (ConfigurationErrorsException) { Console.WriteLine("Error reading app settings"); }

            return("Error");
        }

        static public void vvvAddUpdateAppSettings(string key, string value)
        {
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
        }







        static public String GetSavedFilename()
        {
            return( vvvReadSetting("string_key_hexFileName") );

            ////////////
            vvvReadAllSettings();

            System.Collections.Specialized.NameValueCollection appSettings;

            appSettings = ConfigurationManager.AppSettings;
            String filename = appSettings["filename"];
            if (filename == null)
                filename = "";
            return (filename);
        }

        static public void SaveFilename(String new_filename)
        {
            vvvAddUpdateAppSettings("string_key_hexFileName", new_filename);
            return;

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



    }

    

    class appCmn_SerialPorts
    {

        static public int serialPortSearch()
        {
            //http://stackoverflow.com/questions/11458835/finding-information-about-all-serial-devices-connected-through-usb-in-c-sharp
            String did = "";
            String pnp = "";
            String deviceName = "";

            int sPort = 0;

            ManagementObjectSearcher searcher = new ManagementObjectSearcher(
                "root\\CIMV2",
                "SELECT * FROM Win32_PnPEntity WHERE ClassGuid=\"{4d36e978-e325-11ce-bfc1-08002be10318}\""
                );
            foreach (ManagementObject queryObj in searcher.Get())
            {
                // do what you like with the Win32_PnpEntity
                //queryObj.get...
                deviceName = (String)queryObj.GetPropertyValue("Name");
                did = (String)queryObj.GetPropertyValue("DeviceID");       // ex.COM3
                pnp = (String)queryObj.GetPropertyValue("PNPDeviceID");    // ex.USB\VID_0CCF&PID_DE00\0001

                Console.WriteLine("Name：       " + deviceName);
                Console.WriteLine("DeviceID：   " + did);
                Console.WriteLine("PNPDeviceID：" + pnp);
            }
            Console.WriteLine();

            // The stackoverflow article also mentions 
            //   http://www.codeproject.com/Articles/32330/A-Useful-WMI-Tool-How-To-Find-USB-to-Serial-Adapto
            // which introduces WMICodeCreator

            //return(0);

            System.Management.ManagementClass sp = new ManagementClass("Win32_SerialPort");
            //NGSystem.Management.ManagementClass sp = new ManagementClass("ports");

            /**/
            //using (System.Management.ManagementClass sp = new ManagementClass("Win32_SerialPort") )
            {
                foreach (ManagementObject p in (sp.GetInstances()))
                {
                    did = (String)p.GetPropertyValue("DeviceID");       // ex.COM3
                    pnp = (String)p.GetPropertyValue("PNPDeviceID");    // ex.USB\VID_0CCF&PID_DE00\0001

                    Console.WriteLine("DeviceID：" + did);
                    Console.WriteLine("PNPDeviceID：" + pnp);

                    if ((pnp.IndexOf("VID_C251&PID_2505") != -1) || (pnp.IndexOf("VID_0CCF&PID_0801") != -1))
                    {
                        sPort = System.Convert.ToInt32(did.Substring(3));
                        Console.WriteLine(sPort);
                        break;
                    }
                }
            }
            //*/
            return sPort;


        }
        static public int serialPortSearch_1()
        {
            String did = "";
            String pnp = "";

            int sPort = 0;

            System.Management.ManagementClass sp = new ManagementClass("Win32_SerialPort");
            //NGSystem.Management.ManagementClass sp = new ManagementClass("ports");

            /**/
            //using (System.Management.ManagementClass sp = new ManagementClass("Win32_SerialPort") )
            {
                foreach (ManagementObject p in (sp.GetInstances()))
                {
                    did = (String)p.GetPropertyValue("DeviceID");       // ex.COM3
                    pnp = (String)p.GetPropertyValue("PNPDeviceID");    // ex.USB\VID_0CCF&PID_DE00\0001

                    Console.WriteLine("DeviceID：" + did);
                    Console.WriteLine("PNPDeviceID：" + pnp);

                    if ((pnp.IndexOf("VID_C251&PID_2505") != -1) || (pnp.IndexOf("VID_0CCF&PID_0801") != -1))
                    {
                        sPort = System.Convert.ToInt32(did.Substring(3));
                        Console.WriteLine(sPort);
                        break;
                    }
                }
            }
            //*/
            return sPort;
        }
        
        static public List<String> getRTR500UsbSerialPortList()
        {
            String did = "";
            String pnp = "";

            List<String> theList = new List<String>();
            int sPort = 0;

            System.Management.ManagementClass sp = new ManagementClass("Win32_SerialPort");
            //NGSystem.Management.ManagementClass sp = new ManagementClass("ports");

            /**/
            //using (System.Management.ManagementClass sp = new ManagementClass("Win32_SerialPort") )
            {
                foreach (ManagementObject p in (sp.GetInstances()))
                {
                    did = (String)p.GetPropertyValue("DeviceID");       // ex.COM3
                    pnp = (String)p.GetPropertyValue("PNPDeviceID");    // ex.USB\VID_0CCF&PID_DE00\0001

                    Console.WriteLine("DeviceID：" + did);
                    Console.WriteLine("PNPDeviceID：" + pnp);

                    if ((pnp.IndexOf("VID_C251&PID_2505") != -1) || (pnp.IndexOf("VID_0CCF&PID_0801") != -1))
                    {
                        sPort = System.Convert.ToInt32(did.Substring(3));
                        Console.WriteLine(sPort);

                        theList.Add(did);
                        //break;
                    }
                    //theList.Add(did);
                }
            }
            //*/
            
            //return sPort;
            return (theList);
        }

        /* C#
        static public int serialPortSearch_Ex_Sunaba()
        {
            string did = "", pnp = "";

            int sPort = 0;


            using (ManagementClass sp = new ManagementClass("Win32_SerialPort"))
            {
                foreach (ManagementObject p in sp.GetInstances())
                {
                    did = (string)p.GetPropertyValue("DeviceID");       // ex.COM3
                    pnp = (string)p.GetPropertyValue("PNPDeviceID");    // ex.USB\VID_0CCF&PID_DE00\0001

                    Console.WriteLine("DeviceID：" + did);
                    Console.WriteLine("PNPDeviceID：" + pnp);

                    if ((pnp.IndexOf("VID_C251&PID_2505") != -1) || (pnp.IndexOf("VID_0CCF&PID_0801") != -1))
                    {
                        sPort = System.Convert.ToInt32(did.Substring(3));
                        Console.WriteLine(sPort);
                        break;
                    }
                }
            }

            return sPort;
        }
        C# */




    }
}
