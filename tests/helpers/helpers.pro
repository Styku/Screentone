QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += tst_helpers.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/release/ -lscreentone
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/debug/ -lscreentone
else:unix: LIBS += -L$$OUT_PWD/../../src/ -lscreentone

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src
