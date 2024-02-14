
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

	mTriangulation tr;
	afx_msg void OnBnClickedOk();
	CButton OkBtn;

	StartSettingsDlg dlg;

	CEdit EstCtrl;
};
