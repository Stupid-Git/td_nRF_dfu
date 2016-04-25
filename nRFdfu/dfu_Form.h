#pragma once

#include "dfu_MyStuff.h"

#include "stdint.h"
extern "C" {
    void test_memory_is_copied_1(void);
    void gx_load_app_to_mem(void);
    void Controller_upload_firmware_1(void);
    void Controller_upload_firmware(void);

    void test_Dfu_transport(void);

    int32_t my_dfu_OverSerial_program( char* fileName, uint32_t Hexfile_type, char* ComportName, uint32_t baudRate, uint32_t flowControl );
}

//http://www.codeproject.com/Articles/442784/Best-gotchas-of-Cplusplus-CLI#marshal_context
//#include <stdio.h>
//#include <msclr/marshal.h>
//using namespace System;
//using namespace msclr::interop;
//using namespace System::Runtime::InteropServices;
//http://www.codeproject.com/Articles/442784/Best-gotchas-of-Cplusplus-CLI#marshal_context


namespace dfu {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


    //Added by karel
    using namespace System::Xml;
    using namespace System::Xml::Serialization;
    using namespace System::Configuration; //::Configuration;
    //using namespace System::Configuration::Assemblies; //::Configuration;
    using namespace System::Management;

    using namespace testAppConfig;



	/// <summary>
	/// Summary for dfu_Form
	/// </summary>
	public ref class dfu_Form : public System::Windows::Forms::Form
	{
	public:
		dfu_Form(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~dfu_Form()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Button^  btnProgram;
    protected: 
    private: System::Windows::Forms::CheckBox^  checkBox_flowcontrol;
    private: System::Windows::Forms::ComboBox^  comboBox_baudrate;
    private: System::Windows::Forms::ComboBox^  comboBox_Comport;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::RadioButton^  radioButton_Bootloader;
    private: System::Windows::Forms::RadioButton^  radioButton_Application;
    private: System::Windows::Forms::RadioButton^  radioButton_Softdevice;
    private: System::Windows::Forms::TextBox^  tbFilename;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Button^  btnBrowse;
    private: System::Windows::Forms::Button^  btn_test_Dfu_transport;
    private: System::Windows::Forms::Button^  btnTest1;
    private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->btnProgram = (gcnew System::Windows::Forms::Button());
            this->checkBox_flowcontrol = (gcnew System::Windows::Forms::CheckBox());
            this->comboBox_baudrate = (gcnew System::Windows::Forms::ComboBox());
            this->comboBox_Comport = (gcnew System::Windows::Forms::ComboBox());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->radioButton_Bootloader = (gcnew System::Windows::Forms::RadioButton());
            this->radioButton_Application = (gcnew System::Windows::Forms::RadioButton());
            this->radioButton_Softdevice = (gcnew System::Windows::Forms::RadioButton());
            this->tbFilename = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->btnBrowse = (gcnew System::Windows::Forms::Button());
            this->btn_test_Dfu_transport = (gcnew System::Windows::Forms::Button());
            this->btnTest1 = (gcnew System::Windows::Forms::Button());
            this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
            this->groupBox1->SuspendLayout();
            this->SuspendLayout();
            // 
            // btnProgram
            // 
            this->btnProgram->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(128)));
            this->btnProgram->Location = System::Drawing::Point(683, 97);
            this->btnProgram->Name = L"btnProgram";
            this->btnProgram->Size = System::Drawing::Size(97, 23);
            this->btnProgram->TabIndex = 23;
            this->btnProgram->Text = L"Program";
            this->btnProgram->UseVisualStyleBackColor = true;
            this->btnProgram->Click += gcnew System::EventHandler(this, &dfu_Form::btnProgram_Click);
            // 
            // checkBox_flowcontrol
            // 
            this->checkBox_flowcontrol->AutoSize = true;
            this->checkBox_flowcontrol->Location = System::Drawing::Point(502, 101);
            this->checkBox_flowcontrol->Name = L"checkBox_flowcontrol";
            this->checkBox_flowcontrol->Size = System::Drawing::Size(104, 16);
            this->checkBox_flowcontrol->TabIndex = 22;
            this->checkBox_flowcontrol->Text = L"Use flowcontrol";
            this->checkBox_flowcontrol->UseVisualStyleBackColor = true;
            this->checkBox_flowcontrol->CheckedChanged += gcnew System::EventHandler(this, &dfu_Form::checkBox_flowcontrol_CheckedChanged);
            // 
            // comboBox_baudrate
            // 
            this->comboBox_baudrate->FormattingEnabled = true;
            this->comboBox_baudrate->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"38400"});
            this->comboBox_baudrate->Location = System::Drawing::Point(485, 75);
            this->comboBox_baudrate->Name = L"comboBox_baudrate";
            this->comboBox_baudrate->Size = System::Drawing::Size(192, 20);
            this->comboBox_baudrate->TabIndex = 21;
            this->comboBox_baudrate->SelectedIndexChanged += gcnew System::EventHandler(this, &dfu_Form::comboBox_baudrate_SelectedIndexChanged);
            // 
            // comboBox_Comport
            // 
            this->comboBox_Comport->FormattingEnabled = true;
            this->comboBox_Comport->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Predef-Direct", L"Predef-Sim"});
            this->comboBox_Comport->Location = System::Drawing::Point(485, 47);
            this->comboBox_Comport->Name = L"comboBox_Comport";
            this->comboBox_Comport->Size = System::Drawing::Size(192, 20);
            this->comboBox_Comport->TabIndex = 20;
            this->comboBox_Comport->SelectedIndexChanged += gcnew System::EventHandler(this, &dfu_Form::comboBox_Comport_SelectedIndexChanged);
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(387, 78);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(87, 12);
            this->label3->TabIndex = 19;
            this->label3->Text = L"Select baudrate:";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(387, 50);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(92, 12);
            this->label2->TabIndex = 18;
            this->label2->Text = L"Select COM port:";
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->radioButton_Bootloader);
            this->groupBox1->Controls->Add(this->radioButton_Application);
            this->groupBox1->Controls->Add(this->radioButton_Softdevice);
            this->groupBox1->Location = System::Drawing::Point(12, 43);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(360, 52);
            this->groupBox1->TabIndex = 17;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"Hexfile Type";
            // 
            // radioButton_Bootloader
            // 
            this->radioButton_Bootloader->AutoSize = true;
            this->radioButton_Bootloader->Location = System::Drawing::Point(252, 22);
            this->radioButton_Bootloader->Name = L"radioButton_Bootloader";
            this->radioButton_Bootloader->Size = System::Drawing::Size(78, 16);
            this->radioButton_Bootloader->TabIndex = 2;
            this->radioButton_Bootloader->Text = L"Bootloader";
            this->radioButton_Bootloader->UseVisualStyleBackColor = true;
            this->radioButton_Bootloader->CheckedChanged += gcnew System::EventHandler(this, &dfu_Form::radioButton_Bootloader_CheckedChanged);
            // 
            // radioButton_Application
            // 
            this->radioButton_Application->AutoSize = true;
            this->radioButton_Application->Checked = true;
            this->radioButton_Application->Location = System::Drawing::Point(132, 22);
            this->radioButton_Application->Name = L"radioButton_Application";
            this->radioButton_Application->Size = System::Drawing::Size(80, 16);
            this->radioButton_Application->TabIndex = 1;
            this->radioButton_Application->TabStop = true;
            this->radioButton_Application->Text = L"Application";
            this->radioButton_Application->UseVisualStyleBackColor = true;
            this->radioButton_Application->CheckedChanged += gcnew System::EventHandler(this, &dfu_Form::radioButton_Application_CheckedChanged);
            // 
            // radioButton_Softdevice
            // 
            this->radioButton_Softdevice->AutoSize = true;
            this->radioButton_Softdevice->Location = System::Drawing::Point(12, 22);
            this->radioButton_Softdevice->Name = L"radioButton_Softdevice";
            this->radioButton_Softdevice->Size = System::Drawing::Size(77, 16);
            this->radioButton_Softdevice->TabIndex = 0;
            this->radioButton_Softdevice->Text = L"Softdevice";
            this->radioButton_Softdevice->UseVisualStyleBackColor = true;
            this->radioButton_Softdevice->CheckedChanged += gcnew System::EventHandler(this, &dfu_Form::radioButton_Softdevice_CheckedChanged);
            // 
            // tbFilename
            // 
            this->tbFilename->Location = System::Drawing::Point(101, 6);
            this->tbFilename->Name = L"tbFilename";
            this->tbFilename->Size = System::Drawing::Size(576, 19);
            this->tbFilename->TabIndex = 16;
            this->tbFilename->TextChanged += gcnew System::EventHandler(this, &dfu_Form::tbFilename_TextChanged);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(12, 9);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(83, 12);
            this->label1->TabIndex = 15;
            this->label1->Text = L"DFU /Hex File:";
            this->label1->Click += gcnew System::EventHandler(this, &dfu_Form::label1_Click);
            // 
            // btnBrowse
            // 
            this->btnBrowse->Location = System::Drawing::Point(683, 4);
            this->btnBrowse->Name = L"btnBrowse";
            this->btnBrowse->Size = System::Drawing::Size(97, 23);
            this->btnBrowse->TabIndex = 14;
            this->btnBrowse->Text = L"Browse...";
            this->btnBrowse->UseVisualStyleBackColor = true;
            this->btnBrowse->Click += gcnew System::EventHandler(this, &dfu_Form::btnBrowse_Click);
            // 
            // btn_test_Dfu_transport
            // 
            this->btn_test_Dfu_transport->Location = System::Drawing::Point(66, 161);
            this->btn_test_Dfu_transport->Name = L"btn_test_Dfu_transport";
            this->btn_test_Dfu_transport->Size = System::Drawing::Size(143, 23);
            this->btn_test_Dfu_transport->TabIndex = 13;
            this->btn_test_Dfu_transport->Text = L"test_Dfu_transport";
            this->btn_test_Dfu_transport->UseVisualStyleBackColor = true;
            this->btn_test_Dfu_transport->Click += gcnew System::EventHandler(this, &dfu_Form::btn_test_Dfu_transport_Click);
            // 
            // btnTest1
            // 
            this->btnTest1->Location = System::Drawing::Point(282, 161);
            this->btnTest1->Name = L"btnTest1";
            this->btnTest1->Size = System::Drawing::Size(75, 23);
            this->btnTest1->TabIndex = 12;
            this->btnTest1->Text = L"Test 1";
            this->btnTest1->UseVisualStyleBackColor = true;
            this->btnTest1->Click += gcnew System::EventHandler(this, &dfu_Form::btnTest1_Click);
            // 
            // dfu_Form
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(794, 251);
            this->Controls->Add(this->btnProgram);
            this->Controls->Add(this->checkBox_flowcontrol);
            this->Controls->Add(this->comboBox_baudrate);
            this->Controls->Add(this->comboBox_Comport);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->tbFilename);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->btnBrowse);
            this->Controls->Add(this->btn_test_Dfu_transport);
            this->Controls->Add(this->btnTest1);
            this->Name = L"dfu_Form";
            this->Text = L"dfu_Form";
            this->Load += gcnew System::EventHandler(this, &dfu_Form::dfu_Form_Load);
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
               
        //=====================================================================
        //=====================================================================
    private:
        System::Void dfu_Form_Load(System::Object^  sender, System::EventArgs^  e)
        {
            tbFilename->Text = GetSavedFilename();
        }

        //=====================================================================
        //=====================================================================
    private:
        //http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
        void updata()
        {
            String ^name = ConfigurationSettings::AppSettings["name"];
            String ^firstName = ConfigurationSettings::AppSettings["firstName"];
            Console::WriteLine("{0} - {1}", firstName, name);

            System::Configuration::Configuration ^config = 
                ConfigurationManager::OpenExeConfiguration(
                ConfigurationUserLevel::None);

            config->AppSettings->Settings->Remove("firstName");
            config->AppSettings->Settings->Add("firstName", "New firstName");

            config->AppSettings->Settings->Remove("filename");
            config->AppSettings->Settings->Add("filename", "C:/abc.hex");

            config->Save(ConfigurationSaveMode::Modified);
            ConfigurationManager::RefreshSection("appSettings");

            firstName = ConfigurationSettings::AppSettings["firstName"];
            Console::WriteLine("{0} - {1}", firstName, name);
        }

        void mydata()
        {
            MySection^ mySection =  (MySection^)ConfigurationManager::GetSection("MySection"); 
            
            Console::WriteLine("--- MySection ---");
            Console::WriteLine("{0} ; {1}", mySection->Name, mySection->FirstName);
            Console::WriteLine("");
        }

        void testMySection(void)
        {
            MySection ^section = (MySection^)ConfigurationManager::GetSection("MySection");

            Console::WriteLine("{0} ; {1}", section->Name, section->FirstName);

            section->Name += " plusN";
            section->FirstName += " plusn";
        }

        void myAppSettingsTest()
        {
            String^ greeting = ConfigurationManager::AppSettings["greeting"];
            Console::WriteLine(greeting);

            String ^name = /*Configuration::*/ConfigurationManager::AppSettings["name"];
            String ^firstName = /*Configuration::*/ConfigurationManager::AppSettings["firstname"];
            Console::WriteLine("My name's {0} {1}", firstName, name);

            String ^name1 = /*Configuration::*/ConfigurationManager::AppSettings[0];

            for each(String ^aValue in ConfigurationManager::AppSettings)
            {
                Console::WriteLine("User: {0} - {1}", 
                    aValue, ConfigurationManager::AppSettings[aValue]);
            }

            updata();
            
            mydata();

            //ConfigurationManager::AppSettings["firstname"] = "NewFirstName";
            testMySection();
        }

        System::Void btnTest1_Click(System::Object^  sender, System::EventArgs^  e)
        {
            /*
            myAppSettingsTest();
            */

            /*
            String^ fname1 = GetSavedFilename();
            Console::WriteLine("[1] filename1 = {0}", fname1);

            if(fname1 == "")
                SaveFilename("c:\\abc.def");
            else
                SaveFilename( fname1 );

            String^ fname2 = GetSavedFilename();
            Console::WriteLine("[2] filename2 = {0}", fname2);
            */

            serialPortSearch();
        }
        
        String^ GetSavedFilename(void)
        {
            String^  filename = ConfigurationSettings::AppSettings["filename"];
            if( filename == nullptr )
                filename = "";
            return(filename);
        }

        void SaveFilename(String^ new_filename)
        {

            System::Configuration::Configuration ^config = 
                ConfigurationManager::OpenExeConfiguration(
                ConfigurationUserLevel::None);

            config->AppSettings->Settings->Remove("filename");
            config->AppSettings->Settings->Add("filename", new_filename);

            config->Save(ConfigurationSaveMode::Modified);
            ConfigurationManager::RefreshSection("appSettings");

            //String^  filename = ConfigurationSettings::AppSettings["filename"];
            //if( filename == nullptr )
            //    filename = "";

        }
        /**/

    private:
        int serialPortSearch()
        {
            //http://stackoverflow.com/questions/11458835/finding-information-about-all-serial-devices-connected-through-usb-in-c-sharp
            String^ did = "";
            String^ pnp = "";
            String^ deviceName = "";

            int sPort = 0;


            ManagementObjectSearcher^ searcher = gcnew ManagementObjectSearcher(
                "root\\CIMV2",
                "SELECT * FROM Win32_PnPEntity WHERE ClassGuid=\"{4d36e978-e325-11ce-bfc1-08002be10318}\""
                );
            for each (ManagementObject^ queryObj in searcher->Get())
            {            
                // do what you like with the Win32_PnpEntity
                //queryObj->get...
                deviceName = (String^)queryObj->GetPropertyValue("Name"); 
                did = (String^)queryObj->GetPropertyValue("DeviceID");       // ex.COM3
                pnp = (String^)queryObj->GetPropertyValue("PNPDeviceID");    // ex.USB\VID_0CCF&PID_DE00\0001

                Console::WriteLine("Name：       " + deviceName);
                Console::WriteLine("DeviceID：   " + did);
                Console::WriteLine("PNPDeviceID：" + pnp);
            }
            Console::WriteLine();

            // The stackoverflow article also mentions 
            //   http://www.codeproject.com/Articles/32330/A-Useful-WMI-Tool-How-To-Find-USB-to-Serial-Adapto
            // which introduces WMICodeCreator

            //return(0);

            System::Management::ManagementClass^ sp = gcnew ManagementClass("Win32_SerialPort");
            //NGSystem::Management::ManagementClass^ sp = gcnew ManagementClass("ports");

            /**/
            //using (System::Management::ManagementClass^ sp = gcnew ManagementClass("Win32_SerialPort") )
            {
                for each (ManagementObject^ p in (sp->GetInstances()) )
                {
                    did = (String^)p->GetPropertyValue("DeviceID");       // ex.COM3
                    pnp = (String^)p->GetPropertyValue("PNPDeviceID");    // ex.USB\VID_0CCF&PID_DE00\0001

                    Console::WriteLine("DeviceID：" + did);
                    Console::WriteLine("PNPDeviceID：" + pnp);

                    if ((pnp->IndexOf("VID_C251&PID_2505") != -1) || (pnp->IndexOf("VID_0CCF&PID_0801") != -1))
                    {
                        sPort = System::Convert::ToInt32(did->Substring(3));
                        Console::WriteLine(sPort);
                        break;
                    }
                }
            }
            //*/
            return sPort;


        }
        int serialPortSearch_1()
        {
            String^ did = "";
            String^ pnp = "";

            int sPort = 0;

            System::Management::ManagementClass^ sp = gcnew ManagementClass("Win32_SerialPort");
            //NGSystem::Management::ManagementClass^ sp = gcnew ManagementClass("ports");

            /**/
            //using (System::Management::ManagementClass^ sp = gcnew ManagementClass("Win32_SerialPort") )
            {
                for each (ManagementObject^ p in (sp->GetInstances()) )
                {
                    did = (String^)p->GetPropertyValue("DeviceID");       // ex.COM3
                    pnp = (String^)p->GetPropertyValue("PNPDeviceID");    // ex.USB\VID_0CCF&PID_DE00\0001

                    Console::WriteLine("DeviceID：" + did);
                    Console::WriteLine("PNPDeviceID：" + pnp);

                    if ((pnp->IndexOf("VID_C251&PID_2505") != -1) || (pnp->IndexOf("VID_0CCF&PID_0801") != -1))
                    {
                        sPort = System::Convert::ToInt32(did->Substring(3));
                        Console::WriteLine(sPort);
                        break;
                    }
                }
            }
            //*/
            return sPort;
        }
        
        /* C#
        private int serialPortSearch()
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


        //http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
        //=====================================================================
        //=====================================================================


        System::Void btn_test_Dfu_transport_Click(System::Object^  sender, System::EventArgs^  e)
        {
            // Check
            //   https://github.com/NordicSemiconductor/pc-nrfutil/blob/master/nordicsemi/__main__.py

            // I basically copied this one 'cos it goes from HEX to Program and is shorter
            // and easier to understand than the 'current' version above
            //   https://github.com/tnesheim/GradOS_FinalProject/tree/master/with_riot/examples/hello-world/sdk7.2/examples/dfu/hci_dfu_send_hex
            //   https://github.com/tnesheim/GradOS_FinalProject/blob/3c97958d79eb87d5f870c98dd7a63354a04e882d/with_riot/examples/hello-world/sdk7.2/examples/dfu/hci_dfu_send_hex/hci_dfu_send_hex.py

            // IntelHex Util form
            //   https://github.com/martin-helmich/libcintelhex

            Controller_upload_firmware_1(); // see hci_dfu_send_hex_1.c
            //Controller_upload_firmware(); // see hci_dfu_send_hex.c

            //test_Dfu_transport();
        }
        
        //=====================================================================
        //=====================================================================
        // http://stackoverflow.com/questions/15576616/how-do-i-read-multiple-files-after-click-opened-in-openfiledialog
        System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e)
        {
            //openFileDialog1->InitialDirectory = "c:\\";  
            openFileDialog1->InitialDirectory = GetSavedFilename();  

            openFileDialog1->Title = "Open Hex Image or ZIP File";  
            //openFileDialog1->Filter = "CP files (*.cp)|*.cp|All files (*.*)|*.*|txt files (*.txt)|*.txt";  
            //openFileDialog1->Filter = "テキスト ファイル (*.txt)|*.txt|すべてのファイル (*.*)|*.*";
            openFileDialog1->Filter = "HEX files (*.hex)|*.hex| Zip files (*.zip)|*.zip";  
            openFileDialog1->FilterIndex = 1;  
            openFileDialog1->Multiselect = false; //true;  

            /*
            if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )  
            {  
                array<String^>^ lines = System::IO::File::ReadAllLines(openFileDialog1->FileName);  
                for each (String^ line in lines) {  
                    //?????  
                    System::Diagnostics::Debug::WriteLine("",line);  
                }  
            }
            */ 
            if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )  
            {  
                if( openFileDialog1->FileName != "" )
                {
                    //tbFilename->Text = openFileDialog1->FileName;
                    SaveFilename(openFileDialog1->FileName);
                    tbFilename->Text = GetSavedFilename();

                }
                for each (String^ file in openFileDialog1->FileNames) 
                {  
                    System::Diagnostics::Debug::WriteLine("",file);  
                    Console::WriteLine(">> {0}",file);  
                    /*
                    array<String^>^ lines = System::IO::File::ReadAllLines(file);  
                    for each (String^ line in lines) 
                    {  
                        System::Diagnostics::Debug::WriteLine("",line);  
                        Console::WriteLine(">> {0}",line);  
                    }
                    */
                }  
            }  
        }

        //---------------------------------------------------------------------
        System::Void radioButton_Softdevice_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
        {
            if(radioButton_Softdevice->Checked)
            {
                radioButton_Application->Checked = false;
                radioButton_Bootloader->Checked = false;
            }
        }
        System::Void radioButton_Application_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
        {
            if(radioButton_Application->Checked)
            {
                radioButton_Softdevice->Checked = false;
                radioButton_Bootloader->Checked = false;
            }
        }
        System::Void radioButton_Bootloader_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
        {
            if(radioButton_Bootloader->Checked)
            {
                radioButton_Softdevice->Checked = false;
                radioButton_Application->Checked = false;
            }
        }

        //---------------------------------------------------------------------
        System::Void comboBox_Comport_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
        {
        }
        System::Void comboBox_baudrate_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
        {
        }
        System::Void checkBox_flowcontrol_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
        {
        }

    private:


        char* fileName;
        uint32_t Hexfile_type;
        char* ComportName;
        uint32_t baudRate;
        uint32_t flowControl;

        //REF http://www.codeproject.com/Articles/442784/Best-gotchas-of-Cplusplus-CLI#marshal_context
        System::Void btnProgram_Click(System::Object^  sender, System::EventArgs^  e)
        {
            int32_t r;

            fileName = (char*)Marshal::StringToHGlobalAnsi(tbFilename->Text).ToPointer();
            ComportName = (char*)Marshal::StringToHGlobalAnsi( comboBox_Comport->Items[0]->ToString() ).ToPointer();

            baudRate = Convert::ToInt32( comboBox_baudrate->Items[0]->ToString() );

            flowControl = checkBox_flowcontrol->Checked ? 1:0;

            if( radioButton_Softdevice->Checked  )        Hexfile_type = 1; //TODO
            if( radioButton_Application->Checked )        Hexfile_type = 2; //TODO
            if( radioButton_Bootloader->Checked  )        Hexfile_type = 3; //TODO

            r = my_dfu_OverSerial_program( fileName, Hexfile_type, ComportName, baudRate, flowControl );

            Marshal::FreeHGlobal(IntPtr(fileName));
            Marshal::FreeHGlobal(IntPtr(ComportName));
            
        }
        //=====================================================================
        //=====================================================================
        


private: System::Void tbFilename_TextChanged(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
         }
};
}
