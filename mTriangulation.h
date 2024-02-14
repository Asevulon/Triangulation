#pragma once
#include<vector>
#include<algorithm>
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
	double abs();
	Vector2d& normalize(double newabs = 1);
};
struct Triangle
{
	mpoint p1;
	mpoint p2;
	mpoint p3;
	Triangle();
	Triangle(mpoint p1, mpoint p2, mpoint p3);
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
