
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
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_START_SETTINGS, &MyDlg::OnStartSettings)
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
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
	m_mpic.SetPadding(50, 50, 50, 50);
	
	SetWindowPos(NULL, 500, 100, 700, 770, SWP_NOMOVE);
	m_mpic.SetWindowPos(NULL, 7, 7, 670, 670, NULL);
	EstCtrl.SetWindowPos(NULL, 99, 683, 580, 20, NULL);
	OkBtn.SetWindowPos(NULL, 7, 683, 0, 0, SWP_NOSIZE);
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
	
	tr.SetNM(dlg.N, dlg.M);
	tr.SetDots(dlg.dots);
	tr.SetNoise(dlg.noiseval);
	tr.SetRectGridState(dlg.rgc);
	tr.SetBoders(dlg.lb, dlg.rb, dlg.tb, dlg.bb);
	m_mpic.SetCircleDrawing(dlg.occ, dlg.icc);
	m_mpic.Invalidate();
}


void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	tr.CreateRandomDots();
	tr.CreateCirle();
	m_mpic.SetData(tr.GetPoints());
	m_mpic.SetOCircleParams(tr.GetCircleCenter(), tr.GetCircleRadius());
	m_mpic.SetICircleParams(tr.GetICircleCenter(), tr.GetICircleRadius());

	if (dlg.trc)
	{
		tr.Triangulation();
		m_mpic.SetTriangles(tr.GetTriangles());

		CString str;
		str.Format(L"Средняя близость к равностороннему треугольнику: %.2f", tr.GetEsimate());
		EstCtrl.SetWindowTextW(str);
	}
	m_mpic.Invalidate();
}
