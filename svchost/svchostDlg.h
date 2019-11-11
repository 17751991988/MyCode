
// svchostDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "ClickableEdit.h"



// CsvchostDlg 对话框
class CsvchostDlg : public CDialog
{
// 构造
public:
	CsvchostDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SVCHOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	LRESULT OnVKHandle(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetKey(WPARAM wParam, LPARAM lParam);
	LRESULT OnDbEdit(WPARAM wParam, LPARAM lParam);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	
	NOTIFYICONDATA NotifyIcon;  //系统托盘类
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
