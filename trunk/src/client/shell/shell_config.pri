
QT += xml network gui


DLIB_DIR = ../../dlib
LIB_DIR = ../lib

!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}
include($$LIB_DIR/lib.pri)

NETWORKC_DIR = ../network
include($$NETWORKC_DIR/network.pri )

