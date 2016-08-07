#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:42:46
#
#-------------------------------------------------

QT     += gui
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mockup
TEMPLATE = app

OBJECTS_DIR=../obj
DESTDIR=../bin

SOURCES += \
    mockup.cpp \
    MockCamera.cpp

HEADERS  += \
    MockCamera.h


RESOURCES += \
    camera.qrc

FORMS    +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../bin/ -lcore

INCLUDEPATH += $$PWD/../core_prj/core
DEPENDPATH += $$PWD/../core_prj/core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gui/release/ -lgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gui/debug/ -lgui
else:unix: LIBS += -L$$OUT_PWD/../bin/ -lgui

INCLUDEPATH += $$PWD/../gui
DEPENDPATH += $$PWD/../gui
