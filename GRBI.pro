#-------------------------------------------------
#
# Project created by QtCreator 2016-09-26T15:50:31
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = $$re_escape("CallSeeker")
TEMPLATE = app

CONFIG += c++14

QMAKE_CXXFLAGS += -std=c++14 -pthread
#QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
#QMAKE_CXXFLAGS += -isystem $$PWD\include\third_party\eigen3\ -isystem $$PWD\include\third_party\eigen3\unsupported\Eigen\CXX11
#QMAKE_CXXFLAGS += -isystem $$PWD/../../mingw540/mingw64/lib/gcc/x86_64-w64-mingw32/5.4.0/include-fixed -isystem $$PWD/../../mingw540/mingw64/lib/gcc/x86_64-w64-mingw32/5.4.0/include
#QMAKE_CXXFLAGS += -isystem $$PWD\..\..\mingw730\mingw64\x86_64-w64-mingw32\include

#for (inc, QT) {
#    QMAKE_CXXFLAGS += -isystem \"$$[QT_INSTALL_HEADERS]/Qt$$system("echo $$inc | sed 's/.*/\u&/'")\"
#}

#QMAKE_CXXFLAGS += -isystem "$$[QT_INSTALL_HEADERS]/qt5"

INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/include/third_party/eigen_archive/
INCLUDEPATH += $$PWD/include/third_party/eigen3/
INCLUDEPATH += $$PWD/src/

LIBS += -L$$PWD/libs -lfftw3f -lsndfile -ltensorflow_framework -Wl,--no-as-needed -ltensorflow_cc

RC_ICONS = images/icon32px.ico

VERSION = 2.1.220430
QMAKE_TARGET_COMPANY = WavX inc.
QMAKE_TARGET_PRODUCT = "CallSeeker"
#QMAKE_TARGET_DESCRIPTION =
QMAKE_TARGET_COPYRIGHT = WavX inc.

FORMS += \
    about_window.ui

HEADERS += \
    src/about_window.h \
    src/fft.h \
    src/lineedit.h \
    src/mainwindow.h \
    src/mel.h \
    src/qsettings_window.h \
    src/settings.h \
    src/ui_mainwindow.h \
    src/ui_qsettings_window.h \
    src/utils.h \
    src/worker.h
SOURCES += \
    src/about_window.cpp \
    src/fft.cpp \
    src/lineedit.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mel.cpp \
    src/qsettings_window.cpp \
    src/worker.cpp
RESOURCES += \
    images.qrc
