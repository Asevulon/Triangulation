#include "pch.h"
#include "mTriangulation.h"

#define pow2(val) (val) * (val)


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


Vector2d::Vector2d(mPoint& p1, mPoint& p2)
{
	x = p2.x - p1.x;
	y = p2.y - p1.y;
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

Vector2d& Vector2d::operator*=(double r)
{
	x *= r;
	y *= r;
	return *this;
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


mPoint::mPoint()
{
}

mPoint::mPoint(double x, double y, mPointType t, bool bOS):x(x),y(y),type(t),IsOS(bOS)
{

}

mPoint mPoint::operator+(mPoint& right)
{
	return mPoint(x + right.x, y + right.y);
}

mPoint mPoint::operator/(double& r)
{
	mPoint res = *this;
	res /= r;
	return res;
}

mPoint& mPoint::operator+=(Vector2d& r)
{
	// TODO: вставьте здесь оператор return
	x += r.x;
	y += r.y;
	return *this;
}

mPoint& mPoint::operator/=(double& r)
{
	// TODO: вставьте здесь оператор return
	x /= r;
	y /= r;
	return *this;
}

bool mPoint::operator==(mPoint& r)
{
	if (x != r.x)return false;
	if (y != r.y)return false;
	return true;
}

Gdiplus::PointF mPoint::AsPointF()
{
	return Gdiplus::PointF(x,y);
}


rTriangle::rTriangle()
{
}

rTriangle::rTriangle(mPoint& p1, mPoint& p2, mPoint& p3) :p1(p1), p2(p2), p3(p3)
{
	bOS = p1.IsOS || p2.IsOS || p3.IsOS;
}

mPoint& rTriangle::operator[](int id)
{
	// TODO: вставьте здесь оператор return
	switch (id)
	{
	case 0: return p1;
	case 1: return p2;
	case 2: return p3;
	default:
		abort();
		break;
	}
}

void rTriangle::FillVector2d(Vector2d& v12, Vector2d& v23, Vector2d& v31)
{
	v12 = Vector2d(p2.x - p1.x, p2.y - p1.y);
	v23 = Vector2d(p3.x - p2.x, p3.y - p2.y);
	v31 = Vector2d(p1.x - p3.x, p1.y - p3.y);
}


mPointType& rTriangle::type1()
{
	// TODO: вставьте здесь оператор return
	return p1.type;
}

mPointType& rTriangle::type2()
{
	// TODO: вставьте здесь оператор return
	return p2.type;
}

mPointType& rTriangle::type3()
{
	// TODO: вставьте здесь оператор return
	return p3.type;
}

bool rTriangle::InCircleRange(mPoint& p)
{
	if (pow2(p.x - CircleCenter.x) + pow2(p.y - CircleCenter.y) <= pow2(CircleRadius))return true;
	return false;
}

bool rTriangle::Contains(mPoint& p)
{
	if (p1 == p)return true;
	if (p2 == p)return true;
	if (p3 == p)return true;
	return false;
}

int rTriangle::IContains(mPoint& p)
{
	if (p1 == p)return 0;
	if (p2 == p)return 1;
	if (p3 == p)return 2;
	return -1;
}

double rTriangle::CalcS()
{
	Vector2d v12(p1, p2);
	Vector2d v13(p1, p3);
	Vector2d v23(p2, p3);
	double a = v12.abs();
	double b = v13.abs();
	double c = v23.abs();
	double p = (a + b + c) / 2.;
	S = sqrt(p * (p - a) * (p - b) * (p - c));
	return S;
}

void rTriangle::CalcABC(int id)
{
	double x1(p1.x), x2(p2.x), x3(p3.x);
	double y1(p1.y), y2(p2.y), y3(p3.y);
	double z1(0), z2(0), z3(0);
	switch (id)
	{
	case 0:z1 = 1;
		break;
	case 1:z2 = 1;
		break;
	case 2:z3 = 1;
		break;
	default:
		abort();
		break;
	}
	A = (y2 - y1) * (z3 - z1) - (y3 - y1) * (z2 - z1);
	B = (x2 - x1) * (z3 - z1) - (x3 - x1) * (z2 - z1);
	C = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
}

void rTriangle::InitParamsForContainingCheck()
{
	v12 = Vector2d(p1, p2);
	v23 = Vector2d(p2, p3);
	v31 = Vector2d(p3, p1);

	k1 = v12.GetK();
	k2 = v23.GetK();
	k3 = v31.GetK();

	b1 = p1.y - k1 * p1.x;
	b2 = p2.y - k2 * p2.x;
	b3 = p3.y - k3 * p3.x;
}

bool rTriangle::NotContainsInInnerArea(mPoint& n)
{
	bool res1 = true;
	bool res2 = true;
	bool res3 = true;

	if (v12.x > 0)
	{
		if ((k1 * n.x + b1) <= n.y)res1 = false;
	}
	else
	{
		if ((k1 * n.x + b1) >= n.y)res1 = false;
	}

	if (v23.x > 0)
	{
		if ((k2 * n.x + b2) <= n.y)res2 = false;
	}
	else
	{
		if ((k2 * n.x + b2) >= n.y)res2 = false;
	}

	if (v31.x > 0)
	{
		if ((k3 * n.x + b3) <= n.y)res3 = false;
	}
	else
	{
		if ((k3 * n.x + b3) >= n.y)res3 = false;
	}

	if (res1 && res2 && res3)return false;
	return true;
}

bool RecursiveTriangulation::InitCircle()
{
	if (!BordersInit)
	{
		throw("Borders not inited");
		return false;
	}
	if (!CircleDotsInit)
	{
		throw("CircleDots not inited");
		return false;
	}

	double Pi = 4 * atan(1);
	double step = 2 * Pi / (double)CircleDots;
	
	m_CircleRadius = rb / 2;
	m_CircleRadius2 = pow2(m_CircleRadius);

	for (int i = 0; i < CircleDots; i++)
	{
		double x = (cos(step * i) + 1) * m_CircleRadius + noise();
		double y = (sin(step * i) + 1) * m_CircleRadius + noise();
		EnterCriticalSection(&cs);
		nodes.push_back(mPoint(x, y, circlep));
		LeaveCriticalSection(&cs);
	}
	CircleInit = true;
	return true;
}

inline double RecursiveTriangulation::CalcCircleRadius(mPoint& center, mPoint& p)
{
	Vector2d v(center, p);
	return v.abs();
}

inline mPoint RecursiveTriangulation::CalcCircleCenter(mPoint& p1, mPoint& p2, mPoint& p3)
{
	mPoint res;

	double x1(p1.x), x2(p2.x), x3(p3.x);
	double y1(p1.y), y2(p2.y), y3(p3.y);


	double s1 = (pow2(x2) - pow2(x1) + pow2(y2) - pow2(y1)) * (y3 - y1);
	double s2 = (pow2(x3) - pow2(x1) + pow2(y3) - pow2(y1)) * (y2 - y1);
	double s3 = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);

	res.x = 0.5 * (s1 - s2) / s3;

	s1 = (pow2(x3) - pow2(x1) + pow2(y3) - pow2(y1)) * (x2 - x1);
	s2 = (pow2(x2) - pow2(x1) + pow2(y2) - pow2(y1)) * (x3 - x1);

	res.y = 0.5 * (s1 - s2) / s3;

	return res;
}

inline double RecursiveTriangulation::CalcICircleRadius(mPoint p1, mPoint p2, mPoint p3)
{
	Vector2d v12(p1, p2);
	Vector2d v13(p1, p3);
	Vector2d v23(p2, p3);

	double a(0), b(0), c(0);
	a = v12.abs();
	b = v13.abs();
	c = v23.abs();

	double p = (a + b + c) / 2.;

	return sqrt((p - a) * (p - b) * (p - c) / p);
}

inline mPoint RecursiveTriangulation::CalcICircleCenter(mPoint p1, mPoint p2, mPoint p3)
{
	mPoint res;

	Vector2d v12(p1, p2);
	Vector2d v13(p1, p3);
	Vector2d v23(p2, p3);

	double ab = v12.abs();
	double ac = v13.abs();
	double bc = v23.abs();

	double x1(p1.x), x2(p2.x), x3(p3.x);
	double y1(p1.y), y2(p2.y), y3(p3.y);


	double lambda = ((x2 - x1) / ab + (x3 - x1) / ac) / ((y2 - y1) / ab + (y3 - y1) / ac);
	double mu = ((x1 - x2) / ab + (x3 - x2) / bc) / ((y1 - y2) / ab + (y3 - y2) / bc);

	res.x = (-mu * x1 + mu * lambda * (y1 - y2) + lambda * x2) / (lambda - mu);
	res.y = (x2 - mu * y2 - x1 + lambda * y1) / (lambda - mu);
	return res;
}

template<typename T>
inline void RecursiveTriangulation::swap(vector<T>& target, int id1, int id2)
{
	auto temp = target[id1];
	target[id1] = target[id2];
	target[id2] = temp;
}

template<typename T>
bool RecursiveTriangulation::InVector(vector<T>& source, T val)
{
	for (auto& item : source)
	{
		if (item == val)return true;
	}
	return false;
}

void RecursiveTriangulation::TrianglesOS()
{
	mPoint tc = CalcCircleCenter(Triangle1.p1, Triangle1.p2, Triangle1.p3);
	tc.IsOS = true;

	Vector2d v(tc, Triangle1.p1);
	v *= OSval;
	mPoint p1 = tc;
	p1 += v;

	v = Vector2d(tc, Triangle1.p2);
	v *= OSval;
	mPoint p2 = tc;
	p2 += v;

	v = Vector2d(tc, Triangle1.p3);
	v *= OSval;
	mPoint p3 = tc;
	p3 += v;

	rTriangle OsTriangle(p1, p2, p3);
	OsTriangle.p1.InBaseTriangle = true;
	OsTriangle.p2.InBaseTriangle = true;
	OsTriangle.p3.InBaseTriangle = true;
	CalcTrianglesNodes(OsTriangle);

	tc = CalcCircleCenter(Triangle2.p1, Triangle2.p2, Triangle2.p3);
	tc.IsOS = true;

	v = Vector2d(tc, Triangle2.p1);
	v *= OSval;
	p1 = tc;
	p1 += v;

	v = Vector2d(tc, Triangle2.p2);
	v *= OSval;
	p2 = tc;
	p2 += v;

	v = Vector2d(tc, Triangle2.p3);
	v *= OSval;
	p3 = tc;
	p3 += v;

	OsTriangle = rTriangle(p1, p2, p3);
	OsTriangle.p1.InBaseTriangle = true;
	OsTriangle.p2.InBaseTriangle = true;
	OsTriangle.p3.InBaseTriangle = true;
	CalcTrianglesNodes(OsTriangle);
}



bool RecursiveTriangulation::rMakeStartGrid()
{
	nodes.clear();
	triangles.clear();

	if (!InitCircle())return false;
	if (!InitCircles())return false; //Вместо InitTriangles
	//if (!InitTriangle())return false;

	mPoint os;
	os.IsOS = true;
	os.x = lb - 0.1 * (rb - lb);
	os.y = bb - 0.1 * (tb - lb);
	EnterCriticalSection(&cs);
	nodes.push_back(os);
	LeaveCriticalSection(&cs);
	os.x = lb - 0.1 * (rb - lb);
	os.y = tb + 0.1 * (tb - lb);
	EnterCriticalSection(&cs);
	nodes.push_back(os);
	LeaveCriticalSection(&cs);
	os.x = rb + 0.1 * (rb - lb);
	os.y = tb + 0.1 * (tb - lb);
	EnterCriticalSection(&cs);
	nodes.push_back(os);
	LeaveCriticalSection(&cs);
	os.x = rb + 0.1 * (rb - lb);
	os.y = bb - 0.1 * (tb - lb);
	EnterCriticalSection(&cs);
	nodes.push_back(os);
	LeaveCriticalSection(&cs);

	//TrianglesOS();
	CircleOS();
	OCircleCenter = CalcCircleCenter(nodes[0], nodes[1], nodes[2]);
	OCircleRadius = CalcCircleRadius(OCircleCenter, nodes[0]);
	ICircleCenter = CalcICircleCenter(nodes[0], nodes[1], nodes[2]);
	ICircleRadius = CalcICircleRadius(nodes[0], nodes[1], nodes[2]);

	FieldInit = true;
	
	return true;
}

bool RecursiveTriangulation::GetRectGrid(vector<mPoint>& out)
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

	vector<mPoint> res;

	double r = rb / 2.;
	double r2 = pow2(r);

	double step = rb / double(RectGridConcentration);
	for (int i = 0; i < RectGridConcentration; i++)
	{
		for (int j = 0; j < RectGridConcentration; j++)
		{
			double x = j * step + noise();
			double y = i * step + noise();
			mPoint n(x, y, gridp);
			if (bCorrectRectGridNodeCircles(n))
			{
				res.push_back(n);
			}


		}
	}

	out = res;

	return true;
}

vector<int> RecursiveTriangulation::GetBrockenTriangles(mPoint& np)
{
	vector<int>res;

	for (int i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].InCircleRange(np))res.push_back(i);
	}

	return res;
}

void RecursiveTriangulation::RemoveBrockenTriangles()
{
	for (int i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].broken)
		{
			EnterCriticalSection(&cs);
			swap(triangles, i, triangles.size() - 1);
			triangles.pop_back();
			LeaveCriticalSection(&cs);
			i--;
		}
	}
}

int RecursiveTriangulation::MarkBrockenTriangles(mPoint& p)
{
	int brocken = 0;
	for (int i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].Protected)continue;
		if (triangles[i].InCircleRange(p))
		{
			triangles[i].broken = true;
			brocken++;
		}
	}
	return brocken;
}

vector<mPoint> RecursiveTriangulation::CollectBrockenNodes()
{
	vector<mPoint> res;
	for (auto& item : triangles)
	{
		if (item.broken)
		{
			if (!InVector(res, item.p1))res.push_back(item.p1);
			if (!InVector(res, item.p2))res.push_back(item.p2);
			if (!InVector(res, item.p3))res.push_back(item.p3);
		}
	}
	return res;
}

int RecursiveTriangulation::ProtectTriangles()
{
	int count = 0;
	for (auto& item : triangles)
	{
		if (item.p1.InBaseTriangle || item.p2.InBaseTriangle || item.p3.InBaseTriangle)
		{
			item.Protected = true;
			count++;
		}
	}
	return count;
}

void RecursiveTriangulation::Triangulate(vector<mPoint>& points)
{
	mPoint c;
	double r = 0;



	for (int i = 0; i < points.size() - 2; i++)
	{
		for (int j = i + 1; j < points.size() - 1; j++)
		{
			for (int k = j + 1; k < points.size(); k++)
			{
				mPoint& p1 = points[i];
				mPoint& p2 = points[j];
				mPoint& p3 = points[k];

				c = CalcCircleCenter(p1, p2, p3);
				r = CalcCircleRadius(c, p1);
				r *= r;

				bool InCircle = false;
				for (int m = 0; m < points.size(); m++)
				{

					if ((m == i) || (m == j) || (m == k))continue;
					mPoint& tp = points[m];
					double testval = pow2(tp.x - c.x) + pow2(tp.y - c.y);
					if (testval <= r)
					{
						InCircle = true;
						break;
					}
				}

				if (!InCircle)
				{
					rTriangle temp(p1, p2, p3);
					if(temp.Contains(points.back()))
					{
						temp.CircleCenter = c;
						temp.CircleRadius = sqrt(r);
						EnterCriticalSection(&cs);
						triangles.push_back(temp);
						LeaveCriticalSection(&cs);
					}
				}
			}
		}
	}
}

RecursiveTriangulation::RecursiveTriangulation()
{
	InitializeCriticalSection(&cs);
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
			double x = j * step + noise();
			double y = i * step + noise();
			mPoint n(x, y, gridp);
			if (bCorrectRectGridNodeCircles(n))
			{
				EnterCriticalSection(&cs);
				nodes.push_back(n);
				LeaveCriticalSection(&cs);
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
	double b2 = Triangle1.p2.y - k2 * Triangle1.p2.x;
	double b3 = Triangle1.p3.y - k3 * Triangle1.p3.x;

	bool res1 = true;
	bool res2 = true;
	bool res3 = true;

	if (t1v12.x > 0)
	{
		if ((k1 * n.x + b1) <= n.y)res1 = false;
	}
	else
	{
		if ((k1 * n.x + b1) >= n.y)res1 = false;
	}

	if (t1v23.x > 0)
	{
		if ((k2 * n.x + b2) <= n.y)res2 = false;
	}
	else
	{
		if ((k2 * n.x + b2) >= n.y)res2 = false;
	}

	if (t1v31.x > 0)
	{
		if ((k3 * n.x + b3) <= n.y)res3 = false;
	}
	else
	{
		if ((k3 * n.x + b3) >= n.y)res3 = false;
	}

	if (res1 && res2 && res3)return false;

	res1 = true;
	res2 = true;
	res3 = true;

	k1 = t2v12.GetK();
	k2 = t2v23.GetK();
	k3 = t2v31.GetK();

	b1 = Triangle2.p1.y - k1 * Triangle2.p1.x;
	b2 = Triangle2.p2.y - k2 * Triangle2.p2.x;
	b3 = Triangle2.p3.y - k3 * Triangle2.p3.x;

	if (t2v12.x > 0)
	{
		if ((k1 * n.x + b1) <= n.y)res1 = false;
	}
	else
	{
		if ((k1 * n.x + b1) >= n.y)res1 = false;
	}

	if (t2v23.x > 0)
	{
		if ((k2 * n.x + b2) <= n.y)res2 = false;
	}
	else
	{
		if ((k2 * n.x + b2) >= n.y)res2 = false;
	}

	if (t2v31.x > 0)
	{
		if ((k3 * n.x + b3) <= n.y)res3 = false;
	}
	else
	{
		if ((k3 * n.x + b3) >= n.y)res3 = false;
	}

	if (res1 && res2 && res3)return false;

	return true;
}

bool RecursiveTriangulation::bCorrectRectGridNodeCircles(mPoint& n)
{
	if ((pow2(n.x - m_CircleRadius) + pow2(n.y - m_CircleRadius)) >= m_CircleRadius2)return false;
	if ((pow2(n.x - circlesOS1.x) + pow2(n.y - circlesOS1.y)) <= circlesOSRadius2)return false;
	if ((pow2(n.x - circlesOS2.x) + pow2(n.y - circlesOS2.y)) <= circlesOSRadius2)return false;
	return true;
}

void RecursiveTriangulation::SetRectGridConc(int rgc)
{
	RectGridConcentration = rgc;
}



vector<mPoint> RecursiveTriangulation::GetPoints()
{
	EnterCriticalSection(&cs);
	vector<mPoint> res = nodes;
	LeaveCriticalSection(&cs);
	return res;
}

bool RecursiveTriangulation::Triangulate()
{
	stop = false;
	done = false;

	if (!FieldInit)
	{
		throw("Field not init");
		return false;
	}
	
	triangles.clear();

	mPoint c;
	double r = 0;

	

	for (int i = 0; i < nodes.size() - 2; i++)
	{

		for (int j = i + 1; j < nodes.size() - 1; j++)
		{

			for (int k = j + 1; k < nodes.size(); k++)
			{

				mPoint& p1 = nodes[i];
				mPoint& p2 = nodes[j];
				mPoint& p3 = nodes[k];

				c = CalcCircleCenter(p1, p2, p3);
				r = CalcCircleRadius(c, p1);
				r *= r;

				bool InCircle = false;
				for (int m = 0; m < nodes.size(); m++)
				{

					if ((m == i) || (m == j) || (m == k))continue;
					mPoint& tp = nodes[m];
					double testval = pow2(tp.x - c.x) + pow2(tp.y - c.y);
					if (testval <= r)
					{
						InCircle = true;
						break;
					}
				}
				
				if (!InCircle)
				{
					rTriangle temp(p1, p2, p3);
					temp.CircleCenter = c;
					temp.CircleRadius = sqrt(r);
					EnterCriticalSection(&cs);
					triangles.push_back(temp);
					LeaveCriticalSection(&cs);
				}
			}
		}
	}

	done = true;
	return true;
}

vector<rTriangle> RecursiveTriangulation::GetTriangles()
{
	EnterCriticalSection(&cs);
	vector<rTriangle> res = triangles;
	LeaveCriticalSection(&cs);
	return res;
}

double RecursiveTriangulation::Estimate()
{
	
	double res = 0;
	for (auto& item : triangles)
	{
		item.ICircleRadius = CalcICircleRadius(item.p1, item.p2, item.p3);
		item.est = 2 * item.ICircleRadius / item.CircleRadius;
		res += item.est;
	}
	return res / triangles.size();
}

mPoint RecursiveTriangulation::GetOCircleCenter()
{
	return OCircleCenter;
}

double RecursiveTriangulation::GetOCircleRadius()
{
	return OCircleRadius;
}

mPoint RecursiveTriangulation::GetICircleCenter()
{
	return ICircleCenter;
}

double RecursiveTriangulation::GetICircleRadius()
{
	return ICircleRadius;
}

bool RecursiveTriangulation::RemoveOS()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].IsOS)
		{
			EnterCriticalSection(&cs);
			swap(nodes, i, nodes.size() - 1);
			nodes.pop_back();
			LeaveCriticalSection(&cs);
			i--;
		}
	}

	for (int i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].bOS)
		{
			EnterCriticalSection(&cs);
			swap(triangles, i, triangles.size() - 1);
			triangles.pop_back();
			LeaveCriticalSection(&cs);
			i--;
		}
	}
	return true;
}

bool RecursiveTriangulation::rTriangulate()
{
	stop = false;
	srand(time(NULL));
	if (!rMakeStartGrid())return false;
	vector<mPoint> rgrid;
	if (!GetRectGrid(rgrid))return false;

	Triangulate();
	ProtectTriangles();
	for (auto& p : rgrid)
	{
		MarkBrockenTriangles(p);

		auto points = CollectBrockenNodes();

		RemoveBrockenTriangles();

		EnterCriticalSection(&cs);
		nodes.push_back(p);
		LeaveCriticalSection(&cs);

		points.push_back(p);

		Triangulate(points);
	}

	return true;
}



void RecursiveTriangulation::SetTrianglesParams(mPoint tr1, mPoint tr2, double r1, double r2, double s1, double s2)
{
	Triangle1Translatiaon = tr1;
	Triangle2Translatiaon = tr2;
	Triangle1Rotation = r1;
	Triangle2Rotation = r2;
	Triangle1Scale = s1 / 100.;
	Triangle2Scale = s2 / 100.;
	TriangleInit = true;
}

void RecursiveTriangulation::SetTriangleDots(int tdots)
{
	TriangleDots = tdots;
	TriangleDotsInit = true;
}

bool RecursiveTriangulation::MakeStartGrid()
{
	nodes.clear();
	triangles.clear();
	srand(time(NULL));

	if (!InitCircle())return false;
	if (!InitCircles())return false;//Вместо InitTriangle
	//if (!InitTriangle())return false;
	if (!InitRectGrid())return false;

	mPoint os;
	os.IsOS = true;
	os.x = lb - 0.1 * (rb - lb);
	os.y = bb - 0.1 * (tb - lb);
	EnterCriticalSection(&cs);
	nodes.push_back(os);
	LeaveCriticalSection(&cs);
	os.x = lb - 0.1 * (rb - lb);
	os.y = tb + 0.1 * (tb - lb);
	EnterCriticalSection(&cs);
	nodes.push_back(os);
	LeaveCriticalSection(&cs);
	os.x = rb + 0.1 * (rb - lb);
	os.y = tb + 0.1 * (tb - lb);
	EnterCriticalSection(&cs);
	nodes.push_back(os);
	LeaveCriticalSection(&cs);
	os.x = rb + 0.1 * (rb - lb);
	os.y = bb - 0.1 * (tb - lb);
	EnterCriticalSection(&cs);
	nodes.push_back(os);
	LeaveCriticalSection(&cs);

	//TrianglesOS();
	CircleOS();

	OCircleCenter = CalcCircleCenter(nodes[0], nodes[1], nodes[2]);
	OCircleRadius = CalcCircleRadius(OCircleCenter, nodes[0]);
	ICircleCenter = CalcICircleCenter(nodes[0], nodes[1], nodes[2]);
	ICircleRadius = CalcICircleRadius(nodes[0], nodes[1], nodes[2]);

	FieldInit = true;
}

void RecursiveTriangulation::SetCircleDots(int cdots)
{
	CircleDots = cdots;
	CircleDotsInit = true;
}

void RecursiveTriangulation::CircleOS()
{
	EnterCriticalSection(&cs);
	nodes.push_back(circlesOS1);
	nodes.push_back(circlesOS2);
	LeaveCriticalSection(&cs);
}

bool RecursiveTriangulation::InitCircles()
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

	double CircleR = (rb - lb) / 2.;
	PointF p1(0, 0);
	PointF p2(CircleR, 0);
	Matrix m;
	m.Translate(Triangle1Translatiaon.x * rb / 100., Triangle1Translatiaon.y * rb / 100.);
	m.Rotate(Triangle1Rotation);
	m.Scale(Triangle1Scale, Triangle1Scale);
	m.TransformPoints(&p1);
	m.TransformPoints(&p2);

	double x0 = p1.X + noise();
	double y0 = p1.Y + noise();

	circlesOS1.x = x0;
	circlesOS1.y = y0;
	circlesOS1.IsOS = true;
	circlesOS1.InBaseTriangle = true;

	const double Pi2 = atan(1) * 8;
	double step = Pi2 / (TriangleDots - 1);
	double radius = sqrt((p2.X - p1.X) * (p2.X - p1.X) + (p2.Y - p1.Y) * (p2.Y - p1.Y));
	for (int i = 0; i < TriangleDots; ++i)
	{
		mPoint p;
		p.x = x0 + radius * cos(step * i) + noise();
		p.y = y0 + radius * sin(step * i) + noise();
		p.type = trianglep;

		EnterCriticalSection(&cs);
		nodes.push_back(p);
		LeaveCriticalSection(&cs);
	}
	
	p1 = PointF(0, 0);
	p2 = PointF(CircleR, 0);

	m.Reset();

	m.Translate(Triangle2Translatiaon.x * rb / 100., Triangle2Translatiaon.y * rb / 100.);
	m.Rotate(Triangle2Rotation);
	m.Scale(Triangle2Scale, Triangle2Scale);

	m.TransformPoints(&p1);
	m.TransformPoints(&p2);

	x0 = p1.X + noise();
	y0 = p1.Y + noise();

	circlesOS2.x = x0;
	circlesOS2.y = y0;
	circlesOS2.IsOS = true;
	circlesOS2.InBaseTriangle = true;

	TriangleNodesInit = true;
	radius = sqrt((p2.X - p1.X) * (p2.X - p1.X) + (p2.Y - p1.Y) * (p2.Y - p1.Y));
	for (int i = 0; i < TriangleDots; ++i)
	{
		mPoint p;
		p.x = x0 + radius * cos(step * i) + noise();
		p.y = y0 + radius * sin(step * i) + noise();
		p.type = trianglep;

		EnterCriticalSection(&cs);
		nodes.push_back(p);
		LeaveCriticalSection(&cs);
	}

	circlesOSRadius2 = pow2(radius);
	
	return true;
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
	TriangleBaseLen = rb;
	PointF p1(0, 0);
	PointF p3(TriangleBaseLen, 0);
	PointF p2(TriangleBaseLen / 2, TriangleBaseLen * sqrt(3) / 2.);

	Matrix m;

	m.Translate(Triangle1Translatiaon.x * rb / 100., Triangle1Translatiaon.y * rb / 100.);
	m.Rotate(Triangle1Rotation);
	m.Scale(Triangle1Scale, Triangle1Scale);

	m.TransformPoints(&p1);
	m.TransformPoints(&p2);
	m.TransformPoints(&p3);

	Triangle1.p1.x = p1.X + noise();
	Triangle1.p1.y = p1.Y + noise();

	Triangle1.p2.x = p2.X + noise();
	Triangle1.p2.y = p2.Y + noise();

	Triangle1.p3.x = p3.X + noise();
	Triangle1.p3.y = p3.Y + noise();
	
	Triangle1.type1() = trianglep;
	Triangle1.type2() = trianglep;
	Triangle1.type3() = trianglep;

	p1 = PointF(0, 0);
	p3 = PointF(TriangleBaseLen, 0);
	p2 = PointF(TriangleBaseLen / 2, TriangleBaseLen * sqrt(3) / 2.);

	m.Reset();


	m.Translate(Triangle2Translatiaon.x * rb / 100., Triangle2Translatiaon.y * rb / 100.);
	m.Rotate(Triangle2Rotation);
	m.Scale(Triangle2Scale, Triangle2Scale);

	m.TransformPoints(&p1);
	m.TransformPoints(&p2);
	m.TransformPoints(&p3);

	Triangle2.p1.x = p1.X + noise();
	Triangle2.p1.y = p1.Y + noise();

	Triangle2.p2.x = p2.X + noise();
	Triangle2.p2.y = p2.Y + noise();

	Triangle2.p3.x = p3.X + noise();
	Triangle2.p3.y = p3.Y + noise();

	Triangle2.type1() = trianglep;
	Triangle2.type2() = trianglep;
	Triangle2.type3() = trianglep;

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

	EnterCriticalSection(&cs);
	nodes.push_back(p);
	LeaveCriticalSection(&cs);
	for (int i = 1; i < TriangleDots; i++)
	{
		p += v12;
		p.x += noise();
		p.y += noise();
		EnterCriticalSection(&cs);
		nodes.push_back(p);
		LeaveCriticalSection(&cs);
	}

	p = target.p2;

	EnterCriticalSection(&cs);
	nodes.push_back(p);
	LeaveCriticalSection(&cs);
	for (int i = 1; i < TriangleDots; i++)
	{
		p += v23;
		p.x += noise();
		p.y += noise();
		EnterCriticalSection(&cs);
		nodes.push_back(p);
		LeaveCriticalSection(&cs);
	}

	p = target.p3;
	
	EnterCriticalSection(&cs);
	nodes.push_back(p);
	LeaveCriticalSection(&cs);
	for (int i = 1; i < TriangleDots; i++)
	{
		p += v31;
		p.x += noise();
		p.y += noise();
		EnterCriticalSection(&cs);
		nodes.push_back(p);
		LeaveCriticalSection(&cs);
	}
}

double RecursiveTriangulation::noise()
{
	return rand(-noiseval * rb, noiseval * rb);
}
