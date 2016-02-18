#include <set>
#include <unordered_set>
#include <unordered_map>
#include <cstdio>
#include <functional>
#include <string>
#include <iostream>
#include <cstdio>
using namespace std;

#include "Face.h"

int Face::faceCounter = 0;

Face::Face():m_pointNum(0), m_points((Point*)NULL)
{
    //default ctor
}

Face::Face(unsigned n):m_pointNum(n), m_points((Point*)NULL)
{
}

Face::Face(unsigned n, Point* points):m_pointNum(n), m_points(points, PointArrayDeleter())
{
    sort(m_points.get(), m_points.get() + m_pointNum, [](Point &a, Point &b){
        return a.getIndex() < b.getIndex();
    });
}

Face::~Face()
{
    //dtor
}


void Face::addPoints(Point* points)
{
}

int Face::getPointNum() const
{
    return m_pointNum;
}

const Point* Face::getPoints() const
{
    return m_points.get();
}

int Face::getIndex() const
{
    return m_index;
}

void Face::setIndex()
{
    m_index = Face::faceCounter++;
}

void Face::toString()
{
	Point *p = m_points.get();
	unsigned i;
    printf("[%d]<", m_index);
	for(i=0; i<m_pointNum; i++)
	{
		printf("%d,", (p+i)->getIndex());
	}
    printf(">");
}


bool Face::operator==(const Face& rhs) const
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


std::size_t FaceHash::operator()(const Face & f) const
{
    char num_str[20] = {0};
    std::string pointSN;
    const Point *ps = f.getPoints();
    for (int i=0; i<f.getPointNum(); ++i) {
        memset(num_str, 0, sizeof(num_str));
        std::sprintf(num_str, "%d", ps[i].getIndex());
        pointSN += num_str;
    }
    return std::hash<std::string>()(pointSN);
}

bool FaceEqual::operator()(const Face & lhs, const Face & rhs ) const
{
    return lhs == rhs;
}

shared_ptr<Simplex> Face::getSimplex()
{
    return m_simplex;
}

void Face::setSimplex(shared_ptr<Simplex> simplex)
{
    m_simplex = simplex;
}


