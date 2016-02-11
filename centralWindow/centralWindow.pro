#-------------------------------------------------
#
# Project created by QtCreator 2016-02-08T13:15:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = centralWindow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    finddialog.cpp \
    gotocelldialog.cpp \
    sortdialog.cpp \
    spreadsheet.cpp

HEADERS  += mainwindow.h \
    cell.h \
    finddialog.h \
    gotocelldialog.h \
    sortdialog.h \
    spreadsheet.h

FORMS    += mainwindow.ui

DISTFILES += \
    centralWindow.pro.user
