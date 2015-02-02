#include "catalogpath.h"

QString CatalogPath::filePath;


CatalogPath::CatalogPath()
{

}

CatalogPath::~CatalogPath()
{

}
//*********************************************************************************
QString CatalogPath::getAbsFilePath(unsigned int position)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return "ERROR";
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line, x;

    while(!in.atEnd())
    {
        line = in.readLine();

        QFileInfoList info = QDir(line).entryInfoList(QStringList()<<"*.jpg"<<"*.jpeg"<<"*.png",
                                                                            QDir::Files, QDir::Name);
        unsigned int iterator = 0;
        bool found = false;
        foreach(const QFileInfo &inf, info)
        {
            if( iterator == position )
            {
                x = inf.absoluteFilePath();
                found = true;
                break;
            }
            ++iterator;
        }
        if(found)
            break;
    }
    file.close();
    return x;

}
//********************************************************************************
unsigned int CatalogPath::getAbsPicsQuantity()
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
                QString line = in.readLine();
                QFileInfoList info = QDir(line).entryInfoList(QStringList()<<"*.jpg"<<"*.jpeg"<<"*.png",
                                                                                        QDir::Files, QDir::Name);
                foreach(const QFileInfo &a, info)
                    ++picsQuantity;

            }
    }
    return picsQuantity;
}


