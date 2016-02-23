#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <cstdio>
using std::FILE;

#include "Point.h"

class DataParser
{
public:
    DataParser();
    DataParser(FILE *file);
    DataParser(const char *filename);
    ~DataParser();
    bool isFileValid();
    unsigned getDimension();
    bool hasNext();
    Point* getNext();

protected:
private:
    FILE *m_file;
    const char *m_filename;
    unsigned m_dimension;
};

#endif // DATAPARSER_H
