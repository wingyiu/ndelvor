#include <cstdio>
#include <iostream>
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
		m_dataPoints.push_back(point);
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
    points[0].setIndex();
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
        pt.setIndex();
        points[i] = pt;
    }
    
    for(unsigned int i=0; i<= m_dimension; i++) {
        m_boundPoints.push_back(points[i]);
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
        ft.setIndex();
        faces[i] = ft;
    }

    shared_ptr<Simplex> bound = make_shared<Simplex>(m_dimension, faces);
    //printf("bound:0x%x\n\r", &bound);
    bound->setIndex();
    // TODO adjacent
    
    addSimplex(bound);
    
    // 测试点(1,1,1,...,1)是否被初始单形包含
//    coords = new double[m_dimension];
//    for(unsigned int j=0; j<m_dimension; j++)
//        coords[j] = 0.9;
//    Point p = Point(m_dimension, coords);
//    int contained = bound.containedPoint(&p);
//    printf("contained point: %d\n", contained);
    
    delete [] points;
}


Delaunay& Delaunay::addSimplex(shared_ptr<Simplex> simplex)
{
	//printf("2.before add to m_tessellation: 0x%x\n\r", &simplex);
    m_tessellation.push_front(simplex);
    return *this;
}


void Delaunay::findContainSimplices(Point point)
{
    list<shared_ptr<Simplex>>::iterator it;
    for(it = m_tessellation.begin(); it != m_tessellation.end(); )
    {
        if(isIntersected(*(*it), point))
        {
        	//(*it).getCircumcenter().toString();printf(" %f", (*it).getSquaredRadii());printf("\n\r");
            //add its faces to m_tmpFace
            const Face *f = (*(*it)).getFaces();
            //list<pair<Face, int> >::iterator it_tmp;
            //bool found;
            for(unsigned i=0; i<=m_dimension; i++)
            {
                ++m_tmpfaces[*(f+i)];

//                for(it_tmp = m_tmpfaces.begin(), found = false; it_tmp != m_tmpfaces.end(); it_tmp++)
//                {
//                    if((*it_tmp).first == *(f+i))
//                    {
//                        (*it_tmp).second ++;
//                        found = true;
//                        break;
//                    }
//                }
//                if(!found)
//                {
//                    pair<Face, int> p(*(f+i), 1);
//                    m_tmpfaces.push_front(p);
//                }
                
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
    //if(simplex.getSquaredRadii() > squaredDistance)
    if (simplex.getSquaredRadii() - squaredDistance > eps)
        return true;
    else if(simplex.getSquaredRadii() - squaredDistance < -eps)
        return false;
    else return true;

}


void Delaunay::formAndAddNewSimplices(Point point)
{
    //list<pair<Face, int> >::iterator it_tmp;
    //pair<Face, int> p;
    std::unordered_map<Face, int, FaceHash, FaceEqual>::iterator it_tmp;
    for(it_tmp = m_tmpfaces.begin(); it_tmp != m_tmpfaces.end(); it_tmp++ )
    {
        auto p = *(it_tmp);
        if(p.second == 1)
        {
            formAndAddNewSimplex(p.first, point);

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
        int k = 0;
        for(unsigned j=0; j<m_dimension; j++)
        {
            if(i != j)
                tp[k++] = p[j];
        }
        
        tp[k] = point;
        Face tf(m_dimension,tp);
        tf.setIndex();
        f[i+1] = tf;
    }
    shared_ptr<Simplex> s = make_shared<Simplex>(m_dimension, f);
    s->setIndex();
    addSimplex(s);
}

const list<shared_ptr<Simplex>> & Delaunay::getSortedCircumsphere()
{
    m_tessellation.sort();
    return m_tessellation;
}

const list<Point> & Delaunay::getBoundPoints()
{
    return m_boundPoints;
}

const list<Point> & Delaunay::getPoints()
{
    return m_dataPoints;
}

void Delaunay::toString()
{
    list<shared_ptr<Simplex>>::iterator it;
    for(it = m_tessellation.begin(); it != m_tessellation.end(); it++)
    {
        (*(*it)).toString();
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


