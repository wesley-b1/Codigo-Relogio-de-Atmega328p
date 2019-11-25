#-------------------------------------------------
#
# Project created by QtCreator 2019-11-08T15:20:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = formaGeometrica
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cutellipsoid.cpp \
    cutsphere.cpp \
    cutvoxel.cpp \
    sculptor.cpp \
    figurageometrica.cpp

HEADERS  += mainwindow.h \
    cutbox.h \
    cutellipsoid.h \
    cutsphere.h \
    cutvoxel.h \
    sculptor.h \
    figurageometrica.h

FORMS    += mainwindow.ui

RESOURCES += \
    imagens.qrc
