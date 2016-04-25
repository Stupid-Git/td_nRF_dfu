namespace nRFdfu_cs
{
    partial class Form1
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
            this.tbFilename = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.btnProgram = new System.Windows.Forms.Button();
            this.checkBox_flowcontrol = new System.Windows.Forms.CheckBox();
            this.comboBox_baudrate = new System.Windows.Forms.ComboBox();
            this.comboBox_Comport = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButton_Bootloader = new System.Windows.Forms.RadioButton();
            this.radioButton_Application = new System.Windows.Forms.RadioButton();
            this.radioButton_Softdevice = new System.Windows.Forms.RadioButton();
            this.btn_test_Dfu_transport = new System.Windows.Forms.Button();
            this.btnTest1 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbFilename
            // 
            this.tbFilename.Location = new System.Drawing.Point(101, 6);
            this.tbFilename.Name = "tbFilename";
            this.tbFilename.Size = new System.Drawing.Size(576, 19);
            this.tbFilename.TabIndex = 19;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 12);
            this.label1.TabIndex = 18;
            this.label1.Text = "DFU /Hex File:";
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(683, 4);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(97, 23);
            this.btnBrowse.TabIndex = 17;
            this.btnBrowse.Text = "Browse...";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // btnProgram
            // 
            this.btnProgram.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnProgram.Location = new System.Drawing.Point(682, 90);
            this.btnProgram.Name = "btnProgram";
            this.btnProgram.Size = new System.Drawing.Size(97, 23);
            this.btnProgram.TabIndex = 32;
            this.btnProgram.Text = "Program";
            this.btnProgram.UseVisualStyleBackColor = true;
            this.btnProgram.Click += new System.EventHandler(this.btnProgram_Click);
            // 
            // checkBox_flowcontrol
            // 
            this.checkBox_flowcontrol.AutoSize = true;
            this.checkBox_flowcontrol.Location = new System.Drawing.Point(501, 94);
            this.checkBox_flowcontrol.Name = "checkBox_flowcontrol";
            this.checkBox_flowcontrol.Size = new System.Drawing.Size(104, 16);
            this.checkBox_flowcontrol.TabIndex = 31;
            this.checkBox_flowcontrol.Text = "Use flowcontrol";
            this.checkBox_flowcontrol.UseVisualStyleBackColor = true;
            // 
            // comboBox_baudrate
            // 
            this.comboBox_baudrate.FormattingEnabled = true;
            this.comboBox_baudrate.Items.AddRange(new object[] {
            "38400"});
            this.comboBox_baudrate.Location = new System.Drawing.Point(484, 68);
            this.comboBox_baudrate.Name = "comboBox_baudrate";
            this.comboBox_baudrate.Size = new System.Drawing.Size(192, 20);
            this.comboBox_baudrate.TabIndex = 30;
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
            this.comboBox_Comport.TabIndex = 29;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(386, 71);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(87, 12);
            this.label3.TabIndex = 28;
            this.label3.Text = "Select baudrate:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(386, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 12);
            this.label2.TabIndex = 27;
            this.label2.Text = "Select COM port:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioButton_Bootloader);
            this.groupBox1.Controls.Add(this.radioButton_Application);
            this.groupBox1.Controls.Add(this.radioButton_Softdevice);
            this.groupBox1.Location = new System.Drawing.Point(11, 36);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(360, 52);
            this.groupBox1.TabIndex = 26;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Hexfile Type";
            // 
            // radioButton_Bootloader
            // 
            this.radioButton_Bootloader.AutoSize = true;
            this.radioButton_Bootloader.Location = new System.Drawing.Point(252, 22);
            this.radioButton_Bootloader.Name = "radioButton_Bootloader";
            this.radioButton_Bootloader.Size = new System.Drawing.Size(78, 16);
            this.radioButton_Bootloader.TabIndex = 2;
            this.radioButton_Bootloader.Text = "Bootloader";
            this.radioButton_Bootloader.UseVisualStyleBackColor = true;
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
            // 
            // radioButton_Softdevice
            // 
            this.radioButton_Softdevice.AutoSize = true;
            this.radioButton_Softdevice.Location = new System.Drawing.Point(12, 22);
            this.radioButton_Softdevice.Name = "radioButton_Softdevice";
            this.radioButton_Softdevice.Size = new System.Drawing.Size(77, 16);
            this.radioButton_Softdevice.TabIndex = 0;
            this.radioButton_Softdevice.Text = "Softdevice";
            this.radioButton_Softdevice.UseVisualStyleBackColor = true;
            // 
            // btn_test_Dfu_transport
            // 
            this.btn_test_Dfu_transport.Location = new System.Drawing.Point(65, 154);
            this.btn_test_Dfu_transport.Name = "btn_test_Dfu_transport";
            this.btn_test_Dfu_transport.Size = new System.Drawing.Size(143, 23);
            this.btn_test_Dfu_transport.TabIndex = 25;
            this.btn_test_Dfu_transport.Text = "test_Dfu_transport";
            this.btn_test_Dfu_transport.UseVisualStyleBackColor = true;
            this.btn_test_Dfu_transport.Click += new System.EventHandler(this.btn_test_Dfu_transport_Click);
            // 
            // btnTest1
            // 
            this.btnTest1.Location = new System.Drawing.Point(281, 154);
            this.btnTest1.Name = "btnTest1";
            this.btnTest1.Size = new System.Drawing.Size(75, 23);
            this.btnTest1.TabIndex = 24;
            this.btnTest1.Text = "Test 1";
            this.btnTest1.UseVisualStyleBackColor = true;
            this.btnTest1.Click += new System.EventHandler(this.btnTest1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(791, 251);
            this.Controls.Add(this.btnProgram);
            this.Controls.Add(this.checkBox_flowcontrol);
            this.Controls.Add(this.comboBox_baudrate);
            this.Controls.Add(this.comboBox_Comport);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btn_test_Dfu_transport);
            this.Controls.Add(this.btnTest1);
            this.Controls.Add(this.tbFilename);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnBrowse);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.dfu_Form_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tbFilename;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button btnProgram;
        private System.Windows.Forms.CheckBox checkBox_flowcontrol;
        private System.Windows.Forms.ComboBox comboBox_baudrate;
        private System.Windows.Forms.ComboBox comboBox_Comport;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton radioButton_Bootloader;
        private System.Windows.Forms.RadioButton radioButton_Application;
        private System.Windows.Forms.RadioButton radioButton_Softdevice;
        private System.Windows.Forms.Button btn_test_Dfu_transport;
        private System.Windows.Forms.Button btnTest1;
    }
}

