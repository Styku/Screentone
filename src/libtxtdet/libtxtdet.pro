TEMPLATE = lib
TARGET = txtdet
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += TXTDET_LIBRARY

SOURCES += \
        activityrecognition.cpp \
        textdetector.cpp \
        textfeatures.cpp

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += x11

HEADERS += \
    activityrecognition.h \
    helpers.h \
    textdetector.h \
    textfeatures.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
