#include <cstdio>
using namespace std;
#include <armadillo>
#include "util.h"


double det(double* a, int n){
	int i, j, k, sign=0;
	double ret=1, t;
    double *b = new double[n*n];
	//if (a.n!=a.m)
	//	return 0;
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			b[i*n+j] = a[i*n+j];
    
	for (i=0; i<n; i++){
		if (zero(b[i*n+i])){
			for (j=i+1; j<n; j++)
				if (!zero(b[j*n+i]))
					break;
			
            if (j == n)
				return 0;
			
            for (k=i; k<n; k++)
				t = b[i*n+k], b[i*n+k]=b[j*n+k], b[j*n+k] = t;
			sign++;
		}
        
		ret *= b[i*n+i];
		for (k=i+1; k<n; k++)
			b[i*n+k] /= b[i*n+i];
		for (j=i+1; j<n; j++)
			for (k=i+1; k<n; k++)
				b[j*n+k] -= b[j*n+i]*b[i*n+k];
	}
    
    delete[] b;
    
	if (sign&1)
		ret = -ret;
	return ret;
}


double det_xxx(double *s, int n)
{
    int z, j, k;
    double r, total = 0;
    //int b[N][N];
    double *b = new double[(n-1)*(n-1)];
    if(n > 2)
    {
        for(z = 0; z<n; z++)
        {
            for(j=0; j<n-1; j++)
            {
                for(k=0; k<n-1; k++)
                {
                    if(k>=z)
                        //b[j][k] = s[j+1][k+1];
                        b[j*(n-1)+k] = s[(j+1)*n+(k+1)];
                    else
                        //b[j][k] = s[j+1][k];
                        b[j*(n-1)+k] = s[(j+1)*n+k];
                }
            }

                if(z%2 == 0)
                    r = s[z] * det_xxx(b, n-1);
                else
                    r = (-1)*s[z]*det_xxx(b, n-1);
                total = total + r;

        }
    }
    else if(n == 2)
    {
        total = s[0]*s[n+1] - s[1]*s[n];
    }
    delete[] b;
    return total;
}


/*
 * c:为d+1个d维点，
 * d:维数
 * cc:输出外心坐标
 */
void circumcenter(double *c, int d, double *cc)
{
    //d+1阶行列式
    double *m = new double[(d+1)*(d+1)];
    //行列式第一列填充1
    for(int i=0; i<=d; i++)
    {
        m[i*(d+1)] = 1.0;
    }
    //其他填充输入的d+1个点坐标
    for(int i=0; i<=d; i++)
        for(int j=1; j<=d; j++)
            m[i*(d+1)+j] = c[i*d+j-1];
    
    //行列式m的值
    //使用armadillo库计算行列式值
    //double D = arma::det(arma::mat(m, d+1, d+1, false));
    //double D = det_xxx(m, d+1);
    double D = det(m, d+1);
    
    // Dj[j]为替换m的第j+1列后的行列式的值
    double *Dj = new double[d];
    // 用于替代j+1列
    double *tt = new double[d+1];

    //计算tt，详情请看参考文献
    for(int i=0; i<=d; i++)
    {
        tt[i] = 0;
    }

    for(int i=0; i<=d; i++)
    {
        for(int j=0; j<d; j++)
        {
            tt[i] += c[i*d+j]*c[i*d+j];
        }
    }

    for(int i=0; i<=d; i++)
    {
        tt[i] *= 0.5;
    }

    //Dj
    for(int j=0; j<d; j++)
    {
        //替换j+1列
        for(int i=0; i<=d; i++)
        {
            m[i*(d+1)+j+1] = tt[i];
        }
        
        //行列式值
        //使用armadillo库计算行列式值
        //Dj[j] = arma::det(arma::mat(m, d+1, d+1, false));
        //Dj[j] = det_xxx(m, d+1);
        Dj[j] = det(m, d+1);
        
        //恢复j+1列
        for(int i=0; i<=d; i++)
        {
            m[i*(d+1)+j+1] = c[i*d+j];
        }
    }

    //外心点，看参考文献
    for(int j=0; j<d; j++)
    {
        cc[j] = Dj[j] / D;

    }
    delete[] m;
    delete[] Dj;
    delete[] tt;
}

double sq_distance(const double *pa, const double *pb, int d)
{
    double sq_dis = 0;
    for(int i=0; i<d; i++)
    {
        sq_dis += (pa[i] - pb[i])*(pa[i] - pb[i]);
    }
    return sq_dis;
}

int sign(double v)
{
    if (v > 0) return 1;
    if (v < 0) return -1;
    return 0;
}


