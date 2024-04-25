#pragma once
#include"Global.h"
#include<vector>
using namespace std;


struct pTriangle
{
	int tid = 0;
	int pid = 0;
	pTriangle();
	pTriangle(int tid,int pid);
};
struct NodeInfo
{
	vector<pTriangle>neighbors;
	vector<pTriangle>triangles;
	vector<pTriangle>bn;
	vector<pTriangle>cn;
	bool Neighbor(int id);
	bool NeighborCN(int id);
	bool NeighborBN(int id);
};
class Galerkin
{
private:
	const int IsolanesCount = 10;

	vector<mPoint> nodes;
	vector<rTriangle>triangles;
	vector<NodeInfo>ni;
	vector<int>bn;
	vector<int>cn;

	vector<vector<double>>matr;
	vector<double>bj;
	double m_fitr = 1;
	double m_fib = 1;
	
	vector<double>m_fi;
	vector<vector<mPoint>>Isolines;
	vector<vector<mPoint>>Powerlines;

	double AreaXmin = 0;
	double AreaXmax = 0;
	double AreaYmin = 0; 
	double AreaYmax = 0;
protected:
	void PrintMartrix();
	void MatchTriangles();
	void MatchNeighbors();
	void SeparateNeighbors();
	void ProcessCN();
	void OnEqualIJ(int i);
	void OnNotEqualIJ(int i, int j);
	void ProcessBJ();
	//карчмарж
	void kazf(vector<vector<double>> a, vector<double> b, vector<double>& x);
	void SetFiFromKazf();
	void SetBorderFi();
	double Galerkin::CalcC(double x1, double x2, double T1, double T2, double val);
	void InitTrianglesForCointainingCheck();
	//возвращает -1 если не найден треугольник, в котором есть заданная точка
	int SeekForContainigTriangle(mPoint& target);
	void CalcABCPowerlines(int* pids, double& A, double& B, double& C);
public:
	void SetNodes(vector<mPoint>& n);
	void SetTriangles(vector<rTriangle>& t);
	void SetBorderFi(double val);
	void SetTriabgleFi(double val);
	vector<vector<mPoint>>GetIsolines();
	vector<vector<mPoint>>GetPowerlines();
	vector<rTriangle>GetTriangles();
	void CalcPL();
	void CalcIsolines();
	void test();
	void SetRange(double xmin, double xmax, double ymin, double ymax);
};