#pragma once


// CSeparatorStatic

class CSeparatorStatic : public CStatic
{
	DECLARE_DYNAMIC(CSeparatorStatic)

public:
	CSeparatorStatic();
	virtual ~CSeparatorStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


