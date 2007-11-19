AUTOMAKE_OPTIONS=no-dependencies

srcdir = @srcdir@
VPATH = @srcdir@

noinst_PROGRAMS=vertexarray

INCLUDES=-I../../include

vertexarray_SOURCES=vertexarray.cpp
vertexarray_LDADD= ../../src/ivfui/libivfui.la ../../src/ivffltk/libivffltk.la ../../src/ivfwidget/libivfwidget.la ../../src/ivf/libivf.la ../../src/ivfmath/libivfmath.la 
vertexarray_LDFLAGS=-lgle -lfltk_gl -lfltk  $(GLLIBS) -lXmu -lX11 $(X_LIBS) -lm

EXTRA_DIST=vertexarray.cpp
