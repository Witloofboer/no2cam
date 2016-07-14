TEMPLATE = app

QT += testlib
QT -= gui

CONFIG += c++11
CONFIG += testcase
CONFIG += console
CONFIG -= app_bundle

TARGET = core_tst

INCLUDEPATH="../.."

SOURCES += main.cpp \
    AotfCrystalTest.cpp \
    TestCase.cpp \
    TestHarness.cpp

HEADERS += \
    AotfCrystalTest.h \
    TestCase.h \
    TestHarness.h


#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
#else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
#DEPENDPATH += $$PWD/../core

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a

#LIBS += -L$$OUT_PWD/../core/debug/ -lcore
LIBS += Y:\no2_cam\debug\core_prj\core\debug\libcore.a
