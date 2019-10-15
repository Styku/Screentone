TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/libtxtdet/release/ -ltxtdet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/libtxtdet/debug/ -ltxtdet
else:unix: LIBS += -L$$OUT_PWD/../src/libtxtdet/ -ltxtdet

INCLUDEPATH += $$PWD/../src/libtxtdet
DEPENDPATH += $$PWD/../src/libtxtdet

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += x11

HEADERS +=
