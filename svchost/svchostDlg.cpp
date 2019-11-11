
// svchostDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "svchost.h"
#include "svchostDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
WORD key_f10;
WORD key_space;
TCHAR str[64];
HWND hWnd;
HWND shWnd;
HHOOK OldHook;                  //接收由SetWindowsHookEx返回的旧的钩子
int CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
int CALLBACK SetKeyboardProc(int code, WPARAM wParam, LPARAM lParam);
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	LRESULT OnVKHandle(WPARAM wParam, LPARAM lParam);
	void UpCode(WORD);
// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedOk();
	u_short Temp;
};
void CAboutDlg::UpCode(WORD code)
{
	TCHAR tmp[64] = { 0 };
	TCHAR str[64] = { 0 };
	GetKeyNameText(MapVirtualKey((code >> 8), 0) << 16, tmp, 64);
	if (code & 0x1)
		_tcscat(str, _T("Ctrl+"));
	if (code & 0x2)
		_tcscat(str, _T("Shift+"));
	if (code & 0x4)
		_tcscat(str, _T("Alt+"));
	_tcscat(str, tmp);
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);
}
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	shWnd = m_hWnd;
	OldHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)&SetKeyboardProc, GetModuleHandle(NULL), NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
LRESULT CAboutDlg::OnVKHandle(WPARAM wParam, LPARAM lParam)
{
	Temp = lParam;
	UpCode(Temp);
	return 0;
}

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CAboutDlg::OnBnClickedOk2)
	ON_MESSAGE(WM_WKCODE, &CAboutDlg::OnVKHandle)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

int CALLBACK SetKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	u_short n = 0;
	if (code == HC_ACTION)
	{
		PKBDLLHOOKSTRUCT p;
		switch (wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			p = (PKBDLLHOOKSTRUCT)lParam;
			if (p->vkCode <32 || p->vkCode >135)
				break;

			if (((GetAsyncKeyState(VK_CONTROL) && 0x8000) ? 1 : 0))
			{
				n += 1;
			}

			if (((GetAsyncKeyState(VK_SHIFT) && 0x8000) ? 1 : 0))
			{
				n += 2;
			}

			if (((p->flags & LLKHF_ALTDOWN) != 0))
			{
				n += 4;
			}
			u_short tmp = (u_short)p->vkCode << 8;
			tmp |= n;
			::PostMessage(hWnd, WM_SETKEY, 0, (LPARAM)tmp);
			break;
		}
	}
	return CallNextHookEx(OldHook, code, wParam, lParam);  //将消息还给钩子链，不要影响别人
}
int CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	WORD* pWd;
	int ret = -1;
	if (code == HC_ACTION)
	{
		PKBDLLHOOKSTRUCT p;
		switch (wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			p = (PKBDLLHOOKSTRUCT)lParam;
			if (p->vkCode == (key_f10 >> 8))
			{
				ret = F10;
				pWd = &key_f10;
			}
			else if (p->vkCode == (key_space >> 8))
			{
				ret = SPACE;
				pWd = &key_space;
			}
			else
			{
				break;
			}
			if (((*pWd) & 0x01))
			{
				if (!((GetAsyncKeyState(VK_CONTROL) && 0x8000) ? 1 : 0))
				{
					break;
				}
			}
			if (((*pWd) & 0x02))
			{
				if (!((GetAsyncKeyState(VK_SHIFT) && 0x8000) ? 1 : 0))
				{
					break;
				}
			}
			if (((*pWd) & 0x04))
			{
				if (!((p->flags & LLKHF_ALTDOWN) != 0))
				{
					break;
				}
			}
			::PostMessage(hWnd, WM_WKCODE, 0, ret);
			break;
		}
	}
	return CallNextHookEx(OldHook, code, wParam, lParam);  //将消息还给钩子链，不要影响别人
}

LRESULT CsvchostDlg::OnDbEdit(WPARAM wParam, LPARAM lParam)
{
	if (Runing)
	{
		AfxMessageBox(_T("请先停止程序再修改!!!"));
		return 0;
	}
	if (OldHook != NULL)
	{
		UnhookWindowsHookEx(OldHook);
	}
	pEdit = (CEdit*)GetFocus();
	pEdit->SetReadOnly(FALSE);
	OldHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)&SetKeyboardProc, GetModuleHandle(NULL), NULL);
	return 0;
}

LRESULT CsvchostDlg::OnSetKey(WPARAM wParam, LPARAM lParam)
{
	UpCode(pEdit, lParam);
	UnhookWindowsHookEx(OldHook);
	OldHook = NULL;
	pEdit->SetReadOnly(TRUE);
	switch (pEdit->GetDlgCtrlID())
	{
	case IDC_EDIT4://空格
		key_space = lParam;
		break;
	case IDC_EDIT2://F10
		key_f10 = lParam;
		break;
	}
	return 0;
}

UINT CsvchostDlg::ThreadFunc1(LPVOID lpParam)
{
	CoInitializeEx(NULL, 0);
	((CsvchostDlg *)lpParam)->KeyPress();
	CoUninitialize();
	return 0;
}

UINT CsvchostDlg::KeyPress()
{
	DWORD dwRet;
	for (;;)
	{
		dwRet = WaitForSingleObject(hEvent, 1000);
		if (dwRet == WAIT_TIMEOUT)
		{
			if (EXIT) break;
			continue;
		}
		int seed = rand() % 7 - 3;
		DyDelay += (counter != 0) * step;
		if (DyDelay > max)
		{
			counter = 0;
			DyDelay = nDelay;
		}
		counter++;
		Sleep(((check == 1) ? DyDelay : delay) + seed * 100);
		dm.KeyPress(keys);
		work = FALSE;
		ResetEvent(hEvent);
	}
	return 0;
}

LRESULT CsvchostDlg::OnVKHandle(WPARAM wParam, LPARAM lParam)
{
	if (work) return 0;
	work = TRUE;
	if (lParam == SPACE)
	{
		keys = 32;
	}
	if (lParam == F10)
	{
		keys = 121;
	}
	SetEvent(hEvent);
	return 0;
}

CsvchostDlg::CsvchostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsvchostDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsvchostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ms_Edit1);
	DDX_Control(pDX, IDC_EDIT4, ms_Edit4);
	DDX_Control(pDX, IDC_EDIT2, ms_Edit2);
	DDX_Control(pDX, IDC_EDIT4, ms_Edit4);
	DDX_Control(pDX, IDC_CHECK2, ms_loop);
}

BEGIN_MESSAGE_MAP(CsvchostDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DBEDIT, &CsvchostDlg::OnDbEdit)
	ON_MESSAGE(WM_SETKEY, &CsvchostDlg::OnSetKey)
	ON_MESSAGE(WM_WKCODE, &CsvchostDlg::OnVKHandle)
	ON_MESSAGE(WM_SYSTEMTRAY, &CsvchostDlg::OnSystemtray)
	ON_COMMAND(ID_32774, &CsvchostDlg::On32774)
	ON_COMMAND(ID_32773, &CsvchostDlg::On32773)
	ON_BN_CLICKED(IDC_START1, &CsvchostDlg::OnBnClickedStart1)
	ON_BN_CLICKED(IDC_STOP1, &CsvchostDlg::OnBnClickedStop1)
	ON_BN_CLICKED(IDC_BUTTON3, &CsvchostDlg::OnBnClickedButton3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CsvchostDlg::OnEnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CsvchostDlg::OnEnKillfocusEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &CsvchostDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK2, &CsvchostDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CsvchostDlg 消息处理程序


// CsvchostDlg 消息处理程序

void CsvchostDlg::UpCode(CWnd* wnd,WORD wd)
{
	TCHAR tmp[64] = { 0 };
	TCHAR str[64] = { 0 };
	GetKeyNameText(MapVirtualKey((wd >> 8), 0) << 16, tmp, 64);
	if (wd & 0x1)
		_tcscat(str, _T("Ctrl+"));
	if (wd & 0x2)
		_tcscat(str, _T("Shift+"));
	if (wd & 0x4)
		_tcscat(str, _T("Alt+"));
	_tcscat(str, tmp);
	wnd->SetWindowText(str);
}

BOOL CsvchostDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	Runing = FALSE;
	work = FALSE;
	dm.EnableRealKeypad(1);
	dm.SetKeypadDelay(_T("normal"), 50);
	//MyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)&KeyboardProc, GetModuleHandle(NULL), NULL);
	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	srand((unsigned)time(NULL));
	//设置系统托盘
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	//NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hIcon = m_hIcon;  //上面那句也可以
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, _T(""));
	NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   //添加系统托盘

	// TODO:  在此添加额外的初始化代码
	//ShowWindow(SW_MINIMIZE);

	_tcscpy(szIniPath, szRoot);
	_tcscat(szIniPath, _T("\\config.ini"));
	hWnd = m_hWnd;
	key_f10 = GetPrivateProfileInt(_T("配置"), _T("F10"), 0, szIniPath);
	key_space = GetPrivateProfileInt(_T("配置"), _T("Space"), 0, szIniPath);
	delay = GetPrivateProfileInt(_T("配置"), _T("delay"), 3000, szIniPath);
	nDelay = GetPrivateProfileInt(_T("配置"), _T("nDelay"), 2000, szIniPath);
	max = GetPrivateProfileInt(_T("配置"), _T("max"), 30000, szIniPath);
	step = GetPrivateProfileInt(_T("配置"), _T("step"), 1000, szIniPath);
	check = GetPrivateProfileInt(_T("配置"), _T("check"), 0, szIniPath);

	if (delay < 1000) delay = 1000;
	counter = 0;
	DyDelay = delay;
	TCHAR str[64] = { 0 };
	_itot(delay,str,64);
	GetDlgItem(IDC_EDIT1)->SetWindowText(_itot(delay, str,10));
	GetDlgItem(IDC_MIN)->SetWindowText(_itot(nDelay, str, 10));
	GetDlgItem(IDC_MAX)->SetWindowText(_itot(max, str, 10));
	GetDlgItem(IDC_STEP)->SetWindowText(_itot(step, str, 10));
	GetDlgItem(IDC_MIN)->EnableWindow(check);
	GetDlgItem(IDC_MAX)->EnableWindow(check);
	GetDlgItem(IDC_STEP)->EnableWindow(check);
	GetDlgItem(IDC_STOP1)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(check);
	UpCode(GetDlgItem(IDC_EDIT4), key_space);
	UpCode(GetDlgItem(IDC_EDIT2), key_f10);
	EXIT = FALSE;
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	pThread = AfxBeginThread(ThreadFunc1, this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CsvchostDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsvchostDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsvchostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsvchostDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	if (OldHook != NULL)
	{
		UnhookWindowsHookEx(OldHook);
		OldHook = NULL;
	}
	if (hwnd != -1)
		dm.UnBindWindow();
	EXIT = TRUE;
	WaitForSingleObject(pThread->m_hThread, 10000);
}


afx_msg LRESULT CsvchostDlg::OnSystemtray(WPARAM wParam, LPARAM lParam)
{
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
	//  if(wParam!=IDR_MAINFRAME)     
	//      return    1;     
	switch (lParam)
	{
	case  WM_RBUTTONDOWN://右键起来时弹出快捷菜单
	{
							 CMenu menuexit;
							 //menu.LoadMenuW(IDR_MENU1);//加载菜单资源
							 menuexit.LoadMenu(IDR_MENU1);
							 CMenu *pPopup = menuexit.GetSubMenu(0);
							 CPoint mypoint;
							 GetCursorPos(&mypoint);
							 //ClientToScreen(&mypoint);//将客户区坐标转换为屏幕坐标
							 SetForegroundWindow();
							 PostMessage(WM_NULL, 0, 0);


							 //显示右键菜单，由视类窗口拥有。
							 pPopup->TrackPopupMenu(TPM_LEFTALIGN, mypoint.x, mypoint.y, this);
	}
		break;
	case  WM_LBUTTONDOWN://左键单击的处理     
	{
							 ModifyStyleEx(0, WS_EX_TOPMOST);   //可以改变窗口的显示风格
							 ShowWindow(SW_SHOWNORMAL);
	}
		break;
	}
	return 0;
}
void CsvchostDlg::OnCancel()  //点击X 按钮，最小化到系统托盘
{
	// TODO: 在此添加专用代码和/或调用基类
	this->ShowWindow(HIDE_WINDOW);
	//CDialogEx::OnCancel();
}


void CsvchostDlg::On32774()
{
	// TODO:  在此添加命令处理程序代码
	DestroyWindow();
}
BOOL CsvchostDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);//消除托盘图标
	return CDialog::DestroyWindow();
}


void CsvchostDlg::On32773()
{
	// TODO:  在此添加命令处理程序代码

}



void CAboutDlg::OnBnClickedOk2()
{
	// TODO:  在此添加控件通知处理程序代码
	UnhookWindowsHookEx(OldHook);
	CDialog::OnCancel();
	
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UnhookWindowsHookEx(OldHook);
	CDialog::OnOK();
	
}


void CsvchostDlg::OnBnClickedStart1()
{
	// TODO:  在此添加控件通知处理程序代码
	Runing = TRUE;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_START1)->EnableWindow(FALSE);
	GetDlgItem(IDC_MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_STEP)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP1)->EnableWindow(TRUE);
	OldHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)&KeyboardProc, GetModuleHandle(NULL), NULL);
}


void CsvchostDlg::OnBnClickedStop1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	GetDlgItem(IDC_START1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_MIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_MAX)->EnableWindow(TRUE);
	GetDlgItem(IDC_STEP)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP1)->EnableWindow(FALSE);
	if (OldHook != NULL)
	{
		UnhookWindowsHookEx(OldHook);
		OldHook = NULL;
	}
	Runing = FALSE;
}


void CsvchostDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	int tmax, tstep, ndey;
	TCHAR str[64] = { 0 };
	CString TMin,TMax,TStep;
	WritePrivateProfileString(_T("配置"), _T("delay"), _itot(delay, str, 10), szIniPath);
	WritePrivateProfileString(_T("配置"), _T("F10"), _itot(key_f10, str, 10), szIniPath);
	WritePrivateProfileString(_T("配置"), _T("Space"), _itot(key_space, str, 10), szIniPath);
	GetDlgItem(IDC_MIN)->GetWindowText(TMin);
	GetDlgItem(IDC_MAX)->GetWindowText(TMax);
	GetDlgItem(IDC_STEP)->GetWindowText(TStep);
	tstep = _ttoi(TStep.GetBuffer());
	ndey = _ttoi(TMin.GetBuffer());
	tmax = _ttoi(TMax.GetBuffer());
	if (check == 1)
		if (tmax < delay)
		{
			AfxMessageBox(_T("最大延迟小于延迟值，请重新设置！！！"));
			return;
		}
	max = tmax;
	nDelay = ndey;
	step = tstep;
	WritePrivateProfileString(_T("配置"), _T("max"), TMax.GetBuffer(), szIniPath);
	WritePrivateProfileString(_T("配置"), _T("nDelay"), TMin.GetBuffer(), szIniPath);
	WritePrivateProfileString(_T("配置"), _T("step"), TStep.GetBuffer(), szIniPath);
	AfxMessageBox(_T("保存成功..."));
}


void CsvchostDlg::OnEnKillfocusEdit4()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!Runing && OldHook != NULL)
	{
		UnhookWindowsHookEx(OldHook);
		OldHook = NULL;
	}
	((CEdit*)GetDlgItem(IDC_EDIT4))->SetReadOnly(TRUE);
}


void CsvchostDlg::OnEnKillfocusEdit2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!Runing && OldHook != NULL)
	{
		UnhookWindowsHookEx(OldHook);
		OldHook = NULL;
	}
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetReadOnly(TRUE);
}


void CsvchostDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString a;
	GetDlgItem(IDC_EDIT1)->GetWindowText(a);
	delay = _ttoi(a.GetBuffer());
	if (delay < 1000) delay = 1000;
}


void CsvchostDlg::OnBnClickedCheck2()
{
	// TODO:  在此添加控件通知处理程序代码
	check = ms_loop.GetCheck();
	GetDlgItem(IDC_MIN)->EnableWindow(check);
	GetDlgItem(IDC_MAX)->EnableWindow(check);
	GetDlgItem(IDC_STEP)->EnableWindow(check);
	WritePrivateProfileString(_T("配置"), _T("check"), _itot(check, str, 10), szIniPath);
}
