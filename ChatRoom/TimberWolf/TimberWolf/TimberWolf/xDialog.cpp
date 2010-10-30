//////////////////////////////////////////////////////////////////////
// �Զ��ı�ؼ�λ�úʹ�С�ĶԻ�����
// �ļ�����lxDialog.cpp
// ���ߣ�StarLee(coolstarlee@sohu.com)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xDialog.h"

// ��ʾ�ɸı��С��ͼ��ID
#ifndef OBM_SIZE
#define OBM_SIZE 32766
#endif

ClxDialog::ClxDialog(UINT nID, CWnd* pParent /*=NULL*/)
    : CDialog(nID, pParent)
    , m_iClientWidth(0)
    , m_iClientHeight(0)
    , m_iMinWidth(0)
    , m_iMinHeight(0)
    , m_pControlArray(NULL)
    , m_iControlNumber(0)
    , m_bShowSizeIcon(TRUE)
{
	
}

BEGIN_MESSAGE_MAP(ClxDialog, CDialog)
    ON_WM_SIZE()
    ON_WM_SIZING()
    ON_WM_ERASEBKGND()
    ON_WM_GETMINMAXINFO()
    ON_WM_NCHITTEST()
END_MESSAGE_MAP()


POINT g_ptMinSize = { 0 };
BOOL g_bDone = FALSE;
CRect g_gripRect;

BOOL ClxDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���öԻ���Ϊ�ɱ��С��
	ModifyStyle(0, WS_SIZEBOX);

	// �ԶԻ���ĳ�ʼ��С��Ϊ�Ի���Ŀ�Ⱥ͸߶ȵ���Сֵ
	CRect rectDlg;
	GetWindowRect(rectDlg);
// 	m_iMinWidth = rectDlg.Width();
// 	m_iMinHeight = rectDlg.Height();

    g_ptMinSize.x = rectDlg.Width();
    g_ptMinSize.y = rectDlg.Height();

    g_bDone = TRUE;

	// �õ��Ի��� client ����Ĵ�С 
	CRect rectClient;
	GetClientRect(rectClient);
	m_iClientWidth = rectClient.Width();
	m_iClientHeight = rectClient.Height();

	// Load ͼ��
	m_bmpSizeIcon.LoadOEMBitmap(OBM_SIZE);
	m_bmpSizeIcon.GetBitmap(&m_bitmap);

	// ������ʾͼ��ľ�̬�ؼ������ڶԻ������½�
	m_wndSizeIcon.Create(NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, CRect(0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight), this, 0);
	m_wndSizeIcon.SetBitmap(m_bmpSizeIcon);
	
    g_gripRect.left = m_iClientWidth - m_bitmap.bmWidth;
    g_gripRect.top = m_iClientHeight - m_bitmap.bmHeight;
    g_gripRect.right = g_gripRect.left + m_bitmap.bmWidth;
    g_gripRect.bottom = g_gripRect.top +  m_bitmap.bmHeight;

//    m_wndSizeIcon.MoveWindow(m_iClientWidth - m_bitmap.bmWidth, m_iClientHeight - m_bitmap.bmHeight, m_bitmap.bmWidth, m_bitmap.bmHeight);
    m_wndSizeIcon.MoveWindow(&g_gripRect);
	
    

    // ��ʾͼ��
	m_wndSizeIcon.ShowWindow(m_bShowSizeIcon);

	return TRUE;
}

void ClxDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

//     if (nType != SIZE_MAXHIDE && nType != SIZE_MAXSHOW)
//     {
//         ArrangeLayout();
//         if (nType == SIZE_RESTORED)
//             GetWindowRect(&m_rcWnd);
//     }

    if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
        return;

	// �Ի����Ⱥ͸߶ȵ����� 
	int iIncrementX = cx - m_iClientWidth;
	int iIncrementY = cy - m_iClientHeight;

	// ��С��ʱ����Ϊ0
	if (nType == SIZE_MINIMIZED)
	{
		iIncrementX = iIncrementY = 0;
	}
	
    HDWP hdwp = BeginDeferWindowPos(m_iControlNumber);

	for (int i = 0; i < m_iControlNumber; i++)
	{
		CWnd *pWndCtrl = NULL;
		int iId = m_pControlArray[i].iId;
		int iFlag = m_pControlArray[i].iFlag;
		int iPercent = m_pControlArray[i].iPercent;

		// ��Чֵ
		if ((iPercent < 0) || (iPercent > 100))
			continue;

		// �õ��ؼ�ָ��
		pWndCtrl = GetDlgItem(iId);
		if ((NULL != pWndCtrl) && IsWindow(pWndCtrl->GetSafeHwnd()))
		{
			CRect rectCtrl;
			pWndCtrl->GetWindowRect(rectCtrl);
			ScreenToClient(rectCtrl);

			int iLeft = rectCtrl.left;
			int iTop = rectCtrl.top;
			int iWidth = rectCtrl.Width();
			int iHeight = rectCtrl.Height();

            switch (iFlag)
            {
                // ���������ΪĬ�ϣ����봦��
            case RLT_L:
            case RLT_T:
            case RLT_LT:
                break;
    
                // �����һ�������ͬ����
            case RLT_R:
            case RLT_TR:
                iLeft += (iIncrementX * iPercent / 100);
                break;

            case RLT_B:
            case RLT_LB:
                iTop += (iIncrementY * iPercent / 100);
                break;

            case RLT_LR:
            case RLT_LTR:
                iWidth += (iIncrementX * iPercent / 100);
                break;

            case RLT_TB:
            case RLT_LTB:
                iHeight += (iIncrementY * iPercent / 100);
                break;

            case RLT_RB:
                iLeft += (iIncrementX * iPercent / 100);
                //iHeight += (iIncrementY * iPercent / 100);
                iTop += (iIncrementY * iPercent / 100);
                break;

            case RLT_LRB:
                iWidth += (iIncrementX * iPercent / 100);
                iTop += (iIncrementY * iPercent / 100);
                break;

            case RLT_TRB:
                iLeft += (iIncrementX * iPercent / 100);
                iHeight += (iIncrementY * iPercent / 100);
                break;

            case RLT_LTRB:
                iWidth += (iIncrementX * iPercent / 100);
                iHeight += (iIncrementY * iPercent / 100);
                break;

            default:
                ;
            }

            CRect rcNew(CPoint(iLeft, iTop), CSize(iWidth, iHeight));

            if (!rcNew.EqualRect(&rectCtrl))
            {
                DeferWindowPos(hdwp, pWndCtrl->m_hWnd, NULL, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(), SWP_NOZORDER);

//                 if (!pl->bg_safe)
//                 {
//                     pWndCtrl->MoveWindow(0x4000,0x4000,0,0,FALSE);
//                     InvalidateRect(&rectCtrl);
//                 }
            }
		}
	}

	// ��ͼ���ƶ����Ի������½�
	if (IsWindow(m_wndSizeIcon.GetSafeHwnd()))
		m_wndSizeIcon.MoveWindow(cx - m_bitmap.bmWidth, cy - m_bitmap.bmHeight, m_bitmap.bmWidth, m_bitmap.bmHeight);
	
    EndDeferWindowPos(hdwp);

	// ��¼�Ի��� client ����Ĵ�С
	if (nType != SIZE_MINIMIZED)
	{
		m_iClientWidth = cx;
		m_iClientHeight = cy;
	}
}
void ClxDialog::OnSizing(UINT nSide, LPRECT lpRect)
{
	CDialog::OnSizing(nSide, lpRect);

    /*
	// �Ի�����С�ڳ�ʼ��С
	int iWidth = lpRect->right - lpRect->left;
	int iHeight = lpRect->bottom - lpRect->top;

	if (iWidth <= m_iMinWidth)
		lpRect->right = lpRect->left + m_iMinWidth;

	if(iHeight <= m_iMinHeight)
		lpRect->bottom = lpRect->top + m_iMinHeight;
        */
}
BOOL ClxDialog::SetControlProperty(PDLGCTLINFO lp, int nElements)
{
	// ���ÿؼ�������Ϣ
	if (NULL == lp)
		return FALSE;
	
	if (nElements <= 0)
		return FALSE;
	
	m_pControlArray = lp;
	m_iControlNumber = nElements;
	return TRUE;
}
void ClxDialog::ShowSizeIcon(BOOL bShow /*=NULL*/)
{
	m_bShowSizeIcon = bShow;
}

BOOL ClxDialog::OnEraseBkgnd(CDC* pDC)
{
     return CDialog::OnEraseBkgnd(pDC);
}
void ClxDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // TODO: Add your message handler code here and/or call default

    //CDialog::OnGetMinMaxInfo(lpMMI);
    if (g_bDone)
        lpMMI->ptMinTrackSize = g_ptMinSize;

   // CDialog::OnGetMinMaxInfo(lpMMI);
}

LRESULT ClxDialog::OnNcHitTest(CPoint point)
{
    CPoint pt = point;
    ScreenToClient(&pt);

    if (g_gripRect.PtInRect(pt))
        return HTBOTTOMRIGHT;

    return CDialog::OnNcHitTest(point);
}
