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
    interface.cpp

HEADERS += \
    gestionprojet.h \
    calendrier.h \
    interface.h
