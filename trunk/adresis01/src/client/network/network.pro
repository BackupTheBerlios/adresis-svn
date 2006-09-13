# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/client/network
# Destiono es una biblioteca:  

QT += xml network
KDEV_QTVER = 4 
LIBS += -ldgui \
        -ldcore \
        ../../../src/client/lib/libadlib.a 
INCLUDEPATH += ../../../src/client/lib \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          staticlib 
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
