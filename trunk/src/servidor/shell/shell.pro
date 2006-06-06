# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/servidor/shell
# Destiono es una aplicaci??n: ../../../bin/adserver.bin

QT += xml network sql
KDEV_QTVER = 4 
INSTALLS += target 
target.path = /bin/ 
TARGETDEPS += ../../../src/servidor/network/libnetwork.a \
              ../../../src/dlib/dgui/libdgui.so \
              ../../../src/dlib/dcore/libdcore.so \
              ../../../src/servidor/packages/libpackages.a \
              ../../../3rdparty/psql/libpsql.a 
LIBS += ../../../src/servidor/network/libnetwork.a \
        -ldgui \
        -ldcore \
        ../../../src/servidor/packages/libpackages.a \
        ../../../3rdparty/psql/libpsql.a 
INCLUDEPATH += ../../../src/servidor/network \
               ../../../src/servidor/packages \
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
