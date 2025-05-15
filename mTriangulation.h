#pragma once
#include<vector>
#include<algorithm>
#include<gdiplus.h>
using namespace Gdiplus;

using namespace std;
#include"Global.h"





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

	mPoint circlesOS1;
	mPoint circlesOS2;
	double circlesOSRadius2 = 0;
protected:
	bool InitCircle();
	bool InitRectGrid();
	bool bCorrectRectGridNode(mPoint& n);
	bool bCorrectRectGridNodeCircles(mPoint& n);
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
	void CircleOS();

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

	bool InitCircles();
};
