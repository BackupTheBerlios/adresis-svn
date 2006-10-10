
QT += xml network sql
CONFIG += release


DLIB_DIR = ../../dlib

!include($$DLIB_DIR/dlib.pri) {
error("No encuentro dlib")
}

ADLIB_DIR = ../../lib
!include($$ADLIB_DIR/lib.pri) {
error("No encuentro adlib")
}

PACKAGES_DIR = ../packages
!include($$PACKAGES_DIR/packages.pri){
error("No encuentro packages")
}

SNETWORK_DIR = ../snetwork
!include($$SNETWORK_DIR/snetwork.pri ){
	error ("No encuentro network")
}

PSQL_DIR = ../../../3rdparty/psql 
!include($$PSQL_DIR/psql.pri ){
        error ("No encuentro psql")
}



