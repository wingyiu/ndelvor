#ifndef FACE_H
#define FACE_H

#include "point.h"
#include <memory>
using namespace std;

class Face
{
public:
    Face();
    Face(unsigned n);
    Face(unsigned n, Point *points);
    ~Face();
    void toString();
    const Point * getPoints()const;
    void addPoints(Point *points);
    bool operator==(const Face &rhs);
protected:
private:
    unsigned m_pointNum;
    std::shared_ptr<Point> m_points;
//    double *m_points;
};

class PointArrayDeleter
{
public:
    void operator()(Point *);
};

#endif // FACE_H
