/*
 * dataProducer.h
 *
 *  Created on: 2011-10-18
 *      Author: wingyiu
 */

#ifndef DATAPRODUCER_H_
#define DATAPRODUCER_H_

#include "Point.h"

class DataProducer {
public:
	DataProducer(unsigned d);
	Point getNext();

	~DataProducer();

private:
	unsigned m_dimension;
};

#endif /* DATAPRODUCER_H_ */
