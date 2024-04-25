
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Triangulation.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRIANGULATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_PICTURE, m_mpic);
	DDX_Control(pDX, IDOK, OkBtn);
	DDX_Control(pDX, IDC_EDIT1, EstCtrl);
	DDX_Control(pDX, IDC_BUTTON1, prevbtm);
	DDX_Control(pDX, IDC_BUTTON2, nextbtn);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_START_SETTINGS, &MyDlg::OnStartSettings)
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MyDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	m_mpic.SetPadding(5, 5, 5, 5);
	
	SetWindowPos(NULL, 500, 100, 700, 800, SWP_NOMOVE);
	m_mpic.SetWindowPos(NULL, 7, 7, 670, 670, NULL);
	EstCtrl.SetWindowPos(NULL, 99, 683, 580, 20, NULL);
	OkBtn.SetWindowPos(NULL, 7, 683, 0, 0, SWP_NOSIZE);
	prevbtm.SetWindowPos(NULL, 7, 713, 0, 0, SWP_NOSIZE);
	nextbtn.SetWindowPos(NULL, 99, 713, 0, 0, SWP_NOSIZE);

	InProcEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	SetEvent(InProcEvent);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnStartSettings()
{
	// TODO: добавьте свой код обработчика команд
	if (dlg.DoModal() != IDOK)return;
	
	tr.SetCircleDots(dlg.cdots);
	tr.SetRectGridConc(dlg.RectGridConc);
	tr.SetBorders(dlg.lb, dlg.rb, dlg.tb, dlg.bb);
	tr.SetTriangleDots(dlg.dots);
	tr.SetTrianglesParams(mPoint(dlg.vtx1, dlg.vty1), mPoint(dlg.vtx2, dlg.vty2), dlg.vr1, dlg.vr2, dlg.vs1, dlg.vs2);
	galerkin.SetBorderFi(dlg.Ukr);
	galerkin.SetTriabgleFi(dlg.Utr);
	galerkin.SetRange(dlg.lb, dlg.rb, dlg.bb, dlg.tb);
	m_mpic.SetCircleDrawing(dlg.occ, dlg.icc);
	m_mpic.Invalidate();
}

void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	tr.stop = true;
	EnterCriticalSection(&tr.cs);
	TerminateThread(thread, 0);
	LeaveCriticalSection(&tr.cs);
	timerid = SetTimer(123, 100, NULL);
	
	CloseHandle(thread);

	thread = CreateThread(NULL, NULL, ThreadFunc, this, NULL, NULL);
}


void MyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	m_mpic.SetData(tr.GetPoints());
	m_mpic.SetTriangles(tr.GetTriangles());
	m_mpic.Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}


void MyDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	switch (nChar)
	{
	case VK_LEFT:m_mpic.PrevTriangle();
		break;
	case VK_RIGHT:m_mpic.NextTriangle();
		break;
	default:
		break;
	}
	m_mpic.Invalidate();
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void MyDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_mpic.PrevTriangle();
}


void MyDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_mpic.NextTriangle();
}

DWORD __stdcall ThreadFunc(LPVOID p)
{
	MyDlg* dlg = (MyDlg*)p;

	dlg->m_mpic.drawTriangulation = true;
	dlg->tr.MakeStartGrid();
	dlg->m_mpic.SetOCircleParams(dlg->tr.GetOCircleCenter(), dlg->tr.GetOCircleRadius());
	dlg->m_mpic.SetICircleParams(dlg->tr.GetICircleCenter(), dlg->tr.GetICircleRadius());
	if (dlg->dlg.trc)
	{
		if (dlg->dlg.rtc)dlg->tr.rTriangulate();
		else dlg->tr.Triangulate();

		if (dlg->dlg.rgc)dlg->tr.RemoveOS();

		CString str;
		str.Format(L"Средняя близость к равностороннему треугольнику: %.2f, расчет потенциала...", dlg->tr.Estimate());
		dlg->EstCtrl.SetWindowTextW(str);
	}
	
	

	dlg->m_mpic.SetData(dlg->tr.GetPoints());
	dlg->m_mpic.SetTriangles(dlg->tr.GetTriangles());
	dlg->m_mpic.Invalidate();
	dlg->KillTimer(dlg->timerid);

	if (dlg->dlg.IsCalcU)
	{
		dlg->galerkin.SetNodes(dlg->tr.GetPoints());
		dlg->galerkin.SetTriangles(dlg->tr.GetTriangles());
		dlg->galerkin.test();

		dlg->m_mpic.SetTrianglesData(dlg->galerkin.GetTriangles());
		dlg->m_mpic.SetIsolines(dlg->galerkin.GetIsolines());
		dlg->m_mpic.SetPowerlines(dlg->galerkin.GetPowerlines());
		dlg->m_mpic.drawTriangulation = false;
		dlg->m_mpic.Invalidate();
	}

	if (dlg->dlg.trc)
	{
		CString str;
		str.Format(L"Средняя близость к равностороннему треугольнику: %.2f, потенциал расчитан.", dlg->tr.Estimate());
		dlg->EstCtrl.SetWindowTextW(str);
	}

	return 0;
}
