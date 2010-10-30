/////////////////////////////////////////////////////////////////////////////////////
// �Զ��ı�ؼ�λ�úʹ�С�ĶԻ�����
// �ļ�����lxDialog.h
// ���ߣ�StarLee(coolstarlee@sohu.com)
// �޸ģ�
//    ����ػ浼����˸
//    ��д���ڿؼ�λ�ù�ϵ�궨��
//    �Ժ� 2010-10-30
/////////////////////////////////////////////////////////////////////////////////////

#ifndef XDIALOG_H_H
#define XDIALOG_H_H

/*

// ��ֹ������ؼ��غϲ����ػ浼����˸
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
// ���ڿؼ�λ�ù�ϵ�궨��
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

	// ���ÿؼ���Ϣ
	BOOL SetControlProperty(PDLGCTLINFO lp, int nElements);
	
	// �Ƿ��ڶԻ������½���ʾ��ʾ�ɸı��С��ͼ��
	void ShowSizeIcon(BOOL bShow = TRUE);

private:
    int m_iClientWidth;     // �Ի���client����Ŀ��
    int m_iClientHeight;    // �Ի���client����ĸ߶�
    int m_iMinWidth;        // �Ի������С���
    int m_iMinHeight;       // �Ի������С�߶�

    PDLGCTLINFO m_pControlArray; // �ؼ���Ϣ����ָ��

    int     m_iControlNumber;   // ���ÿؼ���Ϣ�Ŀؼ�����
    BOOL    m_bShowSizeIcon;    // �Ƿ���ʾ��ʾ�ɸı��С��ͼ��
    CStatic m_wndSizeIcon;      // ��ͼ��ľ�̬�ؼ�
    CBitmap m_bmpSizeIcon; 
    BITMAP  m_bitmap;           // ����ͼ���bitmap

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