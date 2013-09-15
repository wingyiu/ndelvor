#include "point.h"
#include "define.h"

//#include <cstddef>
#include <cstdio>
using std::printf;


Point::Point():m_dimension(0)
{
}

Point::Point(unsigned dimension, double *coordinate)
 :m_dimension(dimension), m_coordinate(coordinate, DoubleArrayDeleter())
{

}


Point::Point(unsigned dimension):m_dimension(dimension), m_coordinate((double*)NULL)
{

}


Point::~Point()
{
    //dtor
}



void Point::toString() const
{
//    printf("        Point[dimension:%d, coords:[", m_dimension);
//    for(int i = 0; i<m_dimension; i++)
//    {
//        printf("%f, ", *(m_coordinate.get()+i));
//    }
//    //printf("%d, ", getCoordinate());
//    printf("]]\n\r");
	int i;
	for(i = 0; i<m_dimension-1; i++)
	{
		printf("%f ", *(m_coordinate.get()+i));
	}
	printf("%f", *(m_coordinate.get()+i));
}

const double* Point::getCoordinate() const
{
    return m_coordinate.get();
}

const std::shared_ptr<double>& Point::getSharedPtr() const
{
    return m_coordinate;
}

bool Point::operator==(const Point& rhs)
{
    const double *a = m_coordinate.get();
    const double *b = rhs.getCoordinate();
    if(a == b)
        return true;
    else
    {
        bool isSame = true;
        for(unsigned i=0; i<m_dimension; i++)
        {
            if(*(a+i) != *(b+i))
            {
                isSame = false;
                break;
            }
        }
        if(isSame)
            return true;
        else return  false;
    }
}

void DoubleArrayDeleter::operator()(double *p)
{
    delete [] p;
    p = NULL;
    //printf("double[] deleted.\n\r");
}
