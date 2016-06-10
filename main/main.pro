#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:34:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gui/release/ -lgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gui/debug/ -lgui

INCLUDEPATH += $$PWD/../gui
DEPENDPATH += $$PWD/../gui
