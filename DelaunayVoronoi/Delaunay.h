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
    void addPoint(Point point);
    Delaunay& addSimplex(shared_ptr<Simplex> simplex);
    const list<Point>& getBoundPoints();
    const list<Point>& getPoints();
    const list<shared_ptr<Simplex>>& getSortedCircumsphere();
    void toString();
protected:
private:
    list<Point> m_boundPoints;
    list<Point> m_dataPoints;
    unsigned m_dimension;
    //all simplices composite the tessellation
    list<shared_ptr<Simplex>> m_tessellation;
    //faces of intersected n-simplices and occur times
    unordered_map<Face, int, FaceHash, FaceEqual> m_tmpfaces;
    //new create simplices when add point
    list<shared_ptr<Simplex>> m_tmpNewSimplices;
    //
    list<shared_ptr<Simplex>> m_pendingDeleteSimplices;
    void findContainSimplices(Point point);
    bool isIntersected(Simplex simplex, Point point);
    void formAndAddNewSimplices(Point point);
    void formAndAddNewSimplex(Face face, Point point);
    bool isUnique(Point point);
};

#endif // DELAUNAY_H
