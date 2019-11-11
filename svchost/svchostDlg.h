
// svchostDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ClickableEdit.h"



// CsvchostDlg �Ի���
class CsvchostDlg : public CDialog
{
// ����
public:
	CsvchostDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SVCHOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	LRESULT OnVKHandle(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetKey(WPARAM wParam, LPARAM lParam);
	LRESULT OnDbEdit(WPARAM wParam, LPARAM lParam);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL DestroyWindow();
	afx_msg void OnCancel();
	afx_msg void OnDestroy();

public:
	afx_msg LRESULT OnSystemtray(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void UpCode(CWnd * wnd, WORD);
	
private:
	CComboBox ms_Combox;
	dmsoft dm;
	long hwnd;
	
	NOTIFYICONDATA NotifyIcon;  //ϵͳ������
	TCHAR szIniPath[_MAX_PATH];
	CEdit* pEdit;
public:
	afx_msg void On32774();
	afx_msg void On32773();

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedStart1();
	afx_msg void OnBnClickedStop1();
	afx_msg void OnBnClickedButton3();
	BOOL Runing;
	BOOL work;
	BOOL EXIT;
	int delay;
	int DyDelay;
	int counter;
	int nDelay;
	int max;
	int step;
	int check;
	int keys;
	CWinThread* pThread;
	CEdit ms_Edit1;
	CClickableEdit ms_Edit2;
	CClickableEdit ms_Edit4;
	HANDLE hEvent;
	UINT KeyPress();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnChangeEdit1();
	CButton ms_loop;
	afx_msg void OnBnClickedCheck2();
	static UINT ThreadFunc1(LPVOID lpParam);
};
