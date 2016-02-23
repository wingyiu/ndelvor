#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <memory>
#include <unordered_map>
using namespace std;

#include "Point.h"
#include "Face.h"

class Simplex
{
public:
    Simplex();
    Simplex(unsigned dimension);
    Simplex(unsigned dimension, Face** faces);
    ~Simplex();
    void toString() const;
    unsigned getDimension() const;
    Point* getCircumcenter() const;
    double getSquaredRadii() const;
    Face** getFaces() const;
    bool operator<(const Simplex& rhs) const;
    bool circumscribedPoint(Point* point) const;
    int containedPoint(Point* point) const;
    int getIndex() const;
    void setIndex();
    void setAdjacent(Face* face, Simplex* simplex);
    Simplex* getAdjacent(Face* face);
    bool hasFace(Face* face);
    Face* getFace(Face* face);
    void updateFaceBelong(Simplex*);
    bool willDelete;
protected:
private:
    unsigned m_dimension;
    double m_squareRadii;
    Point* m_circumcenter;
    int m_index;
    static int simplexCounter;
    Point** m_points;
    Face** m_faces; //array
    std::unordered_map<Face*, Simplex *, FaceHash, FaceEqual> m_adjacent;
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
