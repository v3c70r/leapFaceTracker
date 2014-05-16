#include <gmath.h>
#include <gvector3d.h>
#include <gmatrix3d.h>
#include <gpoint3d.h>

int gRound(double x)
{
	return x > 0 ? (int)(x+0.5) : (int)(x-0.5);
}

int gTrunc(double x)
{
	return (int)x;
}

double gSqr(double x)
{
	return x*x;
}

double gDistance(const GPoint3d &v0, const GPoint3d &v1)
{
	return sqrt((v1.x()-v0.x())*(v1.x()-v0.x()) + 
				(v1.y()-v0.y())*(v1.y()-v0.y()) +
				(v1.z()-v0.z())*(v1.z()-v0.z()));
}

bool gInverseMatrix(double *m, int n)
{
    int i, j, k, l, u, v;
    double d, p;
    
    int *is = new int[n];
    int *js = new int[n];
    
    for(k=0; k<=n-1; k++)
    {
        d = 0;
        for(i=k; i<=n-1; i++)
        {
            for(j=k; j<=n-1; j++)
            {
                l = i*n+j;
                p = fabs(m[l]);
                if(p > d) { d = p;  is[k] = i;  js[k] = j; }
            }
        }
        
        if(IS_ZERO(d))
        {
            delete []is;
            delete []js;
            return false;
        }
        
        if(is[k] != k)
        {
            for(j=0; j<=n-1; j++)
            {
                u = k*n+j;
                v = is[k]*n+j;
                gswap(m[u], m[v]);
            }
        }
        if(js[k] != k)
        {
            for(i=0; i<=n-1; i++)
            {
                u = i*n+k;
                v = i*n+js[k];
                gswap(m[u], m[v]);
            }
        }
        
        l = k*n+k;
        m[l] = 1/m[l];
        for(j=0; j<=n-1; j++)
        {
            if(j != k)
            {
                u = k*n+j;
                m[u] = m[u]*m[l];
            }
        }
        
        for(i=0; i<=n-1; i++)
        {
            if(i != k)
            {
                for(j=0; j<=n-1; j++)
                {
                    if(j != k)
                    {
                        u = i*n+j;
                        m[u] = m[u] - m[i*n+k]*m[k*n+j];
                    }
                }
            }
        }
        
        for(i=0; i<=n-1; i++)
        {
            if(i != k)
            {
                u = i*n+k;
                m[u] = -m[u]*m[l];
            }
        }
    }
        
    for(k=n-1; k>=0; k--)
    {
        if(js[k] != k)
        {
            for(j=0; j<=n-1; j++)
            {
                u = k*n+j;
                v = js[k]*n+j;
                gswap(m[u], m[v]);
            }
        }
        if(is[k] != k)
        {
            for(i=0; i<=n-1; i++)
            {
                u = i*n+k;
                v = i*n+is[k];
                gswap(m[u], m[v]);
            }
        }
    }
    delete []is;
    delete []js;
    return true;
}
