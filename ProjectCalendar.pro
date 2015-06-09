#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T01:34:53
#
#-------------------------------------------------

QT       += core
QT       += xml
QT       +=widgets


TARGET = ProjectCalendar
CONFIG   += console
CONFIG   -= app_bundle
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder

TEMPLATE = app


SOURCES += main.cpp \
    gestionprojet.cpp \
    calendrier.cpp \
    interface.cpp \
    mainwindow.cpp \
    fenajoutereve.cpp \
    fenajoutertacheuni.cpp \
    fentachesprecedentes.cpp

HEADERS += \
    gestionprojet.h \
    calendrier.h \
    interface.h \
    mainwindow.h \
    fenajoutereve.h \
    fenajoutertacheuni.h \
    fentachesprecedentes.h

FORMS += \
    mainwindow.ui \
    fenajoutereve.ui \
    fenajoutertacheuni.ui \
    fentachesprecedentes.ui
