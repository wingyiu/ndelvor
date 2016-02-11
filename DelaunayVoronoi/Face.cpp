#include <set>
#include <unordered_set>
#include <cstdio>
using namespace std;

#include "Face.h"

Face::Face():m_pointNum(0), m_points((Point*)NULL)
{
    //default ctor
}

Face::Face(unsigned n):m_pointNum(n), m_points((Point*)NULL)
{
}

Face::Face(unsigned n, Point* points):m_pointNum(n), m_points(points, PointArrayDeleter())
{
}

Face::~Face()
{
    //dtor
}


void Face::addPoints(Point* points)
{
}

const Point* Face::getPoints() const
{
    return m_points.get();
}


void Face::toString()
{
	Point *p = m_points.get();
	unsigned i;
	for(i=0; i<m_pointNum; i++)
	{
		printf(" ");
		(p+i)->toString();
	}
	printf("\n");
}


bool Face::operator==(const Face& rhs)
{
    const Point *pl = m_points.get();
    const Point *pr = rhs.getPoints();
    unordered_set<int> s;
    for(unsigned i=0; i<m_pointNum; i++)
    {
        s.insert((pl+i)->getIndex());
        s.insert((pr+i)->getIndex());
    }
    if(s.size() == m_pointNum)
        return true;
    else return false;
}




void PointArrayDeleter::operator()(Point *p)
{
    delete [] p;
    p = NULL;
    //printf("Point[] deleted.\n");
}
