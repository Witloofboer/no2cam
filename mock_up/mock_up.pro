#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:42:46
#
#-------------------------------------------------

QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mock_up
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

FORMS    +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core_prj/core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core_prj/core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core_prj/core/ -lcore

INCLUDEPATH += $$PWD/../core_prj/core
DEPENDPATH += $$PWD/../core_prj/core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gui/release/ -lgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gui/debug/ -lgui
else:unix: LIBS += -L$$OUT_PWD/../gui/ -lgui

INCLUDEPATH += $$PWD/../gui
DEPENDPATH += $$PWD/../gui
