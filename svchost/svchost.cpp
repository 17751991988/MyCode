
// svchost.cpp : 定义应用程序的类行为。
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


// CsvchostApp 构造

CsvchostApp::CsvchostApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CsvchostApp 对象

CsvchostApp theApp;

// 定义一个全局的dm对象
dmsoft * g_dm = NULL;
// CsvchostApp 初始化
BOOL CsvchostApp::ReleaseRes(LPCTSTR dir, LPCTSTR strFileName, WORD wResID, LPCTSTR strFileType)
{
	// 资源大小
	DWORD        dwWrite = 0;
	TCHAR tmpPath[_MAX_PATH];
	_tcscpy_s(tmpPath, _MAX_PATH, dir);
	_tcscat(tmpPath, strFileName);

	// 查找资源文件中、加载资源到内存、得到资源大小
	HRSRC        hResInfo = FindResource(NULL, MAKEINTRESOURCE(wResID), strFileType);
	if (hResInfo == NULL)
		return FALSE;
	HGLOBAL hResData = LoadResource(NULL, hResInfo);
	LPVOID pvResData = LockResource(hResData);
	DWORD        dwSize = SizeofResource(NULL, hResInfo);
	// 创建文件
	HANDLE  hFile = CreateFile(tmpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)//创建文件失败 
	{
		FreeResource(hResData);
		return FALSE;
	}

	// 写入文件
	WriteFile(hFile, pvResData, dwSize, &dwWrite, NULL);
	CloseHandle(hFile);
	FreeResource(hResData);

	return TRUE;
}
BOOL CsvchostApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	// 初始化COM(mta)
	CoInitializeEx(NULL, 0);
	GetTempPath(MAX_PATH, TempPath);
	//ReleaseRes(TempPath, _T("dm.dll"), IDR_DM1, _T("DM"));
	//ReleaseRes(TempPath, _T("DmReg.dll"), IDR_DMREG1, _T("DMREG"));
	HANDLE Event = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("MyShare"));
	ShareData data;
	// 打开共享的文件对象
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, _T("ShareMemorySZHC"));
	if (hMapFile) {
		LPVOID lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		// 将共享内存数据拷贝出来
		memcpy(&data, lpBase, sizeof(data));
		// 解除文件映射
		UnmapViewOfFile(lpBase);
		// 关闭内存映射文件对象句柄
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
	// 创建对象
	g_dm = new dmsoft;
	if (g_dm == NULL)
	{
		return FALSE;
	}

	 //注册
	long dm_ret = g_dm->Reg(_T("85119423354f46264f83cbed0c04ceb108dbde915"), _T("lj"));
	if (dm_ret != 1)
	{
		TCHAR info[256] = { 0 };

		_stprintf_s(info, _T("注册失败,错误码是:%d"), dm_ret);
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
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	delete g_dm;
	FreeLibrary(hDll);
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

