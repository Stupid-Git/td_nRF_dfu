using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace nRFdfu_cs
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new FormCreateZip());
            //Application.Run(new FormSimple());
            //Application.Run(new FormRTR500BLE());
            //Application.Run(new Form1());
            
        }
    }
}
