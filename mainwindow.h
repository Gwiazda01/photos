#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "custombutton.h"
#include "catalogpath.h"
#include "picturepath.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>


namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static bool appFirstStarted, catalog, picture;
    static unsigned int absolutePicsQuantity, part, picsPerPart, newPart, k ,w, page;

protected:
    void paintEvent(QPaintEvent *);
    void createButtons(unsigned int, unsigned int);
    void makeGray(QPixmap, int );
    QString getFileName();
    void openError();
    void noPicturesError();

    Interface *polimorf;
    QString abc, root;
    QPushButton *resizeButton, *nextPage, *previousPage, *acceptButton, *changePage;
    std::vector <CustomButton*> picButton;
    unsigned int x,y, picsQuantity, size, startPicsDisplay, endPicsDisplay, lastPicsPerPart;
    QLineEdit *columns, *lines, *pageEditLine;
    bool isStarted;

public slots:
    void picButtons();
    void createButtons();
    void nextButton();
    void previousButton();
    void resizeBtn();
    void acceptAction();
    void restartAction();
    void pageEditAction();
    //void changePicsPerPartAction();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
