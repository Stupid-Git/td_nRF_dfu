using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace nRFdfu_cs
{

    class DfuMain
    {
        //public void update_progress(String S)
        //{
        //}
        public void update_progress(int progress = 0, bool done = false, String log_message = "")
        {
            logger.update_progress(progress, done, log_message);
        }

        //def update_progress(progress=0, done=False, log_message=""):
        //    del done, log_message  # Unused parameters
        //    if global_bar:
        //        global_bar.update(max(1, progress))

        public bool serial_RTR500(String package, String port, int baudrate, int flowcontrol)
        {
            //QQ DDD1 = new timeout_event_handler_del(update_progress);
            //QQ timeout_event_handler_del DDD2 = new timeout_event_handler_del(update_progress);


            Dfu dfu;
            DfuTransportSerial serial_backend;
            //"""Program a device with bootloader that support serial DFU"""

            DfuSerial_UW dfuSerial = new DfuSerial_UW();
            dfuSerial.portName = port;

            serial_backend = new DfuTransportSerial(dfuSerial);
            //serial_backend = new DfuTransportSerial(port, baudrate, flowcontrol);

            serial_backend.register_events_callback(DfuEvent.PROGRESS_EVENT, this.update_progress);

            dfu = new Dfu(package, /*dfu_transport= (DfuTransport)*/serial_backend);

            //click.echo("Upgrading target on {1} with DFU package {0}. Flow control is {2}."
            //   .format(package, port, "enabled" if flowcontrol else "disabled"))


            //try:
            //    with click.progressbar(length=100) as bar:
            //        global global_bar
            //        global_bar = bar
            dfu.dfu_send_images();

            //except Exception as e:
            //    click.echo("")
            //    click.echo("Failed to upgrade target. Error is: {0}".format(e.message))
            //    click.echo("")
            //    click.echo("Possible causes:")
            //    click.echo("- bootloader, SoftDevice or application on target "
            //               "does not match the requirements in the DFU package.")
            //    click.echo("- baud rate or flow control is not the same as in the target bootloader.")
            //    click.echo("- target is not in DFU mode. If using the SDK examples, "
            //               "press Button 4 and RESET and release both to enter DFU mode.")

            //    return False

            //click.echo("Device programmed.")

            return (true);
        }

        public bool update_RTR500_bin(String comPortName, String binFileName, String datFileName)
        {
            //QQ DDD1 = new timeout_event_handler_del(update_progress);
            //QQ timeout_event_handler_del DDD2 = new timeout_event_handler_del(update_progress);


            Dfu dfu;
            DfuTransportSerial serial_backend;
            //"""Program a device with bootloader that support serial DFU"""

            DfuSerial_UW dfuSerial = new DfuSerial_UW();
            dfuSerial.portName = comPortName;

            serial_backend = new DfuTransportSerial(dfuSerial);
            //serial_backend = new DfuTransportSerial(port, baudrate, flowcontrol);

            serial_backend.register_events_callback(DfuEvent.PROGRESS_EVENT, this.update_progress);

            dfu = new Dfu(binFileName, datFileName, /*dfu_transport= (DfuTransport)*/serial_backend);

            //click.echo("Upgrading target on {1} with DFU package {0}. Flow control is {2}."
            //   .format(package, port, "enabled" if flowcontrol else "disabled"))
            logger.info(String.Format("Upgrading target on {1} with DFU package {0}. Flow control is {2}.", binFileName, comPortName, "Not used"));

            try
            {
                //    with click.progressbar(length=100) as bar:
                //        global global_bar
                //        global_bar = bar
                dfu.dfu_send_images();
            }
            catch (Exception ex)
            {
                //except Exception as e:
                //    click.echo("")
                //    click.echo("Failed to upgrade target. Error is: {0}".format(e.message))
                //    click.echo("")
                //    click.echo("Possible causes:")
                //    click.echo("- bootloader, SoftDevice or application on target "
                //               "does not match the requirements in the DFU package.")
                //    click.echo("- baud rate or flow control is not the same as in the target bootloader.")
                //    click.echo("- target is not in DFU mode. If using the SDK examples, "
                //               "press Button 4 and RESET and release both to enter DFU mode.")

                return false;
            }

            //click.echo("Device programmed.")
            logger.info(String.Format("Device programmed."));

            return (true);
        }


        // Pass Method as Parameter using C#
        // http://stackoverflow.com/questions/2082615/pass-method-as-parameter-using-c-sharp
        // Func<int, object,int> arg

        Func<int, object, int> op_2;

        public bool update_RTR500_bin_2(Func<int, object, int> arg, String comPortName, String binFileName, String datFileName)
        {
            op_2 = arg;
            logger.op_2 = op_2;

            /*
            System.Threading.Thread.Sleep(1000);
            logger.error("Error test");
            System.Threading.Thread.Sleep(1000);
            logger.debug("Debug test");
            System.Threading.Thread.Sleep(1000);
            logger.info("Info test");
            */
            bool bSts;

            bSts = update_RTR500_bin(comPortName, binFileName, datFileName);

            return (bSts);
        }


        public bool update_Me_bin(String Address, Int32 Baud_rate, String comPortName, String binFileName, String datFileName)
        {
            bool bSts;

            Dfu dfu;
            //e.g. DfuTransportSerial serial_backend;
            DfuTransportMe me_backend;
            //"""Program a device with bootloader that support seri


            bSts = true;
            
            //# TODO: Fill DfuTransportMe() constructor with init parameters
            //# args.address needed
            //# args.baud_rate needed
            me_backend = new DfuTransportMe(Address, Baud_rate, comPortName);

            dfu = new Dfu(binFileName, datFileName, me_backend);
            
            //# Transmit the hex image to peer device.
            dfu.dfu_send_images();

            return (bSts);
        }
        public bool update_Me_bin_2(Func<int, object, int> arg, String Address, Int32 Baud_rate, String comPortName, String binFileName, String datFileName)
        {
            op_2 = arg;
            logger.op_2 = op_2;

            /*
            System.Threading.Thread.Sleep(1000);
            logger.error("Error test");
            System.Threading.Thread.Sleep(1000);
            logger.debug("Debug test");
            System.Threading.Thread.Sleep(1000);
            logger.info("Info test");
            */
            bool bSts;

            bSts = update_Me_bin(Address, Baud_rate, comPortName, binFileName, datFileName);

            return (bSts);
        }
    }
    


    public class logger
    {
        public static Func<int, object, int> op_2;


        public static void update_progress(int progress = 0, bool done = false, String log_message = "")
        {
            if (op_2 == null)
                return;
            string[] workerResult = new string[2];
            workerResult[0] = "update_progress";
            workerResult[1] = String.Format("Progress = {0}" + " - " + log_message, progress);
            int r = op_2(progress, workerResult);
        }

        public static void error(String S)
        {
            if (op_2 == null)
                return;
            string[] workerResult = new string[2];
            workerResult[0] = "error";
            workerResult[1] = S;
            int r = op_2(42, workerResult);
        }
        public static void debug(String S)
        {
            if (op_2 == null)
                return;
            string[] workerResult = new string[2];
            workerResult[0] = "debug";
            workerResult[1] = S;
            int r = op_2(42, workerResult);
        }
        public static void info(String S)
        {
            if (op_2 == null)
                return;
            string[] workerResult = new string[2];
            workerResult[0] = "info";
            workerResult[1] = S;
            int r = op_2(42, workerResult);
        }

        public static void log(String S)
        {
            if (op_2 == null)
                return;
            string[] workerResult = new string[2];
            workerResult[0] = "log";
            workerResult[1] = S;
            int r = op_2(42, workerResult);
        }

    }


}
