# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./3rdparty/psql
# Destiono es una biblioteca:  

INSTALLS += target 
target.path = /plugins/ 
HEADERS += qsql_psql.h \
           libpq-fe.h \
           postgres_ext.h 
SOURCES += qsql_psql.cpp 
QT += sql
DEPENDPATH = .
LIBS += -lpq
INCLUDEPATH += .
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
