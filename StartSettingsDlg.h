#pragma once
#include "afxdialogex.h"


// Диалоговое окно StartSettingsDlg

class StartSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(StartSettingsDlg)

public:
	StartSettingsDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~StartSettingsDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_START_SETTINGS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int N;
	int M;
	int dots;
	bool icc = true;
	bool occ = true;
	bool trc = true;
	bool rgc = true;

	afx_msg void OnBnClickedOk();
	CButton ICCheck;
	CButton OCCheck;
	virtual BOOL OnInitDialog();
	CButton TrCheck;
	double noiseval;
	CButton RGCheck;
	double lb;
	double rb;
	double tb;
	double bb;
};
