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
    interface.cpp \
    mainwindow.cpp \
<<<<<<< HEAD
    fenajoutereve.cpp \
    fenajoutertacheuni.cpp \
    fentachesprecedentes.cpp
=======
    agenda.cpp
>>>>>>> origin/master

HEADERS += \
    gestionprojet.h \
    interface.h \
    mainwindow.h \
<<<<<<< HEAD
    fenajoutereve.h \
    fenajoutertacheuni.h \
    fentachesprecedentes.h
=======
    agenda.h
>>>>>>> origin/master

FORMS += \
    mainwindow.ui \
    fenajoutereve.ui \
    fenajoutertacheuni.ui \
    fentachesprecedentes.ui
