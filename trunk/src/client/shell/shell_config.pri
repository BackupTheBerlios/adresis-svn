
QT += xml network gui
CONFIG += release

DLIB_DIR = ../../dlib
!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}

ADLIB_DIR = ../../lib
!include($$ADLIB_DIR/lib.pri) {
error("No encuentro adlib")
}

#CNETWORK_DIR = ../cnetwork
#!include($$CNETWORK_DIR/cnetwork.pri ){
#error("No encuentro cnerwork"
#}

ADCLIB_DIR = ../adclib
include($$ADCLIB_DIR/adclib.pri)

