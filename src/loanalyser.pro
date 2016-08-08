# ------------------------------------------------
#
# Project created by QtCreator 2012-05-22T15:18:55
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = loanalyser
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS   += mainwindow.ui

mac:{
LIBS    += -lqwt
}else 
unix:{
LIBS    += -lqwt -lQtSvg
}
QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_CFLAGS += $$(CFLAGS)
QMAKE_LFLAGS += $$(LDFLAGS)
