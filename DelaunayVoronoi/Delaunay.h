#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <memory>
#include <list>
#include <utility>
#include <set>
#include <unordered_map>
using namespace std;

#include "Point.h"
#include "Face.h"
#include "Simplex.h"

class Delaunay
{
public:
    Delaunay(unsigned dimension);
    ~Delaunay();
    void initialization();
    void addPoint(Point* point);
    Delaunay& addSimplex(Simplex* simplex);
    const list<Point*>& getBoundPoints();
    const list<Point*>& getPoints();
    const list<Simplex*>& getSimplices();
    void toString();
protected:
private:
    list<Point*> m_boundPoints;
    list<Point*> m_dataPoints;
    unsigned m_dimension;
    //all simplices composite the tessellation
    list<Simplex*> m_tessellation;
    //faces of intersected n-simplices and occur times
    unordered_map<Face*, int, FaceHash, FaceEqual> m_tmpfaces;
    //new create simplices when add point
    list<Simplex*> m_tmpNewSimplices;
    //
    list<Simplex*> m_pendingDeleteSimplices;
    void findContainSimplices(Point* point);
    void formAndAddNewSimplices(Point* point);
    void formAndAddNewSimplex(Face* face, Point* point);
    bool isUnique(Point* point);
};

#endif // DELAUNAY_H
