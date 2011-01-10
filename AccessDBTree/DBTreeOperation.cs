using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Windows.Data;
using System.Data.OleDb;
using System.Xml;
using System.IO;
namespace AccessDBTree
{
    public struct DirItem
    {
        public int ID {get;set;}
        public string Name {get;set;}
    }

    public class DBTreeOperation
    {
        private OleDbConnection m_conn;
        private Dictionary<string, string> m_dicDirNames;
        private XmlDataProvider m_dirProvider;
        public XmlDataProvider DirProvider
        {
            get { return m_dirProvider; }
        }

        public string ConnString { get; private set; }
        public string LastErrorString { private set; get; }

        public Dictionary<string, string> DicDirNames
        {
            get{ return m_dicDirNames;}
        }

        public DBTreeOperation(string strDBPath)
        {
            ConnString = "Provider=Microsoft.Jet.OleDb.4.0;Data Source=";
            ConnString += strDBPath;
            m_conn = new OleDbConnection(ConnString);
            m_dicDirNames = new Dictionary<string, string>();
            m_dirProvider = new XmlDataProvider();
            m_dirProvider.XPath = "*";
        }

        public bool DeleleFile( string strFileID )
        {
            string strSql = string.Format( @"DELETE FROM FILE WHERE ID = {0}", strFileID );
            return ExecuteSQLNonquery(strSql);
        }

        public bool DeleteFiles( List<string> strFiles)
        {
            foreach ( string name in strFiles )
            {
                if ( !DeleleFile(name) )
                {
                    return false;
                }
            }
            return true;
        }

        private bool GetAllDirNames()
        {
            OleDbCommand cmd = m_conn.CreateCommand();
            DicDirNames.Clear();
            cmd.CommandText = "SELECT * FROM Directory";
            try
            {
                OleDbDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    DicDirNames.Add(reader.GetInt32(0).ToString(), reader.GetString(2));
                }
                if (DicDirNames.Count == 0)
                {
                    LastErrorString = "数据库中数据为空！";
                    return false;
                }
            }
            catch (System.Exception ex)
            {
                LastErrorString = ex.Message;
                return false;
            }
            return true;
        }

        public bool OpenDB()
        {
            try
            {
                if (m_conn.State == ConnectionState.Open)
                {
                    return true;
                }
                m_conn.Open();
            }
            catch (System.Exception ex)
            {
                LastErrorString = ex.Message;
                return false;
            }
            return true;
        }

        public bool ExecuteSQLNonquery(string strSql)
        {
            OleDbCommand cmd = m_conn.CreateCommand();
            cmd.CommandText = strSql;
            try
            {
                cmd.ExecuteNonQuery();
                return true;
            }
            catch (Exception error)
            {
                LastErrorString = error.Message;
                return false;
            }
        }

        public string GetFileDirID(string fileID)
        {
            OleDbCommand cmd = new OleDbCommand(string.Format("select FatherID from File Where ID = {0}", fileID), m_conn);
            try
            {
                OleDbDataReader reader = cmd.ExecuteReader();
                if ( reader.Read() )
                {
                    int dirID = reader.GetInt32(0);
                    reader.Close();
                    return dirID.ToString();
                }
                else
                {
                    reader.Close();
                    return null;
                }
            }
            catch (System.Exception ex)
            {
                LastErrorString = ex.Message;
                return null;
            }
        }

        //1存在，0不存在，-1数据库错误
        public int IsFileNameExist(string strDirID, string strFileName)
        {
            List<DirItem> items = GetFileNames(strDirID);
            if ( items == null )
            {
                return -1;
            }

            foreach ( DirItem item in items )
            {
                if ( item.Name == strFileName )
                {
                    return 1;
                }
            }
            return -1;
        }

        public int IsDirNameExist(string strFatherID, string strDirName)
        {
            OleDbCommand cmd = m_conn.CreateCommand();
            cmd.CommandText = string.Format(@"SELECT * FROM Directory WHERE FatherID = {0} AND Name = '{1}'", strFatherID, strDirName);
            try
            {
                OleDbDataReader reader = cmd.ExecuteReader();
                if ( reader.Read() )
                {
                    reader.Close();
                    return 1;
                }
                else
                {
                    reader.Close();
                    return 0;
                }
            }
            catch (System.Exception e)
            {
                return -1;
            }
        }
        public List<DirItem> GetFileNames(string dirID)
        {
            List<DirItem> ids = new List<DirItem>();
            OleDbCommand cmd = m_conn.CreateCommand();
            cmd.CommandText = string.Format("SELECT * FROM File WHERE FatherID = {0}", dirID);
            try
            {
                OleDbDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    DirItem item = new DirItem();
                    item.ID = reader.GetInt32(0);
                    item.Name = reader.GetString(2);
                    ids.Add(item);
                }
                reader.Close();
                return ids;
            }
            catch (Exception error)
            {
                LastErrorString = error.Message;
                return null;
            }
        }

        public List<DirItem> GetChildDirID(string parentID)
        {
            List<DirItem> ids = new List<DirItem>();
            OleDbCommand cmd = m_conn.CreateCommand();
            cmd.CommandText = string.Format("SELECT * FROM Directory WHERE FatherID = {0}", parentID);
            try
            {
                OleDbDataReader reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    DirItem item = new DirItem();
                    item.ID = reader.GetInt32(0);
                    item.Name = reader.GetString(2);
                    ids.Add(item);
                }
                reader.Close();
            }
            catch (Exception error)
            {
                LastErrorString = error.Message;
                return null;
            }
            return ids;
        }

        public bool AddChildNode(ref XmlDocument xmlDoc, ref XmlElement parentNode)
        {
            string strID = "";
            try
            {
                strID = parentNode.Name;
                if (strID == "")
                {
                    return false;
                }
                List<DirItem> children = GetChildDirID(strID);
                if (children == null)
                {
                    return false;
                }
                if (children.Count == 0)
                {
                    return true;
                }
                foreach (DirItem item in children)
                {
                    XmlElement ele = xmlDoc.CreateElement(item.ID.ToString());
                    ele.SetAttribute("DirName", item.Name);
                    //添加ele的属性
                    parentNode.AppendChild(ele);

                    if (AddChildNode(ref xmlDoc, ref ele) == false)
                    {
                        return false;
                    }
                }
                return true;
            }
            catch (System.Exception ex)
            {
                LastErrorString = ex.Message;
                return false;
            }

        }


        public bool UpdateDirTree()
        {
            if (!GetAllDirNames())
            {
                return false;
            }
            XmlDocument doc = new XmlDocument();
            doc.PreserveWhitespace = true;
            doc.AppendChild(doc.CreateXmlDeclaration("1.0", "UTF-8", null));

            doc.AppendChild(doc.CreateSignificantWhitespace("\r\n"));
            XmlElement ele = doc.CreateElement("Root");
            doc.AppendChild(ele);
            XmlElement ele2 = doc.CreateElement("1");
            XmlElement ele3 = doc.CreateElement("2");
            ele.AppendChild(ele2);
            ele.AppendChild(ele3);
            
            if (AddChildNode(ref doc, ref ele2))
            {
                m_dirProvider.Document = doc;
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool AddDirectory(string strFatherDir, string dirName)
        {
            bool bRes = ExecuteSQLNonquery(string.Format("insert into Directory (FatherID,DirName) VALUES( {0},\'{1}\')", 
                strFatherDir, dirName));
            if ( !bRes)
            {
                return false;
            }
            UpdateDirTree();
            return true;
        }

        public bool DelDirectory( string strDirToDel )
        {
            if (strDirToDel == "1" || strDirToDel == "2")
            {
                LastErrorString = "不能删除根目录";
                return false;
            }
            //递归删除子目录
            if ( !DelAllSubDirectory(strDirToDel ) )
            {
                return false;
            }
            //删除当前目录
            if( !ExecuteSQLNonquery(string.Format("delete from Directory where ID = {0}", strDirToDel)) )
            {
                return false;
            }
            UpdateDirTree();
            return true;
        }

        private bool DelAllSubDirectory( string strSubDirToDel)
        {
            List<DirItem> children = GetChildDirID(strSubDirToDel);
            if ( children == null )
            {
                return false;
            }
            if ( children.Count == 0 )
            {
                bool bRes = ExecuteSQLNonquery(string.Format("delete from Directory where ID = {0}", strSubDirToDel));
                if (!bRes)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            foreach ( DirItem item in children )
            {
                if ( !DelDirectory(item.ID.ToString()))
                {
                    return false;
                }
            }

            return true;
        }

        public void CloseDB()
        {
            if (m_conn.State == ConnectionState.Open)
            {
                m_conn.Close();
            }
        }
       
        public bool AddFile(string strDirID, string Name, byte[] buffer)
        {
            try
            {
                OleDbCommand cmd = new OleDbCommand("insert into File(FatherID,FileName,Content,Length) values(@FatherID,@FileName,@Content,@Length)", m_conn);
                cmd.Parameters.Add("FatherID", OleDbType.BigInt).Value = Convert.ToInt32(strDirID);
                cmd.Parameters.Add("FileName", OleDbType.Char).Value = Name;
                cmd.Parameters.Add("Content", OleDbType.Binary).Value = buffer;
                cmd.Parameters.Add("Length", OleDbType.BigInt).Value = buffer.Length;
                cmd.ExecuteNonQuery();
                return true;
            }
            catch (System.Exception ex)
            {
                LastErrorString = ex.Message;
                return false;
            }
        }

       
        public byte[] ReadFile(string strFileID)
        {
            byte[] ret = null;
            try
            {
                OleDbCommand cmd = new OleDbCommand(string.Format("select Length,Content FROM File Where ID = {0}", strFileID), m_conn);
                OleDbDataReader reader = cmd.ExecuteReader(CommandBehavior.SequentialAccess);
                if ( reader.Read() )
                {
                    int len = reader.GetInt32(0);
                    ret = new byte[len];
                    reader.GetBytes(1, 0, ret, 0, len);
                }
                reader.Close();
                return ret;
            }
            catch (System.Exception ex)
            {
                LastErrorString = ex.Message;
                return null;
            }
        }
    }
}
