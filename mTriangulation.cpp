#include "pch.h"
#include "mTriangulation.h"

#define pow2(val) (val) * (val)

double mTriangulation::CalcCircleRadius(mpoint& center, mpoint& p)
{
	return sqrt(pow2(center.second - p.second) + pow2(center.first - p.first));
}
//переписать
mpoint mTriangulation::CalcCircleCenter(mpoint& p1, mpoint& p2, mpoint& p3)
{
	mpoint res;

	double x1(0), x2(0), x3(0);
	double y1(0), y2(0), y3(0);

	ParsePoint(x1, y1, p1);
	ParsePoint(x2, y2, p2);
	ParsePoint(x3, y3, p3);

	double s1 = (pow2(x2) - pow2(x1) + pow2(y2) - pow2(y1)) * (y3 - y1);
	double s2 = (pow2(x3) - pow2(x1) + pow2(y3) - pow2(y1)) * (y2 - y1);
	double s3 = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);

	res.second = 0.5 * (s1 - s2) / s3;

	s1 = (pow2(x3) - pow2(x1) + pow2(y3) - pow2(y1)) * (x2 - x1);
	s2 = (pow2(x2) - pow2(x1) + pow2(y2) - pow2(y1)) * (x3 - x1);

	res.first = 0.5 * (s1 - s2) / s3;

	return res;
}

double mTriangulation::CalcICircleRadius(mpoint p1, mpoint p2, mpoint p3)
{
	Vector2d v12(CalcVector(p1, p2));
	Vector2d v13(CalcVector(p1, p3));
	Vector2d v23(CalcVector(p2, p3));

	double a(0), b(0), c(0);
	a = v12.abs();
	b = v13.abs();
	c = v23.abs();

	double p = (a + b + c) / 2.;

	return sqrt((p - a) * (p - b) * (p - c) / p);
}

mpoint mTriangulation::CalcICircleCenter(mpoint p1, mpoint p2, mpoint p3)
{
	mpoint res;
	
	Vector2d v12(CalcVector(p1, p2));
	Vector2d v13(CalcVector(p1, p3));
	Vector2d v23(CalcVector(p2, p3));
	
	double ab = v12.abs();
	double ac = v13.abs();
	double bc = v23.abs();

	double x1(0), x2(0), x3(0);
	double y1(0), y2(0), y3(0);

	ParsePoint(x1, y1, p1);
	ParsePoint(x2, y2, p2);
	ParsePoint(x3, y3, p3);

	double lambda = ((x2 - x1) / ab + (x3 - x1) / ac) / ((y2 - y1) / ab + (y3 - y1) / ac);
	double mu = ((x1 - x2) / ab + (x3 - x2) / bc) / ((y1 - y2) / ab + (y3 - y2) / bc);

	res.second = (-mu * x1 + mu * lambda * (y1 - y2) + lambda * x2) / (lambda - mu);
	res.first = (x2 - mu * y2 - x1 + lambda * y1) / (lambda - mu);
	return res;
}

mpoint mTriangulation::CalcVector(mpoint p1, mpoint p2)
{
	mpoint res;
	res.first = p2.first - p1.first;
	res.second = p2.second - p1.second;
	return res;
}

double mTriangulation::abs(mpoint p)
{
	return sqrt(pow2(p.first) + pow2(p.second));
}

void mTriangulation::NormalizeVector(mpoint& p, double newabs)
{
	double curabs = abs(p);
	p.first *= newabs / curabs;
	p.second *= newabs / curabs;
}

void mTriangulation::ParsePoint(double& x, double& y, mpoint from)
{
	x = from.second;
	y = from.first;
}

void mTriangulation::ErrOk()
{
	Error = L"Ok";
}

bool mTriangulation::CheckIfPointInCircle(mpoint& c, mpoint& p, double& r)
{
	return false;
}

void mTriangulation::SetNM(int n, int m)
{
	N = n;
	M = m;

	nodes.resize(N);
	for (auto& item : nodes)item.resize(M);
}

void mTriangulation::SetDots(int Dots)
{
	dots = Dots;
	if (dots < N * M)Error = L"Число точек меньше размеров поля";
}

vector<vector<mpoint>> mTriangulation::GetNodes()
{
	return nodes;
}

vector<mpoint> mTriangulation::GetPoints()
{
	vector<mpoint> res;
	res.resize(min(dots, M * N));

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (i * M + j >= dots)break;
			res[i * M + j] = nodes[i][j];
		}
	}

	return res;
}

CString mTriangulation::GetLastError()
{
	return Error;
}

bool mTriangulation::CreateRandomDots()
{
	srand(time(NULL));
	if ((!RectGrid) && (!BordersInit))
	{
		Error = L"Неинициализированы границы произвольных точек";
		return false;
	}
	NodesInit = true;

	if (RectGrid)
	{
		for (int i = 0; i < dots; i++)
		{
			if (i / M >= N)
			{
				Error = L"Число точек меньше размеров поля";
				return false;
			}
			double x = i % M + rand(-noiseval, noiseval);
			double y = i / M + rand(-noiseval, noiseval);
			nodes[i / M][i % M] = mpoint(y, x);
		}
	}
	else
	{
		for (int i = 0; i < dots; i++)
		{
			if (i / M >= N)
			{
				Error = L"Число точек меньше размеров поля";
				return false;
			}
			double x = rand(LBorder, RBorder);
			double y = rand(BBorder, TBorder);
			nodes[i / M][i % M] = mpoint(y, x);
		}
	}
	return true;
}

bool mTriangulation::CreateCirle()
{
	if (!NodesInit)
	{
		Error = L"Узлы не инициализированы";
		return false;
	}
	if (dots < 3)
	{
		Error = L"dots < 3";
		return false;
	}
	mpoint p1, p2, p3;
	p1 = nodes[0][0];
	p2 = nodes[0][1];
	p3 = (M < 3) ? nodes[1][0] : nodes[0][2];
	CircleCenter = CalcCircleCenter(p1, p2, p3);
	CircleRadius = CalcCircleRadius(CircleCenter, nodes[0][0]);
	
	ICircleCenter = CalcICircleCenter(p1, p2, p3);
	ICircleRadius = CalcICircleRadius(p1, p2, p3);

	ErrOk();
	return true;
}

mpoint mTriangulation::GetCircleCenter()
{
	return CircleCenter;
}

double mTriangulation::GetCircleRadius()
{
	return CircleRadius;
}

double mTriangulation::Estimate()
{
	double res = 0;
	for (auto& item : triangles)
	{
		mpoint oc = CalcCircleCenter(item.p1, item.p2, item.p3);
		mpoint ic = CalcICircleCenter(item.p1, item.p2, item.p3);
		double d = sqrt(pow2(oc.first - ic.first) + pow2(oc.second - ic.second));
		double r = CalcCircleRadius(oc, item.p1);
		res += d / r;
	}
	return 1 - res / triangles.size();
}

mpoint mTriangulation::GetICircleCenter()
{
	return ICircleCenter;
}

double mTriangulation::GetICircleRadius()
{
	return ICircleRadius;
}

bool mTriangulation::Triangulation()
{
	if (!NodesInit)
	{
		Error = L"Не инициализированы узлы";
		return false;
	}

	triangles.clear();

	mpoint c;
	double r = 0;

	int CicleRange = min(dots, M * N);

	for (int i = 0; i < CicleRange - 2; i++)
	{
		for (int j = i + 1; j < CicleRange - 1; j++)
		{
			for (int k = j + 1; k < CicleRange; k++)
			{
				mpoint& p1 = nodes[i / M][i % M];
				mpoint& p2 = nodes[j / M][j % M];
				mpoint& p3 = nodes[k / M][k % M];

				c = CalcCircleCenter(p1, p2, p3);
				r = CalcCircleRadius(c, p1);
				r *= r;
				
				bool InCircle = false;
				for (int m = 0; m < CicleRange; m++)
				{
					if ((m == i) || (m == j) || (m == k))continue;
					mpoint& tp = nodes[m / M][m % M];
					double testval = pow2(tp.second - c.second) + pow2(tp.first - c.first);
					if (testval <= r)
					{
						InCircle = true;
						break;
					}
				}

				if (!InCircle)
				{
						triangles.push_back(Triangle(p1, p2, p3));
				}
			}
		}
	}

	estimate = Estimate();
	ErrOk();
	return true;
}

vector<Triangle> mTriangulation::GetTriangles()
{
	return triangles;
}

void mTriangulation::SetNoise(double val)
{
	noiseval = val;
}

void mTriangulation::SetBoders(double l, double r, double t, double b)
{
	LBorder = l;
	RBorder = r;
	TBorder = t;
	BBorder = b;
	BordersInit = true;
}

void mTriangulation::SetRectGridState(bool b)
{
	RectGrid = b;
}

double mTriangulation::GetEsimate()
{
	return estimate;
}


double rand(double left, double right)
{
	return left + (right - left) * double(rand()) / double(RAND_MAX);
}

Vector2d::Vector2d()
{
}

Vector2d::Vector2d(double x, double y) :x(x), y(y)
{
}

Vector2d::Vector2d(mpoint p)
{
	x = p.second;
	y = p.first;
}

Vector2d Vector2d::operator+(Vector2d& r)
{
	// TODO: вставьте здесь оператор return
	x += r.x;
	y += r.y;
	return *this;
}

Vector2d Vector2d::operator-(Vector2d& r)
{
	// TODO: вставьте здесь оператор return
	x -= r.x;
	y -= r.y;
	return *this;
}

Vector2d& Vector2d::operator/=(double& r)
{
	// TODO: вставьте здесь оператор return
	x /= r;
	y /= r;
	return *this;
}

Vector2d& Vector2d::operator/=(int& r)
{
	// TODO: вставьте здесь оператор return
	x /= r;
	y /= r;
	return *this;
}

Vector2d Vector2d::operator*(double& r)
{
	return Vector2d(x * r, y * r);
}

Vector2d Vector2d::operator*(int& r)
{
	return Vector2d(x * r, y * r);
}

double Vector2d::abs()
{
	return sqrt(pow2(x) + pow2(y));
}

double Vector2d::GetK()
{
	return y / x;
}

Vector2d& Vector2d::normalize(double newabs)
{
	double cabs = abs();
	x *= newabs / cabs;
	y *= newabs / cabs;
	return *this;
}

Triangle::Triangle()
{
}

Triangle::Triangle(mpoint p1, mpoint p2, mpoint p3) :p1(p1), p2(p2), p3(p3)
{
}

mPoint::mPoint()
{
}

mPoint::mPoint(double x, double y, bool bOS) :x(x), y(y), IsOS(bOS)
{
}

mPoint mPoint::operator+(mPoint& right)
{
	return mPoint(x + right.x, y + right.y);
}

mPoint& mPoint::operator+=(Vector2d& r)
{
	// TODO: вставьте здесь оператор return
	x += r.x;
	y += r.y;
	return *this;
}

mpoint mPoint::AsMP()
{
	return mpoint(y, x);
}

rTriangle::rTriangle()
{
}

rTriangle::rTriangle(mPoint& p1, mPoint& p2, mPoint& p3) :p1(p1), p2(p2), p3(p3)
{
	bOS = p1.IsOS || p2.IsOS || p3.IsOS;
}

void rTriangle::FillVector2d(Vector2d& v12, Vector2d& v23, Vector2d& v31)
{
	v12 = Vector2d(p2.x - p1.x, p2.y - p1.y);
	v23 = Vector2d(p3.x - p2.x, p3.y - p2.y);
	v31 = Vector2d(p1.x - p3.x, p1.y - p3.y);
}

bool RecursiveTriangulation::InitCircle()
{
	if (!BordersInit)
	{
		throw("Borders not inited");
		return false;
	}
	

	double Pi = 4 * atan(1);
	double step = 2 * Pi / (double)CircleDots;
	
	m_CircleRadius = rb / 2;
	m_CircleRadius2 = pow2(m_CircleRadius);

	for (int i = 0; i < CircleDots; i++)
	{
		double x = cos(step * i) * m_CircleRadius;
		double y = sin(step * i) * m_CircleRadius;
		nodes.push_back(mPoint(x, y));
	}
	return true;
}

bool RecursiveTriangulation::SetBorders(double l, double r, double t, double b)
{
	if (l == r)
	{
		throw("Wrong borders(l == r)");
		return false;
	}

	if (t == b)
	{
		throw("Wrong borders (t == b");
		return false;
	}

	double x = fabs(r - l);
	double y = fabs(t - b);

	lb = 0;
	rb = max(x, y);
	bb = 0;
	tb = rb;

	BordersInit = true;
	return true;
}


//надо вырезать треугольники
bool RecursiveTriangulation::InitRectGrid()
{
	if (RectGridConcentration == 0)
	{
		throw("RectGridConc == 0");
		return false;
	}
	if (!CircleInit)
	{
		throw("Circle not init");
		return false;
	}
	if (!TriangleNodesInit)
	{
		throw("Triangles nodes not init");
		return false;
	}

	double r = rb / 2.;
	double r2 = pow2(r);

	double step = rb / double(RectGridConcentration);
	for (int i = 0; i < RectGridConcentration; i++)
	{
		for (int j = 0; j < RectGridConcentration; j++)
		{
			double x = j * step + rand(-1e-5 * rb, 1e-5 * rb);
			double y = i * step + rand(-1e-5 * rb, 1e-5 * rb);
			mPoint n(x, y);
			if (bCorrectRectGridNode(n))
			{
				nodes.push_back(n);
			}
			
			
		}
	}


	return true;
}

bool RecursiveTriangulation::bCorrectRectGridNode(mPoint& n)
{
	if ((pow2(n.x - m_CircleRadius) + pow2(n.y - m_CircleRadius)) >= m_CircleRadius2)return false;


	double k1 = t1v12.GetK();
	double k2 = t1v23.GetK();
	double k3 = t1v31.GetK();

	double b1 = Triangle1.p1.y - k1 * Triangle1.p1.x;
	double b2 = Triangle1.p2.y - k1 * Triangle1.p2.x;
	double b3 = Triangle1.p3.y - k1 * Triangle1.p3.x;

	if (t1v12.x > 0)
	{
		if ((k1 * n.x + b1) <= n.y)return false;
	}
	else
	{
		if ((k1 * n.x + b1) >= n.y)return false;
	}

	if (t1v23.x > 0)
	{
		if ((k2 * n.x + b2) <= n.y)return false;
	}
	else
	{
		if ((k2 * n.x + b2) >= n.y)return false;
	}

	if (t1v31.x > 0)
	{
		if ((k3 * n.x + b3) <= n.y)return false;
	}
	else
	{
		if ((k3 * n.x + b3) >= n.y)return false;
	}

	k1 = t2v12.GetK();
	k2 = t2v23.GetK();
	k3 = t2v31.GetK();

	b1 = Triangle2.p1.y - k1 * Triangle2.p1.x;
	b2 = Triangle2.p2.y - k1 * Triangle2.p2.x;
	b3 = Triangle2.p3.y - k1 * Triangle2.p3.x;

	if (t2v12.x > 0)
	{
		if ((k1 * n.x + b1) <= n.y)return false;
	}
	else
	{
		if ((k1 * n.x + b1) >= n.y)return false;
	}

	if (t2v23.x > 0)
	{
		if ((k2 * n.x + b2) <= n.y)return false;
	}
	else
	{
		if ((k2 * n.x + b2) >= n.y)return false;
	}

	if (t2v31.x > 0)
	{
		if ((k3 * n.x + b3) <= n.y)return false;
	}
	else
	{
		if ((k3 * n.x + b3) >= n.y)return false;
	}

	return true;
}

void RecursiveTriangulation::SetRectGridConc(int rgc)
{
	RectGridConcentration = rgc;
}



vector<mpoint> RecursiveTriangulation::GetPoints()
{
	vector<mpoint> res;
	res.resize(nodes.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = nodes[i].AsMP();
	}
	return res;
}

void RecursiveTriangulation::SetTrianglesParams(mPoint tr1, mPoint tr2, double r1, double r2, double s1, double s2)
{
	Triangle1Translatiaon = tr1;
	Triangle2Translatiaon = tr2;
	Triangle1Rotation = r1;
	Triangle2Rotation = r2;
	Triangle1Scale = s1;
	Triangle2Scale = s2;
	TriangleInit = true;
}

void RecursiveTriangulation::SetTriangleDots(int tdots)
{
	TriangleDots = tdots;
	TriangleDotsInit = true;
}

bool RecursiveTriangulation::MakeStartGrid()
{
	if (!InitCircle())return false;
	if (!InitTriangle())return false;
	if (!InitRectGrid())return false;
}

bool RecursiveTriangulation::InitTriangle()
{
	if (!TriangleInit)
	{
		throw("Triangles params not init");
		return false;
	}
	if (!BordersInit)
	{
		throw("Borders not init");
		return false;
	}
	if (!TriangleDotsInit)
	{
		throw("TriangleDots not init");
		return false;
	}
	TriangleBaseLen = 0.33 * rb;
	PointF p1(0, 0);
	PointF p2(TriangleBaseLen, 0);
	PointF p3(TriangleBaseLen / 2, TriangleBaseLen * sqrt(3) / 2.);
	PointF c(p3.X, p3.Y / 3.);

	Matrix m;
	m.Translate(c.X, c.Y);
	m.Rotate(Triangle1Rotation);
	m.Scale(Triangle1Scale, Triangle1Scale);
	
	m.TransformPoints(&p1);
	m.TransformPoints(&p2);
	m.TransformPoints(&p3);

	Triangle1.p1.x = p1.X;
	Triangle1.p1.y = p1.Y;

	Triangle1.p2.x = p2.X;
	Triangle1.p2.y = p2.Y;

	Triangle1.p3.x = p3.X;
	Triangle1.p3.y = p3.Y;

	p1 = PointF(0, 0);
	p2 = PointF(TriangleBaseLen, 0);
	p3 = PointF(TriangleBaseLen / 2, TriangleBaseLen * sqrt(3) / 2.);

	m.Reset();

	m.Translate(c.X, c.Y);
	m.Rotate(Triangle2Rotation);
	m.Scale(Triangle2Scale, Triangle2Scale);

	m.TransformPoints(&p1);
	m.TransformPoints(&p2);
	m.TransformPoints(&p3);

	Triangle2.p1.x = p1.X;
	Triangle2.p1.y = p1.Y;

	Triangle2.p2.x = p2.X;
	Triangle2.p2.y = p2.Y;

	Triangle2.p3.x = p3.X;
	Triangle2.p3.y = p3.Y;

	CalcTrianglesNodes(Triangle1);
	CalcTrianglesNodes(Triangle2);

	Triangle1.FillVector2d(t1v12, t1v23, t1v31);
	Triangle2.FillVector2d(t2v12, t2v23, t2v31);

	TriangleNodesInit = true;

	return true;
}

void RecursiveTriangulation::CalcTrianglesNodes(rTriangle& target)
{
	Vector2d v12(target.p2.x - target.p1.x, target.p2.y - target.p1.y);
	Vector2d v23(target.p3.x - target.p2.x, target.p3.y - target.p2.y);
	Vector2d v31(target.p1.x - target.p3.x, target.p1.y - target.p3.y);

	v12 /= TriangleDots;
	v23 /= TriangleDots;
	v31 /= TriangleDots;

	mPoint p = target.p1;
	nodes.push_back(p);
	for (int i = 1; i < TriangleDots; i++)
	{
		p += v12;
		nodes.push_back(p);
	}

	p = target.p2;
	nodes.push_back(p);
	for (int i = 1; i < TriangleDots; i++)
	{
		p += v23;
		nodes.push_back(p);
	}

	p = target.p3;
	nodes.push_back(p);
	for (int i = 1; i < TriangleDots; i++)
	{
		p += v31;
		nodes.push_back(p);
	}
}
