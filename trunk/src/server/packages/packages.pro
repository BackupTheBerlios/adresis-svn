# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/server/packages
# Destiono es una biblioteca:  

QT += xml network 
KDEV_QTVER = 4 
LIBS += -ldgui \
        -ldcore 
INCLUDEPATH += ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          staticlib 
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
