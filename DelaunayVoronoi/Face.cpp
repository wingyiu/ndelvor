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

Face::Face():m_pointNum(0), m_points((Point**)NULL)
{
    //default ctor
}

Face::Face(unsigned n):m_pointNum(n), m_points((Point**)NULL)
{
}

Face::Face(unsigned n, Point** points):m_pointNum(n), m_points(points)
{
    sort(m_points, m_points + m_pointNum, [](Point* a, Point* b){
        return a->getIndex() < b->getIndex();
    });
    
    //计算hash值
    char num_str[20] = {0};
    std::string pointSN;
    Point** ps = getPoints();
    for (int i=0; i< m_pointNum; ++i) {
        memset(num_str, 0, sizeof(num_str));
        std::sprintf(num_str, "%d", ps[i]->getIndex());
        pointSN += num_str;
    }
    m_hash =  std::hash<std::string>()(pointSN);
}

Face::~Face()
{
    //dtor
    delete [] m_points;
}


void Face::addPoints(Point* points)
{
}

int Face::getPointNum() const
{
    return m_pointNum;
}

Point** Face::getPoints() const
{
    return m_points;
}

int Face::getIndex() const
{
    return m_index;
}

void Face::setIndex()
{
    m_index = Face::faceCounter++;
}

void Face::toString() const
{
	Point** p = m_points;
	unsigned i;
    printf("[%d]<", m_index);
	for(i=0; i<m_pointNum; i++)
	{
		printf("%d,", p[i]->getIndex());
	}
    printf(">");
}


bool Face::operator==(const Face& rhs) const
{
//#ifdef DEBUG
//    printf("==");
//    toString();
//    rhs.toString();
//    printf("==\n");
//#endif
    Point** pl = m_points;
    Point** pr = rhs.getPoints();
    // 由于已经按点序号排序，直接逐一比较
//    for(unsigned i=0; i<m_pointNum; i++)
//    {
//        if (pl[i] != pr[i]){
//            return false;
//        }
//    }
//    return true;
    return memcmp(pl, pr, sizeof(Point*)*m_pointNum) == 0;
}

size_t Face::getHash() const
{
    return m_hash;
}


void PointArrayDeleter::operator()(Point *p)
{
    delete [] p;
    p = NULL;
    //printf("Point[] deleted.\n");
}


std::size_t FaceHash::operator()(const Face* f) const
{
    return f->getHash();
}

bool FaceEqual::operator()(const Face* lhs, const Face* rhs ) const
{
    return (*lhs) == (*rhs);
}

Simplex* Face::getSimplex()
{
    return m_simplex;
}

void Face::setSimplex(Simplex* simplex)
{
    m_simplex = simplex;
}


