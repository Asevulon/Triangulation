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
	int RectGridConc;
	int dots;
	bool icc = false;
	bool occ = false;
	bool trc = true;
	bool rgc = true;
	bool rtc = true;

	afx_msg void OnBnClickedOk();
	CButton ICCheck;
	CButton OCCheck;
	virtual BOOL OnInitDialog();
	CButton TrCheck;
	CButton RGCheck;
	double lb;
	double rb;
	double tb;
	double bb;
	CButton RTCheck;
	CSliderCtrl trx1;
	CSliderCtrl try1;
	CSliderCtrl r1;
	CSliderCtrl s1;
	CSliderCtrl trx2;
	CSliderCtrl try2;
	CSliderCtrl r2;
	CSliderCtrl s2;

	double vtx1 = 30;
	double vty1 = 30;
	double vtx2 = 60;
	double vty2 = 60;
	double vr1 = 0;
	double vr2 = 0;
	double vs1 = 27;
	double vs2 = 27;
	int cdots;
	double Utr;
	double Ukr;
	BOOL IsCalcU;
};
