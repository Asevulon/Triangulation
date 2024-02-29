
// MyDlg.h: файл заголовка
//

#pragma once
#include"Drawer.h"
#include"StartSettingsDlg.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRIANGULATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Drawer m_mpic;
	afx_msg void OnStartSettings();

	RecursiveTriangulation tr;
	afx_msg void OnBnClickedOk();
	CButton OkBtn;

	StartSettingsDlg dlg;

	CEdit EstCtrl;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CButton prevbtm;
	CButton nextbtn;


	HANDLE thread;
	UINT_PTR timerid;
	HANDLE InProcEvent;
};


DWORD WINAPI ThreadFunc(LPVOID p);