#include "picturepath.h"
#include <QMessageBox>

QString PicturePath::filePath;

PicturePath::PicturePath()
{

}

PicturePath::~PicturePath()
{

}
//***************************************************************
QString PicturePath::getAbsFilePath(unsigned int position)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return "ERROR";
    QTextStream in(&file);
    in.setCodec("UTF-8");
    unsigned int iterator = 0;
    QString line;

    while(!in.atEnd())
    {
        line = in.readLine();
        if(iterator == position)
            break;
        iterator ++;
    }

    file.close();
    return line;
}
//*********************************************************************
unsigned int PicturePath::getAbsPicsQuantity()
{
    unsigned int picsQuantity = 0;
    if(!filePath.isEmpty())
    {
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return 0;
        QTextStream in(&file);
        in.setCodec("UTF-8");
            while(!in.atEnd())
            {
                in.readLine();
                ++picsQuantity;
            }
    }
    return picsQuantity;
}
