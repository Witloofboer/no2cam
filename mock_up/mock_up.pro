#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:42:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mock_up
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

FORMS    +=


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gui/release/ -lgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gui/debug/ -lgui
else:unix: LIBS += -L$$OUT_PWD/../gui/ -lgui

INCLUDEPATH += $$PWD/../gui
DEPENDPATH += $$PWD/../gui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
