# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/cliente/network
# Destiono es una biblioteca:  

QT += xml 
KDEV_QTVER = 4 
TARGETDEPS += ../../../src/cliente/lib/libadlib.a 
LIBS += ../../../src/cliente/lib/libadlib.a 
INCLUDEPATH += ../../../src/cliente/lib \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += adconnectpackage.h \
           addeletepackage.h \
           adinsertpackage.h \
           adpackageparser.h \
           adselectpackage.h \
           adsqlpackagebase.h \
           adupdatepackage.h 
SOURCES += adconnectpackage.cpp \
           addeletepackage.cpp \
           adinsertpackage.cpp \
           adpackageparser.cpp \
           adselectpackage.cpp \
           adsqlpackagebase.cpp \
           adupdatepackage.cpp 
