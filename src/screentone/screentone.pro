TARGET = screentone
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        screenparams.cpp


unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += x11

LIBS += -pthread

HEADERS += \
    screencap.h \
    screenparams.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libtxtdet/release/ -ltxtdet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libtxtdet/debug/ -ltxtdet
else:unix: LIBS += -L$$OUT_PWD/../libtxtdet/ -ltxtdet

INCLUDEPATH += $$PWD/../libtxtdet
DEPENDPATH += $$PWD/../libtxtdet
