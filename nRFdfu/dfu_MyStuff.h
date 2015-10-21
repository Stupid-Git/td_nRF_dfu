namespace testAppConfig {

    using namespace System;
    //using namespace System::ComponentModel;
    //using namespace System::Collections;
    //using namespace System::Windows::Forms;
    //using namespace System::Data;
    //using namespace System::Drawing;

    using namespace System::Xml;
    using namespace System::Xml::Serialization;
    using namespace System::Configuration;

    
    public ref class MySection
    {
    public:
        property String ^Name;
        property String ^FirstName;
    };

    ref class MySectionHandler: IConfigurationSectionHandler
    {
    public:
        virtual Object^ Create(Object^ parent, 
            Object ^configContext, XmlNode ^section)
        {
            XmlSerializer ^xs = gcnew XmlSerializer(MySection::typeid);
            XmlNodeReader ^xnr = gcnew XmlNodeReader(section);
            return xs->Deserialize(xnr);
        }
    };
    /*
    //http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
    //using namespace System::Configuration; //::Configuration;
    //using namespace System::Configuration::Assemblies; //::Configuration;
    ref class MySection: ConfigurationSection
    {
    public:
        [ConfigurationProperty("name", IsRequired = true)]
        property String ^name
        {
            String ^ get() { return (String ^)this["name"]; }
            void set(String ^value) { this["nale"] = value; }
        }

        [ConfigurationProperty("firstname", IsRequired = true)]
        property String ^firstname
        {
            String ^ get() { return (String^)this["firstname"]; }
            void set(String ^value) { this["firstname"] = value; }
        }
    };
    //http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
 */

}




//http://www.codeproject.com/Articles/442784/Best-gotchas-of-Cplusplus-CLI#marshal_context
#include <stdio.h>
using namespace System;
using namespace System::Runtime::InteropServices;

namespace testAppConfig 
{

    ref class OOO
    {
    public:
        void test()
        {
            String^ line = Console::ReadLine();
            char* cStr = (char*)Marshal::StringToHGlobalAnsi(line).ToPointer();
            printf(cStr);
            Marshal::FreeHGlobal(IntPtr(cStr));
        }
    };
}


/*
#include <stdio.h>
#include <msclr/marshal.h>

using namespace System;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

namespace testAppConfig 
{

    ref class OOO
    {
    public:
        void test()
        {
            String^ line = Console::ReadLine();
            marshal_context context;
            const char* cStr = context.marshal_as(line);
            printf(cStr); //Marshal_Context free things correctly when out of scope.
        }
    };
}
*/