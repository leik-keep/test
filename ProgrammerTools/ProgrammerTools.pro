#-------------------------------------------------
#
# Project created by QtCreator 2016-07-22T09:50:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProgrammerTools
TEMPLATE = app

DESTDIR += $$PWD/Bin

CONFIG(debug, debug|release){
    contains(QMAKE_HOST.arch, x86_64){
            win32:TARGET=$$join(TARGET,,,64d)
    } else {
            win32:TARGET=$$join(TARGET,,,d)
    }
}

CONFIG(release, debug|release){
    contains(QMAKE_HOST.arch, x86_64) {
        win32:TARGET=$$join(TARGET,,,64)
    }
}

include($(GLDRS)/Glodon/shared/GLDStaticLib.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    programmertools.cpp \
    ProgrammertoolsUtils.cpp \
    programmertoolsconsts.cpp \
    CommonUtils.cpp

HEADERS  += mainwindow.h \
    programmertools.h \
    ProgrammertoolsUtils.h \
    programmertoolstyle.h \
    programmertoolsconsts.h \
    CommonUtils.h

FORMS    += mainwindow.ui

RESOURCES += \
    programmer.qrc \
    icons.qrc

DISTFILES += \
    programmer.rc

RC_FILE = \
    programmer.rc
