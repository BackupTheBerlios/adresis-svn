
QT += xml network gui

DLIB_DIR = ../../dlib
!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}

ADLIB_DIR =../../lib
!include($$ADLIB_DIR/lib.pri) {
error("No encuentro a adlib")
}

