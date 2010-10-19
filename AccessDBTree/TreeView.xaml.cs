using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Globalization;
using System.Xml;
namespace AccessDBTree
{
    /// <summary>
    /// TreeView.xaml 的交互逻辑
    /// </summary>
    public partial class MyTreeView : UserControl
    {
        public MyTreeView()
        {
            InitializeComponent();
        }

        public void SetData(XmlDataProvider data)
        {
            Binding bind = new Binding();
            bind.XPath = "*";
            bind.Source = data;
            BindingOperations.SetBinding(treeTest, TreeView.ItemsSourceProperty, bind);
        }

        private void treeTest_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            //TreeView tree = sender as TreeView;
            //XmlElement ele = tree.SelectedItem as XmlElement;
            //MessageBox.Show(ele.Name);
            //List<DirItem> items = Form1.m_dbTree.GetFileNames(ele.Name);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            XmlElement ele = treeTest.SelectedItem as XmlElement;
            string txt = newName.Text.Trim();
            if (txt == "")
            {
                return;
            }
            if (ele == null)
            {
                return;
            }
            Form1.m_dbTree.AddDirectory(ele.Name, newName.Text);
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            XmlElement ele = treeTest.SelectedItem as XmlElement;
            if (ele == null)
            {
                return;
            }
            Form1.m_dbTree.DelDirectory(ele.Name);
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            XmlElement ele = treeTest.SelectedItem as XmlElement;
            if (ele == null)
            {
                return;
            }
            string test = "1111111111111111111111111111";
            byte []data = Encoding.ASCII.GetBytes(test);
            if ( Form1.m_dbTree.AddFile(ele.Name, "测试",data))
            {
                MessageBox.Show("插入成功！");
            }
            else
            {
                MessageBox.Show(Form1.m_dbTree.LastErrorString);
            }
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            byte[] data = Form1.m_dbTree.ReadFile("18");
            if ( data == null )
            {
                return;
            }

            MessageBox.Show(Encoding.ASCII.GetString(data));
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {

        }
    }

    public class NameConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string strName = "";
            if ( Form1.m_dbTree.DicDirNames.TryGetValue((string)value, out strName))
            {
                return strName;
            }
            else
            {
                return value;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value;
        }
    }

}
