#ifndef PICTUREPATH_H
#define PICTUREPATH_H

#include "interface.h"

class PicturePath : public Interface
{
public:
    PicturePath();
    ~PicturePath();
    QString getAbsFilePath(unsigned int position);
    unsigned int getAbsPicsQuantity();
    static QString filePath;
};

#endif // PICTUREPATH_H
