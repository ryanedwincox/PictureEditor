#-------------------------------------------------
#
# Project created by QtCreator 2014-12-12T10:54:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PictureEditor
TEMPLATE = app

LIBS += -lOpenCL
LIBS += `pkg-config opencv --libs`

INCLUDEPATH += '/usr/local/cuda-6.5/include'\
        '/opt/AMDAPP/include'

SOURCES += main.cpp\
        mainwindow.cpp\
        filter.cpp \
        filterthread.cpp

HEADERS  += mainwindow.h\
        filter.h \
        filterthread.h

OTHER_FILES += cl/copy_image.cl\
        cl/low_pass.cl


