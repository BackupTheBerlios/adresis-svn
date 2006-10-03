
QT += xml network gui
CONFIG += release

DLIB_DIR = ../../dlib
LIB_DIR = ../lib

!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}
include($$LIB_DIR/lib.pri)

CNETWORK_DIR = ../cnetwork
!include($$CNETWORK_DIR/cnetwork.pri ){
error("No encuentro cnerwork"
}
