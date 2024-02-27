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
	, N(2)
	, M(2)
	, dots(3)
	, noiseval(0.01)
	, lb(0)
	, rb(10)
	, tb(10)
	, bb(0)
{

}

StartSettingsDlg::~StartSettingsDlg()
{
}

void StartSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, N);
	DDX_Text(pDX, IDC_EDIT2, M);
	DDX_Text(pDX, IDC_EDIT3, dots);
	DDX_Control(pDX, IDC_INSIDE_CIRCLE_CHECK, ICCheck);
	DDX_Control(pDX, IDC_OUT_CIRCLE_CHECK, OCCheck);
	DDX_Control(pDX, IDC_TRIANGULATION_CHECK, TrCheck);
	DDX_Text(pDX, IDC_EDIT4, noiseval);
	DDX_Control(pDX, IDC_CHECK4, RGCheck);
	DDX_Text(pDX, IDC_EDIT5, lb);
	DDX_Text(pDX, IDC_EDIT6, rb);
	DDX_Text(pDX, IDC_EDIT7, tb);
	DDX_Text(pDX, IDC_EDIT8, bb);
	DDX_Control(pDX, IDC_CHECK5, RTCheck);
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}
