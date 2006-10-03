
QT += xml network gui
CONFIG += release

DLIB_DIR = ../../dlib

!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}

ADCLIB_DIR = ../adclib
include($$ADCLIB_DIR/adclib.pri)

CNETWORK_DIR = ../cnetwork
!include($$CNETWORK_DIR/cnetwork.pri ){
error("No encuentro cnerwork"
}
