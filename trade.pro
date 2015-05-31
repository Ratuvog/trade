#-------------------------------------------------
#
# Project created by QtCreator 2015-05-31T11:46:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = trade
TEMPLATE = app


SOURCES += main.cpp \
           mainwindow.cpp \
           qcustomplot.cpp \
    statisticcalculator.cpp

HEADERS  += mainwindow.h \
            qcustomplot.h \
    statisticcalculator.h

FORMS    += mainwindow.ui
