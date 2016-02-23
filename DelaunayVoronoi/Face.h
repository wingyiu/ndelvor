#ifndef FACE_H
#define FACE_H

#include <memory>
using namespace std;

#include "Point.h"

class Simplex;

class Face
{
public:
    Face();
    Face(unsigned n);
    Face(unsigned n, Point** points);
    ~Face();
    void toString() const;
    int getPointNum() const;
    Point** getPoints()const;
    void addPoints(Point *points);
    bool operator==(const Face &rhs) const;
    int getIndex() const;
    void setIndex();
    Simplex * getSimplex();
    void setSimplex(Simplex* simplex);
    size_t getHash() const;
protected:
private:
    unsigned m_pointNum;
    int m_index;
    static int faceCounter;
    Simplex* m_simplex;
    Point** m_points;
    size_t m_hash;
};

class PointArrayDeleter
{
public:
    void operator()(Point *);
};


class FaceHash
{
public:
    std::size_t operator()(const Face* f) const;
};


class FaceEqual
{
public:
    bool operator()( const Face* lhs, const Face* rhs ) const;
};
#endif // FACE_H
