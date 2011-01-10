using System;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;
using VhdWrapper;

namespace TestVirtualDisk
{
    public partial class NewVhdForm : Form
    {
        VHD MyNewVhd;

        public NewVhdForm()
        {
            InitializeComponent();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            //Abonnement événements sur la création asynchrone.
            VHD.CreateVirtualDiskProgress += new VHD.CreateDiskOperationProgressHandler(VHD_CreateVirtualDiskProgress);
            VHD.CreateVirtualDiskComplete += new VHD.CreateDiskAsyncCompleteHandler(VHD_CreateVirtualDiskCompleteAsync);

            //Démarre la création du fichier vhd.
            VHD.CreateAsync(textBoxVHD.Text, (ulong)numericUpDownSizeNewVhd.Value * 1024 * 1024, (VHD_CREATE_FLAG)e.Argument);
        }

        void VHD_CreateVirtualDiskCompleteAsync(VHD newCreatedVhd)
        {
            if (newCreatedVhd != null)
            {
                MyNewVhd = newCreatedVhd;
            }
        }

        void VHD_CreateVirtualDiskProgress(VirtualDiskOperationProgress value)
        {
            backgroundWorkerCreate.ReportProgress(value.ProgressPercentage);
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (VHD.GetLastError() == VHD.ERROR_SUCCESS)
            {
                progressBarCreateVhd.Value = 0;
                VHD.CreateVirtualDiskProgress -= new VHD.CreateDiskOperationProgressHandler(VHD_CreateVirtualDiskProgress);
                VHD.CreateVirtualDiskComplete -= new VHD.CreateDiskAsyncCompleteHandler(VHD_CreateVirtualDiskCompleteAsync);

                MessageBox.Show("Creation de l'image disque virtuel réussi.", "VHD Creation", MessageBoxButtons.OK, MessageBoxIcon.Information);

                using (ToolsVhdForm toolsForm = new ToolsVhdForm())
                {
                    toolsForm.buttonOpen.Enabled = false;
                    toolsForm.myVhd = MyNewVhd;
                    toolsForm.ShowDialog();
                }
            }
            else
            {
                MessageBox.Show(string.Format("Echec de la création du fichier image disque virtuel. Code erreur {0}.", VHD.GetLastError()),
                    "VHD Creation", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBarCreateVhd.Value = e.ProgressPercentage;
        }

        private void buttonSelectPathCreate_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog sfd = new SaveFileDialog())
            {
                sfd.AddExtension = true;
                sfd.AutoUpgradeEnabled = true;
                sfd.CheckPathExists = true;
                sfd.Filter = "VHD file (.vhd)|*.vhd";

                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    if (File.Exists(sfd.FileName))
                    {
                        MessageBox.Show("Le fichier vhd sélectionné existe déjà ! Choisir un fichier inexistant SVP.", "VHD Creation", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }

                    textBoxVHD.Text = sfd.FileName;
                }
            }
        }

        private void buttonNewVhd_Click(object sender, EventArgs e)
        {
            if (MyNewVhd != null)
            {
                if (MyNewVhd.IsOpen)
                    MyNewVhd.Close();

                MyNewVhd.Dispose();
            }

            if (!string.IsNullOrEmpty(textBoxVHD.Text))
            {
                VHD_CREATE_FLAG createFlg = radioButtonNewVhdFixed.Checked ?
                    VHD_CREATE_FLAG.FULL_PHYSICAL_ALLOCATION : VHD_CREATE_FLAG.NONE;

                if (checkBoxAsyncCreate.Checked)
                {
                    backgroundWorkerCreate.RunWorkerAsync(createFlg);
                }
                else
                {
                    //Créer un fichier vhd en mode synchrone.
                    MyNewVhd = VHD.Create(textBoxVHD.Text, (ulong)numericUpDownSizeNewVhd.Value * 1024 * 1024, createFlg);
                    if (VHD.GetLastError() == VHD.ERROR_SUCCESS)
                    {
                        MessageBox.Show("Creation de l'image disque virtuel réussie.", "VHD Creation", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        using (ToolsVhdForm toolsForm = new ToolsVhdForm())
                        {
                            toolsForm.buttonOpen.Enabled = false;
                            toolsForm.myVhd = MyNewVhd;
                            toolsForm.ShowDialog();
                        }
                    }
                    else
                    {
                        MessageBox.Show(string.Format("Echec de la création du fichier image disque virtuel. Code erreur {0}.", VHD.GetLastError()),
                            "VHD Creation", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }
    }
}
