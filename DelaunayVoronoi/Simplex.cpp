#include <memory>
#include <cassert>
#include <cstdio>

#include "Point.h"
#include "Face.h"
#include "Simplex.h"
#include "util.h"

int Simplex::simplexCounter = 0;

Simplex::Simplex():m_dimension(0), m_faces((Face*)NULL)
{
}

Simplex::Simplex(unsigned dimension):m_dimension(dimension), m_faces((Face*)NULL)
{
    //ctor
}

Simplex::Simplex(unsigned dimension, Face* faces):m_dimension(dimension), m_faces(faces, FaceArrayDeleter())
{
    willDelete = false;
    mergePoints();
    calculateCircumcenter();
    calculateSquaredRadii();
}

Simplex::~Simplex()
{
    //dtor
}

unsigned Simplex::getDimension() const
{
    return m_dimension;
}


void Simplex::addFaces(Face* faces)
{
}

const Face* Simplex::getFaces() const
{
    return m_faces.get();
}

Point Simplex::getCircumcenter() const
{
    return m_circumcenter;
}

double Simplex::getSquaredRadii() const
{
    return m_squareRadii;
}

int Simplex::getIndex() const
{
    return m_index;
}

void Simplex::setIndex()
{
    m_index = simplexCounter++;
}

void Simplex::mergePoints()
{
    Point *p = new Point[m_dimension+1];
    for(unsigned i=0; i<m_dimension; i++)
    {
        p[i] = m_faces.get()[0].getPoints()[i];
    }

    assert(m_dimension > 1);
    
    for(unsigned i=0; i<m_dimension; i++)
    {
        Point pt = m_faces.get()[1].getPoints()[i];
        unsigned j;
        for(j=0; j<m_dimension; j++) // j !!!!
        {
            if(pt == p[j])
                break;
        }
        if(j >= m_dimension)
        {
            p[m_dimension] = pt;
            break;
        }
    }

    m_points = std::shared_ptr<Point>(p, PointArrayDeleter());
}


void Simplex::calculateCircumcenter()
{
    double *c = new double[(m_dimension+1)*m_dimension]();
    // cc用于输出
    double *cc = new double[m_dimension]();
    // m_dimension+1个点
    for(unsigned i=0; i<=m_dimension; i++)
    {
        Point p = *(m_points.get()+i);
        for(unsigned j=0; j<m_dimension; j++)
        {
            c[i*m_dimension+j] = p.getCoordinate()[j];
        }
    }
    circumcenter(c, m_dimension, cc);
    m_circumcenter = Point(m_dimension, cc);
    delete[] c;
}

void Simplex::calculateSquaredRadii()
{
    m_squareRadii = sq_distance(m_circumcenter.getCoordinate(), m_points->getCoordinate(), m_dimension); //
}

bool Simplex::operator<(const Simplex & rhs) const
{
    return m_squareRadii < rhs.getSquaredRadii();
}


void Simplex::toString() const
{
    Point *p = m_points.get();
    printf("[%d]<", m_index);
    for(unsigned i = 0; i<= m_dimension; i++)
    {
        printf("%d,", (p+i)->getIndex());
    }
    printf(">");
}

void FaceArrayDeleter::operator()(Face *p)
{
    delete [] p;
    p = NULL;
}


int Simplex::containedPoint(Point *point) const
{
    // x0 = point
    Point *x0 = point;
    
    double *Ajk = new double[m_dimension * m_dimension]();
    // 构造A11, 排除第0个点，并减去第0个点的坐标值
    Point x1 = *(m_points.get() + 0);
    for(unsigned i=1; i<= m_dimension; i++)
    {
        Point xi = *(m_points.get()+i);
        for(unsigned j=0; j<m_dimension; j++)
        {
            Ajk[(i-1)*m_dimension+j] = xi.getCoordinate()[j] - x1.getCoordinate()[j];
        }
    }
    double detA11 = det(Ajk, m_dimension);
    //printf("detA11 %lf\n", detA11);
    int signDetA11 = sign(detA11);
    printf("signDetA11 %d\n", signDetA11);
    
    int ret = 1; // inside the simplex
    int signDetAjj = signDetA11;
    // 依次求Aj0
    for (int pi = 0; pi <= m_dimension; ++pi) {
        //构造Aj0, 排除第pi个点，并减去x0的坐标值
        for (int i=0, k=0; i<=m_dimension; ++i) {
            if (i == pi) {
                continue;
            }
            Point xi = *(m_points.get()+i);
            for(unsigned j=0; j<m_dimension; j++)
            {
                Ajk[k*m_dimension+j] = xi.getCoordinate()[j] - (*x0).getCoordinate()[j];
            }
            ++k;
        }
        //求解sign(det(Aj0))
        double detAj0 = det(Ajk, m_dimension);
        //printf("detAj0 %lf\n", detAj0);
        int signDetAj0 = sign(detAj0);
        printf("signDetAj0 %d\n", signDetAj0);
        
        printf("signDetAjj %d\n", signDetAj0);
        if (signDetAj0) {
            if (signDetAj0 != signDetAjj) {
                ret = -1;// outside the simplex
                break;
            }
            signDetAjj = -signDetAjj;
        } else {
            // ==0 , at boundary
            ret = 0; // on the simplex
            break;
        }

    }
    delete [] Ajk;
    return ret;
}


void Simplex::setAdjacent(Face face, Simplex * simplex)
{
    m_adjacent[face] = simplex;
}

Simplex * Simplex::getAdjacent(Face face)
{
    auto search = m_adjacent.find(face);
    if(search != m_adjacent.end()) {
        return search->second;
    } else {
        return NULL;
    }
}

bool Simplex::hasFace(Face face)
{
    for(unsigned i=0; i<=m_dimension; i++)
    {
        if ((m_faces.get())[i] == face){
            return true;
        }
    }
    return false;
}

Face Simplex::getFace(Face face)
{
    for(unsigned i=0; i<=m_dimension; i++)
    {
        if ((m_faces.get())[i] == face){
            return (m_faces.get())[i];
        }
    }
}


void Simplex::updateFaceBelong(Simplex * simplex)
{
    for (int i=0; i<=m_dimension; ++i) {
        (m_faces.get())[i].setSimplex(simplex);
    }
}
