#include "mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::appFirstStarted = true;
bool MainWindow::catalog, MainWindow::picture;

unsigned int MainWindow::absolutePicsQuantity = 0;
unsigned int MainWindow::part, MainWindow::picsPerPart, MainWindow::newPart = 0;
unsigned int MainWindow::k = 5, MainWindow::w = 3, MainWindow::page;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isStarted = false;
    picsQuantity = 0;

    QRect rec = QApplication::desktop()->screenGeometry();
    x = rec.width();
    y = rec.height();
    CatalogPath abcd;
    PicturePath xyz;

    if(appFirstStarted)
    {
        QMessageBox fPath;
        fPath.setWindowTitle("Wybór danych");
        fPath.setText("Skąd pobrać zdjęcia?");
        fPath.setIcon(QMessageBox::Question);
        QPushButton *catalogButton = fPath.addButton(tr("Katalog ze zdjęciami"), QMessageBox::ActionRole);
        QPushButton *fileButton = fPath.addButton(tr("Scieżki do zdjęć"), QMessageBox::ActionRole);

        fPath.exec();
        page = 1;
        part = 1;
        if (fPath.clickedButton() == catalogButton)
        {
            abcd.filePath = getFileName();
            catalog = true;
        }

        else if (fPath.clickedButton() == fileButton)
        {
            xyz.filePath = getFileName();
            picture = true;

        }

    }
        isStarted = true;
        if(catalog)
            polimorf = &abcd;
        else if(picture)
            polimorf = &xyz;

        absolutePicsQuantity = polimorf->getAbsPicsQuantity();
        if(!absolutePicsQuantity)
        {
            noPicturesError();
        }

        if(newPart>0)
            part = newPart;

        picsPerPart = 2 * k * w;

        endPicsDisplay = part * picsPerPart;
        startPicsDisplay = endPicsDisplay - picsPerPart;


        int iterator = 0;
        QFileInfo *f;
        QString picturePath;
        for(unsigned int i = startPicsDisplay; i < endPicsDisplay && i < absolutePicsQuantity; ++i)
        {
            picturePath = polimorf->getAbsFilePath(i);
            if(picturePath == "ERROR")
                openError();
            else
            {
                f = new QFileInfo(polimorf->getAbsFilePath(i));
                picButton.push_back(new CustomButton(f->absoluteFilePath(), this));
                picButton[iterator]->fileName = f->fileName();
                delete f;
                picButton[iterator]->setObjectName(QString::number(iterator));
                ++iterator;
            }
        }


    columns = new QLineEdit(this);
    lines = new QLineEdit(this);
    pageEditLine = new QLineEdit(this);

    resizeButton = new QPushButton("Resize",this);
    nextPage = new QPushButton("Next Page",this);
    previousPage = new QPushButton("Previous Page", this);
    acceptButton = new QPushButton("Accept photos", this);
    changePage = new QPushButton("Go", this);

    lines->setGeometry(x-200,30,100,20);
    columns->setGeometry(x-320,30,100,20);
    pageEditLine->setGeometry(175,30,50,20);

    resizeButton->setGeometry(x-95,30,50,20);
    nextPage->setGeometry(x-425,30,85,20);
    previousPage->setGeometry(x-510,30,90,20);
    acceptButton->setGeometry(x-630,30,95,20);
    changePage->setGeometry(230,30,50,20);


    connect(resizeButton, SIGNAL(released()), this, SLOT(resizeBtn()));
    connect(nextPage, SIGNAL(released()), this, SLOT(nextButton()));
    connect(previousPage, SIGNAL(released()), this, SLOT(previousButton()));
    connect(acceptButton, SIGNAL(released()), this, SLOT(acceptAction()));
    connect(changePage, SIGNAL(released()), this, SLOT(pageEditAction()));

    createButtons();
    for(unsigned int i=0; i<picsPerPart && ((part-1)*picsPerPart + i < absolutePicsQuantity); ++i)
        connect(picButton[i], SIGNAL(released()), this, SLOT(picButtons()));

    QDir dir = QDir::home();
    root = dir.absolutePath();
    dir.mkpath(root + "/Zdjecia/Usuniete/");
    dir.mkpath(root + "/Zdjecia/Do rozpatrzenia/");
    dir.mkpath(root + "/Zdjecia/Zaakceptowane/");

}



MainWindow::~MainWindow()
{
    for(unsigned int i=0; i<lastPicsPerPart && ((part-1)*lastPicsPerPart + i < absolutePicsQuantity); ++i)
    {
        delete picButton[i];

    }

    delete acceptButton;
    delete resizeButton;

    delete columns;
    delete lines;
    delete nextPage;
    delete previousPage;
    delete ui;
}
//***************************************************************
void MainWindow::createButtons()
{

    if(!absolutePicsQuantity)
    {
        resizeButton->setEnabled(false);
        acceptButton->setEnabled(false);
        changePage->setEnabled(false);
    }


    if(absolutePicsQuantity%(k*w))
        size = absolutePicsQuantity/(k*w) + 1;
    else
        size = absolutePicsQuantity/(k*w);


    if(page!=1)
        previousPage->setEnabled(true);
    else
        previousPage->setEnabled(false);


    if(page<size)
        nextPage->setEnabled(true);
    else
        nextPage->setEnabled(false);



    if(k<=0 || w<=0 || k>10 || w>10)
    {
        QMessageBox::information(this,
           "File input error",
           "Bledne dane wprowadzone do funkcji CreateButtons\nDane wejsciowe musza zmiescic sie w zakresie (1; 10)"
           );
    }
    else
    {
        unsigned int bufor;
        if(page % 2)
            bufor = 1;
        else
            bufor = 2;
        for(unsigned int i=bufor*k*w, j = page * k * w ; (j < endPicsDisplay) && (j < absolutePicsQuantity); ++i, ++j)
        {
            picButton[i]->setGeometry(0,0,0,0);
        }
        for(unsigned int i=0 ; i<( (bufor-1)*k*w); ++i)
        {
            picButton[i]->setGeometry(0,0,0,0);
        }
        unsigned int index;
        int picX;
        int picY;
        float scale;
        if(k>w) scale = (float)5/(float)k;
        else scale = (float)5/(float)w;
        picX = (float)200*scale;
        picY = (float)150*scale;
        int gapX = (x-(k*picX))/(k+1);
        int gapY = (y-(w*picY))/(w+1);
        if(gapX<50)
        {
            int moreSpace =(50-gapX)*(k+1);
            picX = picX - moreSpace/k;
            gapX=50;
        }
        if(gapY<50)
        {
            int moreSpace = (50-gapY)*(w+1);
            picY = picY - moreSpace/w;
            gapY=50;
        }
        for(unsigned int i=0; i<picsPerPart && ((part-1)*picsPerPart + i < absolutePicsQuantity); ++i)
        {
            picButton[i]->picture = picButton[i]->originalPicture.scaled(picX,picY,Qt::IgnoreAspectRatio,Qt::FastTransformation);
            makeGray(picButton[i]->picture, i);
            if(!picButton[i]->isGray)
                picButton[i]->picBrush.setTexture(picButton[i]->picture);
            else
                picButton[i]->picBrush.setTexture(picButton[i]->grayPicture);
            picButton[i]->picPalette.setBrush(QPalette::Button,picButton[i]->picBrush);
            picButton[i]->setPalette(picButton[i]->picPalette);
        }

        for(unsigned int j=0; j<(unsigned int)w; ++j)
        {
            for(unsigned int i=0 ; i<(unsigned int)k; ++i)
            {
                index = i+(unsigned int)k*j+ (bufor-1)*k*w;
                if(index>=picsPerPart || ((part-1)*picsPerPart + index>=absolutePicsQuantity))
                    break;
                picButton[index]->setGeometry((gapX+i*(picX+gapX)),(gapY+j*(picY+gapY)),picX,picY);
            }
            if(index>=picsPerPart || ((part-1)*picsPerPart + index>=absolutePicsQuantity))
                break;
        }
    }
    update();
}

//****************************************************************
void MainWindow::makeGray(QPixmap pixmap, int a)
{

    QImage image = pixmap.toImage();
    QRgb col;
    int gray;
    int width = pixmap.width();
    int height = pixmap.height();
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            col = image.pixel(i, j);
            gray = qGray(col);
            image.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }
    for(int z = 0; z < width; ++z)
    for (int i = z; i < width; i+=5)
    {
        for (int j = z; j < height; j+=5)
        {
            gray = qGray(0);
            image.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }
    pixmap = pixmap.fromImage(image);
    picButton[a]->grayPicture = pixmap;

}
//******************************************************************
void MainWindow::picButtons()
{
        int a = sender()->objectName().toInt();
        QMessageBox msgBox;
        msgBox.setWindowTitle("Przeniesienie zdjęcia");
        msgBox.setText("Do jakiego folderu przeniesc zdjecie?");
        msgBox.setIcon(QMessageBox::Question);
        QPushButton *deleteButton = msgBox.addButton(tr("Usuniete"), QMessageBox::ActionRole);
        QPushButton *forConsiderationButton = msgBox.addButton(tr("Do rozpatrzenia"), QMessageBox::ActionRole);
        QPushButton *acptButton = msgBox.addButton(tr("Zaakceptowane"), QMessageBox::ActionRole);
        QPushButton *abortButton = msgBox.addButton(tr("Cofnij zmiany/Anuluj"), QMessageBox::ActionRole);

        msgBox.exec();

        picButton[a]->picBrush.setTexture(picButton[a]->grayPicture);
        picButton[a]->picPalette.setBrush(QPalette::Button,picButton[a]->picBrush);
        picButton[a]->setPalette(picButton[a]->picPalette);

/*filePath przechowuje sciezke do folderu, do ktorego zostal skopiowany obrazek, dzieki czemu bedzie mozna pozniej
cofnac zaznaczenie przyciskiem abortButton*/

        if (msgBox.clickedButton() == deleteButton)
        {
            QFile::copy(picButton[a]->filePath, (root + "/Zdjecia/Usuniete/"+picButton[a]->fileName) );
            picButton[a]->filePath = root + "/Zdjecia/Usuniete/"+picButton[a]->fileName;
            picButton[a]->isGray = true;
        }
        else if (msgBox.clickedButton() == forConsiderationButton)
        {
            QFile::copy(picButton[a]->filePath, (root + "/Zdjecia/Do rozpatrzenia/"+picButton[a]->fileName) );
            picButton[a]->filePath = root + "/Zdjecia/Do rozpatrzenia/"+picButton[a]->fileName;
            picButton[a]->isGray = true;
        }
        else if (msgBox.clickedButton() == acptButton)
        {
            QFile::copy(picButton[a]->filePath, (root + "/Zdjecia/Zaakceptowane/"+picButton[a]->fileName) );
            picButton[a]->filePath = root + "/Zdjecia/Zaakceptowane/"+picButton[a]->fileName;
            picButton[a]->isGray = true;
        }
        else if(msgBox.clickedButton() == abortButton)
        {
            picButton[a]->picBrush.setTexture(picButton[a]->picture);
            picButton[a]->picPalette.setBrush(QPalette::Button,picButton[a]->picBrush);
            picButton[a]->setPalette(picButton[a]->picPalette);
            if(picButton[a]->isGray)
                QFile::remove(picButton[a]->filePath);
            picButton[a]->isGray = false;

        }
}
//********************************************************************
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(isStarted)
    {

        painter.setFont(QFont("Arial", 14));
        painter.drawText(x-216,46, "X");
        painter.setFont(QFont("Times", 10));
        painter.drawText(x-315,28, "Columns:");
        painter.drawText(x-195,28, "Lines:");
        painter.setFont(QFont("Times", 11));
        painter.drawText(50,32,"Page:");
        painter.drawText(70,48, QString::number(page) + "/" + QString::number(size));
    }
}
//*********************************************************************
void MainWindow::nextButton()
{
        if(page!=size)
            ++page;

        if( (page * k * w) > endPicsDisplay )
        {
            newPart = part + 1;
            restartAction();
        }
        else
            createButtons();
}
//*********************************************************************
void MainWindow::previousButton()
{
    if(page!=1)
        --page;
    if( (page * k * w) <= startPicsDisplay )
    {
        newPart = part - 1;
        restartAction();
    }
    else
        createButtons();
}
//********************************************************************
void MainWindow::resizeBtn()
{
    page = 1;
    newPart = 1;
    if(!columns->text().isEmpty())
        k = columns->text().toInt();

    if(!lines->text().isEmpty())
        w = lines->text().toInt();
    restartAction();
}
//****************************************************************************
void MainWindow::acceptAction()
{
        QMessageBox acceptBox;
        acceptBox.setWindowTitle("Akceptacja zdjęć?");
        acceptBox.setText("Czy przenieść niezaznaczone zdjęcia do folderu \"Zaakceptowane\"?");
        acceptBox.setIcon(QMessageBox::Question);
        QPushButton *YesButton = acceptBox.addButton(tr("Tak"), QMessageBox::ActionRole);
        QPushButton *NoButton = acceptBox.addButton(tr("Nie"), QMessageBox::ActionRole);
        acceptBox.exec();
        if(acceptBox.clickedButton() == YesButton)
        {
                for(unsigned int i=0; i<picsPerPart && i<absolutePicsQuantity; ++i)
                {
                    if(!picButton[i]->isGray)
                    {
                        picButton[i]->picBrush.setTexture(picButton[i]->grayPicture);
                        picButton[i]->picPalette.setBrush(QPalette::Button,picButton[i]->picBrush);
                        picButton[i]->setPalette(picButton[i]->picPalette);
                        QFile::copy(picButton[i]->filePath, (root + "/Zdjecia/Zaakceptowane/"+picButton[i]->fileName) );
                        picButton[i]->filePath = root + "/Zdjecia/Zaakceptowane/"+picButton[i]->fileName;
                        picButton[i]->isGray = true;
                    }
                }
        }
        else if(acceptBox.clickedButton() == NoButton)
        {
            acceptBox.close();
        }
}
//**********************************************************************************
void MainWindow::restartAction()
{
    MainWindow::appFirstStarted = false;   
    this->close();
    lastPicsPerPart = picsPerPart;
    delete this;
    picsPerPart = 2 * k * w;
    MainWindow *wnd;
    wnd = new MainWindow();
    wnd->showMaximized();
}
//*****************************************************************************************
void MainWindow::pageEditAction()
{
    if(!pageEditLine->text().isEmpty())
    {
        unsigned int a = pageEditLine->text().toInt();
        if(a >= 1 && a != page && a <= size)
        {
            page = a;
            if( (page * k * w) <= startPicsDisplay || (page * k * w) > endPicsDisplay )
            {
                if( (page * k * w) % picsPerPart)
                    newPart = (page * k * w) / picsPerPart + 1;
                else
                    newPart = (page * k * w) / picsPerPart;
                restartAction();
            }
            else
                createButtons();
        }
    }

}
//*****************************************************************************************
void MainWindow::openError()
{
        QMessageBox::information(this,
        "File open error",
        "Nie można otworzyć pliku ze ścieżkami do zdjęć."
        );
}
//****************************************************************************************
void MainWindow::noPicturesError()
{
        QMessageBox::information(this,
        "Błąd",
        "Brak zdjęć do wyświetlenia."
        );
}

//****************************************************************************************
QString MainWindow::getFileName()
{
    QString result = QFileDialog::getOpenFileName(this, tr("Ścieżka do zdjęć"),
                                                                QDir::homePath(),
                                                                tr("Text files (*.txt);;Any file (*)"));
    return result;
}
