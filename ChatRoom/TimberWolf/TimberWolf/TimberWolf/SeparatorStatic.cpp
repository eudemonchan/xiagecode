// SeparatorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "TimberWolf.h"
#include "SeparatorStatic.h"

#include <assert.h>
#include "TimberWolfDlg.h"

// CSeparatorStatic

IMPLEMENT_DYNAMIC(CSeparatorStatic, CStatic)

CSeparatorStatic::CSeparatorStatic()
{

}

CSeparatorStatic::~CSeparatorStatic()
{
}


BEGIN_MESSAGE_MAP(CSeparatorStatic, CStatic)
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CSeparatorStatic message handlers

static BOOL g_bExpand = TRUE;  // 初始为展开
#define VER_OFFSET 10

void CSeparatorStatic::OnMouseMove(UINT nFlags, CPoint point)
{

    CStatic::OnMouseMove(nFlags, point);
}

void CSeparatorStatic::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CStatic::OnPaint() for painting messages

    RECT rcSeparator = { 0 };
    GetClientRect(&rcSeparator);

    int nVerCenter = RECT_H(rcSeparator) / 2;

    CRect rc;
    SetRect(&rc, rcSeparator.left, nVerCenter - VER_OFFSET, rcSeparator.right, nVerCenter + VER_OFFSET);

    CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
    CPen* ppenOld = dc.SelectObject(&pen);
    
    dc.Rectangle(&rc);

    COLORREF cSel = GetSysColor(COLOR_HIGHLIGHT);
    CBrush brush(cSel);

    rc.DeflateRect(1, 1);
    dc.FillRect(&rc, &brush);
    brush.DeleteObject();

    dc.SelectObject(ppenOld);
}

void CSeparatorStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
    RECT rcSeparator = { 0 };
    GetClientRect(&rcSeparator);

    int nVerCenter = RECT_H(rcSeparator) / 2;

    RECT rcBtn;
    SetRect(&rcBtn, rcSeparator.left, nVerCenter - VER_OFFSET, rcSeparator.right, nVerCenter + VER_OFFSET);

    if (PtInRect(&rcBtn, point))
    {
        GetWindowRect(&rcSeparator);

        CTimberWolfDlg* pdlg = (CTimberWolfDlg*)theApp.GetMainWnd();
        assert(pdlg != NULL);
        pdlg->ScreenToClient(&rcSeparator);

        static int s_nOffset = 0;

        RECT rcDlg = { 0 };
        pdlg->GetClientRect(&rcDlg);
        
        RECT rcDlgEx = rcDlg;

        // 若展开，则收缩
        if (g_bExpand)
        {
            //rcDlgEx.right = rcSeparator.right;
            s_nOffset = rcDlg.right - rcSeparator.right;

            //pdlg->MoveWindow(&rcDlgEx);
            ::SetWindowPos(pdlg->m_hWnd, NULL, 0, rcSeparator.right, 0, 0, SWP_NOMOVE | SWP_NOZORDER);
            g_bExpand = FALSE;
        }
        else
        {
            rcDlgEx.right += s_nOffset;
            //pdlg->MoveWindow(&rcDlgEx);
            ::SetWindowPos(pdlg->m_hWnd, NULL, 0, rcDlgEx.right, 0, 0, SWP_NOMOVE | SWP_NOZORDER);
            g_bExpand = TRUE;
        }
    }

    CStatic::OnLButtonUp(nFlags, point);
}
