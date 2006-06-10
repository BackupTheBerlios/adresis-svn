# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/server/network
# Destiono es una biblioteca:  

QT += xml network
KDEV_QTVER = 4 
LIBS += -ldgui \
        -ldcore \
        ../../../src/server/packages/libpackages.a \
        ../../../3rdparty/psql/libpsql.a 
INCLUDEPATH += ../../../src/server/network \
               ../../../src/server/packages \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += adserverconnection.h \
           adserverclient.h \
           adspackageparser.h \
           adsglobal.h \
           adquery.h 
SOURCES += adserverconnection.cpp \
           adserverclient.cpp \
           adspackageparser.cpp \
           adquery.cpp 
