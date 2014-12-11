#include "optimize.h"
#include "vectorN.h"
using math::vectorN;

#define TOL 2.0e-4

int    ncom;
double (*nrfunc) (vectorN const&);
static vectorN pcom, xicom;

double f1dim(double x)
{
	static vectorN xt; xt.setSize(ncom);

	for (int j=0; j<ncom; j++)
		xt[j] = pcom[j] + x * xicom[j];
	double f = (*nrfunc) (xt);

	return f;
}

void math::linmin( vectorN &p, vectorN &xi, int n, double &fret, double (*func) (vectorN const&))
{
	double xx, xmin, fx, fb, fa, bx, ax;

	ncom = n;
	pcom.setSize(n);
	xicom.setSize(n);
	nrfunc = func;

	int j;

	for (j=0; j<n; j++)
	{
		pcom[j] = p[j];
		xicom[j] = xi[j];
	}
  
	ax = 0.0;
	xx = 1.0;
	mnbrak(ax, xx, bx, fa, fx, fb, f1dim);
	fret = brent(ax, xx, bx, f1dim, TOL, xmin);
  
	for (j=0; j<n; j++)
	{
		xi[j] *= xmin;
		p[j] += xi[j];
	}
}

#define ITMAX 200

void math::gradient_descent(vectorN &p, int n, double ftol, int &iter, double &fret,
			double (*func)  (vectorN const&),
			double (*dfunc) (vectorN const&,vectorN&), int max_iter )
{
	double fp;
	static vectorN xi; xi.setSize(n);

	for (iter=0; iter < max_iter; iter++)
	{
		fp = (*dfunc) (p, xi);

		linmin(p, xi, n, fret, func);
		if (2.0 * fabs(fret - fp) <= ftol * (fabs(fret) + fabs(fp) + EPS))
   	    	return;
	}

	error("Too many iterations in gradient_descent");
}

void math::frprmn(vectorN &p, int n, double ftol, int &iter, double &fret,
			double (*func)  (vectorN const&),
			double (*dfunc) (vectorN const&,vectorN&))
{
	double gg, gam, fp, dgg;

	static vectorN g;   g.setSize(n);
	static vectorN h;   h.setSize(n);
	static vectorN xi; xi.setSize(n);
  
	fp = (*dfunc) (p, xi);

	int j;

	for (j=0; j<n; j++)
	{
		g[j] = -xi[j];
		xi[j] = h[j] = g[j];
	}

	for (iter=0; iter<ITMAX; iter++)
	{
		linmin(p, xi, n, fret, func);
		if (2.0 * fabs(fret - fp) <= ftol * (fabs(fret) + fabs(fp) + EPS)) return;
		
		fp = (*dfunc) (p, xi);
		dgg = gg = 0.0;

		for (j=0; j<n; j++)
		{
			gg += g[j] * g[j];
			dgg += (xi[j] + g[j]) * xi[j];
		}

		if (gg == 0.0) return;

		gam = dgg / gg;

		for (j=0; j<n; j++)
		{
			g[j] = -xi[j];
			xi[j] = h[j] = g[j] + gam * h[j];
		}
	}

	error("Too many iterations in frprmn");
}

#undef ITMAX

