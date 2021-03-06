# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/client/shell
# Target is an application:  ../../../bin/adclient.bin

INSTALLS += target 
target.path = /bin/ 
HEADERS += adcapplication.h \
           adresis.h \
           adconnector.h \
           adobject.h \
           aduser.h \
           adusermodulelist.h \
           aduserform.h \
           adspacemodulelist.h \
           adaudiovisualmodulelist.h \
           adaudiovisual.h \
           adspace.h \
           adcfirstrundialog.h \
           adaudiovisualform.h \
           adlistselect.h \
           adspaceform.h \
           admainwindow.h \
           adreservemodulelist.h \
           adreserveform.h \
           adschedule.h \
           adreserve.h \
           adconfigschoollmodule.h \
           adinsertresource.h \
           adevent.h 
SOURCES += main_cliente.cpp \
           adcapplication.cpp \
           adresis.cpp \
           adconnector.cpp \
           adobject.cpp \
           aduser.cpp \
           adusermodulelist.cpp \
           aduserform.cpp \
           adspacemodulelist.cpp \
           adaudiovisualmodulelist.cpp \
           adaudiovisual.cpp \
           adspace.cpp \
           adcfirstrundialog.cpp \
           adaudiovisualform.cpp \
           adlistselect.cpp \
           adspaceform.cpp \
           admainwindow.cpp \
           adreservemodulelist.cpp \
           adreserveform.cpp \
           adschedule.cpp \
           adreserve.cpp \
           adconfigschoollmodule.cpp \
           adinsertresource.cpp \
           adevent.cpp 
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
