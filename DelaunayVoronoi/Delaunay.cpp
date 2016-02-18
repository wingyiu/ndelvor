#include <cstdio>
#include <iostream>
#include <cassert>
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
        m_tmpNewSimplices.clear();
        m_pendingDeleteSimplices.clear();
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
    bound->updateFaceBelong(bound);
    // adjacent
    for(unsigned int i=0; i<=m_dimension; i++) {
        bound->setAdjacent(faces[i], shared_ptr<Simplex>(NULL));
    }
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
        	// count face
            const Face *f = (*(*it)).getFaces();
            for(unsigned i=0; i<=m_dimension; i++)
            {
                ++m_tmpfaces[*(f+i)];
                
            }
            (*it)->willDelete = true;
            m_pendingDeleteSimplices.push_back(*it);
            //remove (*it)
            it = m_tessellation.erase(it);
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

void Delaunay::formAndAddNewSimplex(Face face, Point point)
{
#ifdef DEBUG
    face.toString();
    printf("belong simplex:%d\n", face.getSimplex()->getIndex());
#endif
    const Point *p = face.getPoints();
    Face *f = new Face[m_dimension+1];
    f[0] = face;
    f[0].setIndex();
    
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
    s->updateFaceBelong(s);
#ifdef DEBUG
    s->toString();
    printf("\n");
#endif
    //假设face原来属于D，且D与M共享face，且M不与插入点冲突，这新单形s与M共享face
#ifdef DEBUG
    f[0].toString();
    printf("\n");
#endif
    shared_ptr<Simplex> neighbour = face.getSimplex()->getAdjacent(face);
    //边界边邻居可能是空
    if (neighbour.get()) {
        assert(neighbour->willDelete == false);
        neighbour->setAdjacent(neighbour->getFace(face), s);
    }
    s->setAdjacent(f[0], neighbour);
    
    //遍历新单行，逐个查找是否有与f[i]相同的面，有则他们相邻
    for(int i=1; i<=m_dimension; i++)
    {
        list<shared_ptr<Simplex>>::iterator it;
#ifdef DEBUG
        f[i].toString();
        printf("\n");
#endif
        for(it = m_tmpNewSimplices.begin(); it != m_tmpNewSimplices.end(); ++it)
        {
            // new simplex has face f[i]
            if ((*it)->hasFace(f[i]))
            {
#ifdef DEBUG
                printf("found adjacent face");
                (*it)->getFace(f[i]).toString();
                printf("\n");
#endif
                //(*it)->setAdjacent(f[i], s);
                (*it)->setAdjacent((*it)->getFace(f[i]), s);
                s->setAdjacent(f[i], *it);
            }
        }
    }
    m_tmpNewSimplices.push_back(s);
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


