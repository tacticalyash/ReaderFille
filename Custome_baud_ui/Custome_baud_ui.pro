#-------------------------------------------------
#
# Project created by QtCreator 2023-07-27T11:29:41
#
#-------------------------------------------------

QT       += core gui serialport
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Custome_baud_ui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    thread_conv.cpp \
    therad_convo.cpp

HEADERS  += mainwindow.h \
    thread_conv.h \
    therad_convo.h

FORMS    += mainwindow.ui
