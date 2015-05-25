#-------------------------------------------------
#
# Project created by QtCreator 2015-05-22T09:16:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ECWGenerator
TEMPLATE = app


SOURCES += main.cpp\
        ecwdialog.cpp\
        ../Common/geodialog.cpp \
    ecwthread.cpp

HEADERS  += ecwdialog.h\
        ../Common/geodialog.h \
    ecwthread.h

FORMS    += \
        ../Common/geodialog.ui


win32: LIBS += -L$$PWD/../../../../../../../FWTools2.4.7/lib/ -lgdal_i

INCLUDEPATH += $$PWD/../../../../../../../FWTools2.4.7/include
DEPENDPATH += $$PWD/../../../../../../../FWTools2.4.7/include
