#include <cstdio>
#include <iostream>
#include <cassert>
#include <queue>
#include "Delaunay.h"
#include "util.h"


Delaunay::Delaunay(unsigned dimension):m_dimension(dimension)
{
    //ctor
}

Delaunay::~Delaunay()
{
    //dtor
    for (auto it = m_boundPoints.begin(); it != m_boundPoints.end(); it++) {
        delete *it;
    }
    for (auto it = m_dataPoints.begin(); it != m_dataPoints.end(); it++) {
        delete *it;
    }
}


/**
 * 构造包含单位超立方的大单形
 */
void Delaunay::initialization()
{
    Point** points = new Point*[m_dimension+1];

    double *coords;
    coords = new double[m_dimension]();
    //原点
    points[0] = new Point(m_dimension, coords); //{0.0, ... , 0.0}
    points[0]->setIndex();
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
        Point* pt = new Point(m_dimension, coords);
        pt->setIndex();
        points[i] = pt;
    }
    
    for(unsigned int i=0; i<= m_dimension; i++) {
        m_boundPoints.push_back(points[i]);
    }

    Face** faces = new Face*[m_dimension+1];
    for(unsigned int i=0; i<=m_dimension; i++)
    {
        Point** pnts = new Point*[m_dimension];
        for(unsigned int j=0, k=0; j<=m_dimension; j++)
        {
            if(i != j)
                pnts[k++] = points[j];
        }

        Face* ft = new Face(m_dimension, pnts);
        ft->setIndex();
        faces[i] = ft;
    }

    Simplex* bound = new Simplex(m_dimension, faces);
    //printf("bound:0x%x\n\r", &bound);
    bound->setIndex();
    bound->updateFaceBelong(bound);
    // adjacent
    for(unsigned int i=0; i<=m_dimension; i++) {
        bound->setAdjacent(faces[i], NULL);
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


Delaunay& Delaunay::addSimplex(Simplex * simplex)
{
	//printf("2.before add to m_tessellation: 0x%x\n\r", &simplex);
    m_tessellation.push_back(simplex);
    return *this;
}


void Delaunay::addPoint(Point* point)
{
    // find n-simplices whose circumsphere contain the new point
    // flag these n-simplices to indicate deletion.
#ifdef DEBUG
    printf("Point adding:"); point->toString();printf("\n");
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
        for (auto it=m_pendingDeleteSimplices.begin(); it != m_pendingDeleteSimplices.end(); it++) {
            delete *it;
            m_tessellation.remove(*it); // 很耗时 24%
        }
        m_pendingDeleteSimplices.clear();
    }
    
}


void Delaunay::findContainSimplices(Point* point)
{
    queue<Simplex *> queue;
    // find one of the conflict simplices
    list<Simplex *>::iterator it;
    for(it = m_tessellation.begin(); it != m_tessellation.end(); it++)
    {
        if((*it)->circumscribedPoint(point))
        {
#ifdef DEBUG
            printf("found first conflict simplex\n");
#endif
        	// count face
            Face** f = (*it)->getFaces();
            for(unsigned i=0; i<=m_dimension; i++) {
                ++m_tmpfaces[f[i]];
            }
            (*it)->willDelete = true;
            m_pendingDeleteSimplices.push_back(*it);
            
            queue.push(*it);
            //stop after first was found
            break;
        }
    }
    
    while (!queue.empty()) {
        
        Simplex * s = queue.front();
#ifdef DEBUG
        printf("queue size: %lu\n", queue.size());
        printf("dealing: ");
        s->toString();
        printf("\n");
#endif
        //
        for (int i = 0; i <= m_dimension; ++i) {
            Face* f = s->getFaces()[i];
#ifdef DEBUG
            printf("dealing f[%d]", i);
            f->toString();
            printf("\n");
#endif
            Simplex* simplexPtr = s->getAdjacent(f);
            
            if (simplexPtr == NULL) {
                continue;
            }
            
            if (simplexPtr->willDelete){
                continue;
            }
            
            if(simplexPtr->circumscribedPoint(point))
            {
                // count face
                Face** fs = simplexPtr->getFaces();
                for(unsigned j=0; j<=m_dimension; j++) {
                    ++m_tmpfaces[fs[j]];
                }
                simplexPtr->willDelete = true;
                m_pendingDeleteSimplices.push_back(simplexPtr);
#ifdef DEBUG
                printf("intersected: ");
                simplexPtr->toString();
                printf("\n");
#endif
                queue.push(simplexPtr);
            }
        }
        queue.pop();
        
    }
    
}


void Delaunay::formAndAddNewSimplices(Point* point)
{
    std::unordered_map<Face*, int, FaceHash, FaceEqual>::iterator it_tmp;
    for(it_tmp = m_tmpfaces.begin(); it_tmp != m_tmpfaces.end(); it_tmp++ )
    {
        auto p = *(it_tmp);
        if(p.second == 1)
        {
            formAndAddNewSimplex(p.first, point);

        }
    }
}

void Delaunay::formAndAddNewSimplex(Face* face, Point* point)
{
#ifdef DEBUG
    face->toString();
    printf("belong simplex:%d\n", face->getSimplex()->getIndex());
#endif
    Point** p = face->getPoints();
    Face** f = new Face*[m_dimension+1];
    
    Point** f0ps = new Point*[m_dimension];
    for (int i = 0; i<m_dimension; ++i) {
        f0ps[i] = p[i];
    }
    f[0] = new Face(m_dimension, f0ps);
    f[0]->setIndex();
    
    Point** tp = NULL;
    for(unsigned i=0; i<m_dimension; i++)
    {
        tp = new Point*[m_dimension];
        int k = 0;
        for(unsigned j=0; j<m_dimension; j++)
        {
            if(i != j)
                tp[k++] = p[j];
        }
        
        tp[k] = point;
        f[i+1] = new Face(m_dimension, tp);
        f[i+1]->setIndex();
    }
    
    Simplex* s = new Simplex(m_dimension, f);
    s->setIndex();
    s->updateFaceBelong(s);
#ifdef DEBUG
    s->toString();
    printf("\n");
#endif
    //假设face原来属于D，且D与M共享face，且M不与插入点冲突，这新单形s与M共享face
#ifdef DEBUG
    f[0]->toString();
    printf("\n");
#endif
    Simplex * neighbour = face->getSimplex()->getAdjacent(face);
#ifdef DEBUG
    f[0]->toString();
    printf("\n");
#endif
    //边界边邻居可能是空
    if (neighbour != NULL) {
        assert(neighbour->willDelete == false);
        neighbour->setAdjacent(neighbour->getFace(face), s);
    }
    s->setAdjacent(f[0], neighbour);
    
    //遍历新单行，逐个查找是否有与f[i]相同的面，有则他们相邻
    for(int i=1; i<=m_dimension; i++)
    {
        list<Simplex *>::iterator it;
#ifdef DEBUG
        f[i]->toString();
        printf("\n");
#endif
        for(it = m_tmpNewSimplices.begin(); it != m_tmpNewSimplices.end(); ++it)
        {
            // new simplex has face f[i]
            if ((*it)->hasFace(f[i]))
            {
#ifdef DEBUG
                printf("found adjacent face");
                (*it)->getFace(f[i])->toString();
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

const list<Simplex *> & Delaunay::getSimplices()
{
    return m_tessellation;
}

const list<Point*>& Delaunay::getBoundPoints()
{
    return m_boundPoints;
}

const list<Point*> & Delaunay::getPoints()
{
    return m_dataPoints;
}

void Delaunay::toString()
{
    list<Simplex *>::iterator it;
    for(it = m_tessellation.begin(); it != m_tessellation.end(); it++)
    {
        (*(*it)).toString();
    }
    printf("\n");
}

bool Delaunay::isUnique(Point* point)
{
	list<Point*>::iterator it;
	bool uk = true;
	for(it = m_dataPoints.begin(); it != m_dataPoints.end(); it++)
	{
		if((*(*it)) == (*point))//or distance
		{
			uk = false;
			break;
		}
	}
	return uk;
}


