#ifndef FACE_H
#define FACE_H

#include <memory>
using namespace std;

#include "Point.h"

class Face
{
public:
    Face();
    Face(unsigned n);
    Face(unsigned n, Point *points);
    ~Face();
    void toString();
    int getPointNum() const;
    const Point * getPoints()const;
    void addPoints(Point *points);
    bool operator==(const Face &rhs) const;
    int getIndex() const;
    void setIndex();
protected:
private:
    unsigned m_pointNum;
    int m_index;
    static int faceCounter;
    std::shared_ptr<Point> m_points;
};

class PointArrayDeleter
{
public:
    void operator()(Point *);
};


class FaceHash
{
public:
    std::size_t operator()(const Face & f) const;
};


class FaceEqual
{
public:
    bool operator()( const Face& lhs, const Face& rhs ) const;
};
#endif // FACE_H
