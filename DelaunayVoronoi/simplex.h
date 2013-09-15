#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "point.h"
#include "face.h"
#include <memory>
//using std::std::shared_ptr;
using namespace std;

class Simplex
{
public:
    Simplex();
    Simplex(unsigned dimension);
    Simplex(unsigned dimension, Face *faces);
    ~Simplex();
    void toString() const;
    unsigned getDimension() const;
    Point getCircumcenter() const;
    double getSquaredRadii() const;
    const Face* getFaces() const;
    bool operator<(const Simplex & rhs) const;
    void addFaces(Face *faces);
protected:
private:
    unsigned m_dimension;
    double m_squareRadii;
    Point m_circumcenter;
    std::shared_ptr<Point> m_points;
    std::shared_ptr<Face> m_faces; //array

    void mergePoints();
    void calculateCircumcenter();
    void calculateSquaredRadii();
};

class FaceArrayDeleter
{
public:
    void operator()(Face *);
};
#endif // SIMPLEX_H
