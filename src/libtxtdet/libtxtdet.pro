TEMPLATE = lib
TARGET = txtdet
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += TXTDET_LIBRARY

SOURCES += \
        textdetector.cpp \
        textfeatures.cpp

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += x11

HEADERS += \
    helpers.h \
    textdetector.h \
    textfeatures.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
