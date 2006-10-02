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

LIB_DIR = ../lib
include($$LIB_DIR/lib.pri)
TEMPLATE = lib 

HEADERS += adconnectpackage.h \
           addeletepackage.h \
           adinsertpackage.h \
           adpackageparser.h \
           adselectpackage.h \
           adsqlpackagebase.h \
           adupdatepackage.h 
SOURCES += adconnectpackage.cpp \
           addeletepackage.cpp \
           adinsertpackage.cpp \
           adpackageparser.cpp \
           adselectpackage.cpp \
           adsqlpackagebase.cpp \
           adupdatepackage.cpp 

