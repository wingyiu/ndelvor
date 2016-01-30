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
    //DataParser dp("/Users/user/Git/delaunay-voronoi/DelaunayVoronoi/data.txt");
    //dimension = dp.getDimension();
    //printf("@dimension:%d\n\r", dimension);
    
    struct tms tmsStart, tmsEnd;
    clock_t start, end;
    long clktck = sysconf(_SC_CLK_TCK);
    
    start = times(&tmsStart);
    
    dimension = 6;
    DataProducer dp(dimension);


    Delaunay del(dimension);
    del.initialization();
    
    
#ifdef DEBUG
    del.toString();
#endif

    //while(dp.hasNext())
    for(int i=0; i<100; i++)
    {
    	Point p = dp.getNext();
        //Point p = dp.getNext();
        del.addPoint(p);
#ifdef DEBUG
        del.toString();
#endif
    }
    
    end = times(&tmsEnd);
    
    printf("real: %7.2f\n", (end - start)/ (double)clktck);
    
    //output the circumsphere and radii of the simplexs order by radii
    printf("Simplexs:\n");
    list<Simplex> tessell = del.getSortedCircumsphere();
    list<Simplex>::iterator it;
    for(it = tessell.begin(); it != tessell.end(); it++)
    {
        printf("CC: ");
        (*it).getCircumcenter().toString();
        printf(" R:%f\n", (*it).getSquaredRadii());
    }

    return 0;
}
