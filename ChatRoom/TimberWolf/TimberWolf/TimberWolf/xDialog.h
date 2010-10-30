/////////////////////////////////////////////////////////////////////////////////////
// 自动改变控件位置和大小的对话框类
// 文件名：lxDialog.h
// 作者：StarLee(coolstarlee@sohu.com)
// 修改：
//    解决重绘导致闪烁
//    重写窗口控件位置关系宏定义
//    迷糊 2010-10-30
/////////////////////////////////////////////////////////////////////////////////////

#ifndef XDIALOG_H_H
#define XDIALOG_H_H

/*

// 防止背景与控件重合部分重绘导致闪烁
#define ERASE_BKGND_BEGIN   \
    CRect bgRect;           \
    GetClientRect(&bgRect); \
    CRgn bgRgn;             \
    bgRgn.CreateRectRgnIndirect(bgRect);

#define ADD_NOERASE_CONTROL(IDC)                                \
{                                                               \
    CRect controlRect;                                          \
    GetDlgItem(IDC)->GetWindowRect(&controlRect);               \
    ScreenToClient(&controlRect);                               \
    CRgn controlRgn;                                            \
    controlRgn.CreateRectRgnIndirect(&controlRect);             \
    if (bgRgn.CombineRgn(&bgRgn, &controlRgn, RGN_XOR) == ERROR) \
    {                                                           \
        OutputDebugStringW(L"CombineRgn() failed.\n");          \
        return FALSE;                                           \
    }                                                           \
}

#define ADD_NOERASE_RECT(noEraseRect)                           \
{                                                               \
    CRgn noEraseRgn;                                            \
    noEraseRgn.CreateRectRgnIndirect(noEraseRect);              \
    if (bgRgn.CombineRgn(&bgRgn, &noEraseRgn, RGN_XOR) == ERROR) \
        return FALSE;                                           \
}

#define ERASE_BKGND_END(pDC, clBrushColor)  \
    CBrush brush;                           \
    brush.CreateSolidBrush(clBrushColor);   \
    (pDC)->FillRgn(&bgRgn, &brush);         \
    brush.DeleteObject();                   \
*/
// 窗口控件位置关系宏定义
#define RLT_L 0x01
#define RLT_T 0x02
#define RLT_R 0x04
#define RLT_B 0x08

#define RLT_LT (RLT_L | RLT_T)
#define RLT_LR (RLT_L | RLT_R)
#define RLT_LB (RLT_L | RLT_B)

#define RLT_TR (RLT_T | RLT_R)
#define RLT_TB (RLT_T | RLT_B)

#define RLT_RB (RLT_R | RLT_B)

#define RLT_LTR (RLT_L | RLT_T | RLT_R)
#define RLT_LTB (RLT_L | RLT_T | RLT_B)
#define RLT_LRB (RLT_L | RLT_R | RLT_B)
#define RLT_TRB (RLT_T | RLT_R | RLT_B)

#define RLT_LTRB (RLT_L | RLT_T | RLT_R | RLT_B)


class ClxDialog : public CDialog
{
public:
	ClxDialog(UINT nID, CWnd* pParent = NULL);
	
	typedef struct _dlgControlTag 
	{
		int iId;
		int iFlag;
		int iPercent;
	} DLGCTLINFO, *PDLGCTLINFO;
	
    enum
    {
        MOVEX     = 0x01,
        MOVEY     = 0x02,
        MOVEXY    = 0x04,
        ELASTICX  = 0x08,
        ELASTICY  = 0x16,
        ELASTICXY = 0x32
    };  

	// 设置控件信息
	BOOL SetControlProperty(PDLGCTLINFO lp, int nElements);
	
	// 是否在对话框右下角显示表示可改变大小的图标
	void ShowSizeIcon(BOOL bShow = TRUE);

private:
    int m_iClientWidth;     // 对话框client区域的宽度
    int m_iClientHeight;    // 对话框client区域的高度
    int m_iMinWidth;        // 对话框的最小宽度
    int m_iMinHeight;       // 对话框的最小高度

    PDLGCTLINFO m_pControlArray; // 控件信息数组指针

    int     m_iControlNumber;   // 设置控件信息的控件个数
    BOOL    m_bShowSizeIcon;    // 是否显示表示可改变大小的图标
    CStatic m_wndSizeIcon;      // 放图标的静态控件
    CBitmap m_bmpSizeIcon; 
    BITMAP  m_bitmap;           // 保存图标的bitmap

    DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
public:
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
public:
    afx_msg LRESULT OnNcHitTest(CPoint point);
};


#endif