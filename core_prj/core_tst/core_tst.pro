QT += core testlib
QT -= gui

CONFIG += c++11

TARGET = tests
CONFIG += console testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    AotfCrystalTest.cpp \
    TestCase.cpp \
    TestHarness.cpp

HEADERS += \
    AotfCrystalTest.h \
    TestCase.h \
    TestHarness.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
