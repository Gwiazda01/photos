#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QMainWindow>
#include <QtWidgets>
#include <QLineEdit>

class CustomButton : public QPushButton
{
    Q_OBJECT
public:
    QString filePath, fileName;
    QPixmap originalPicture, picture, grayPicture;
    QBrush picBrush;
    QPalette picPalette;
    bool isGray;
public:
    explicit CustomButton(QWidget *parent);
    CustomButton();
    explicit CustomButton(QString, QWidget *parent);
    ~CustomButton();

};
#endif // CUSTOMBUTTON_H
