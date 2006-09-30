
QT += xml network gui


DLIB_DIR = ../../dlib
LIB_DIR = ../lib
NETWORK_DIR = ../network

!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}

include($$LIB_DIR/lib.pri)
include($$NETWORK_DIR/network.pri )



