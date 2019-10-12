QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv4

TEMPLATE = app

INCDIR = ../../src
INCLUDEPATH += $$INCDIR
HEADERS += $$INCDIR/textfeatures.h
SOURCES += $$INCDIR/textfeatures.cpp

SOURCES +=  tst_featureextraction.cpp
