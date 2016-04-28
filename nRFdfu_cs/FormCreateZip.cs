using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace nRFdfu_cs
{
    public partial class FormCreateZip : Form
    {
        public FormCreateZip()
        {
            InitializeComponent();
        }

        //---------------------------------------------------------------------
        private void btnProgram_Click(object sender, EventArgs e)
        {

        }

        //---------------------------------------------------------------------
        private void btnBrowseAPP_Click(object sender, EventArgs e)
        {

        }

        private void btnBrowseSD_Click(object sender, EventArgs e)
        {

        }

        private void btnBrowseBL_Click(object sender, EventArgs e)
        {

        }

        //---------------------------------------------------------------------
        private void radioButton_Application_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_Application.Checked)
            {
                //radioButton_Application.Checked = false;
                radioButton_Softdevice.Checked = false;
                radioButton_Bootloader.Checked = false;
                radioButton_SDBL.Checked = false;

                labelApp.Visible = true;
                labelSd.Visible = false;
                labelBl.Visible = false;
                tbAppFile.Visible = true;
                tbSdFile.Visible = false;
                tbBlFile.Visible = false;
                btnBrowseAPP.Visible = true;
                btnBrowseSD.Visible = false;
                btnBrowseBL.Visible = false;
            }
        }

        private void radioButton_Softdevice_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_Softdevice.Checked)
            {
                radioButton_Application.Checked = false;
                //radioButton_Softdevice.Checked = false;
                radioButton_Bootloader.Checked = false;
                radioButton_SDBL.Checked = false;

                labelApp.Visible = false;
                labelSd.Visible = true;
                labelBl.Visible = false;
                tbAppFile.Visible = false;
                tbSdFile.Visible = true;
                tbBlFile.Visible = false;
                btnBrowseAPP.Visible = false;
                btnBrowseSD.Visible = true;
                btnBrowseBL.Visible = false;
            }
        }

        private void radioButton_Bootloader_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_Bootloader.Checked)
            {
                radioButton_Application.Checked = false;
                radioButton_Softdevice.Checked = false;
                //radioButton_Bootloader.Checked = false;
                radioButton_SDBL.Checked = false;

                labelApp.Visible = false;
                labelSd.Visible = false;
                labelBl.Visible = true;
                tbAppFile.Visible = false;
                tbSdFile.Visible = false;
                tbBlFile.Visible = true;
                btnBrowseAPP.Visible = false;
                btnBrowseSD.Visible = false;
                btnBrowseBL.Visible = true;
            }
        }

        private void radioButton_SDBL_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_SDBL.Checked)
            {
                radioButton_Application.Checked = false;
                radioButton_Softdevice.Checked = false;
                radioButton_Bootloader.Checked = false;
                //radioButton_SDBL.Checked = false;

                labelApp.Visible = false;
                labelSd.Visible = true;
                labelBl.Visible = true;
                tbAppFile.Visible = false;
                tbSdFile.Visible = true;
                tbBlFile.Visible = true;
                btnBrowseAPP.Visible = false;
                btnBrowseSD.Visible = true;
                btnBrowseBL.Visible = true;
            }
        }

        //---------------------------------------------------------------------

    }
}
