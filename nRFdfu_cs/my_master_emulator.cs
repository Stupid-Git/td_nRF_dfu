using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading;
using System.Threading.Tasks;

using System.Windows.Forms;

using System.Diagnostics; // Trace
using Nordicsemi;

namespace nRFdfu_cs
{
    class MasterEmulator
    {
        public Nordicsemi.MasterEmulator master;

        String peer_device_address;
        int num_of_errors = 0;
        //logger = TimestampLogger();
        public bool connected = false;
        bool disconnect_event_expected = true;
        bool service_setup_done = true;

        DisconnectReason last_disconnect_reason = DisconnectReason.UnspecifiedError; // = -1

        Int32 baud_rate;

        String own_address = "";// = own_address;
        String bond_info;// = bond_info;

        BtDevice myPeerDevice;

        public MasterEmulator( String peer_device_address, Int32 baud_rate=1000000 )//, own_address=None, bond_info=None):
        {
            this.peer_device_address = peer_device_address;

            this.num_of_errors = 0;
            //logger = TimestampLogger();
            this.connected = false;
            this.disconnect_event_expected = true;
            this.service_setup_done = true;
            this.last_disconnect_reason = DisconnectReason.UnspecifiedError; // = -1;

            this.baud_rate = baud_rate;

            //this.own_address = own_address;
            //this.bond_info = bond_info;
        }


        void log_message_handler(object sender, ValueEventArgs<string> e)
        {
            //""" Log function to be passed to the DLL. """
            logger.log(String.Format("[Emulator] {0}", e.Value)); // %s -> {0}

            string message = e.Value;
            if (message.Contains("Connected to"))
            {
                /* Don't filter out */
            }
            else if (message.Contains("Disconnected"))
            {
                return;
            }
            //else if (!DebugMessagesEnabled)
            //{
            //    return;
            //}

            //AddToLog(string.Format("{0}", arguments.Value));
        }

        public virtual void data_received_handler(Object sender, Nordicsemi.PipeDataEventArgs e)//(sender, e)
        {
            //""" Callback for the DLL if any data is recieved from peer device. """
            System.Text.StringBuilder stringBuffer = new System.Text.StringBuilder();
            foreach (Byte element in e.PipeData)
            {
                stringBuffer.AppendFormat(" 0x{0:X2}", element);
            }
            //if (DebugMessagesEnabled)
            //{
            //    AddToLog(String.Format("Data received on pipe number {0}:{1}", arguments.PipeNumber, stringBuffer.ToString()));
            //}

            int pipe_number = e.PipeNumber;
            
            //data = "".join("%02x" % byte for byte in e.PipeData);
            logger.log(String.Format("Received unhandled data on pipe {0} ({1})!", pipe_number, stringBuffer));//data));
        }


        void connected_handler(Object sender, EventArgs e)
        {
             //""" Callback for the DLL if connected event occur device. """
             this.connected = true;
             logger.log("Connected to peer device");            
        }


        void disconnected_handler(Object sender, ValueEventArgs<DisconnectReason> e)//(sender, e)
        {
            //""" Callback for the DLL if disconnect from peer device occur. """
            this.connected = false;
            this.last_disconnect_reason = e.Value;
            if(this.disconnect_event_expected)
            {
                logger.log("Disconnected from peer device");
                this.disconnect_event_expected = false;
            } else{
                logger.log("Error: Unexpected disconnection event!");
                this.num_of_errors += 1;
            }
        }

        /*REF*/
        void meEv_OnConnectionUpdateRequest(object sender, ConnectionUpdateRequestEventArgs arguments)
        {
            Task.Factory.StartNew(() =>
            {
                master.SendConnectionUpdateResponse(arguments.Identifier,  ConnectionUpdateResponse.Accepted);

                BtConnectionParameters updateParams = new BtConnectionParameters();
                updateParams.ConnectionIntervalMs = arguments.ConnectionIntervalMinMs;
                updateParams.SupervisionTimeoutMs = arguments.ConnectionSupervisionTimeoutMs;
                updateParams.SlaveLatency = arguments.SlaveLatency;
                master.UpdateConnectionParameters(updateParams);
            });
        }
        /**/

        void connection_update_handler(Object sender, ConnectionUpdateRequestEventArgs e)//(sender, e):
        {
            //""" Callback for the DLL if connection update request is received. """
            
            double conn_interval = e.ConnectionIntervalMinMs;
            double conn_timeout = e.ConnectionSupervisionTimeoutMs;
            int    identifier = e.Identifier;
            int    slave_latency = e.SlaveLatency;
            this.master.SendConnectionUpdateResponse(identifier, Nordicsemi.ConnectionUpdateResponse.Accepted);

            Nordicsemi.BtConnectionParameters connection_params;
            connection_params = new Nordicsemi.BtConnectionParameters(); //        connection_params = Nordicsemi.BtConnectionParamters()
            connection_params.ConnectionIntervalMs = conn_interval;
            connection_params.SlaveLatency = slave_latency;
            connection_params.SupervisionTimeoutMs = conn_timeout; //.ConnectionSupervisionTimeoutMs = conn_timeout;
            this.master.UpdateConnectionParameters(connection_params);
        }

        void pipe_error_handler(Object sender, Nordicsemi.PipeErrorEventArgs e)//(sender, e):
        {
            //""" Callback for the DLL if pipe errors occur. """
            logger.log(String.Format("Pipe error received on pipe {0}. ErrorCode = {1}", e.PipeNumber, e.ErrorCode));
            this.num_of_errors += 1;
        }

        void _wait_for_disconnect(int wait_for_disconnect_delay=0, int expected_disconnected_reason=0) //TODO params
        {
            //""" Internal function that waits for N seconds for disconnected_handler
            //to be called, setting this.connected to false. Then, it will compare
            //disconnect reason against the expected. """
            int i;
            this.disconnect_event_expected = true;
            for(i = 0; i<50 ; i++)
            {
                if (this.connected)
                {
                    System.Threading.Thread.Sleep(100); //time.sleep(0.1);
                    Console.Write("."); //sys.stdout.write(".");
                } else {
                    //# Disconnect received. Check disconnect reason if needed.
                    if (expected_disconnected_reason != 0) //None
                    {
                        if (expected_disconnected_reason != (int)(this.last_disconnect_reason))
                        {
                            logger.log(String.Format("Incorrect disconnect reason. Expected = {0} Received = {1}", expected_disconnected_reason, this.last_disconnect_reason));
                            throw new  Exception(String.Format("Incorrect disconnect reason. Expected = {0} Received = {1}", expected_disconnected_reason, this.last_disconnect_reason));
                        }
                    }
                    break;
                }
            }            
            if( i == 50 )
                throw new System.Exception("Peer device did not disconnect!");
            
            /*
            """ Internal function that waits for N seconds for disconnected_handler
            to be called, setting this.connected to false. Then, it will compare
            disconnect reason against the expected. """
            this.disconnect_event_expected = true
            for i in range(50):
                if this.connected:
                    time.sleep(0.1)
                    sys.stdout.write(".")
                else:
                    # Disconnect received. Check disconnect reason if needed.
                    if (expected_disconnected_reason != None):
                        if (expected_disconnected_reason != int(this.last_disconnect_reason)):
                            logger.log("Incorrect disconnect reason. Expected = {0} Received = {1}".format(expected_disconnected_reason, this.last_disconnect_reason))
                            raise(Exception("Incorrect disconnect reason. Expected = {0} Received = {1}".format(expected_disconnected_reason, this.last_disconnect_reason)))
                    break
            else:
                raise(Exception("Peer device did not disconnect!"))
            */
        }

        bool _disconnect()
        {
            //""" Internal function for making device disconnect from peer device. """
            this.disconnect_event_expected = true;
            return (this.master.Disconnect());
            /*
            """ Internal function for making device disconnect from peer device. """
            this.disconnect_event_expected = true
            return this.master.Disconnect()
            */
        }

        BtDevice _discover_peer_named(String btle_address)
        {
            //""" Internal function which starts Master Emulator discovery, compare found devices against
            //name and address. If a match is found, return its address. """
            
            BtDevice peer_device = null;//None

            System.Collections.ObjectModel.Collection<BtDevice> found_devices = this.master.DiscoverDevices();

            if( found_devices != null)
            {
                foreach(var device in found_devices)
                {
                    //# print "device %r"%device.DeviceInfo
                    Console.WriteLine("Karel >>  "); // karel just testing
                    logger.log(String.Format("Device address {0}", device.DeviceAddress.Value));

                    if(btle_address == device.DeviceAddress.Value)
                    {
                        peer_device = device;
                        break;
                    }
                }

            }
            return peer_device;

            /*
            """ Internal function which starts Master Emulator discovery, compare found devices against
                name and address. If a match is found, return its address. """
            peer_device = None

            found_devices = self.master.DiscoverDevices()

            if found_devices:
                for device in found_devices:
                    # print "device %r"%device.DeviceInfo
                    print "Karel >>  " # karel just testing
                    self.log_handler.log("Device address %r" % (device.DeviceAddress.Value))

                    if btle_address == device.DeviceAddress.Value:
                        peer_device = device
                        break
            return peer_device    
            */
        }

        BtDevice _discover_peer_device()
        {            
            //""" Internal function that does 10 retries to find the peer device
            //with correct name and address. """            
            for( int i=0; i< 10; i++)
            {
                BtDevice peerDevice = this._discover_peer_named(this.peer_device_address);
                if(peerDevice != null)
                    return peerDevice;
            }

            throw new Exception( String.Format("Peer device with address {0} not found", this.peer_device_address) );
            //return (null);

            /*
            """ Internal function that does 10 retries to find the peer device
            with correct name and address. """
            for i in range(10):
                peerDevice = self._discover_peer_named(self.peer_device_address)
                if peerDevice != None:
                    return peerDevice
            else:
                raise(Exception("Peer device with address {0} not found".format(self.peer_device_address)))
            */

        }

        public void send_data(int pipe, byte[] msg, int count) //count == retry count
        {
            //  """ Send data to peer device. """
            if( !this.connected )
                return;

            for(int i = 0; i< count; i++)
            {
                if( ! this.master.SendData(pipe, msg) )
                    throw new Exception("Data could not be sent - failed!");
            }
            /*
            """ Send data to peer device. """
            if not this.connected:
                return
            for i in range(count):
                if not this.master.SendData(pipe, msg):
                    raise Exception("Data could not be sent - failed!")
            */
        }


        public virtual void setup_service()
        {
            //""" Function for setting up services.
            //If not no child class implements this function, service setup will be
            //flagged as not done. This will prevent service discovery.
            //"""

            logger.log("Not setting up any services!");
            this.service_setup_done = false;
        }

        public virtual void set_local_data()
        {
            //""" Function for setting up local data.
            //If not no child class implements this function, nothing will be set up.
            //"""

            logger.log("Not setting local data before run!");       
        }

        void restore_bond_info()
        {
            logger.info("TODO: restore_bond_info");
        /*
        if not this.bond_info:
            return

        LTK, RAND, EDIV = this.bond_info

        bondedDevice =  {this.peer_device_address:
                            {
                                'LTK': LTK,
                                'EDIV': EDIV,
                                'RAND': RAND
                            }
                        }

        serializedBondInfo = pickle.dumps(bondedDevice)

        this.master.RestoreBondInformation(serializedBondInfo)
        */
        }

        String filter_emulator_device(System.Collections.ObjectModel.Collection<string> emulator_devices, String emulator_filter) //TODO params
        {           
            //if not type(emulator_filter) is str:
            //    return None
            foreach(var device in emulator_devices)
            {
                Console.WriteLine("TODO device = {0}", device);
                //TODO if(emulator_filter in device)
                //TODO     return device;
                if( device.Contains( emulator_filter ) )
                    return device;
            }
            return null;
        }

        String change_byte_endianess(String hex_str)
        {
            String  transformed_str = "";

            for(int i=0; i<hex_str.Length; i+=2)// in xrange(0, len(hex_str), 2):
            {
                //char v1 = hex_str[-(i + 1)]; //TODO won't work in C#
                //char v2 = hex_str[-(i + 2)]; //TODO won't work in C#
                char v1 = hex_str[hex_str.Length - (i + 1)]; //TODO won't work in C#
                char v2 = hex_str[hex_str.Length - (i + 2)]; //TODO won't work in C#
                transformed_str += v2;
                transformed_str += v1;
            }
            return transformed_str;
        }

        public bool scan_and_connect(String emulator_filter="")
        {

            System.Collections.ObjectModel.Collection<string> emulator_devices;
    
            //""" Function for doing discovery of the peer device, connect and
            //discover pipes. The function will also open the Master Emulator. """
            try
            {
                this.master = new Nordicsemi.MasterEmulator();

                this.master.LogMessage += this.log_message_handler;
                this.master.DataReceived += this.data_received_handler;
                this.master.Connected += this.connected_handler;
                this.master.Disconnected += this.disconnected_handler;
                this.master.PipeError += this.pipe_error_handler;
                this.master.ConnectionUpdateRequest += this.connection_update_handler;

                emulator_devices = this.master.EnumerateUsb(Nordicsemi.UsbDeviceType.AnyDevice);

                String emulator_device = this.filter_emulator_device(emulator_devices, emulator_filter);

                if( emulator_device == null )
                {
                    //raise Exception("Could not find emulator device");
                    throw new System.Exception("Could not find emulator device");
                }

                this.master.SerialPortBaudRate = this.baud_rate;

                this.master.Open(emulator_device); //void Open(string usbSerialNumber);

                this.setup_service();

                //# Start Master Emulator
                this.master.Run();
                
                this.set_local_data();


                if( this.own_address != "" )
                {
                    String byte_endianess_transformed_address = this.change_byte_endianess(this.own_address);
                    BtDeviceAddress bt_device_address = new Nordicsemi.BtDeviceAddress(byte_endianess_transformed_address);
                    this.master.SetBDAddress(bt_device_address);
                }
                this.restore_bond_info();

                //# Run discovery procedure
                this.myPeerDevice = this._discover_peer_device();
                logger.log("Found peer device");

                //# Connect and bond to peer device
                Nordicsemi.BtConnectionParameters conn_params = new Nordicsemi.BtConnectionParameters();
                conn_params.ConnectionIntervalMs = 15;
                if( this.master.Connect(this.myPeerDevice.DeviceAddress) )
                {
                    logger.log("--- Connected ---");
                } else {
                    //raise(Exception("Connection failed"));
                    throw new System.Exception("Connection failed");
                }


                if( this.service_setup_done == true )
                {
                    //# Service discovery
                    bool found_pipes = this.master.DiscoverPipes();

                    //print "PIPES RETURN VALUE: %s" % found_pipes

                    if( !found_pipes )
                    {
                        return false;
                    }

                    logger.log("--- Pipes discovered ---");
                }
            }
            catch( Exception ex)
            {
                logger.log(String.Format("[EXCEPTION] {0}", ex));
                logger.log("Call stack:");
                //tb = traceback.extract_tb(sys.exc_info()[2])
                //for f in reversed(tb):
                //logger.log(String.Format("  File {0}, line {1}",os.path.basename(f[0]), f[1]));
                this.num_of_errors += 1;
                logger.log(String.Format("number of errors: {0}", this.num_of_errors));
                return false;
            }
            return true;

            /*
            """ Function for doing discovery of the peer device, connect and
                discover pipes. The function will also open the Master Emulator. """
            try:
                this.master = None
                this.master = Nordicsemi.MasterEmulator()

                this.master.LogMessage += this.log_message_handler
                this.master.DataReceived += this.data_received_handler
                this.master.Connected += this.connected_handler
                this.master.Disconnected += this.disconnected_handler
                this.master.PipeError += this.pipe_error_handler
                this.master.ConnectionUpdateRequest += this.connection_update_handler

                emulator_devices = this.master.EnumerateUsb(Nordicsemi.UsbDeviceType.AnyDevice)

                emulator_device = this.filter_emulator_device(emulator_devices, emulator_filter)

                if emulator_device is None:
                    raise Exception("Could not find emulator device")

                this.master.SerialPortBaudRate = this.baud_rate

                this.master.Open(emulator_device)

                this.setup_service()

                # Start Master Emulator
                this.master.Run()

                this.set_local_data()


                if this.own_address:
                    byte_endianess_transformed_address = this.change_byte_endianess(this.own_address)
                    bt_device_address = Nordicsemi.BtDeviceAddress(byte_endianess_transformed_address)
                    this.master.SetBDAddress(bt_device_address)

                this.restore_bond_info()

                # Run discovery procedure
                this.myPeerDevice = this._discover_peer_device()
                logger.log("Found peer device")

                # Connect and bond to peer device
                conn_params = Nordicsemi.BtConnectionParameters()
                conn_params.ConnectionIntervalMs = 15
                if this.master.Connect(this.myPeerDevice.DeviceAddress):
                    logger.log("--- Connected ---")
                else:
                    raise(Exception("Connection failed"))

                if this.service_setup_done:
                    # Service discovery
                    found_pipes = this.master.DiscoverPipes()

                    print "PIPES RETURN VALUE: %s" % found_pipes

                    if not found_pipes:
                        return false

                    logger.log("--- Pipes discovered ---")

            except Exception, ex:
                logger.log("[EXCEPTION] %s" % ex)
                logger.log("Call stack:")
                tb = traceback.extract_tb(sys.exc_info()[2])
                for f in reversed(tb):
                    logger.log("  File {0}, line {1}".format(os.path.basename(f[0]), f[1]))
                this.num_of_errors += 1
                logger.log("number of errors: %i" % this.num_of_errors)
                return false

            return true
            */
        }

        public void disconnect()
        {
            //""" Function for disconnecting from peer device and close the
            //Master Emulator if opened. """
            if (this.connected)
            {
                try {
                    logger.log("Will disconnect now");
                    this._disconnect();
                    this._wait_for_disconnect();
                }
                catch(Exception ex)
                {
                    logger.log(String.Format("[EXCEPTION] {0}", ex));
                    logger.log("Call stack:");
                    //tb = traceback.extract_tb(sys.exc_info()[2]);
                    //for f in reversed(tb):
                    //    logger.log("  File {0}, line {1}".format(os.path.basename(f[0]), f[1]))
                    this.num_of_errors += 1;
                }
            } else {
                logger.log("Not connected - Skipping");
            }
            //# close MasterEmulator if open
            if ((this.master != null) & (this.master.IsOpen))
                this.master.Close();

            /*
            """ Function for disconnecting from peer device and close the
            Master Emulator if opened. """
            if this.connected:
                try:
                    logger.log("Will disconnect now")
                    this._disconnect()
                    this._wait_for_disconnect()
                except Exception, ex:
                    logger.log("[EXCEPTION] %s" % ex)
                    logger.log("Call stack:")
                    tb = traceback.extract_tb(sys.exc_info()[2])
                    for f in reversed(tb):
                        logger.log("  File {0}, line {1}".format(os.path.basename(f[0]), f[1]))
                    this.num_of_errors += 1
            else:
                logger.log("Not connected - Skipping")

            # close MasterEmulator if open
            if (this.master != None) and this.master.IsOpen:
                this.master.Close()
            */
        }

    }
}
