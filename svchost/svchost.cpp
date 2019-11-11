
// svchost.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "svchost.h"
#include "svchostDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct ShareData
{
	char szDm[MAX_PATH];
	char szReg[MAX_PATH];
	char szRoot[MAX_PATH];
	char DmPath[MAX_PATH];
	char RegPath[MAX_PATH];
};
// CsvchostApp

BEGIN_MESSAGE_MAP(CsvchostApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CsvchostApp ����

CsvchostApp::CsvchostApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CsvchostApp ����

CsvchostApp theApp;

// ����һ��ȫ�ֵ�dm����
dmsoft * g_dm = NULL;
// CsvchostApp ��ʼ��
BOOL CsvchostApp::ReleaseRes(LPCTSTR dir, LPCTSTR strFileName, WORD wResID, LPCTSTR strFileType)
{
	// ��Դ��С
	DWORD        dwWrite = 0;
	TCHAR tmpPath[_MAX_PATH];
	_tcscpy_s(tmpPath, _MAX_PATH, dir);
	_tcscat(tmpPath, strFileName);

	// ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С
	HRSRC        hResInfo = FindResource(NULL, MAKEINTRESOURCE(wResID), strFileType);
	if (hResInfo == NULL)
		return FALSE;
	HGLOBAL hResData = LoadResource(NULL, hResInfo);
	LPVOID pvResData = LockResource(hResData);
	DWORD        dwSize = SizeofResource(NULL, hResInfo);
	// �����ļ�
	HANDLE  hFile = CreateFile(tmpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)//�����ļ�ʧ�� 
	{
		FreeResource(hResData);
		return FALSE;
	}

	// д���ļ�
	WriteFile(hFile, pvResData, dwSize, &dwWrite, NULL);
	CloseHandle(hFile);
	FreeResource(hResData);

	return TRUE;
}
BOOL CsvchostApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	// ��ʼ��COM(mta)
	CoInitializeEx(NULL, 0);
	GetTempPath(MAX_PATH, TempPath);
	//ReleaseRes(TempPath, _T("dm.dll"), IDR_DM1, _T("DM"));
	//ReleaseRes(TempPath, _T("DmReg.dll"), IDR_DMREG1, _T("DMREG"));
	HANDLE Event = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("MyShare"));
	ShareData data;
	// �򿪹�����ļ�����
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, _T("ShareMemorySZHC"));
	if (hMapFile) {
		LPVOID lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		// �������ڴ����ݿ�������
		memcpy(&data, lpBase, sizeof(data));
		// ����ļ�ӳ��
		UnmapViewOfFile(lpBase);
		// �ر��ڴ�ӳ���ļ�������
		CloseHandle(hMapFile);
	}
	SetEvent(Event);
	CloseHandle(Event);
	ZeroMemory(szRoot, sizeof(szRoot));
	strcpy(szRoot, data.szRoot);
	typedef int(__stdcall *lpSetDllPathA)(const char *, int);
	HINSTANCE hDll = LoadLibraryA(data.RegPath);
	lpSetDllPathA SetDllPathA;
	if (hDll != NULL)
	{
		SetDllPathA = (lpSetDllPathA)GetProcAddress(hDll, "SetDllPathA");
		if (SetDllPathA != NULL)
		{
			SetDllPathA(data.DmPath, 0);
		}
	}
	// ��������
	g_dm = new dmsoft;
	if (g_dm == NULL)
	{
		return FALSE;
	}

	 //ע��
	long dm_ret = g_dm->Reg(_T("85119423354f46264f83cbed0c04ceb108dbde915"), _T("lj"));
	if (dm_ret != 1)
	{
		TCHAR info[256] = { 0 };

		_stprintf_s(info, _T("ע��ʧ��,��������:%d"), dm_ret);
		::MessageBox(NULL, info, _T(""), MB_OK);
		delete g_dm;
		return FALSE;
	}
	char hmDm[MAX_PATH] = { 0 };
	char hmReg[MAX_PATH] = { 0 };
	sprintf(hmDm, "hm %s 1", data.szDm);
	sprintf(hmReg, "hm %s 1", data.szReg);
	dm_ret = g_dm->DmGuard(1, hmReg);
	dm_ret = g_dm->DmGuard(1, hmDm);
	if (dm_ret != 1)
		return FALSE;
	CsvchostDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	delete g_dm;
	FreeLibrary(hDll);
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

