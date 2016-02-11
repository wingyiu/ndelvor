#include <set>
#include <unordered_set>
#include <cstdio>
#include <functional>
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
//    for (int i=0; i<m_pointNum; ++i) {
//        printf("%d,", (m_points.get() + i)->getIndex());
//    }
//    printf("\n");
    sort(m_points.get(), m_points.get() + m_pointNum, [](Point &a, Point &b){
        return a.getIndex() < b.getIndex();
    });
//    for (int i=0; i<m_pointNum; ++i) {
//        printf("%d,", (m_points.get() + i)->getIndex());
//    }
//    printf("\n");
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
	for(i=0; i<m_pointNum; i++)
	{
		printf("%d,", (p+i)->getIndex());
	}
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


template<>
class FaceHash<Face>
{
public:
    std::size_t operator()(Face const& f) const
    {
        return 0;
    }
};
