MAINTAINERCLEANFILES = Makefile.in


lib_LTLIBRARIES = libeyesight.la

includesdir = $(includedir)/eyesight
includes_HEADERS = Eyesight.h

libeyesight_la_SOURCES = \
eyesight.c \
eyesight_index.c \
eyesight_private.h

libeyesight_la_CPPFLAGS = \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
@EFL_EYESIGHT_BUILD@ \
@EYESIGHT_CFLAGS@
libeyesight_la_CFLAGS =
libeyesight_la_LIBADD = @EYESIGHT_LIBS@
libeyesight_la_LDFLAGS = -no-undefined @lt_enable_auto_import@ -version-info @version_info@

if EYESIGHT_STATIC_BUILD_IMG
libeyesight_la_SOURCES += $(top_srcdir)/src/modules/img/eyesight_img.c
endif

if EYESIGHT_STATIC_BUILD_PDF
if HAVE_PDF_BACKEND_MUPDF
   libeyesight_la_SOURCES += $(top_srcdir)/src/modules/pdf/eyesight_pdf_mupdf.c
else
   libeyesight_la_SOURCES += $(top_srcdir)/src/modules/pdf/eyesight_pdf_poppler.cpp
   libeyesight_la_CPPFLAGS += @POPPLER_CFLAGS@
   libeyesight_la_LIBADD += @POPPLER_LIBS@
endif
endif

if EYESIGHT_STATIC_BUILD_POSTSCRIPT
libeyesight_la_SOURCES += $(top_srcdir)/src/modules/ps/eyesight_ps.c
libeyesight_la_CPPFLAGS += @POSTSCRIPT_CFLAGS@
libeyesight_la_LIBADD += @POSTSCRIPT_LIBS@
endif

if EYESIGHT_STATIC_BUILD_TXT
libeyesight_la_SOURCES += $(top_srcdir)/src/modules/txt/eyesight_txt.c
endif

if EYESIGHT_STATIC_BUILD_PDF
if HAVE_PDF_BACKEND_MUPDF
libeyesight_la_LINK = $(LINK) $(libeyesight_la_LDFLAGS)
else
libeyesight_la_LINK = $(CXXLINK) $(libeyesight_la_LDFLAGS)
endif
else
libeyesight_la_LINK = $(LINK) $(libeyesight_la_LDFLAGS)
endif
