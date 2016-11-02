#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:34:01
#
#-------------------------------------------------

QT     += widgets
CONFIG += c++11

TARGET = gui
TEMPLATE = lib

DEFINES += GUI_LIBRARY

OBJECTS_DIR=../obj
DESTDIR=../bin

INCLUDEPATH += ../core_prj/

SOURCES +=\
    tooling.cpp \
    MainWindow.cpp \
    ConfigurationDlg.cpp \
    CameraBtnBox.cpp \
    gui_global.cpp \
    HistogramWidget.cpp \
    SnapshotWidget.cpp \
    BaseParameterPane.cpp \
    SweepParameterPane.cpp \
    SnapshotParameterPane.cpp \
    ObservationParameterPane.cpp

HEADERS +=\
    gui_global.h \
    tooling.h \
    MainWindow.h \
    ConfigurationDlg.h \
    CameraBtnBox.h \
    HistogramWidget.h \
    SnapshotWidget.h \
    BaseParameterPane.h \
    SnapshotParameterPane.h \
    ObservationParameterPane.h \
    SweepParameterPane.h

RESOURCES += \
    resources.qrc

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../bin/ -lcore

INCLUDEPATH += $$PWD/../core_prj/core
DEPENDPATH += $$PWD/../core_prj/core
