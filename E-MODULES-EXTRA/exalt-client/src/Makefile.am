MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I. \
	   -I$(top_srcdir) \
	   -I$(includedir) \
	   -I$(includedir)/exalt \
	   -I$(includedir)/exalt_dbus \
	   @E_CFLAGS@

pkgdir = $(datadir)/$(MODULE_ARCH)
pkg_LTLIBRARIES = module.la
module_la_SOURCES = e_mod_main.h \
						  e_mod_main.c \
						  e_mod_config.c \
						  popup.c \
						  wired_dialog.c \
						  wired_dialog_basic.c \
						  wireless_dialog.c \
						  network_dialog_new.c \
						  network_dialog.c \
						  network_dialog_basic.c \
						  dns_dialog.c \
						  network_conf_dialog.c

module_la_LIBADD = @E_LIBS@ @EXALT_LIBS@ @EXALT_DBUS_LIBS@ @E_NOTIFY_LIBS@
module_la_LDFLAGS = -module -avoid-version
module_la_DEPENDENCIES = $(top_builddir)/config.h

clean-local:
	rm -rf *~
