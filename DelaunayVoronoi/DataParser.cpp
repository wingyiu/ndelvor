#include "DataParser.h"



bool DataParser::hasNext()
{
    if(feof(m_file) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}


Point DataParser::getNext()
{
    double *coord = new double[m_dimension];
    for(int i=0; i<m_dimension; i++)
    {
        fscanf(m_file, "%lf", coord+i);
        fscanf(m_file, "%*[ ,\n]");
    }
    Point p(m_dimension, coord);
    return p;
}


unsigned DataParser::getDimension()
{
    //int d = 0;
    fscanf(m_file, " @dimension:%d", &m_dimension);
    return m_dimension;
}


bool DataParser::isFileValid()
{
    return false;
}


 DataParser::~DataParser()
{
    if(m_file != NULL)
    {
        fclose(m_file);
    }
}


 DataParser::DataParser(const char *filename):m_file(NULL), m_filename(filename), m_dimension(0)
{
    m_file = fopen(m_filename, "r");
}


 DataParser::DataParser(FILE *file):m_file(file), m_filename(NULL), m_dimension(0)
{

}


 DataParser::DataParser():m_file(NULL), m_filename(NULL), m_dimension(0)
{

}

