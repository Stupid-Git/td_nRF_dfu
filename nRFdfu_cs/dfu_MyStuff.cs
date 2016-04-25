using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


//using System.ComponentModel;
//using System.Collections;
//using System.Windows.Forms;
//using System.Data;
//using System.Drawing;

using System.Xml;
using System.Xml.Serialization;
using System.Configuration;


namespace testAppConfig
{
    //class dfu_MyStuff
    //{
    //}



    public class MySection
    {/*
    public:
        property String Name;
        property String FirstName;*/

        String _Name;
        String _FirstName;

        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }

        public string FirstName
        {
            get { return _FirstName; }
            set { _FirstName = value; }
        }
    };

    class MySectionHandler : IConfigurationSectionHandler
    {
        /**/
        public virtual Object Create(Object parent, Object configContext, XmlNode section)
        {
            XmlSerializer xs = new XmlSerializer(typeof(MySection));
            XmlNodeReader xnr = new XmlNodeReader(section);
            return xs.Deserialize(xnr);
        }
        /* */
    };
    /*
    //http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
    //using System.Configuration; //.Configuration;
    //using System.Configuration.Assemblies; //.Configuration;
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


//////////////////////////////////////////////////////////////////////////////////////////////////////
namespace JUNK
{

    //+++++
    public class DfuEventX
    {

    }
    //+++++

    public class Mediator
    {
        //instruct the robot to move.
        public delegate void Callback(string sender, string receiver, string msg);


        Callback sendMessage;

        //Assign the callback method to the delegate          
        public void SignOn(Callback moveMethod)
        {
            sendMessage += moveMethod;
        }

        public void SendMessage(string sender, string receiver, string msg)
        {
            sendMessage(sender, receiver, msg);
        }


        //+++++
        public delegate void timeout_event_handler_del(String S);
        timeout_event_handler_del callback = null;
        public void register_events_callback(DfuEventX event_type, timeout_event_handler_del callback)
        {
            this.callback = callback;
        }
        //+++++
    }


    public class Controller //: Asset
    {
        string /*readonly*/ _name;
        Mediator _mediator;
        public Controller(Mediator m, string name)
        {
            _name = name;
            _mediator = m;
            //assign the call back method
            _mediator.SignOn(Notification);
        }

        public void Notification(string sender, string receiver, string msg)
        {
            if (receiver == _name)
            {
                Console.WriteLine("{0}: Message for {1} - {2}"/*.FormateText(*/, sender, receiver, msg/*)*/); //I have create extension method for FormatText.
            }
        }

        public void Mobilize(string receiver, string msg)
        {
            _mediator.SendMessage(_name, receiver, msg);
        }

    }

    class XXX
    {
        /*static*/
        void XXXMain(string[] args)
        {

            Mediator mediator;
            mediator = new Mediator();

            //accept name here...

            Controller controller1 = new Controller(mediator, "name1");
            Controller controller2 = new Controller(mediator, "name2");
            controller1.Mobilize("name2", "Hello");
            controller1.Mobilize("name1", "How are you?");


            //+++++
            mediator.register_events_callback(new DfuEventX(), ON_XXX);
            //+++++

        }

        public void ON_XXX(String S)
        {
        }

        //output will be:

        //name1: Message for name2 - Hello
        //name2: Message for name1 - How are you?

    }
}

