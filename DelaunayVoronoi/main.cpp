#include <iostream>
#include <cstdio>
using namespace std;

#include <sys/times.h>
#include <unistd.h>

#include "DataParser.h"
#include "Point.h"
#include "Simplex.h"
#include "Delaunay.h"
#include "util.h"
#include "DataProducer.h"


int main()
{
#ifdef DEBUG
    printf("short:%d\n", sizeof(short));
    printf("int:%d\n", sizeof(int));
    printf("long:%d\n", sizeof(long));
    printf("float:%d\n", sizeof(float));
    printf("double:%d\n", sizeof(double));
    printf("longlong:%d\n", sizeof(long long));
#endif

    int dimension;
    
    // read the points from file
    DataParser dp("/Users/user/Git/delaunay-voronoi/DelaunayVoronoi/data_2_10000.txt");
    dimension = dp.getDimension();
    //printf("@dimension:%d\n\r", dimension);
    
    struct tms tmsStart, tmsEnd;
    clock_t start, end;
    long clktck = sysconf(_SC_CLK_TCK);
    
    start = times(&tmsStart);
    
    //dimension = 2;
    //DataProducer dp(dimension);


    Delaunay del(dimension);
    del.initialization();
    
    
#ifdef DEBUG
    del.toString();
#endif

    while(dp.hasNext())
    {
        Point p = dp.getNext();
        p.setIndex();
        del.addPoint(p);
    }
    
//    for(int i=0; i<10000; i++)
//    {
//    	Point p = dp.getNext();
//        p.setIndex();
//        del.addPoint(p);
//    }
    
    end = times(&tmsEnd);
    
    printf("cost real time: %7.2f\n", (end - start)/ (double)clktck);
    printf("Points:\n");
    list<Point> points = del.getBoundPoints();
    list<Point>::iterator it1;
    for (it1 = points.begin(); it1 != points.end(); it1++) {
        printf("[%d]", (*it1).getIndex());
        (*it1).toString();
        printf("\n");
    }
    points = del.getPoints();
    for (it1 = points.begin(); it1 != points.end(); it1++) {
        printf("[%d]", (*it1).getIndex());
        (*it1).toString();
        printf("\n");
    }
    //output the circumsphere and radii of the simplexs order by radii
    printf("Simplexs:\n");
    list<Simplex> tessell = del.getSortedCircumsphere();
    list<Simplex>::iterator it;
    for(it = tessell.begin(); it != tessell.end(); it++)
    {
        printf("[%d]CC:", (*it).getIndex());
        (*it).getCircumcenter().toString();
        printf(" RD:%f ", (*it).getSquaredRadii());
        printf(" PS:");
        (*it).toString();
        printf("\n");
    }

    return 0;
}
