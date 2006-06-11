# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/server/network
# Target is a library:  

QT += xml 
KDEV_QTVER = 4 
LIBS += -ldgui \
        -ldcore \
        ../../../src/server/packages/libpackages.a 
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
