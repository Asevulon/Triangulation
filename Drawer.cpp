// Drawer.cpp: файл реализации
//

#include "pch.h"
#include "Drawer.h"


// Drawer

IMPLEMENT_DYNAMIC(Drawer, CStatic)

Drawer::Drawer()
{
	GdiplusStartupInput si;
	GdiplusStartup(&token, &si, NULL);
}

Drawer::~Drawer()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(Drawer, CStatic)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// Обработчики сообщений Drawer




void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Graphics ToWindow(lpDrawItemStruct->hDC);
	width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
	Bitmap bmp(width, height);
	Graphics gr(&bmp);
	gr.Clear(Color::White);

	if (data.empty())
	{
		ToWindow.DrawImage(&bmp, 0, 0);
		return;
	}

	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	double actWidth(right - left);
	double actHeight(top - bot);
	double actLP = actWidth * lPadding / 100.;
	double actRP = actWidth * rPadding / 100.;
	double actTP = actHeight * tPadding / 100.;
	double actBP = actHeight * bPadding / 100.;

	double actTop = top + actTP;
	double actBot = bot - actBP;
	double actLeft = left - actLP;
	double actRight = right + actRP;

	actWidth = actRight - actLeft;
	actHeight = actTop - actBot;


	double xScale = EternalScale * width / actWidth;
	double yScale = EternalScale * -height / actHeight;

	Pen BackGroundPen(Color(200, 200, 200));

	Matrix matr;
	matr.Translate(dx, dy);
	matr.Scale(xScale, yScale);
	matr.Translate(actLP - left, -actTop);
	
	PointF p1, p2;
	Pen DataPen(Color(200, 100, 0));

	PointF p3;
	for (auto& item : triangles)
	{
		p1.X = item.p1.x;
		p2.X = item.p2.x;
		p3.X = item.p3.x;

		p1.Y = item.p1.y;
		p2.Y = item.p2.y;
		p3.Y = item.p3.y;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);
		matr.TransformPoints(&p3);

		Pen TrianglePen(DetermineTriangleColor(item.est));
		gr.DrawLine(&TrianglePen, p1, p2);
		gr.DrawLine(&TrianglePen, p2, p3);
		gr.DrawLine(&TrianglePen, p3, p1);
	}
	for (auto& item : triangles)
	{
		if (!item.broken)continue;
		p1.X = item.p1.x;
		p2.X = item.p2.x;
		p3.X = item.p3.x;

		p1.Y = item.p1.y;
		p2.Y = item.p2.y;
		p3.Y = item.p3.y;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);
		matr.TransformPoints(&p3);

		Pen TrianglePen(Color::Black);
		gr.DrawLine(&TrianglePen, p1, p2);
		gr.DrawLine(&TrianglePen, p2, p3);
		gr.DrawLine(&TrianglePen, p3, p1);
	}



	if (NeedOCircle && OCircleInit)
	{
		Pen CirclePen(Color(0, 100, 200));

		p1.X = OCircleCenter.x - OCircleRadius;
		p1.Y = OCircleCenter.y - OCircleRadius;
		matr.TransformPoints(&p1);

		p2.X = OCircleCenter.x + OCircleRadius;
		p2.Y = OCircleCenter.y + OCircleRadius;
		matr.TransformPoints(&p2);

		gr.DrawEllipse(&CirclePen, p1.X, p1.Y, p2.X - p1.X, p2.Y - p1.Y);
	}
	if (NeedICircle && ICircleInit)
	{
		Pen CirclePen(Color(0, 200, 100));

		p1.X = ICircleCenter.x - ICircleRadius;
		p1.Y = ICircleCenter.y - ICircleRadius;
		matr.TransformPoints(&p1);

		p2.X = ICircleCenter.x + ICircleRadius;
		p2.Y = ICircleCenter.y + ICircleRadius;
		matr.TransformPoints(&p2);

		gr.DrawEllipse(&CirclePen, p1.X, p1.Y, p2.X - p1.X, p2.Y - p1.Y);
	}
	if (NeedOCircle || NeedICircle)
	{
		p1.X = data[0].x;
		p1.Y = data[0].y;
		matr.TransformPoints(&p1);

		int datasize = min(data.size(), 3);
		for (int i = 1; i < datasize; i++)
		{
			p2 = PointF(data[i].x, data[i].y);
			matr.TransformPoints(&p2);

			gr.DrawLine(&DataPen, p1, p2);

			p1 = p2;
		}

		p2.X = data[0].x;
		p2.Y = data[0].y;
		matr.TransformPoints(&p2);
		gr.DrawLine(&DataPen, p1, p2);
	}

	if ((!triangles.empty()) && (CurrentTriangle < triangles.size()))
	{
		Pen CirclePen(Color::DeepPink,2);

		p1.X = triangles[CurrentTriangle].CircleCenter.x - triangles[CurrentTriangle].CircleRadius;
		p1.Y = triangles[CurrentTriangle].CircleCenter.y - triangles[CurrentTriangle].CircleRadius;
		matr.TransformPoints(&p1);

		p2.X = triangles[CurrentTriangle].CircleCenter.x + triangles[CurrentTriangle].CircleRadius;
		p2.Y = triangles[CurrentTriangle].CircleCenter.y + triangles[CurrentTriangle].CircleRadius;
		matr.TransformPoints(&p2);

		gr.DrawEllipse(&CirclePen, p1.X, p1.Y, p2.X - p1.X, p2.Y - p1.Y);

		p1.X = triangles[CurrentTriangle].p1.x;
		p2.X = triangles[CurrentTriangle].p2.x;
		p3.X = triangles[CurrentTriangle].p3.x;

		p1.Y = triangles[CurrentTriangle].p1.y;
		p2.Y = triangles[CurrentTriangle].p2.y;
		p3.Y = triangles[CurrentTriangle].p3.y;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);
		matr.TransformPoints(&p3);

		gr.DrawLine(&CirclePen, p1, p2);
		gr.DrawLine(&CirclePen, p2, p3);
		gr.DrawLine(&CirclePen, p3, p1);
	}
	



	SolidBrush GridBrush(Color(200, 100, 0));
	SolidBrush CircleBrush(Color(100, 200, 0));
	SolidBrush TriangleBrush(Color(0, 100, 200));
	SolidBrush BlackBrush(Color::Black);

	for (int i = 0; i < data.size(); i++)
	{
		Brush* cBrush = &BlackBrush;
		if (data[i].type == circlep)cBrush = &CircleBrush;
		else if (data[i].type == gridp)cBrush = &GridBrush;
		else if (data[i].type == trianglep)cBrush = &TriangleBrush;

		p2 = PointF(data[i].x,data[i].y);
		matr.TransformPoints(&p2);
		gr.FillEllipse(cBrush, p2.X - pointsize, p2.Y - pointsize, 2. * pointsize, 2. * pointsize);
	}

	ToWindow.DrawImage(&bmp, 0, 0);
}


void Drawer::SetData(vector<mPoint>& points)
{
	if (points.empty())return;

	data = points;

	top = data[0].y;
	bot = data[0].y;

	left = data[0].x;
	right = data[0].x;

	for (int i = 1; i < points.size(); i++)
	{
		if (top < data[i].y)top = data[i].y;
		if (bot > data[i].y)bot = data[i].y;

		if (right < data[i].x)right = data[i].x;
		if (left > data[i].x)left = data[i].x;
	}

	double lenv = top - bot;
	double lenh = right - left;
	if (lenv >= lenh)
	{
		left -= (lenv - lenh) / 2.;
		right = left + lenv;
	}
	else
	{
		bot -= (lenh - lenv) / 2.;
		top = bot + lenh;
	}

}

void Drawer::SetPadding(double lP, double rP, double tP, double bP)
{
	lPadding = lP;
	rPadding = rP;
	tPadding = tP;
	bPadding = bP;
}
void Drawer::SetMarksLen(int len)
{
	MarksLen = len;
}
void Drawer::SetTitle(CString str)
{
	title = str;
}

void Drawer::SetTriangles(vector<rTriangle>& in)
{
	triangles = in;
	CurrentTriangle = 0;
}

void Drawer::SetCircleDrawing(bool OCState, bool ICState)
{
	NeedOCircle = OCState;
	NeedICircle = ICState;
}

void Drawer::SetOCircleParams(mPoint center, double radius)
{
	OCircleCenter = center;
	OCircleRadius = radius;
	OCircleInit = true;
}

void Drawer::SetICircleParams(mPoint center, double radius)
{
	ICircleCenter = center;
	ICircleRadius = radius;
	ICircleInit = true;
}


double Drawer::CalcStringLen(HDC hDC, CString str)
{
	double numlen = 0;
	int size = str.GetLength() + 1;
	for (int i = 0; i < size; i++)
	{
		ABCFLOAT abc;
		GetCharABCWidthsFloatW(hDC, str[i], str[i], &abc);
		numlen += abc.abcfA + abc.abcfB + abc.abcfC;
	}
	return numlen;
}

Color Drawer::DetermineTriangleColor(double& tr)
{
	double r = (1. - tr) * 255.;
	double g = tr * 255;
	return Color(r, g, 0);
}

BOOL Drawer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	double incr = 1.05;
	if (zDelta > 0)
	{
		dx -= ScalePoint.X * EternalScale * (incr - 1);
		dy -= ScalePoint.Y * EternalScale * (incr - 1);
		EternalScale *= incr;
	}
	else
	{
		dx += width * EternalScale * (incr - 1)/ 2.;
		dy += height * EternalScale * (incr - 1)/2.;
		EternalScale /= incr;
	}
	Invalidate();
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


void Drawer::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	ScalePoint.X = point.x;
	ScalePoint.Y = point.y;

	if (lbLock)
	{
		dx = point.x - lbLockPoint.X + tdx;
		dy = point.y - lbLockPoint.Y + tdy;
		Invalidate();
	}
	CStatic::OnMouseMove(nFlags, point);
}


void Drawer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	lbLock = true;
	SetCapture();
	lbLockPoint.X = point.x;
	lbLockPoint.Y = point.y;
	tdx = dx;
	tdy = dy;
	CStatic::OnLButtonDown(nFlags, point);
}


void Drawer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	lbLock = false;
	ReleaseCapture();
	CStatic::OnLButtonUp(nFlags, point);
}


void Drawer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	EternalScale = 1;
	dx = 0;
	dy = 0;
	Invalidate();
	CStatic::OnLButtonDblClk(nFlags, point);
}

void Drawer::NextTriangle()
{
	CurrentTriangle++;
	if (CurrentTriangle >= triangles.size())CurrentTriangle = 0;
	Invalidate();
}

void Drawer::PrevTriangle()
{
	CurrentTriangle--;
	if (CurrentTriangle < 0)CurrentTriangle = triangles.size() - 1;
	Invalidate();
}


void Drawer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	switch (nChar)
	{
	case VK_LEFT:PrevTriangle();
		break;
	case VK_RIGHT:NextTriangle();
		break;
	default:
		break;
	}
	Invalidate();
	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}
