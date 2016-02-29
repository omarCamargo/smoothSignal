#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T12:54:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pruebaSmooth
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


    INCLUDEPATH += /usr/local/include/


    LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_photo -lopencv_video -lopencv_features2d
