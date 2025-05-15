// StartSettingsDlg.cpp: файл реализации
//

#include "pch.h"
#include "Triangulation.h"
#include "afxdialogex.h"
#include "StartSettingsDlg.h"


// Диалоговое окно StartSettingsDlg

IMPLEMENT_DYNAMIC(StartSettingsDlg, CDialogEx)

StartSettingsDlg::StartSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_START_SETTINGS_DIALOG, pParent)
	, RectGridConc(15)
	, dots(20)
	, lb(0)
	, rb(1)
	, tb(1)
	, bb(0)
	, cdots(50)
	, Utr(5)
	, Ukr(0)
	, IsCalcU(TRUE)
{

}

StartSettingsDlg::~StartSettingsDlg()
{
}

void StartSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, RectGridConc);
	DDX_Text(pDX, IDC_EDIT3, dots);
	DDX_Control(pDX, IDC_INSIDE_CIRCLE_CHECK, ICCheck);
	DDX_Control(pDX, IDC_OUT_CIRCLE_CHECK, OCCheck);
	DDX_Control(pDX, IDC_TRIANGULATION_CHECK, TrCheck);
	DDX_Control(pDX, IDC_CHECK4, RGCheck);
	DDX_Text(pDX, IDC_EDIT5, lb);
	DDX_Text(pDX, IDC_EDIT6, rb);
	DDX_Text(pDX, IDC_EDIT7, tb);
	DDX_Text(pDX, IDC_EDIT8, bb);
	DDX_Control(pDX, IDC_CHECK5, RTCheck);
	DDX_Control(pDX, IDC_SLIDER1, trx1);
	DDX_Control(pDX, IDC_SLIDER2, try1);
	DDX_Control(pDX, IDC_SLIDER3, r1);
	DDX_Control(pDX, IDC_SLIDER4, s1);
	DDX_Control(pDX, IDC_SLIDER5, trx2);
	DDX_Control(pDX, IDC_SLIDER6, try2);
	DDX_Control(pDX, IDC_SLIDER7, r2);
	DDX_Control(pDX, IDC_SLIDER8, s2);
	DDX_Text(pDX, IDC_EDIT4, cdots);
	DDX_Text(pDX, IDC_EDIT9, Utr);
	DDX_Text(pDX, IDC_EDIT10, Ukr);
	DDX_Check(pDX, IDC_CHECK6, IsCalcU);
}


BEGIN_MESSAGE_MAP(StartSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &StartSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений StartSettingsDlg


void StartSettingsDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (OCCheck.GetCheck() == BST_CHECKED)occ = true;
	else occ = false;
	if (ICCheck.GetCheck() == BST_CHECKED)icc = true;
	else icc = false;
	if (TrCheck.GetCheck() == BST_CHECKED)trc = true;
	else trc = false;
	if (RGCheck.GetCheck() == BST_CHECKED)rgc = true;
	else rgc = false;
	if (RTCheck.GetCheck() == BST_CHECKED)rtc = true;
	else rtc = false;
	UpdateData();

	vtx1 = trx1.GetPos();
	vtx2 = trx2.GetPos();
	vty1 = try1.GetPos();
	vty2 = try2.GetPos();

	vr1 = r1.GetPos();
	vr2 = r2.GetPos();
	vs1 = s1.GetPos();
	vs2 = s2.GetPos();
	CDialogEx::OnOK();
}


BOOL StartSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию

	if (occ)OCCheck.SetCheck(BST_CHECKED);
	else OCCheck.SetCheck(BST_UNCHECKED);
	if (icc)ICCheck.SetCheck(BST_CHECKED);
	else ICCheck.SetCheck(BST_UNCHECKED);
	if (trc)TrCheck.SetCheck(BST_CHECKED);
	else TrCheck.SetCheck(BST_UNCHECKED);
	if (rgc)RGCheck.SetCheck(BST_CHECKED);
	else RGCheck.SetCheck(BST_UNCHECKED);
	if (rtc)RTCheck.SetCheck(BST_CHECKED);
	else RTCheck.SetCheck(BST_UNCHECKED);


	trx1.SetRange(0, 100);
	trx2.SetRange(0, 100);
	try1.SetRange(0, 100);
	try2.SetRange(0, 100);
	
	r1.SetRange(0, 120);
	r2.SetRange(0, 120);

	s1.SetRange(1, 100);
	s2.SetRange(1, 100);

	trx1.SetTic(1);
	trx2.SetTic(1);
	try1.SetTic(1);
	try2.SetTic(1);

	r1.SetTic(1);
	r2.SetTic(1);
	s1.SetTic(1);
	s2.SetTic(1);

	trx1.SetPos(vtx1);
	trx2.SetPos(vtx2);
	try1.SetPos(vty1);
	try2.SetPos(vty2);

	r1.SetPos(vr1);
	r2.SetPos(vr2);
	s1.SetPos(vs1);
	s2.SetPos(vs2);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}
