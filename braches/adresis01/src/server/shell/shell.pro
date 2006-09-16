# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/server/shell
# Target is an application:  ../../../bin/adserver.bin

QT += network xml sql 
INSTALLS += target 
target.path = /bin/ 
KDEV_QTVER = 4 
TARGETDEPS += ../../../src/server/packages/libpackages.a \
              ../../../3rdparty/psql/libpsql.a \
              ../../../src/server/network/libnetwork.a 
LIBS += -ldgui \
        -ldcore \
        ../../../src/server/network/libnetwork.a \
        ../../../src/server/packages/libpackages.a \
        ../../../3rdparty/psql/libpsql.a \
        -lpq 
INCLUDEPATH += ../../../src/server/network \
               ../../../src/server/packages \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore \
               ../../../3rdparty/psql 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
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