#-------------------------------------------------
#
# Project created by QtCreator 2016-11-24T14:32:02
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = UEyeCamera
TEMPLATE = lib

DEFINES += UEYECAMERA_LIBRARY

SOURCES += ueyecamera.cpp

HEADERS += ueyecamera.h\
        ueyecamera_global.h \
    ICamera.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix:!macx: LIBS += -lueye_api
