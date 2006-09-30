
QT += xml network sql



DLIB_DIR = ../../dlib

!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}

PACKAGES_DIR = ../packages
include($$PACKGES_DIR/packages.pri)
include($$PACKGES_DIR/packages_config.pri)

NETWORK_DIR = ../network
include($$NETWORK_DIR/network.pri )

