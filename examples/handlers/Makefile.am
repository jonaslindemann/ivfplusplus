AUTOMAKE_OPTIONS=no-dependencies

srcdir = @srcdir@
VPATH = @srcdir@

noinst_PROGRAMS=handlers

INCLUDES=-I../../include

handlers_SOURCES=handlers.cpp
handlers_LDADD= ../../src/ivfui/libivfui.la \
	../../src/ivffltk/libivffltk.la \
	../../src/ivfwidget/libivfwidget.la \
	../../src/ivf/libivf.la -lgle \
	../../src/ivfmath/libivfmath.la \
	-lfltk -lfltk_gl $(GLLIBS) -lXmu -lX11 $(X_LIBS) -lm

EXTRA_DIST=handlers.cpp
