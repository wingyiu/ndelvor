#include <iostream>
#include <cstdio>
using namespace std;

#include "dataParser.h"
#include "point.h"
#include "simplex.h"
#include "delaunay.h"
#include "util.h"
#include "dataProducer.h"

#define DEBUG

int main()
{
    printf("short:%d\n\r", sizeof(short));
    printf("int:%d\n\r", sizeof(int));
    printf("long:%d\n\r", sizeof(long));
    printf("float:%d\n\r", sizeof(float));
    printf("double:%d\n\r", sizeof(double));
    printf("longlong:%d\n\r", sizeof(long long));

    int dimension;
    //DataParser dp("data.txt");
    //dimension = dp.getDimension();
    //printf("@dimension:%d\n\r", dimension);

    DataProducer dp(2);
    dimension = 2;

    Delaunay del(dimension);
    del.initialization();
#ifdef DEBUG
    del.toString();
#endif

    //while(dp.hasNext())
    for(int i=0; i<10; i++)
    {
    	Point p = dp.getNext();
        //Point p = dp.getNext();
        del.addPoint(p);
#ifdef DEBUG
        del.toString();
#endif
    }

    list<Simplex> tessell = del.getSortedCircumsphere();
    list<Simplex>::iterator it;
    for(it = tessell.begin(); it != tessell.end(); it++)
    {
        (*it).getCircumcenter().toString();printf(" %f\n\r", (*it).getSquaredRadii());
    }

    return 0;
}
