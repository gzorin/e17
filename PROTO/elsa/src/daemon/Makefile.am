
MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/daemon \
-I$(top_builddir)/src/daemon \
-DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)\" \
-DSYSTEM_CONFIG_DIR=\"$(sysconfdir)\" \
-DPACKAGE_BIN_DIR=\"$(libdir)/$(PACKAGE)\" \
@ELSA_CFLAGS@

if HAVE_CONSOLEKIT
AM_CPPFLAGS += @ELSA_CK_CFLAGS@
endif

sbin_PROGRAMS = elsa

internal_bindir=$(libdir)/elsa
internal_bin_PROGRAMS = elsa_wait

if HAVE_CONSOLEKIT
internal_bin_PROGRAMS += elsa_ck_launch
endif

elsa_SOURCES =     \
elsa_config.h      \
elsa_config.c      \
elsa_session.h     \
elsa_session.c     \
elsa_xserver.h     \
elsa_xserver.c     \
../event/elsa_event.c \
../event/elsa_event.h \
elsa_server.h     \
elsa_server.c     \
elsa_history.h    \
elsa_history.c    \
elsa_action.h     \
elsa_action.c     \
elsa.h            \
elsa.c


elsa_LDFLAGS =
elsa_LDADD = @ELSA_LIBS@ -lrt

elsa_wait_SOURCES = \
elsa_wait.c

elsa_wait_LDFLAGS =
elsa_wait_LDADD =

if HAVE_PAM
elsa_SOURCES += elsa_pam.h elsa_pam.c
elsa_LDADD += -lpam
else
elsa_LDADD += -lcrypt
endif

if HAVE_CONSOLEKIT
elsa_ck_launch_SOURCES = \
elsa_ck_launch.c

elsa_ck_launch_LDFLAGS =
elsa_ck_launch_LDADD = @ELSA_CK_LIBS@
endif

