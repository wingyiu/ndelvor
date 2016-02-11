#include <cstdio>
using std::printf;

#include "Point.h"
#include "define.h"

int Point::pointCounter = 0;

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
    printf("<");
	int i;
	for(i = 0; i<m_dimension-1; i++)
	{
		printf("%f, ", *(m_coordinate.get()+i));
	}
	printf("%f>", *(m_coordinate.get()+i));
}

const double* Point::getCoordinate() const
{
    return m_coordinate.get();
}

const std::shared_ptr<double>& Point::getSharedPtr() const
{
    return m_coordinate;
}

int Point::getIndex() const
{
    return m_index;
}

void Point::setIndex()
{
    m_index = Point::pointCounter++;
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

bool Point::operator<(const Point& rhs)
{
    return this->getIndex() < rhs.getIndex();
}

void DoubleArrayDeleter::operator()(double *p)
{
    delete [] p;
    p = NULL;
    //printf("double[] deleted.\n\r");
}
