#pragma once
#include<vector>
#include<algorithm>
#include<gdiplus.h>
using namespace Gdiplus;

using namespace std;


typedef pair<double, double> mpoint;
struct Vector2d
{
	double x = 0;
	double y = 0;
	Vector2d();
	Vector2d(double x, double y);
	Vector2d(mpoint p);
	Vector2d operator +(Vector2d& r);
	Vector2d operator -(Vector2d & r);
	Vector2d& operator /=(double& r);
	Vector2d& operator /=(int& r);
	Vector2d operator *(double& r);
	Vector2d operator *(int& r);
	double abs();
	double GetK();
	Vector2d& normalize(double newabs = 1);
};
struct mPoint
{
	double x = 0;
	double y = 0;
	bool IsOS = false;
	mPoint();
	mPoint(double x, double  y, bool bOS = false);
	mPoint operator + (mPoint& right);
	mPoint& operator +=(Vector2d& r);
	mpoint AsMP();
};
struct Triangle
{
	mpoint p1;
	mpoint p2;
	mpoint p3;
	Triangle();
	Triangle(mpoint p1, mpoint p2, mpoint p3);
};
struct rTriangle
{
	mPoint p1, p2, p3;
	bool bOS = false;
	rTriangle();
	rTriangle(mPoint& p1, mPoint& p2, mPoint& p3);
	void FillVector2d(Vector2d& v12, Vector2d& v23, Vector2d& v31);
};
class mTriangulation
{
	//число узлов по горизонтали
	int N = 0;
	//число узлов по вертикали
	int M = 0;
	//кол-во точек
	int dots = 0;

	bool NodesInit = false;
	vector<vector<mpoint>>nodes;
	vector<Triangle>triangles;

	CString Error = L"Ok";

	double noiseval = 0;

	mpoint CircleCenter;
	double CircleRadius;

	mpoint ICircleCenter;
	double ICircleRadius;

	double LBorder;
	double RBorder;
	double TBorder;
	double BBorder;
	bool BordersInit = false;
	bool RectGrid = true;

	double estimate;
private:
protected:
	inline double CalcCircleRadius(mpoint& center, mpoint& p);
	inline mpoint CalcCircleCenter(mpoint& p1, mpoint& p2, mpoint& p3);
	double CalcICircleRadius(mpoint p1, mpoint p2, mpoint p3);
	mpoint CalcICircleCenter(mpoint p1, mpoint p2, mpoint p3);
	inline mpoint CalcVector(mpoint p1, mpoint p2);
	inline double abs(mpoint p);
	inline void NormalizeVector(mpoint& p, double newabs = 1);
	void ParsePoint(double& x, double& y, mpoint from);
	

	bool CheckIfPointInCircle(mpoint& c, mpoint& p, double& r);
	void ErrOk();

	double Estimate();
public:
	void SetNM(int n, int m);
	void SetDots(int Dots);
	vector<vector<mpoint>> GetNodes();
	vector<mpoint> GetPoints();
	CString GetLastError();

	bool CreateRandomDots();

	bool CreateCirle();
	mpoint GetCircleCenter();
	double GetCircleRadius();
	mpoint GetICircleCenter();
	double GetICircleRadius();

	bool Triangulation();
	vector<Triangle>GetTriangles();

	void SetNoise(double val);

	void SetBoders(double l, double r, double t, double b);
	void SetRectGridState(bool b);
	double GetEsimate();
};


double rand(double left, double right);



class RecursiveTriangulation
{
	vector<mPoint>nodes;
	vector<rTriangle> triangles;
	
	double lb = 0;
	double rb = 0;
	double tb = 0;
	double bb = 0;
	bool BordersInit = false;
	
	mPoint Triangle1Translatiaon;
	mPoint Triangle2Translatiaon;
	
	double Triangle1Rotation;
	double Triangle2Rotation;

	double Triangle1Scale;
	double Triangle2Scale;

	double TriangleBaseLen = 0;
	
	rTriangle Triangle1;
	rTriangle Triangle2;

	Vector2d t1v12, t1v23, t1v31;
	Vector2d t2v12, t2v23, t2v31;

	int TriangleDots = 0;

	bool TriangleDotsInit = false;
	bool TriangleInit = false;
	bool TriangleNodesInit = false;

	bool CircleInit = false;
	int CircleDots = 0;

	int RectGridConcentration = 0;

	double m_CircleRadius = 0;
	double m_CircleRadius2 = 0;

protected:
	bool InitCircle();
	bool InitRectGrid();
	bool bCorrectRectGridNode(mPoint& n);
	bool InitTriangle();
	void CalcTrianglesNodes(rTriangle& target);

public:
	bool SetBorders(double l, double r, double t, double b);
	void SetRectGridConc(int rgc);

	void SetTrianglesParams(mPoint tr1, mPoint tr2, double r1, double r2, double s1, double s2);
	void SetTriangleDots(int tdots);
	vector<mpoint> GetPoints();
};
