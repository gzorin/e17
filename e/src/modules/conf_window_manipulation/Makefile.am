MAINTAINERCLEANFILES = Makefile.in
MODULE = conf_window_manipulation

# data files for the module
filesdir = $(libdir)/enlightenment/modules/$(MODULE)
files_DATA = \
e-module-conf_winmanip.edj module.desktop

EXTRA_DIST = $(files_DATA)

# the module .so file
INCLUDES               = -I. \
                         -I$(top_srcdir) \
                         -I$(top_srcdir)/src/modules/$(MODULE) \
                         -I$(top_srcdir)/src/bin \
                         -I$(top_builddir)/src/bin \
                         -I$(top_srcdir)/src/modules \
                         @e_cflags@
pkgdir                 = $(libdir)/enlightenment/modules/$(MODULE)/$(MODULE_ARCH)
pkg_LTLIBRARIES        = module.la
module_la_SOURCES      = e_mod_main.c \
                         e_mod_main.h \
                         e_int_config_window_manip.h \
			 e_int_config_window_geometry.c \
			 e_int_config_window_maxpolicy.c \
			 e_int_config_window_stacking.c \
                         e_int_config_window_process.c \
                         e_int_config_window_display.c \
                         e_int_config_window_display.h \
                         e_int_config_focus.c \
                         e_int_config_focus.h \
                         e_int_config_clientlist.c \
                         e_int_config_clientlist.h
			 
module_la_LIBADD       = @e_libs@ @dlopen_libs@
module_la_LDFLAGS      = -module -avoid-version
module_la_DEPENDENCIES = $(top_builddir)/config.h

uninstall:
	rm -rf $(DESTDIR)$(libdir)/enlightenment/modules/$(MODULE)
