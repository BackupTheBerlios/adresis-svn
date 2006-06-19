# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/client/shell
# Destiono es una aplicaci??n: ../../../bin/adclient.bin

INSTALLS += target 
target.path = /bin/ 
HEADERS += admainwindow.h \
           adcapplication.h \
           adresis.h \
           adconnector.h \
           adobject.h \
           aduser.h \
           adusermodulelist.h \
           aduserform.h \
           adspacemodulelist.h \
           adaudiovisualmodulelist.h \
           adspaceform.h \
           adaudiovisual.h \
           adspace.h 
SOURCES += admainwindow.cpp \
           main_cliente.cpp \
           adcapplication.cpp \
           adresis.cpp \
           adconnector.cpp \
           adobject.cpp \
           aduser.cpp \
           adusermodulelist.cpp \
           aduserform.cpp \
           adspacemodulelist.cpp \
           adaudiovisualmodulelist.cpp \
           adspaceform.cpp \
           adaudiovisual.cpp \
           adspace.cpp 
QT += xml network
KDEV_QTVER = 4
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
