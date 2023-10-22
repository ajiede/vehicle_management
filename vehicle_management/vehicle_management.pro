#-------------------------------------------------
#
# Project created by QtCreator 2023-09-25T22:54:14
#
#-------------------------------------------------

QT       += core gui
QT += sql multimedia multimediawidgets network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vehicle_management
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    reportform.cpp \
    mydb.cpp \
    scrollingtextwidget.cpp

HEADERS += \
        widget.h \
    reportform.h \
    mydb.h \
    scrollingtextwidget.h

FORMS += \
        widget.ui \
    reportform.ui

RESOURCES += \
    img.qrc
