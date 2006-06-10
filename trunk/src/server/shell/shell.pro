# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/server/shell
# Destiono es una aplicaci??n: ../../../bin/adserver.bin

QT += xml network sql
KDEV_QTVER = 4 
INSTALLS += target 
target.path = /bin/ 
LIBS += -ldgui \
        -ldcore \
        ../../../src/server/network/libnetwork.a \
        ../../../src/server/packages/libpackages.a \
        ../../../3rdparty/psql/libpsql.a 
INCLUDEPATH += ../../../3rdparty/psql \
               ../../../src/server/network \
               ../../../src/server/packages \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
TARGET = ../../../bin/adserver.bin 
CONFIG += release \
          warn_on 
TEMPLATE = app 
HEADERS += adserver.h \
           postgreserrorhandler.h \
           sdatabase.h 
SOURCES += adserver.cpp \
           server_main.cpp \
           postgreserrorhandler.cpp \
           sdatabase.cpp 
