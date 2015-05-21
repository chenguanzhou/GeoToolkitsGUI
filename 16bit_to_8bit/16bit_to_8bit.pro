#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T16:12:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 16bit_to_8bit
TEMPLATE = app


SOURCES += main.cpp\
        maindialog.cpp \
    formimagetraninfo.cpp \
    qimageinfomodel.cpp

HEADERS  += maindialog.h \
    formimagetraninfo.h \
    qimageinfomodel.h

FORMS    += maindialog.ui \
    formimagetraninfo.ui
