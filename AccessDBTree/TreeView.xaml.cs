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
using System.Windows.Threading;
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
            TreeView tree = sender as TreeView;
            XmlElement ele = tree.SelectedItem as XmlElement;
            if ( ele == null )
            {
                return;
            }
            //MessageBox.Show(ele.Name);
            List<DirItem> items = Form1.m_dbTree.GetFileNames(ele.Name);
            lbBooks.ItemsSource = items;
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
            if (Form1.m_dbTree.IsDirNameExist( ele.Name, newName.Text) == 1 )
            {
                MessageBox.Show("文件名已经存在！");
                return;
            }
            Form1.m_dbTree.AddDirectory(ele.Name, newName.Text);

           // Expand(treeTest, treeTest.Items, ele.Name);
            ExpandAll(null, null);
        }

        private void Expand(ItemsControl collect, ItemCollection items, string strID)
        {

            for (int i = 0; i < items.Count; i++)
            {

                TreeViewItem item = collect.ItemContainerGenerator.ContainerFromItem(items[i]) as TreeViewItem;

                if (item != null)
                {
                    item.IsExpanded = true;
                    WaitForPriority(DispatcherPriority.ContextIdle);
                    if ((items[i] as XmlElement).Name == strID)
                    {
                        return;
                        
                    }

                    Expand(item, item.Items, strID);
                }
            }

        }
        internal static void WaitForPriority(DispatcherPriority priority)
        {
            DispatcherFrame frame = new DispatcherFrame();
            DispatcherOperation dispatcherOperation = Dispatcher.CurrentDispatcher.BeginInvoke(priority, new DispatcherOperationCallback(ExitFrameOperation), frame);
            Dispatcher.PushFrame(frame);
            if (dispatcherOperation.Status != DispatcherOperationStatus.Completed)
            {
                dispatcherOperation.Abort();
            }
        }

        private static object ExitFrameOperation(object obj)
        {
            ((DispatcherFrame)obj).Continue = false;
            return null;
        }



        private void ExpandAll(object sender, RoutedEventArgs e)
        {
            ApplyActionToAllTreeViewItems(itemsControl =>
            {
                itemsControl.IsExpanded = true;
                System.Windows.Forms.Application.DoEvents();
                WaitForPriority(DispatcherPriority.ContextIdle);
            },
            treeTest);
        }

        private void ApplyActionToAllTreeViewItems(Action<TreeViewItem> itemAction, ItemsControl itemsControl)
        {
            Stack<ItemsControl> itemsControlStack = new Stack<ItemsControl>();
            itemsControlStack.Push(itemsControl);

            while (itemsControlStack.Count != 0)
            {
                ItemsControl currentItem = itemsControlStack.Pop() as ItemsControl;
                TreeViewItem currentTreeViewItem = currentItem as TreeViewItem;
                if (currentTreeViewItem != null)
                {
                    itemAction(currentTreeViewItem);
                }
                if (currentItem != null) // this handles the scenario where some TreeViewItems are already collapsed
                {
                    foreach (object dataItem in currentItem.Items)
                    {
                        ItemsControl childElement = (ItemsControl)currentItem.ItemContainerGenerator.ContainerFromItem(dataItem);
                        itemsControlStack.Push(childElement);
                    }
                }
            }
        }

        private void CollapseTopLevel(object sender, RoutedEventArgs e)
        {
            foreach (XmlElement item in treeTest.Items)
            {
                TreeViewItem tvi = treeTest.ItemContainerGenerator.ContainerFromItem(item) as TreeViewItem;
                tvi.IsExpanded = false;
            }
        }

        private void CollapseAll(object sender, RoutedEventArgs e)
        {
            ApplyActionToAllTreeViewItems(itemsControl => itemsControl.IsExpanded = false, treeTest);
        }

        //private void SelectOne(object sender, RoutedEventArgs e)
        //{

        //    foreach (XmlElement firstLevelDataItem in treeTest.Items)
        //    {
        //            // Expand superclasses
        //            TreeViewItem parentTreeViewItem = (TreeViewItem)treeTest.ItemContainerGenerator.ContainerFromItem(firstLevelDataItem);
        //            parentTreeViewItem.IsExpanded = true;
        //            WaitForPriority(DispatcherPriority.Background);

        //            foreach ()
        //            {
        //                TreeViewItem treeViewItemToExpand = (TreeViewItem)parentTreeViewItem.ItemContainerGenerator.ContainerFromItem(superclassToExpand);
        //                treeViewItemToExpand.IsExpanded = true;
        //                WaitForPriority(DispatcherPriority.Background);
        //                parentTreeViewItem = treeViewItemToExpand;
        //            }

        //            // Select node
        //            TreeViewItem treeViewItemToSelect = (TreeViewItem)parentTreeViewItem.ItemContainerGenerator.ContainerFromItem(elementToExpand);
        //            treeViewItemToSelect.IsSelected = true;
        //    }
        //}

        //private Collection<Taxonomy> GetSuperclasses(Taxonomy currentItem, Taxonomy itemToLookFor)
        //{
        //    if (itemToLookFor == currentItem)
        //    {
        //        Collection<Taxonomy> results = new Collection<Taxonomy>();
        //        return results;
        //    }
        //    else
        //    {
        //        foreach (Taxonomy subclass in currentItem.Subclasses)
        //        {
        //            Collection<Taxonomy> results = GetSuperclasses(subclass, itemToLookFor);
        //            if (results != null)
        //            {
        //                results.Insert(0, currentItem);
        //                return results;
        //            }
        //        }
        //        return null;
        //    }
        //}




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
            if ( Form1.m_dbTree.IsFileNameExist( ele.Name, "测试") == 1 )
            {
                MessageBox.Show("文件名已经存在！");
                return;
            }
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

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            Expand(treeTest, treeTest.Items, "9");
           // ExpandAll(null, null);
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            CollapseAll(null, null);
            //CollapseTopLevel(null, null);
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
