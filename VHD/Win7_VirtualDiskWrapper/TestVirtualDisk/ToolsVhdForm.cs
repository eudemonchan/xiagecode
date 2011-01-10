using System;
using System.ComponentModel;
using System.Windows.Forms;
using VhdWrapper;

namespace TestVirtualDisk
{
    public partial class ToolsVhdForm : Form
    {
        public VHD myVhd { get; set; }

        public ToolsVhdForm()
        {
            InitializeComponent();
        }

        private void ToolsCreatedVhdForm_Load(object sender, EventArgs e)
        {
            if (myVhd != null)
            {
                //Charge le fichier vhd.
                textBoxFile.Text = myVhd.FileName;
            }
            else
            {
                myVhd = new VHD();
            }

            //Source d'infos disponibles.
            comboBoxInfos.DataSource = Enum.GetNames(typeof(VHD_INFO_REQUEST));
        }

        private void ToolsCreatedVhdForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (myVhd != null)
            {
                //Ferme et libère les ressources non managées utilisées.
                myVhd.Close();
                myVhd.Dispose();
            }
        }

        private void buttonDetach_Click(object sender, EventArgs e)
        {
            if (myVhd != null)
            {
                //Détache le disque virtuel.
                if (myVhd.Detach())
                    MessageBox.Show("Disque virtuel détaché !", "Détachement Vhd", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                else
                    MessageBox.Show(string.Format("Erreur pendant l'opération de détachement du disque virtuel. Code erreur {0}",
                        VHD.GetLastError()), "Détachement Vhd", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void buttonAttach_Click(object sender, EventArgs e)
        {
            if (myVhd != null)
            {
                //Attache le disque virtuel.
                if (myVhd.Attach(checkBoxReadOnly.Checked, checkBoxNoDriveLetter.Checked, checkBoxNoLocalHost.Checked, checkBoxPermLifetime.Checked))
                    MessageBox.Show("Disque virtuel attaché. Si vous venez de créer votre disque aller dans la console mmc du gestionnaire de disque et initialiser le et formatter le pour le voir monter par la suite dans l'explorateur !", "Attach Vhd", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                else
                    MessageBox.Show(string.Format("Erreur pendant l'opération de détachement du disque virtuel. Code erreur {0}",
                        VHD.GetLastError()), "Attach Vhd", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void buttonCompact_Click(object sender, EventArgs e)
        {
            if (myVhd != null)
            {
                try
                {
                    //Compression de l'image du disque virtuel en asynchrone.
                    if (checkBoxAsyncCreate.Checked)
                    {
                        backgroundWorkerCompact.RunWorkerAsync();
                    }
                    else
                    {
                        //Compression en synchrone.
                        myVhd.Compact();
                        if (VHD.GetLastError() == VHD.ERROR_SUCCESS)
                        {
                            MessageBox.Show("Compression réussi du disque virtuel.", "VHD compression", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        }
                        else
                        {
                            MessageBox.Show(string.Format("Erreur pendant l'opération de compression du disque virtuel. Code erreur {0}", VHD.GetLastError()),
                                "VHD compression", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "VHD compression erreur", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void backgroundWorkerCompact_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                //Abonnement événement sur la progression de la compression du disque virtuel.
                myVhd.CompactVirtualDiskProgress += new VHD.CompactDiskOperationProgressHandler(VHDCreated_CompactVirtualDiskProgress);
                //Démarre la compression de l'image du disque virtuel en asynchrone.
                myVhd.CompactAsync();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Erreur compression", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        void VHDCreated_CompactVirtualDiskProgress(VirtualDiskOperationProgress value)
        {
            backgroundWorkerCompact.ReportProgress(value.ProgressPercentage);
        }

        private void backgroundWorkerCompact_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBarChangeVhd.Value = e.ProgressPercentage;
        }

        private void backgroundWorkerCompact_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (VHD.GetLastError() == VHD.ERROR_SUCCESS)
            {
                progressBarChangeVhd.Value = 0;
                myVhd.CompactVirtualDiskProgress -= new VHD.CompactDiskOperationProgressHandler(VHDCreated_CompactVirtualDiskProgress);

                MessageBox.Show("Compression réussi du disque virtuel.", "VHD compression", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show(string.Format("Erreur pendant l'opération de compression du disque virtuel. Code erreur {0}", VHD.GetLastError()),
                                                "VHD compression", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void buttonExpand_Click(object sender, EventArgs e)
        {
            if (myVhd != null)
            {
                try
                {
                    //Expansion de l'image du disque virtuel en asynchrone.
                    if (checkBoxAsyncCreate.Checked)
                    {
                        backgroundWorkerExpand.RunWorkerAsync();
                    }
                    else
                    {
                        //Expansion synchrone.
                        myVhd.Expand((ulong)numericUpDownExpand.Value * 1024 * 1024);
                        if (VHD.GetLastError() == VHD.ERROR_SUCCESS)
                        {
                            MessageBox.Show("Expansion du disque virtuel réussi.", "Expansion VHD", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        }
                        else
                        {
                            MessageBox.Show(string.Format("Erreur pendant l'opération d'expansion du disque virtuel. Code erreur {0}", VHD.GetLastError()),
                                "Expansion VHD", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Erreur expansion", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void backgroundWorkerExpand_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                //Abonnement événement sur la progression de l'expansion du disque virtuel.
                myVhd.ExpandVirtualDiskProgress += new VHD.ExpandDiskOperationProgressHandler(VHDCreated_ExpandVirtualDiskProgress);
                //Démarre l'expansion de l'image du disque virtuel en asynchrone.
                myVhd.ExpandAsync((ulong)numericUpDownExpand.Value * 1024 * 1024);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Erreur expansion", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void backgroundWorkerExpand_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBarChangeVhd.Value = e.ProgressPercentage;
        }

        private void backgroundWorkerExpand_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (VHD.GetLastError() == VHD.ERROR_SUCCESS)
            {
                progressBarChangeVhd.Value = 0;
                myVhd.ExpandVirtualDiskProgress -= new VHD.ExpandDiskOperationProgressHandler(VHDCreated_ExpandVirtualDiskProgress);

                MessageBox.Show("Expansion du disque virtuel réussi.", "Expansion VHD", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show(string.Format("Erreur pendant l'opération d'expansion du disque virtuel. Code erreur {0}", VHD.GetLastError()),
                    "Expansion VHD", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        void VHDCreated_ExpandVirtualDiskProgress(VirtualDiskOperationProgress value)
        {
            backgroundWorkerExpand.ReportProgress(value.ProgressPercentage);
        }

        private void buttonOpen_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.AddExtension = true;
                ofd.AutoUpgradeEnabled = true;
                ofd.CheckPathExists = true;
                ofd.Filter = "VHD file (.vhd)|*.vhd";

                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    textBoxFile.Text = ofd.FileName;
                    myVhd.Open(textBoxFile.Text);
                }
            }
        }

        private void buttonGetInfo_Click(object sender, EventArgs e)
        {
            //Demande d'info sur l'image du disque virtuel.
            var val = (VHD_INFO_REQUEST)Enum.Parse(typeof(VHD_INFO_REQUEST), comboBoxInfos.SelectedValue.ToString());

            if (myVhd != null)
            {
                //Récupère l'info demandée.
                labelInfos.Text = myVhd.GetVirtualDiskInfo(val).ToString();
            }
        }
    }
}
