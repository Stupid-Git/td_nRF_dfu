namespace nRFdfu_cs
{
    partial class FormRTR500BLE
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButton_Application = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.btnProgram = new System.Windows.Forms.Button();
            this.comboBox_Comport = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btn_test_Dfu_transport = new System.Windows.Forms.Button();
            this.btnTest1 = new System.Windows.Forms.Button();
            this.tbFilename = new System.Windows.Forms.TextBox();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.btSerialPortScan = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioButton_Application);
            this.groupBox1.Location = new System.Drawing.Point(11, 36);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(360, 52);
            this.groupBox1.TabIndex = 38;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Hexfile Type";
            // 
            // radioButton_Application
            // 
            this.radioButton_Application.AutoSize = true;
            this.radioButton_Application.Checked = true;
            this.radioButton_Application.Location = new System.Drawing.Point(132, 22);
            this.radioButton_Application.Name = "radioButton_Application";
            this.radioButton_Application.Size = new System.Drawing.Size(80, 16);
            this.radioButton_Application.TabIndex = 1;
            this.radioButton_Application.TabStop = true;
            this.radioButton_Application.Text = "Application";
            this.radioButton_Application.UseVisualStyleBackColor = true;
            this.radioButton_Application.CheckedChanged += new System.EventHandler(this.radioButton_Application_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 12);
            this.label1.TabIndex = 34;
            this.label1.Text = "DFU /Hex File:";
            // 
            // btnProgram
            // 
            this.btnProgram.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnProgram.Location = new System.Drawing.Point(682, 90);
            this.btnProgram.Name = "btnProgram";
            this.btnProgram.Size = new System.Drawing.Size(97, 23);
            this.btnProgram.TabIndex = 41;
            this.btnProgram.Text = "Program";
            this.btnProgram.UseVisualStyleBackColor = true;
            this.btnProgram.Click += new System.EventHandler(this.btnProgram_Click);
            // 
            // comboBox_Comport
            // 
            this.comboBox_Comport.FormattingEnabled = true;
            this.comboBox_Comport.Items.AddRange(new object[] {
            "Predef-Direct",
            "Predef-Sim"});
            this.comboBox_Comport.Location = new System.Drawing.Point(484, 40);
            this.comboBox_Comport.Name = "comboBox_Comport";
            this.comboBox_Comport.Size = new System.Drawing.Size(192, 20);
            this.comboBox_Comport.TabIndex = 40;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(386, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 12);
            this.label2.TabIndex = 39;
            this.label2.Text = "Select COM port:";
            // 
            // btn_test_Dfu_transport
            // 
            this.btn_test_Dfu_transport.Location = new System.Drawing.Point(65, 154);
            this.btn_test_Dfu_transport.Name = "btn_test_Dfu_transport";
            this.btn_test_Dfu_transport.Size = new System.Drawing.Size(143, 23);
            this.btn_test_Dfu_transport.TabIndex = 37;
            this.btn_test_Dfu_transport.Text = "test_Dfu_transport";
            this.btn_test_Dfu_transport.UseVisualStyleBackColor = true;
            this.btn_test_Dfu_transport.Click += new System.EventHandler(this.btn_test_Dfu_transport_Click);
            // 
            // btnTest1
            // 
            this.btnTest1.Location = new System.Drawing.Point(281, 154);
            this.btnTest1.Name = "btnTest1";
            this.btnTest1.Size = new System.Drawing.Size(75, 23);
            this.btnTest1.TabIndex = 36;
            this.btnTest1.Text = "Test 1";
            this.btnTest1.UseVisualStyleBackColor = true;
            this.btnTest1.Click += new System.EventHandler(this.btnTest1_Click);
            // 
            // tbFilename
            // 
            this.tbFilename.Location = new System.Drawing.Point(101, 6);
            this.tbFilename.Name = "tbFilename";
            this.tbFilename.Size = new System.Drawing.Size(576, 19);
            this.tbFilename.TabIndex = 35;
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(683, 4);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(97, 23);
            this.btnBrowse.TabIndex = 33;
            this.btnBrowse.Text = "Browse...";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // btSerialPortScan
            // 
            this.btSerialPortScan.Location = new System.Drawing.Point(682, 208);
            this.btSerialPortScan.Name = "btSerialPortScan";
            this.btSerialPortScan.Size = new System.Drawing.Size(97, 23);
            this.btSerialPortScan.TabIndex = 42;
            this.btSerialPortScan.Text = "SerialPortScan";
            this.btSerialPortScan.UseVisualStyleBackColor = true;
            this.btSerialPortScan.Click += new System.EventHandler(this.btSerialPortScan_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // FormRTR500BLE
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(791, 362);
            this.Controls.Add(this.btSerialPortScan);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnProgram);
            this.Controls.Add(this.comboBox_Comport);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btn_test_Dfu_transport);
            this.Controls.Add(this.btnTest1);
            this.Controls.Add(this.tbFilename);
            this.Controls.Add(this.btnBrowse);
            this.Name = "FormRTR500BLE";
            this.Text = "RTR-500BLE Bluetooth Application Update";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton radioButton_Application;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnProgram;
        private System.Windows.Forms.ComboBox comboBox_Comport;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btn_test_Dfu_transport;
        private System.Windows.Forms.Button btnTest1;
        private System.Windows.Forms.TextBox tbFilename;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.Button btSerialPortScan;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
    }
}