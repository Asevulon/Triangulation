#pragma once


// Drawer
#include<gdiplus.h>
#include<vector>
#include<algorithm>
using namespace std;
using namespace Gdiplus;

#include"mTriangulation.h"

class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)
private:
	vector<mpoint>data;
	vector<Triangle>triangles;

	double lPadding;
	double rPadding;
	double tPadding;
	double bPadding;
	int MarksLen;

	double left;
	double right;
	double bot;
	double top;

	int pointsize = 5;
	ULONG_PTR token;
	
	CString title;

	bool NeedOCircle = false;
	bool NeedICircle = false;

	bool OCircleInit = false;
	mpoint OCircleCenter;
	double OCircleRadius;

	bool ICircleInit = false;
	mpoint ICircleCenter;
	double ICircleRadius;

	double EternalScale = 1;

	bool lbLock = false;
	PointF lbLockPoint;
	
	double dx = 0;
	double dy = 0;
	double tdx = 0;
	double tdy = 0;

	PointF ScalePoint;

	int width = 0;
	int height = 0;
public:
	Drawer();
	virtual ~Drawer();

protected:
	double CalcStringLen(HDC hDC, CString string);
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetData(vector<mpoint>& points);
	//Отступ задается в процентах (диапазон значений от 0 до 100)
	void SetPadding(double lPadding, double rPadding, double tPadding, double bPadding);
	void SetMarksLen(int len);
	void SetTitle(CString str);

	void SetTriangles(vector<Triangle>& in);
	void SetCircleDrawing(bool OCState, bool ICState);
	void SetOCircleParams(mpoint center, double radius);
	void SetICircleParams(mpoint center, double radius);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


