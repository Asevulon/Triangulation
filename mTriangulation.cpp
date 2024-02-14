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
	/*vector<double> temp;
	temp.resize(triangles.size());
	for (int i = 0; i < triangles.size(); i++)
	{
		mpoint oc = CalcCircleCenter(triangles[i].p1, triangles[i].p2, triangles[i].p3);
		mpoint ic = CalcICircleCenter(triangles[i].p1, triangles[i].p2, triangles[i].p3);
		temp[i] = pow2(oc.first - ic.first) + pow2(oc.second - ic.second);
	}
	double tm = *max_element(temp.begin(), temp.end());
	for (int i = 0; i < triangles.size(); i++)
	{
		res += temp[i] / tm;
	}
	res /= min(dots, M * N);*/

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

double Vector2d::abs()
{
	return sqrt(pow2(x) + pow2(y));
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
