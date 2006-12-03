# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/lib
# Target is a library:  adlib

INSTALLS += target 
target.path = /lib 
HEADERS += adevent.h \
           adeventhandler.h \
           global.h \
           adobject.h \
           adaudiovisual.h \
           adpermission.h \
           adschedule.h \
           adspace.h \
           aduser.h \
           adeventfactory.h \
           adreserve.h \
           adcancellation.h 
SOURCES += adevent.cpp \
           adeventhandler.cpp \
           adobject.cpp \
           adaudiovisual.cpp \
           adpermission.cpp \
           adschedule.cpp \
           adspace.cpp \
           aduser.cpp \
           adeventfactory.cpp \
           adreserve.cpp \
           adcancellation.cpp 
TARGET = adlib
CONFIG += release \
warn_on \
dll
TEMPLATE = lib
include(lib_config.pri)
