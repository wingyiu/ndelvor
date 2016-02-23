/*
 * DataProducer.cpp
 *
 *  Created on: 2011-10-18
 *      Author: wingyiu
 */

#include <cstdlib>
#include <ctime>
using namespace std;

#include "DataProducer.h"

DataProducer::DataProducer(unsigned d):m_dimension(d) {
	// TODO Auto-generated constructor stub
	srand((unsigned)time(0));
}

Point* DataProducer::getNext()
{
	double *coord = new double[m_dimension]();
	for(unsigned i = 0; i<m_dimension; i++)
	{
		coord[i] = (double)rand()/(double)RAND_MAX;
	}
	return new Point(m_dimension, coord);//???eff
}

DataProducer::~DataProducer() {
	// TODO Auto-generated destructor stub
}

