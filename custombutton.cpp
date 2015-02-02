#include "custombutton.h"

CustomButton::CustomButton(QWidget * parent = 0): QPushButton(parent)
{
}
CustomButton::CustomButton()
{

}

CustomButton::CustomButton(QString a, QWidget * parent = 0): QPushButton(parent)
{    
    originalPicture.load(a);
    isGray = false;
    filePath = a;

    this->setFlat(true);
    this->setAutoFillBackground(true);
}

CustomButton::~CustomButton()
{

}

