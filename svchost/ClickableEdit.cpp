#include "stdafx.h"
#include "ClickableEdit.h"


CClickableEdit::CClickableEdit()
{
}


CClickableEdit::~CClickableEdit()
{
}
BEGIN_MESSAGE_MAP(CClickableEdit, CEdit)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
END_MESSAGE_MAP()


void CClickableEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::PostMessage(hWnd, WM_DBEDIT, 0, 1);
}
void CClickableEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}
