using System;
using System.Windows.Forms;

namespace TestVirtualDisk
{
    public partial class StarterForm : Form
    {
        public StarterForm()
        {
            InitializeComponent();
        }

        private void buttonVHD_Click(object sender, EventArgs e)
        {
            using (NewVhdForm fCreate = new NewVhdForm())
            {
                fCreate.ShowDialog();
            }
        }

        private void buttonTools_Click(object sender, EventArgs e)
        {
            using (ToolsVhdForm fTools = new ToolsVhdForm())
            {
                fTools.buttonOpen.Enabled = true;
                fTools.ShowDialog();
            }
        }
    }
}
