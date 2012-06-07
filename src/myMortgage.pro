# ------------------------------------------------
#
# Project created by QtCreator 2012-05-22T15:18:55
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = myMortgage
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

mac:{
LIBS    += -L/usr/local/qwt-6.0.1/lib -lqwt
}else unix:{
LIBS    += -L/usr/local/qwt-6.0.1/lib -lqwt -lQtSvg
}
INCLUDEPATH += /usr/local/qwt-6.0.1/include
