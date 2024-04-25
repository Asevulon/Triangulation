#pragma once
#include<gdiplus.h>
struct mPoint;

struct Vector2d
{
	double x = 0;
	double y = 0;
	Vector2d();
	Vector2d(double x, double y);
	Vector2d(mPoint& p1, mPoint& p2);
	Vector2d operator +(Vector2d& r);
	Vector2d operator -(Vector2d& r);
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
	double fi = 0;
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
	Gdiplus::PointF AsPointF();
};

struct rTriangle
{
	mPoint p1, p2, p3;
	mPoint CircleCenter;
	double CircleRadius = 0;
	double ICircleRadius = 0;
	double est = 0;
	int pid[3] = { 0,0,0 };

	double A = 0;
	double B = 0;
	double C = 0;
	double S = 0;

	double k1 = 0;
	double k2 = 0;
	double k3 = 0;

	double b1 = 0;
	double b2 = 0;
	double b3 = 0;

	Vector2d v12;
	Vector2d v23;
	Vector2d v31;

	bool bOS = false;
	bool broken = false;
	bool Protected = false;
	rTriangle();
	rTriangle(mPoint& p1, mPoint& p2, mPoint& p3);

	mPoint& operator [] (int id);
	void FillVector2d(Vector2d& v12, Vector2d& v23, Vector2d& v31);
	mPointType& type1();
	mPointType& type2();
	mPointType& type3();

	bool InCircleRange(mPoint& p);
	bool Contains(mPoint& p);
	//≈сли треугольник содержит указанный узел то возвращает его номер, иначе -1
	int IContains(mPoint& p);

	double CalcS();
	void CalcABC(int id);
	void InitParamsForContainingCheck();
	bool NotContainsInInnerArea(mPoint& p);
};


double rand(double left, double right);


