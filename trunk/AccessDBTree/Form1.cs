using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Data;

namespace AccessDBTree
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            m_dbTree = new DBTreeOperation("DBInit.mdb");
        }
        public static DBTreeOperation m_dbTree;
        MyTreeView view = null;
        private void MainLoaded(object sender, EventArgs e)
        {
            view = new MyTreeView();
            elementHost1.Child = view;
            if( !m_dbTree.OpenDB() )
            {
                MessageBox.Show(m_dbTree.LastErrorString);
            }
            m_dbTree.UpdateDirTree();
            view.SetData(m_dbTree.DirProvider);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            m_dbTree.UpdateDirTree();
        }

        private void button2_Click(object sender, EventArgs e)
        {

        }
    }
}
