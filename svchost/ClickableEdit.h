#pragma once
#include "afxwin.h"
class CClickableEdit :
	public CEdit
{
public:
	CClickableEdit();
	~CClickableEdit();
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

