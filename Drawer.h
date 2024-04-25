#pragma once


// Drawer
#include<vector>
#include<algorithm>
using namespace std;

#include"mTriangulation.h"
#include"galerkin.h"
class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)
private:
	vector<mPoint>data;
	vector<rTriangle>triangles;

	vector<rTriangle>TrianglesData;
	vector<vector<mPoint>>Isolines;
	vector<vector<mPoint>>Powerlines;
	double lPadding;
	double rPadding;
	double tPadding;
	double bPadding;
	int MarksLen;

	double left;
	double right;
	double bot;
	double top;

	int pointsize = 4;
	ULONG_PTR token;
	
	CString title;

	bool NeedOCircle = false;
	bool NeedICircle = false;

	bool OCircleInit = false;
	mPoint OCircleCenter;
	double OCircleRadius;

	bool ICircleInit = false;
	mPoint ICircleCenter;
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

	int CurrentTriangle = 0;
public:
	Drawer();
	virtual ~Drawer();

protected:
	double CalcStringLen(HDC hDC, CString string);
	Color DetermineTriangleColor(double& tr);

	void TriangulationMode(LPDRAWITEMSTRUCT);
	void UMode(LPDRAWITEMSTRUCT);
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetData(vector<mPoint>& points);
	//Отступ задается в процентах (диапазон значений от 0 до 100)
	void SetPadding(double lPadding, double rPadding, double tPadding, double bPadding);
	void SetMarksLen(int len);
	void SetTitle(CString str);

	void SetTriangles(vector<rTriangle>& in);
	void SetCircleDrawing(bool OCState, bool ICState);
	void SetOCircleParams(mPoint center, double radius);
	void SetICircleParams(mPoint center, double radius);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	void NextTriangle();
	void PrevTriangle();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	bool drawTriangulation = true;
	void SetTrianglesData(vector<rTriangle>& data);
	void SetIsolines(vector<vector<mPoint>>& data);
	void SetPowerlines(vector<vector<mPoint>>&data);
};


