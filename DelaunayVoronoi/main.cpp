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
    //DataParser dp("/Users/user/Git/delaunay-voronoi/DelaunayVoronoi/data_2_10000.txt");
    //DataParser dp("/Users/user/Git/delaunay-voronoi/DelaunayVoronoi/data_3_10000.txt");
    //DataParser dp("/Users/user/Git/delaunay-voronoi/DelaunayVoronoi/data_4_10000.txt");
    DataParser dp("/Users/user/testdata/data_10_50.txt");
    dimension = dp.getDimension();
    //printf("@dimension:%d\n\r", dimension);
    
    struct tms tmsStart, tmsEnd;
    clock_t start, end;
    long clktck = sysconf(_SC_CLK_TCK);
    
    start = times(&tmsStart);
    
    //dimension = 8;
    //DataProducer dp(dimension);


    Delaunay del(dimension);
    del.initialization();
    
    
#ifdef DEBUG
    del.toString();
#endif

    while(dp.hasNext())
    {
        Point* p = dp.getNext();
        p->setIndex();
        del.addPoint(p);
    }

//    for(int i=0; i<50; i++)
//    {
//    	Point* p = dp.getNext();
//        p->setIndex();
//        del.addPoint(p);
//    }

    end = times(&tmsEnd);
    
    printf("cost real time: %7.2f\n", (end - start)/ (double)clktck);
    printf("Points:\n");
    list<Point*> points = del.getBoundPoints();
    list<Point*>::iterator it1;
    for (it1 = points.begin(); it1 != points.end(); it1++) {
        printf("[%d]", (*it1)->getIndex());
        (*it1)->toString();
        printf("\n");
    }
    points = del.getPoints();
    for (it1 = points.begin(); it1 != points.end(); it1++) {
        printf("[%d]", (*it1)->getIndex());
        (*it1)->toString();
        printf("\n");
    }
    printf("total created simplices count:%d\n", Simplex::getCounter());
    printf("total valid simplices count:%ld\n", del.getSimplices().size());

    list<Simplex *> tessell = del.getSimplices();
    list<Simplex *>::iterator it;
    //output the circumsphere and radii of the simplexs order by radii
    printf("Delaunay Simplexs:\n");
    for(it = tessell.begin(); it != tessell.end(); it++)
    {
        printf("[%d]CC:", (*it)->getIndex());
        (*it)->getCircumcenter()->toString();
        printf(" RD:%f ", (*it)->getSquaredRadii());
        printf(" PS:");
        (*(*it)).toString();
        printf(" AD:{");
        for (int i=0; i<=dimension; ++i) {
            Face* f = (*it)->getFaces()[i];
            Simplex* adjsimplex = (*it)->getAdjacent(f);
            f->toString();
            printf(":%d, ", adjsimplex == del.getDE() ? -1 : adjsimplex->getIndex());
        }
        printf("}");
        printf("\n");
    }
    printf("Voronoi lines:\n");
    for(it = tessell.begin(); it != tessell.end(); it++)
    {
        for (int i=0; i<=dimension; ++i) {
            (*it)->getCircumcenter()->toString();
            printf("  ");
            Face* f = (*it)->getFaces()[i];
            Simplex * adjsimplex = (*it)->getAdjacent(f);
            if (adjsimplex == del.getDE()) {
                printf("<");
                for (int j=0; j<dimension; ++j) {
                    printf("∞,");
                }
                printf(">");
            } else {
                adjsimplex->getCircumcenter()->toString();
            }
            printf("\n");
        }
    }
    
    // free all simplex
    for(it = tessell.begin(); it != tessell.end(); it++)
    {
        delete (*it);
        
    }
    return 0;
}
