
// svchost.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CsvchostApp: 
// �йش����ʵ�֣������ svchost.cpp
//

class CsvchostApp : public CWinApp
{
public:
	CsvchostApp();
	BOOL ReleaseRes(LPCTSTR dir, LPCTSTR strFileName, WORD wResID, LPCTSTR strFileType);
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	TCHAR TempPath[_MAX_PATH];
};

extern CsvchostApp theApp;