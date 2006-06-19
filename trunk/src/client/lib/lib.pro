# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/client/lib
# Destiono es una biblioteca:  adlib

INSTALLS += target 
target.path = /lib/ 
HEADERS += adcfirstrundialog.h \
           adcmodulelist.h \
           global.h \
           adconnectorbase.h \
           cconnectiondialog.h \
           admodulebuttonbar.h \
           adformbase.h \
           adpermissionsview.h \
           ktthemeselector.h \
           ktpreferences.h 
SOURCES += adcfirstrundialog.cpp \
           adcmodulelist.cpp \
           adconnectorbase.cpp \
           cconnectiondialog.cpp \
           admodulebuttonbar.cpp \
           adformbase.cpp \
           adpermissionsview.cpp \
           ktthemeselector.cpp \
           ktpreferences.cpp 
QT += xml network
KDEV_QTVER = 4
LIBS += -ldgui \
-ldcore
INCLUDEPATH += ../../../src/server/packages \
../../../src/dlib/dgui \
../../../src/dlib/dcore
QMAKE_LIBDIR = ../../../src/dlib/dgui \
../../../src/dlib/dcore
TARGET = adlib
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
