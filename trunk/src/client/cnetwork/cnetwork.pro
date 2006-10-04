# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/client/network
# Destiono es una biblioteca:  

KDEV_QTVER = 4 
CONFIG += release \
          warn_on \
          staticlib 

TEMPLATE = lib 

HEADERS += adconnectpackage.h \
           adsqlpackagebase.h \
           addeletepackage.h \
           adinsertpackage.h \
           adpackageparser.h \
           adselectpackage.h \
           adupdatepackage.h 
SOURCES += adconnectpackage.cpp \
           adsqlpackagebase.cpp \
           addeletepackage.cpp \
           adinsertpackage.cpp \
           adpackageparser.cpp \
           adselectpackage.cpp \
           adupdatepackage.cpp 

include(cnetwork_config.pri)


