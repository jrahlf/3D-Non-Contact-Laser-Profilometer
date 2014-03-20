#-------------------------------------------------
#
# Project created by QtCreator 2014-01-29T02:20:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pcDaemon2
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE += -O2

MATLAB_BASE = /home/jonas/matlab

INCLUDEPATH += $${MATLAB_BASE}/extern/include
LIBS += -L$${MATLAB_BASE}/bin/glnxa64/ $${MATLAB_BASE}/bin/glnxa64/libeng.so


SOURCES += main.cpp\
        mainwindow.cpp \
    debuglog.cpp \
    infolog.cpp \
    errorlog.cpp \
    slantCorrection.cpp

HEADERS  += mainwindow.h \
    myThread.h \
    debuglog.h \
    infolog.h \
    errorlog.h \
    slantCorrection.h

FORMS    += mainwindow.ui \
    debuglog.ui \
    infolog.ui \
    errorlog.ui
