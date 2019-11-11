
// svchostDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "svchost.h"
#include "svchostDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
WORD key_f10;
WORD key_space;
TCHAR str[64];
HWND hWnd;
HWND shWnd;
HHOOK OldHook;                  //������SetWindowsHookEx���صľɵĹ���
int CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
int CALLBACK SetKeyboardProc(int code, WPARAM wParam, LPARAM lParam);
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	LRESULT OnVKHandle(WPARAM wParam, LPARAM lParam);
	void UpCode(WORD);
// ʵ��
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	return CallNextHookEx(OldHook, code, wParam, lParam);  //����Ϣ��������������ҪӰ�����
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
	return CallNextHookEx(OldHook, code, wParam, lParam);  //����Ϣ��������������ҪӰ�����
}

LRESULT CsvchostDlg::OnDbEdit(WPARAM wParam, LPARAM lParam)
{
	if (Runing)
	{
		AfxMessageBox(_T("����ֹͣ�������޸�!!!"));
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
	case IDC_EDIT4://�ո�
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


// CsvchostDlg ��Ϣ�������


// CsvchostDlg ��Ϣ�������

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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	Runing = FALSE;
	work = FALSE;
	dm.EnableRealKeypad(1);
	dm.SetKeypadDelay(_T("normal"), 50);
	//MyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)&KeyboardProc, GetModuleHandle(NULL), NULL);
	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	srand((unsigned)time(NULL));
	//����ϵͳ����
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	//NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hIcon = m_hIcon;  //�����Ǿ�Ҳ����
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, _T(""));
	NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   //���ϵͳ����

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//ShowWindow(SW_MINIMIZE);

	_tcscpy(szIniPath, szRoot);
	_tcscat(szIniPath, _T("\\config.ini"));
	hWnd = m_hWnd;
	key_f10 = GetPrivateProfileInt(_T("����"), _T("F10"), 0, szIniPath);
	key_space = GetPrivateProfileInt(_T("����"), _T("Space"), 0, szIniPath);
	delay = GetPrivateProfileInt(_T("����"), _T("delay"), 3000, szIniPath);
	nDelay = GetPrivateProfileInt(_T("����"), _T("nDelay"), 2000, szIniPath);
	max = GetPrivateProfileInt(_T("����"), _T("max"), 30000, szIniPath);
	step = GetPrivateProfileInt(_T("����"), _T("step"), 1000, szIniPath);
	check = GetPrivateProfileInt(_T("����"), _T("check"), 0, szIniPath);

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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CsvchostDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CsvchostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsvchostDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������
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
	//wParam���յ���ͼ���ID����lParam���յ���������Ϊ
	//  if(wParam!=IDR_MAINFRAME)     
	//      return    1;     
	switch (lParam)
	{
	case  WM_RBUTTONDOWN://�Ҽ�����ʱ������ݲ˵�
	{
							 CMenu menuexit;
							 //menu.LoadMenuW(IDR_MENU1);//���ز˵���Դ
							 menuexit.LoadMenu(IDR_MENU1);
							 CMenu *pPopup = menuexit.GetSubMenu(0);
							 CPoint mypoint;
							 GetCursorPos(&mypoint);
							 //ClientToScreen(&mypoint);//���ͻ�������ת��Ϊ��Ļ����
							 SetForegroundWindow();
							 PostMessage(WM_NULL, 0, 0);


							 //��ʾ�Ҽ��˵��������ര��ӵ�С�
							 pPopup->TrackPopupMenu(TPM_LEFTALIGN, mypoint.x, mypoint.y, this);
	}
		break;
	case  WM_LBUTTONDOWN://��������Ĵ���     
	{
							 ModifyStyleEx(0, WS_EX_TOPMOST);   //���Ըı䴰�ڵ���ʾ���
							 ShowWindow(SW_SHOWNORMAL);
	}
		break;
	}
	return 0;
}
void CsvchostDlg::OnCancel()  //���X ��ť����С����ϵͳ����
{
	// TODO: �ڴ����ר�ô����/����û���
	this->ShowWindow(HIDE_WINDOW);
	//CDialogEx::OnCancel();
}


void CsvchostDlg::On32774()
{
	// TODO:  �ڴ���������������
	DestroyWindow();
}
BOOL CsvchostDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);//��������ͼ��
	return CDialog::DestroyWindow();
}


void CsvchostDlg::On32773()
{
	// TODO:  �ڴ���������������

}



void CAboutDlg::OnBnClickedOk2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UnhookWindowsHookEx(OldHook);
	CDialog::OnCancel();
	
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UnhookWindowsHookEx(OldHook);
	CDialog::OnOK();
	
}


void CsvchostDlg::OnBnClickedStart1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int tmax, tstep, ndey;
	TCHAR str[64] = { 0 };
	CString TMin,TMax,TStep;
	WritePrivateProfileString(_T("����"), _T("delay"), _itot(delay, str, 10), szIniPath);
	WritePrivateProfileString(_T("����"), _T("F10"), _itot(key_f10, str, 10), szIniPath);
	WritePrivateProfileString(_T("����"), _T("Space"), _itot(key_space, str, 10), szIniPath);
	GetDlgItem(IDC_MIN)->GetWindowText(TMin);
	GetDlgItem(IDC_MAX)->GetWindowText(TMax);
	GetDlgItem(IDC_STEP)->GetWindowText(TStep);
	tstep = _ttoi(TStep.GetBuffer());
	ndey = _ttoi(TMin.GetBuffer());
	tmax = _ttoi(TMax.GetBuffer());
	if (check == 1)
		if (tmax < delay)
		{
			AfxMessageBox(_T("����ӳ�С���ӳ�ֵ�����������ã�����"));
			return;
		}
	max = tmax;
	nDelay = ndey;
	step = tstep;
	WritePrivateProfileString(_T("����"), _T("max"), TMax.GetBuffer(), szIniPath);
	WritePrivateProfileString(_T("����"), _T("nDelay"), TMin.GetBuffer(), szIniPath);
	WritePrivateProfileString(_T("����"), _T("step"), TStep.GetBuffer(), szIniPath);
	AfxMessageBox(_T("����ɹ�..."));
}


void CsvchostDlg::OnEnKillfocusEdit4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!Runing && OldHook != NULL)
	{
		UnhookWindowsHookEx(OldHook);
		OldHook = NULL;
	}
	((CEdit*)GetDlgItem(IDC_EDIT4))->SetReadOnly(TRUE);
}


void CsvchostDlg::OnEnKillfocusEdit2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!Runing && OldHook != NULL)
	{
		UnhookWindowsHookEx(OldHook);
		OldHook = NULL;
	}
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetReadOnly(TRUE);
}


void CsvchostDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString a;
	GetDlgItem(IDC_EDIT1)->GetWindowText(a);
	delay = _ttoi(a.GetBuffer());
	if (delay < 1000) delay = 1000;
}


void CsvchostDlg::OnBnClickedCheck2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	check = ms_loop.GetCheck();
	GetDlgItem(IDC_MIN)->EnableWindow(check);
	GetDlgItem(IDC_MAX)->EnableWindow(check);
	GetDlgItem(IDC_STEP)->EnableWindow(check);
	WritePrivateProfileString(_T("����"), _T("check"), _itot(check, str, 10), szIniPath);
}
