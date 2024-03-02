#pragma once
#include<vector>
#include<algorithm>
#include<gdiplus.h>
using namespace Gdiplus;

using namespace std;



struct mPoint;

struct Vector2d
{
	double x = 0;
	double y = 0;
	Vector2d();
	Vector2d(double x, double y);
	Vector2d(mPoint& p1, mPoint& p2);
	Vector2d operator +(Vector2d& r);
	Vector2d operator -(Vector2d & r);
	Vector2d& operator /=(double& r);
	Vector2d& operator /=(int& r);
	Vector2d operator *(double& r);
	Vector2d operator *(int& r);
	Vector2d& operator *=(double r);
	double abs();
	double GetK();
	Vector2d& normalize(double newabs = 1);

};

enum mPointType
{
	none,
	gridp,
	circlep,
	trianglep
};
struct mPoint
{
	double x = 0;
	double y = 0;
	bool IsOS = false;
	bool InBaseTriangle = false;
	mPointType type = mPointType::none;
	mPoint();
	mPoint(double x, double  y, mPointType t = mPointType::none, bool bOS = false);
	mPoint operator + (mPoint& right);
	mPoint operator / (double& r);
	mPoint& operator +=(Vector2d& r);
	mPoint& operator /=(double& r);
	bool operator == (mPoint& r);
};

struct rTriangle
{
	mPoint p1, p2, p3;
	mPoint CircleCenter;
	double CircleRadius = 0;
	double ICircleRadius = 0;
	double est = 0;
	bool bOS = false;
	bool broken = false;
	bool Protected = false;
	rTriangle();
	rTriangle(mPoint& p1, mPoint& p2, mPoint& p3);
	void FillVector2d(Vector2d& v12, Vector2d& v23, Vector2d& v31);
	mPointType& type1();
	mPointType& type2();
	mPointType& type3();

	bool InCircleRange(mPoint& p);
	bool Contains(mPoint& p);
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
	bool CircleDotsInit = false;

	int RectGridConcentration = 0;

	double m_CircleRadius = 0;
	double m_CircleRadius2 = 0;

	bool FieldInit = false;

	double noiseval = 1e-10;

	mPoint OCircleCenter;
	double OCircleRadius = 0;
	mPoint ICircleCenter;
	double ICircleRadius = 0;

	bool done = false;

	double OSval = 0.85;

protected:
	bool InitCircle();
	bool InitRectGrid();
	bool bCorrectRectGridNode(mPoint& n);
	bool InitTriangle();
	void CalcTrianglesNodes(rTriangle& target);
	inline double noise();

	inline double CalcCircleRadius(mPoint& center, mPoint& p);
	inline mPoint CalcCircleCenter(mPoint& p1, mPoint& p2, mPoint& p3);
	inline double CalcICircleRadius(mPoint p1, mPoint p2, mPoint p3);
	inline mPoint CalcICircleCenter(mPoint p1, mPoint p2, mPoint p3);

	template<typename T>
	inline void swap(vector<T>& target, int id1, int id2);

	void TrianglesOS();

	bool rMakeStartGrid();
	bool GetRectGrid(vector<mPoint>&out);

	vector<int>GetBrockenTriangles(mPoint& np);
	void RemoveBrockenTriangles();
	int MarkBrockenTriangles(mPoint&p);
	vector<mPoint> CollectBrockenNodes();
	template<typename T> bool InVector(vector<T>& source, T val);

	void Triangulate(vector<mPoint>& points);

	int ProtectTriangles();
public:
	RecursiveTriangulation();
	bool SetBorders(double l, double r, double t, double b);
	void SetRectGridConc(int rgc);

	void SetTrianglesParams(mPoint tr1, mPoint tr2, double r1, double r2, double s1, double s2);
	void SetTriangleDots(int tdots);
	void SetCircleDots(int cdots);
	bool MakeStartGrid();
	vector<mPoint> GetPoints();

	bool Triangulate();
	vector<rTriangle>GetTriangles();
	double Estimate();

	mPoint GetOCircleCenter();
	double GetOCircleRadius();
	mPoint GetICircleCenter();
	double GetICircleRadius();

	bool RemoveOS();

	bool rTriangulate();
	
	bool stop = false;
	CRITICAL_SECTION cs;

};
