
LIBS += -laspell
DEFINES += HAVE_ASPELL
LIBS += -Wl,--export-dynamic -pthread -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lxml2 -lz -lm -lglib-2.0
QMAKE_CXXFLAGS_RELEASE += -pthread -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/libxml2
DEFINES += HAVE_GST10
