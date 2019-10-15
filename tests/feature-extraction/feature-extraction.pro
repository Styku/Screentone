QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

TEMPLATE = app

SOURCES +=  tst_featureextraction.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/libtxtdet/release/ -ltxtdet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/libtxtdet/debug/ -ltxtdet
else:unix: LIBS += -L$$OUT_PWD/../../src/libtxtdet -ltxtdet

INCLUDEPATH += $$PWD/../../src/libtxtdet
DEPENDPATH += $$PWD/../../src/libtxtdet
