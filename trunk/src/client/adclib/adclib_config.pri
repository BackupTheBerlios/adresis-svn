
QT += xml network gui

DLIB_DIR = ../../dlib
!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}


