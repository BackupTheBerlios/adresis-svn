# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/server/packages
# Destiono es una biblioteca:  

KDEV_QTVER = 4 
TEMPLATE = lib 
HEADERS += schatpackage.h \
           serrorpackage.h \
           sresourcepackage.h \
           sresultset.h \
           ssuccesspackage.h 
SOURCES += schatpackage.cpp \
           serrorpackage.cpp \
           sresourcepackage.cpp \
           sresultset.cpp \
           ssuccesspackage.cpp

QT += xml
CONFIG += release \
          warn_on \
	   staticlib
		    
DLIB_DIR = ../../dlib
!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}

NETWORK_DIR = ../network
include($$NETWORK_DIR/network.pri)

ADLIB_DIR=../../lib
include($$ADLIB_DIR/lib.pri)

