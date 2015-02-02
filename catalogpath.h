#ifndef CATALOGPATH_H
#define CATALOGPATH_H

#include "interface.h"

class CatalogPath : public Interface
{
public:
    CatalogPath();
    ~CatalogPath();
    QString getAbsFilePath(unsigned int position);
    unsigned int getAbsPicsQuantity();
    static QString filePath;

};

#endif // CATALOGPATH_H
