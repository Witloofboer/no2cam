#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:34:43
#
#-------------------------------------------------

QT += gui serialport
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

OBJECTS_DIR=../obj
DESTDIR=../bin

SOURCES += \
    main.cpp \
    Camera.cpp \
    Driver.cpp \
    Generator.cpp \
    probe.cpp

HEADERS  += \
    Camera.h \
    Driver.h \
    Generator.h \
    probe.h

RESOURCES += \
    scene.qrc

FORMS    +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lcore

INCLUDEPATH += $$PWD/../core_prj
DEPENDPATH += $$PWD/../core_prj

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gui/release/ -lgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gui/debug/ -lgui

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

INCLUDEPATH += "C:/Program Files (x86)/Hamamatsu/DCAMSDK/2015-11/inc"
LIBS += -L"C:/Program Files (x86)/Hamamatsu/DCAMSDK/2015-11/lib/win32" -ldcamapi

INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"
LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc" -lTLTSP_32 -lvisa32
