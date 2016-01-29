#include <cstdio>

#include "Delaunay.h"
#include "util.h"


Delaunay::Delaunay(unsigned dimension):m_dimension(dimension)
{
    //ctor
}

Delaunay::~Delaunay()
{
    //dtor
}

void Delaunay::addPoint(Point point)
{
    // find n-simplices whose circumsphere contain the new point
    // flag these n-simplices to indicate deletion.
#ifdef DEBUG
	printf("Point adding:"); point.toString();printf("\n");
#endif
	//check whether the adding-point  unique
	if(isUnique(point))
	{
		//
		findContainSimplices(point);
		//
		formAndAddNewSimplices(point);
		//add the point to data point list
		m_dataPoints.push_front(point);
		//clear the m_tmpFaces list
		m_tmpfaces.clear();
	}

}

/**
 * 构造包含单位超立方的大单形
 */
void Delaunay::initialization()
{
    Point *points = new Point[m_dimension+1];

    double *coords;
    coords = new double[m_dimension]();
    //原点
    points[0] = Point(m_dimension, coords); //{0.0, ... , 0.0}
    //其他点
    for(unsigned int i=1; i<= m_dimension; i++)
    {
        coords = new double[m_dimension];
        //printf("%d\n", coords);
        for(unsigned int j=0; j<m_dimension; j++)
        {
            if(i == j+1)
                coords[j] = m_dimension;
            else coords[j] = 0.0;
        }
        Point pt = Point(m_dimension, coords);
        points[i] = pt;
    }

    Face *faces = new Face[m_dimension+1];
    for(unsigned int i=0; i<=m_dimension; i++)
    {
        Point *pnts = new Point[m_dimension];
        for(unsigned int j=0, k=0; j<=m_dimension; j++)
        {
            if(i != j)
                pnts[k++] = points[j];
        }

        Face ft = Face(m_dimension, pnts);

        faces[i] = ft;
    }

    Simplex bound(m_dimension, faces);
    //printf("bound:0x%x\n\r", &bound);
    addSimplex(bound);
    delete [] points;
}


Delaunay& Delaunay::addSimplex(Simplex simplex)
{
	//printf("2.before add to m_tessellation: 0x%x\n\r", &simplex);
    m_tessellation.push_front(simplex);
    return *this;
}


void Delaunay::findContainSimplices(Point point)
{
    list<Simplex>::iterator it;
    for(it = m_tessellation.begin(); it != m_tessellation.end(); )
    {
        if(isIntersected((*it), point))
        {
        	//(*it).getCircumcenter().toString();printf(" %f", (*it).getSquaredRadii());printf("\n\r");
            //add its faces to m_tmpFace
            const Face *f = (*it).getFaces();
            list<pair<Face, int> >::iterator it_tmp;
            bool found;
            for(unsigned i=0; i<=m_dimension; i++)
            {

                for(it_tmp = m_tmpfaces.begin(), found = false; it_tmp != m_tmpfaces.end(); it_tmp++)
                {
                    if((*it_tmp).first == *(f+i))
                    {
                        (*it_tmp).second ++;
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    pair<Face, int> p(*(f+i), 1);
                    m_tmpfaces.push_front(p);
                }
            }
            //delete (*it)
            it = m_tessellation.erase(it); //
        }
        else
        {
        	it++;
        }
    }
}


bool Delaunay::isIntersected(Simplex simplex, Point point)
{
    Point cc = simplex.getCircumcenter();
#ifdef DEBUG
    cc.toString();
    printf(" %f", simplex.getSquaredRadii());
    printf("\n");
#endif
    double squaredDistance;
    squaredDistance = sq_distance(cc.getCoordinate(), point.getCoordinate(), m_dimension);
    // TODO change code blow
    //fabs(simplex.m_squareRadii - squaredDistance) > EPS
    if(simplex.getSquaredRadii() > squaredDistance) //eps = 0.0000000000000000000000001
        return true;
    else if(simplex.getSquaredRadii() < squaredDistance)
        return false;
    else return true;

}


void Delaunay::formAndAddNewSimplices(Point point)
{
    list<pair<Face, int> >::iterator it_tmp;
    pair<Face, int> p;
    for(it_tmp = m_tmpfaces.begin(); it_tmp != m_tmpfaces.end(); it_tmp++ )
    {
        p = *(it_tmp);
//        p.first.toString();
//        printf("%d\n\r", p.second);
        if(p.second == 1)
        {
            formAndAddNewSimplex(p.first,point);

        }
    }
}

void Delaunay::formAndAddNewSimplex(Face face,Point point)
{
    const Point *p = face.getPoints();
    Face *f = new Face[m_dimension+1];
    f[0] = face;
    Point *tp = NULL;
    for(unsigned i=0; i<m_dimension; i++)
    {
        tp = new Point[m_dimension];
        tp[0] = point;
        for(unsigned j=0, k=1; j<m_dimension; j++)
        {
            if(i != j)
                tp[k++] = p[j];
        }
        Face tf(m_dimension,tp);
        f[i+1] = tf;
    }
    Simplex s(m_dimension,f);
    //printf("1.before add to m_tessellation: 0x%x\n\r", &s);
    //s.toString();
    addSimplex(s);
}

const list<Simplex> & Delaunay::getSortedCircumsphere()
{
    m_tessellation.sort();
    return m_tessellation;

}



void Delaunay::toString()
{
    list<Simplex>::iterator it;
    for(it = m_tessellation.begin(); it != m_tessellation.end(); it++)
    {
        (*it).toString();
        //printf("dimension:%d\n\r[", s.getDimension());
        //Face *f = s.
        //for()
    }
    printf("\n");
}

bool Delaunay::isUnique(Point point)
{
	list<Point>::iterator it;
	bool uk = true;
	for(it = m_dataPoints.begin(); it != m_dataPoints.end(); it++)
	{
		if((*it) == point)//or distance
		{
			uk = false;
			break;
		}
	}
	return uk;
}


