#-------------------------------------------------
#
# Project created by QtCreator 2014-12-06T11:12:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photos
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    custombutton.cpp \
    interface.cpp \
    catalogpath.cpp \
    picturepath.cpp

HEADERS  += mainwindow.h \
    custombutton.h \
    interface.h \
    catalogpath.h \
    picturepath.h

FORMS    += mainwindow.ui

RESOURCES += \
    pics.qrc
