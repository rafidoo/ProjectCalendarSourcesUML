#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T19:06:11
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT       += xml
QT       +=widgets

TARGET = ProjectCalendar
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    agenda.cpp \
    ../ProjectCalendarasup/fenajoutereve.cpp \
    fenajoutertacheuni.cpp \
    fentachesprecedentes.cpp \
    gestionprojet.cpp \
    interface.cpp \
    main.cpp \
    mainwindow.cpp \
    timing.cpp

FORMS += \
    fenajoutereve.ui \
    fenajoutertacheuni.ui \
    fentachesprecedentes.ui \
    mainwindow.ui

HEADERS += \
    agenda.h \
    fenajoutereve.h \
    fenajoutertacheuni.h \
    fentachesprecedentes.h \
    gestionprojet.h \
    interface.h \
    mainwindow.h \
    timing.h
