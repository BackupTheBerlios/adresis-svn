# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/client/adclib
# Target is a library:  adclib

INSTALLS += target 
target.path = /lib/ 
HEADERS += adcfirstrundialog.h \
           adcmodulelist.h \
           adconnectorbase.h \
           cconnectiondialog.h \
           admodulebuttonbar.h \
           adformbase.h \
           adpermissionsview.h \
           ktthemeselector.h \
           ktpreferences.h \
           adconsultschedule.h 
SOURCES += adcfirstrundialog.cpp \
           adcmodulelist.cpp \
           adconnectorbase.cpp \
           cconnectiondialog.cpp \
           admodulebuttonbar.cpp \
           adformbase.cpp \
           adpermissionsview.cpp \
           ktthemeselector.cpp \
           ktpreferences.cpp \
           adconsultschedule.cpp 
KDEV_QTVER = 4
TARGET = adclib
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
include(adclib_config.pri)
