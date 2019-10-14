TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lscreentone
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lscreentone
else:unix: LIBS += -L$$OUT_PWD/../src/ -lscreentone

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += x11

HEADERS +=
