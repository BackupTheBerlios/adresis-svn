# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/client/network
# Destiono es una biblioteca:  

KDEV_QTVER = 4 
CONFIG += release \
          warn_on \
          staticlib 
QT += xml network

DLIB_DIR = ../../dlib
!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}

ADLIB_DIR = ../../lib
include($$ADLIB_DIR/lib.pri)

ADCLIB_DIR = ../adclib
include($$ADCLIB_DIR/adclib.pri)


TEMPLATE = lib 

HEADERS += adconnectpackage.h \
           addeletepackage.h \
           adinsertpackage.h \
           adpackageparser.h \
           adselectpackage.h \
           adupdatepackage.h 
SOURCES += adconnectpackage.cpp \
           addeletepackage.cpp \
           adinsertpackage.cpp \
           adpackageparser.cpp \
           adselectpackage.cpp \
           adupdatepackage.cpp 

