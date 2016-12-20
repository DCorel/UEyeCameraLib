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

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += .
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/local/include/opencv2/core
INCLUDEPATH += /usr/local/include/opencv2/highgui
INCLUDEPATH += /usr/local/include/opencv2/imgproc
INCLUDEPATH += /usr/local/include/opencv2/flann
INCLUDEPATH += /usr/local/include/opencv2/photo
INCLUDEPATH += /usr/local/include/opencv2/video
INCLUDEPATH += /usr/local/include/opencv2/features2d
INCLUDEPATH += /usr/local/include/opencv2/objdetect
INCLUDEPATH += /usr/local/include/opencv2/calib3d
INCLUDEPATH += /usr/local/include/opencv2/ml
INCLUDEPATH += /usr/local/include/opencv2/contrib
LIBS += `pkg-config opencv --cflags --libs`
