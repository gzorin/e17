AUTOMAKE_OPTIONS = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir) \
-I$(top_srcdir)/src/include \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)\" \
-DPACKAGE_TMP_DIR=\"/tmp\" \
@ELEV8_CFLAGS@

bin_PROGRAMS = elev8

elev8_SOURCES = \
	elm.cc \
	main.cc main.h \
	xmlhttprequest.cc xmlhttprequest.h \
	dbus_library.cc dbus_library.h


elev8_LDADD = @ELEV8_LIBS@ @V8_LIBS@
