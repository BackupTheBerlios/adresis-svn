# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./3rdparty/psql
# Target is a library:  

QT += sql 
DEPENDPATH += . 
KDEV_QTVER = 4 
LIBS += -lpq 
INCLUDEPATH += . 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += qsql_psql.h 
SOURCES += qsql_psql.cpp 
target.path = /plugins/ 