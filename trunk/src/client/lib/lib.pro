# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/client/lib
# Destiono es una biblioteca:  adlib

QT += xml network
KDEV_QTVER = 4 
INSTALLS += target 
target.path = /lib/ 
LIBS += -ldgui \
        -ldcore 
INCLUDEPATH += ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
TARGET = adlib 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += adcfirstrundialog.h \
           adcmodulelist.h \
           global.h \
           adconnectorbase.h \
           cconnectiondialog.h 
SOURCES += adcfirstrundialog.cpp \
           adcmodulelist.cpp \
           adconnectorbase.cpp \
           cconnectiondialog.cpp 
