#include"pch.h"
#include"galerkin.h"
#include<fstream>
#include<iomanip>
#define pow2(val) (val) * (val)

void Galerkin::PrintMartrix()
{
	ofstream out("matrix.txt");
	for (int i = 0; i < matr.size(); i++)
	{
		for (int j = 0; j < matr[i].size(); j++)
		{
			out << setw(10) << setfill(' ') << matr[i][j];
		}
		out << '\t' << setw(20) << setfill(' ') << bj[i];
		out << endl;
	}
}

void Galerkin::SetNodes(vector<mPoint>& n)
{
	ni.clear();
	nodes.clear();
	cn.clear();
	bn.clear();
	if (n.empty())return;

	int id_gridp = 0;
	while (n[id_gridp].type != gridp)id_gridp++;
	for (int i = id_gridp; i < n.size(); i++)
	{
		nodes.push_back(n[i]);
	}
	for (int i = 0; i < id_gridp; i++)
	{
		nodes.push_back(n[i]);
	}
}

void Galerkin::SetTriangles(vector<rTriangle>& t)
{
	triangles = t;
}

void Galerkin::SetBorderFi(double val)
{
	m_fib = val;
}

void Galerkin::SetTriabgleFi(double val)
{
	m_fitr = val;
}

vector<vector<mPoint>> Galerkin::GetIsolines()
{
	return Isolines;
}

vector<vector<mPoint>> Galerkin::GetPowerlines()
{
	return Powerlines;
}

vector<rTriangle> Galerkin::GetTriangles()
{
	return triangles;
}

void Galerkin::CalcPL()
{
	Powerlines.clear();

	int cap = 100;
	double step = 0.001 * (AreaXmax - AreaXmin);

	double xstep = 0.05 * (AreaXmax - AreaXmin);
	double ystep = 0.05 * (AreaYmax - AreaYmin);
	for(double iterx = AreaXmin; iterx < AreaXmax; iterx+=xstep)
	{
		for (double itery = AreaYmin; itery < AreaYmax; itery += ystep)
		{


			mPoint p(iterx, itery);
			vector<mPoint>Powerline;

			int ctr = 0;


			int tid = SeekForContainigTriangle(p);
			if (tid == -1)continue;

			while (ctr < cap)
			{

				double A = 0;
				double B = 0;
				double C = 0;

				CalcABCPowerlines(triangles[tid].pid, A, B, C);
				A = -A;
				//double mcos = Cos(x2 - x1, y2 - y1, z2 - z1, A, B, C);
				if (C > 0)
				{
					A = -A;
					B = -B;
				}

				double gradabs = sqrt(A * A + B * B);
				A /= gradabs;
				B /= gradabs;

				p.x -= step * A;
				p.y -= step * B;

				if (triangles[tid].NotContainsInInnerArea(p))
				{
					tid = SeekForContainigTriangle(p);
					if (tid == -1)break;
				}
				Powerline.push_back(p);

				ctr++;
			}

			Powerlines.push_back(Powerline);
		}
	}
}

void Galerkin::MatchTriangles()
{
	ni.resize(nodes.size());

	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < triangles.size(); j++)
		{
			int id = triangles[j].IContains(nodes[i]);
			if (id != -1)
			{
				ni[i].triangles.push_back(pTriangle(j, id));
				triangles[j].pid[id] = i;
			}
		}
	}
}

void Galerkin::MatchNeighbors()
{
	for (int i = 0; i < triangles.size(); i++)
	{
		ni[triangles[i].pid[0]].neighbors.push_back(pTriangle(i, triangles[i].pid[1]));
		ni[triangles[i].pid[0]].neighbors.push_back(pTriangle(i, triangles[i].pid[2]));

		ni[triangles[i].pid[1]].neighbors.push_back(pTriangle(i, triangles[i].pid[0]));
		ni[triangles[i].pid[1]].neighbors.push_back(pTriangle(i, triangles[i].pid[2]));

		ni[triangles[i].pid[2]].neighbors.push_back(pTriangle(i, triangles[i].pid[0]));
		ni[triangles[i].pid[2]].neighbors.push_back(pTriangle(i, triangles[i].pid[1]));
	}
}

void Galerkin::SeparateNeighbors()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].type == gridp)cn.push_back(i);
		else bn.push_back(i);
	}

	for (int i = 0; i < ni.size(); i++)
	{
		for (int j = 0; j < ni[i].neighbors.size(); j++)
		{
			if (nodes[ni[i].neighbors[j].pid].type == gridp)ni[i].cn.push_back(ni[i].neighbors[j]);
			else ni[i].bn.push_back(ni[i].neighbors[j]);
		}
	}
}

void Galerkin::ProcessCN()
{
	matr.resize(cn.size());
	for (int i = 0; i < matr.size(); i++)matr[i].resize(cn.size());
	
	for (int i = 0; i < cn.size(); i++)
	{
		for (int j = 0; j < cn.size(); j++)
		{
			if (cn[i] == cn[j])(OnEqualIJ(cn[i]));
			else OnNotEqualIJ(cn[i], cn[j]);
		}
	}
}

void Galerkin::OnEqualIJ(int i)
{
	double res = 0;

	for (auto& pt : ni[i].triangles)
	{
		rTriangle& ct = triangles[pt.tid];
		ct.CalcABC(pt.pid);
		res += (pow2(ct.A) + pow2(ct.B)) * ct.CalcS();
	}
	
	matr[i][i] = res;
}

void Galerkin::OnNotEqualIJ(int i, int j)
{
	if (!ni[i].NeighborCN(j))
	{
		matr[i][j] = 0;
		return;
	}

	double res = 0;

	for (auto& pt : ni[i].triangles)
	{
		rTriangle& ct = triangles[pt.tid];
		int id = ct.IContains(nodes[j]);
		if (id == -1)continue;

		ct.CalcABC(pt.pid);
		double a1 = ct.A;
		double b1 = ct.B;
		
		ct.CalcABC(id);
		double a2 = ct.A;
		double b2 = ct.B;
		
		res += (a1 * a2 + b1 * b2) * ct.CalcS();
	}
	matr[i][j] = res;
}

void Galerkin::ProcessBJ()
{
	bj.resize(cn.size());
	for (int j = 0; j < cn.size(); j++)
	{
		bj[cn[j]] = 0;

		if (ni[cn[j]].bn.empty())continue;

		for (auto& bp : ni[cn[j]].bn)
		{
			rTriangle& ct = triangles[bp.tid];
			int id = ct.IContains(nodes[cn[j]]);
			int bpid = ct.IContains(nodes[bp.pid]);
			if (id == -1)continue;

			ct.CalcABC(bpid);
			double a1 = ct.A;
			double b1 = ct.B;

			ct.CalcABC(id);
			double a2 = ct.A;
			double b2 = ct.B;

			bj[cn[j]] += (a1 * a2 + b1 * b2) * ct.CalcS() * nodes[bp.pid].fi;

		}
	}
}

void Galerkin::CalcIsolines()
{
	Isolines.clear();
	double step = max(m_fitr, m_fib) + m_fitr;
	step /= IsolanesCount + 1;
	
	for (int i = 1; i < IsolanesCount + 1; i++)
	{
		double curval = -m_fitr + i * step;
		if (curval == m_fib)curval = -m_fitr + (i + 0.001) * step;
		for (auto& triangle : triangles)
		{
			vector<mPoint>Isolane;

			auto& p1 = nodes[triangle.pid[0]];
			auto& p2 = nodes[triangle.pid[1]];
			auto& p3 = nodes[triangle.pid[2]];

			auto mnp = (p1.fi < p2.fi) ? p1 : p2;
			auto mxp = (p1.fi >= p2.fi) ? p1 : p2;
			if ((mnp.fi <= curval) && (mxp.fi >= curval))
			{
				double x = CalcC(mnp.x, mxp.x, mnp.fi, mxp.fi, curval);
				double y = CalcC(mnp.y, mxp.y, mnp.fi, mxp.fi, curval);
				Isolane.push_back(mPoint(x, y));
			}

			mnp = (p1.fi < p3.fi) ? p1 : p3;
			mxp = (p1.fi >= p3.fi) ? p1 : p3;
			if ((mnp.fi <= curval) && (mxp.fi >= curval))
			{
				double x = CalcC(mnp.x, mxp.x, mnp.fi, mxp.fi, curval);
				double y = CalcC(mnp.y, mxp.y, mnp.fi, mxp.fi, curval);
				Isolane.push_back(mPoint(x, y));
			}


			mnp = (p2.fi < p3.fi) ? p2 : p3;
			mxp = (p2.fi >= p3.fi) ? p2 : p3;
			if ((mnp.fi <= curval) && (mxp.fi >= curval))
			{
				double x = CalcC(mnp.x, mxp.x, mnp.fi, mxp.fi, curval);
				double y = CalcC(mnp.y, mxp.y, mnp.fi, mxp.fi, curval);
				Isolane.push_back(mPoint(x, y));
			}

			if (Isolane.size() == 2)Isolines.push_back(Isolane);
		}
	}
}

double Galerkin::CalcC(double x1, double x2, double T1, double T2, double val)
{
	return x2 - (T2 - val) * (x2 - x1) / (T2 - T1);
}

void Galerkin::InitTrianglesForCointainingCheck()
{
	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i].InitParamsForContainingCheck();
	}
}

int Galerkin::SeekForContainigTriangle(mPoint& target)
{
	for (int i = 0; i < triangles.size(); i++)
	{
		if (!triangles[i].NotContainsInInnerArea(target))
		{
			return i;
		}
	}
	return -1;
}

void Galerkin::CalcABCPowerlines(int* pids, double&A, double&B, double&C)
{
	mPoint& p1 = nodes[pids[0]];
	mPoint& p2 = nodes[pids[1]];
	mPoint& p3 = nodes[pids[2]];
	double x1(p1.x), x2(p2.x), x3(p3.x);
	double y1(p1.y), y2(p2.y), y3(p3.y);
	double z1(p1.fi), z2(p2.fi), z3(p3.fi);
	if (p1.type == gridp)z1 = -z1;
	if (p2.type == gridp)z2 = -z2;
	if (p3.type == gridp)z3 = -z3;
	A = (y2 - y1) * (z3 - z1) - (y3 - y1) * (z2 - z1);
	B = (x2 - x1) * (z3 - z1) - (x3 - x1) * (z2 - z1);
	C = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
}

void Galerkin::test()
{
	MatchTriangles();
	MatchNeighbors();
	SeparateNeighbors();
	SetBorderFi();
	ProcessCN();
	ProcessBJ();
	PrintMartrix();
	m_fi.resize(cn.size());
	kazf(matr, bj, m_fi);
	SetFiFromKazf();
	CalcIsolines();
	InitTrianglesForCointainingCheck();
	CalcPL();
}

void Galerkin::SetRange(double xmin, double xmax, double ymin, double ymax)
{
	AreaXmax = xmax;
	AreaXmin = xmin;
	AreaYmin = ymin;
	AreaYmax = ymax;
}

pTriangle::pTriangle()
{
}

pTriangle::pTriangle(int tid, int pid):tid(tid),pid(pid)
{
}

bool NodeInfo::Neighbor(int id)
{
	for (auto& n : neighbors)if (n.pid == id)return true;
	return false;
}

bool NodeInfo::NeighborCN(int id)
{
	for (auto& n : cn)if (n.pid == id)return true;
	return false;
}

bool NodeInfo::NeighborBN(int id)
{
	for (auto& n : bn)if (n.pid == id)return true;
	return false;
}


void Galerkin::kazf(vector<vector<double>> a, vector<double> b, vector<double>& x)
{
	// nn - êîëè÷åñòâî íåèçâåñòíûõ;  ny - êîëè÷åñòâî óðàâíåíèé
	int nn = a[0].size();
	int ny = a.size();

	double eps = 1.e-8f;
	//float s;
	int i, j, k;
	double s1, s2, fa1, t;
	vector<double> x1(x.size());

	x[0] = 0.5;
	for (i = 1; i < nn; i++)  x[i] = 0.;

	s1 = s2 = 1.;
	while (s1 > eps * s2)
	{
		for (i = 0; i < nn; i++) x1[i] = x[i];

		for (i = 0; i < ny; i++)
		{
			s1 = 0.0;
			s2 = 0.0;
			for (j = 0; j < nn; j++)
			{
				fa1 = a[i][j];
				s1 += fa1 * x[j];
				s2 += fa1 * fa1;
			}
			t = (b[i] - s1) / s2;
			for (k = 0; k < nn; k++) x[k] += a[i][k] * t;
		}

		s1 = 0.0;
		s2 = 0.0;
		for (i = 0; i < nn; i++)
		{
			s1 += (x[i] - x1[i]) * (x[i] - x1[i]);
			s2 += x[i] * x[i];
		}
		s1 = (double)sqrt(s1);
		s2 = (double)sqrt(s2);
	}
	x1.clear();
}

void Galerkin::SetFiFromKazf()
{
	for (auto& id : cn)
	{
		nodes[id].fi = m_fi[id];
	}

	

	
}

void Galerkin::SetBorderFi()
{
	for (auto& node : nodes)
	{
		if (node.type == circlep)node.fi = m_fib;
	}

	int left = cn.size();
	while (nodes[left].type != trianglep)left++;
	int actmax = (nodes.size() + left) / 2;
	for (int i = left; i < actmax; i++)
	{
		nodes[i].fi = m_fitr;
	}
	for (int i = actmax; i < nodes.size(); i++)
	{
		nodes[i].fi = -m_fitr;
	}
}
