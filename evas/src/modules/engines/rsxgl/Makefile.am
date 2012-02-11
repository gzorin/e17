
MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/include \
-I$(top_srcdir)/src/modules/engines \
@FREETYPE_CFLAGS@ \
@PIXMAN_CFLAGS@ \
@EINA_CFLAGS@ \
@evas_engine_rsxgl_cflags@

if BUILD_ENGINE_RSXGL

RSXGL_SOURCES = \
evas_engine.c \
rsxutil.c

RSXGL_LIBADD = @evas_engine_rsxgl_libs@


includes_HEADERS = Evas_Engine_RSXGL.h
includesdir = $(includedir)/evas-@VMAJ@

if !EVAS_STATIC_BUILD_RSXGL

pkgdir = $(libdir)/evas/modules/engines/rsxgl/$(MODULE_ARCH)
pkg_LTLIBRARIES = module.la
module_la_SOURCES = $(RSXGL_SOURCES)
module_la_LIBADD = @EINA_LIBS@ $(RSXGL_LIBADD) $(top_builddir)/src/lib/libevas.la
module_la_LDFLAGS = -module -avoid-version
module_la_LIBTOOLFLAGS = --tag=disable-static

else

noinst_LTLIBRARIES = libevas_engine_rsxgl.la

libevas_engine_rsxgl_la_SOURCES = $(RSXGL_SOURCES)
libevas_engine_rsxgl_la_LIBADD = $(RSXGL_LIBADD)

endif
endif

EXTRA_DIST = \
evas_engine.h \
rsxutil.h
