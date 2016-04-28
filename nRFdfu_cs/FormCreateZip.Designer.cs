namespace nRFdfu_cs
{
    partial class FormCreateZip
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
            this.btnProgram = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButton_Bootloader = new System.Windows.Forms.RadioButton();
            this.radioButton_Application = new System.Windows.Forms.RadioButton();
            this.radioButton_Softdevice = new System.Windows.Forms.RadioButton();
            this.btn_test_Dfu_transport = new System.Windows.Forms.Button();
            this.btnTest1 = new System.Windows.Forms.Button();
            this.tbAppFile = new System.Windows.Forms.TextBox();
            this.labelApp = new System.Windows.Forms.Label();
            this.btnBrowseAPP = new System.Windows.Forms.Button();
            this.tbSdFile = new System.Windows.Forms.TextBox();
            this.tbBlFile = new System.Windows.Forms.TextBox();
            this.labelSd = new System.Windows.Forms.Label();
            this.labelBl = new System.Windows.Forms.Label();
            this.btnBrowseSD = new System.Windows.Forms.Button();
            this.btnBrowseBL = new System.Windows.Forms.Button();
            this.radioButton_SDBL = new System.Windows.Forms.RadioButton();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnProgram
            // 
            this.btnProgram.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnProgram.Location = new System.Drawing.Point(683, 257);
            this.btnProgram.Name = "btnProgram";
            this.btnProgram.Size = new System.Drawing.Size(97, 23);
            this.btnProgram.TabIndex = 44;
            this.btnProgram.Text = "Program";
            this.btnProgram.UseVisualStyleBackColor = true;
            this.btnProgram.Click += new System.EventHandler(this.btnProgram_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioButton_SDBL);
            this.groupBox1.Controls.Add(this.radioButton_Bootloader);
            this.groupBox1.Controls.Add(this.radioButton_Application);
            this.groupBox1.Controls.Add(this.radioButton_Softdevice);
            this.groupBox1.Location = new System.Drawing.Point(14, 93);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(465, 52);
            this.groupBox1.TabIndex = 38;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Hexfile Type";
            // 
            // radioButton_Bootloader
            // 
            this.radioButton_Bootloader.AutoSize = true;
            this.radioButton_Bootloader.Location = new System.Drawing.Point(218, 22);
            this.radioButton_Bootloader.Name = "radioButton_Bootloader";
            this.radioButton_Bootloader.Size = new System.Drawing.Size(78, 16);
            this.radioButton_Bootloader.TabIndex = 2;
            this.radioButton_Bootloader.Text = "Bootloader";
            this.radioButton_Bootloader.UseVisualStyleBackColor = true;
            this.radioButton_Bootloader.CheckedChanged += new System.EventHandler(this.radioButton_Bootloader_CheckedChanged);
            // 
            // radioButton_Application
            // 
            this.radioButton_Application.AutoSize = true;
            this.radioButton_Application.Checked = true;
            this.radioButton_Application.Location = new System.Drawing.Point(6, 22);
            this.radioButton_Application.Name = "radioButton_Application";
            this.radioButton_Application.Size = new System.Drawing.Size(80, 16);
            this.radioButton_Application.TabIndex = 1;
            this.radioButton_Application.TabStop = true;
            this.radioButton_Application.Text = "Application";
            this.radioButton_Application.UseVisualStyleBackColor = true;
            this.radioButton_Application.CheckedChanged += new System.EventHandler(this.radioButton_Application_CheckedChanged);
            // 
            // radioButton_Softdevice
            // 
            this.radioButton_Softdevice.AutoSize = true;
            this.radioButton_Softdevice.Location = new System.Drawing.Point(112, 22);
            this.radioButton_Softdevice.Name = "radioButton_Softdevice";
            this.radioButton_Softdevice.Size = new System.Drawing.Size(77, 16);
            this.radioButton_Softdevice.TabIndex = 0;
            this.radioButton_Softdevice.Text = "Softdevice";
            this.radioButton_Softdevice.UseVisualStyleBackColor = true;
            this.radioButton_Softdevice.CheckedChanged += new System.EventHandler(this.radioButton_Softdevice_CheckedChanged);
            // 
            // btn_test_Dfu_transport
            // 
            this.btn_test_Dfu_transport.Location = new System.Drawing.Point(66, 321);
            this.btn_test_Dfu_transport.Name = "btn_test_Dfu_transport";
            this.btn_test_Dfu_transport.Size = new System.Drawing.Size(143, 23);
            this.btn_test_Dfu_transport.TabIndex = 37;
            this.btn_test_Dfu_transport.Text = "test_Dfu_transport";
            this.btn_test_Dfu_transport.UseVisualStyleBackColor = true;
            // 
            // btnTest1
            // 
            this.btnTest1.Location = new System.Drawing.Point(282, 321);
            this.btnTest1.Name = "btnTest1";
            this.btnTest1.Size = new System.Drawing.Size(75, 23);
            this.btnTest1.TabIndex = 36;
            this.btnTest1.Text = "Test 1";
            this.btnTest1.UseVisualStyleBackColor = true;
            // 
            // tbAppFile
            // 
            this.tbAppFile.Location = new System.Drawing.Point(110, 6);
            this.tbAppFile.Name = "tbAppFile";
            this.tbAppFile.Size = new System.Drawing.Size(577, 19);
            this.tbAppFile.TabIndex = 35;
            // 
            // labelApp
            // 
            this.labelApp.AutoSize = true;
            this.labelApp.Location = new System.Drawing.Point(12, 9);
            this.labelApp.Name = "labelApp";
            this.labelApp.Size = new System.Drawing.Size(87, 12);
            this.labelApp.TabIndex = 34;
            this.labelApp.Text = "Application File:";
            // 
            // btnBrowseAPP
            // 
            this.btnBrowseAPP.Location = new System.Drawing.Point(693, 4);
            this.btnBrowseAPP.Name = "btnBrowseAPP";
            this.btnBrowseAPP.Size = new System.Drawing.Size(97, 23);
            this.btnBrowseAPP.TabIndex = 33;
            this.btnBrowseAPP.Text = "Browse...";
            this.btnBrowseAPP.UseVisualStyleBackColor = true;
            this.btnBrowseAPP.Click += new System.EventHandler(this.btnBrowseAPP_Click);
            // 
            // tbSdFile
            // 
            this.tbSdFile.Location = new System.Drawing.Point(110, 31);
            this.tbSdFile.Name = "tbSdFile";
            this.tbSdFile.Size = new System.Drawing.Size(577, 19);
            this.tbSdFile.TabIndex = 45;
            // 
            // tbBlFile
            // 
            this.tbBlFile.Location = new System.Drawing.Point(110, 56);
            this.tbBlFile.Name = "tbBlFile";
            this.tbBlFile.Size = new System.Drawing.Size(577, 19);
            this.tbBlFile.TabIndex = 46;
            // 
            // labelSd
            // 
            this.labelSd.AutoSize = true;
            this.labelSd.Location = new System.Drawing.Point(12, 34);
            this.labelSd.Name = "labelSd";
            this.labelSd.Size = new System.Drawing.Size(90, 12);
            this.labelSd.TabIndex = 47;
            this.labelSd.Text = "Soft Device File:";
            // 
            // labelBl
            // 
            this.labelBl.AutoSize = true;
            this.labelBl.Location = new System.Drawing.Point(12, 59);
            this.labelBl.Name = "labelBl";
            this.labelBl.Size = new System.Drawing.Size(92, 12);
            this.labelBl.TabIndex = 48;
            this.labelBl.Text = "Boot Loader File:";
            // 
            // btnBrowseSD
            // 
            this.btnBrowseSD.Location = new System.Drawing.Point(693, 29);
            this.btnBrowseSD.Name = "btnBrowseSD";
            this.btnBrowseSD.Size = new System.Drawing.Size(97, 23);
            this.btnBrowseSD.TabIndex = 49;
            this.btnBrowseSD.Text = "Browse...";
            this.btnBrowseSD.UseVisualStyleBackColor = true;
            this.btnBrowseSD.Click += new System.EventHandler(this.btnBrowseSD_Click);
            // 
            // btnBrowseBL
            // 
            this.btnBrowseBL.Location = new System.Drawing.Point(693, 54);
            this.btnBrowseBL.Name = "btnBrowseBL";
            this.btnBrowseBL.Size = new System.Drawing.Size(97, 23);
            this.btnBrowseBL.TabIndex = 50;
            this.btnBrowseBL.Text = "Browse...";
            this.btnBrowseBL.UseVisualStyleBackColor = true;
            this.btnBrowseBL.Click += new System.EventHandler(this.btnBrowseBL_Click);
            // 
            // radioButton_SDBL
            // 
            this.radioButton_SDBL.AutoSize = true;
            this.radioButton_SDBL.Location = new System.Drawing.Point(312, 22);
            this.radioButton_SDBL.Name = "radioButton_SDBL";
            this.radioButton_SDBL.Size = new System.Drawing.Size(141, 16);
            this.radioButton_SDBL.TabIndex = 3;
            this.radioButton_SDBL.Text = "Softdevice+BootLoader";
            this.radioButton_SDBL.UseVisualStyleBackColor = true;
            this.radioButton_SDBL.CheckedChanged += new System.EventHandler(this.radioButton_SDBL_CheckedChanged);
            // 
            // FormCreateZip
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(802, 430);
            this.Controls.Add(this.btnBrowseBL);
            this.Controls.Add(this.btnBrowseSD);
            this.Controls.Add(this.labelBl);
            this.Controls.Add(this.labelSd);
            this.Controls.Add(this.tbBlFile);
            this.Controls.Add(this.tbSdFile);
            this.Controls.Add(this.btnProgram);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btn_test_Dfu_transport);
            this.Controls.Add(this.btnTest1);
            this.Controls.Add(this.tbAppFile);
            this.Controls.Add(this.labelApp);
            this.Controls.Add(this.btnBrowseAPP);
            this.Name = "FormCreateZip";
            this.Text = "FormCreateZip";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnProgram;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton radioButton_Bootloader;
        private System.Windows.Forms.RadioButton radioButton_Application;
        private System.Windows.Forms.RadioButton radioButton_Softdevice;
        private System.Windows.Forms.Button btn_test_Dfu_transport;
        private System.Windows.Forms.Button btnTest1;
        private System.Windows.Forms.TextBox tbAppFile;
        private System.Windows.Forms.Label labelApp;
        private System.Windows.Forms.Button btnBrowseAPP;
        private System.Windows.Forms.TextBox tbSdFile;
        private System.Windows.Forms.TextBox tbBlFile;
        private System.Windows.Forms.Label labelSd;
        private System.Windows.Forms.Label labelBl;
        private System.Windows.Forms.Button btnBrowseSD;
        private System.Windows.Forms.Button btnBrowseBL;
        private System.Windows.Forms.RadioButton radioButton_SDBL;
    }
}