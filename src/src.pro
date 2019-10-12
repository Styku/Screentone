TEMPLATE = lib
TARGET = screentone
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += SCREENTONE_LIBRARY

SOURCES += \
#        main.cpp \
        screenparams.cpp \
        textdetector.cpp \
        textfeatures.cpp

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += x11

LIBS += -pthread

HEADERS += \
    helpers.h \
    screencap.h \
    screenparams.h \
    textdetector.h \
    textfeatures.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
