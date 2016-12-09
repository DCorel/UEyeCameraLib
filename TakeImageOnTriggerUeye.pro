#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T15:42:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TakeImageOnTriggerUeye
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

unix:!macx: LIBS += -L$$PWD/ -lUEyeCamera

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

unix:!macx: LIBS += -L$$PWD/../../UEyeCameraOpenCV/ -lUEyeCameraOpenCV

INCLUDEPATH += $$PWD/../../UEyeCameraOpenCV
DEPENDPATH += $$PWD/../../UEyeCameraOpenCV
