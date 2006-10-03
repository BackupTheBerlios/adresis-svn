# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/client/lib
# Target is a library:  adclib

KDEV_QTVER = 4 
INSTALLS += target 
target.path = /lib/ 

TARGET = adclib 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += adcfirstrundialog.h \
           adcmodulelist.h \
           global.h \
           adconnectorbase.h \
           cconnectiondialog.h \
           admodulebuttonbar.h \
           adformbase.h \
           adpermissionsview.h \
           ktthemeselector.h \
           ktpreferences.h 
SOURCES += adcfirstrundialog.cpp \
           adcmodulelist.cpp \
           adconnectorbase.cpp \
           cconnectiondialog.cpp \
           admodulebuttonbar.cpp \
           adformbase.cpp \
           adpermissionsview.cpp \
           ktthemeselector.cpp \
           ktpreferences.cpp 
include(lib_config.pri)
