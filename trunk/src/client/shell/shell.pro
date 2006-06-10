# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/client/shell
# Destiono es una aplicaci??n: ../../../bin/adclient.bin

QT += xml network
KDEV_QTVER = 4 
INSTALLS += target 
target.path = /bin/ 
TARGETDEPS += ../../../src/dlib/dcore/libdcore.so \
              ../../../src/dlib/dgui/libdgui.so \
              ../../../src/server/network/libnetwork.a \
              ../../../src/server/packages/libpackages.a \
              ../../../src/client/network/libnetwork.a \
              ../../../src/client/lib/libadlib.a 
LIBS += -ldcore \
        -ldgui \
        ../../../src/client/network/libnetwork.a \
        ../../../src/client/lib/libadlib.a 
INCLUDEPATH += ../../../src/client/network \
               ../../../src/client/lib \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../src/dlib/dcore \
               ../../../src/dlib/dgui 
TARGET = ../../../bin/adclient.bin 
CONFIG += release \
          warn_on 
TEMPLATE = app 
HEADERS += admainwindow.h \
           adcapplication.h \
           adresis.h \
           adconnector.h 
SOURCES += admainwindow.cpp \
           main_cliente.cpp \
           adcapplication.cpp \
           adresis.cpp \
           adconnector.cpp 
