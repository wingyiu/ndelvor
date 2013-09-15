#ifndef POINT_H
#define POINT_H

#include <memory>
using namespace std;

class Point
{
public:
    Point(); //for new?
    Point(unsigned dimension);
    Point(unsigned dimension, double *coordinate);
    ~Point();
    void toString() const;
    const std::shared_ptr<double>& getSharedPtr() const;
    const double * getCoordinate() const;
    bool operator==(const Point& rhs);
protected:

private:
    unsigned m_dimension;
    //double *m_coordinate;
    std::shared_ptr<double> m_coordinate;
};

//template<class T>
class DoubleArrayDeleter
{
public:
    void operator()(double *);
};

#endif // POINT_H
