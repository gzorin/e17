# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT(imlib2_loaders, 1.4.5, enlightenment-devel@lists.sourceforge.net)
AC_PREREQ(2.52)
AC_CONFIG_SRCDIR(configure.ac)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ISC_POSIX

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(config.h)

AC_C_BIGENDIAN
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST
AM_ENABLE_SHARED
AM_PROG_LIBTOOL

if test "x${bindir}" = 'x${exec_prefix}/bin'; then
  if test "x${exec_prefix}" = "xNONE"; then
    if test "x${prefix}" = "xNONE"; then
      bindir="${ac_default_prefix}/bin";
    else
      bindir="${prefix}/bin";
    fi
  else
    if test "x${prefix}" = "xNONE"; then
      bindir="${ac_default_prefix}/bin";
    else
      bindir="${prefix}/bin";
    fi
  fi
fi

if test "x${libdir}" = 'x${exec_prefix}/lib'; then
  if test "x${exec_prefix}" = "xNONE"; then
    if test "x${prefix}" = "xNONE"; then
      libdir="${ac_default_prefix}/lib";
    else
      libdir="${prefix}/lib";
    fi
  else
    if test "x${prefix}" = "xNONE"; then
      libdir="${ac_default_prefix}/lib";
    else
      libdir="${prefix}/lib";
    fi
  fi
fi

dnl Set PACKAGE_BIN_DIR in config.h.
if test "x${bindir}" = 'xNONE'; then
  if test "x${prefix}" = "xNONE"; then
    AC_DEFINE_UNQUOTED(PACKAGE_BIN_DIR, "${ac_default_prefix}/bin", [Installation directory for user executables])
  else
    AC_DEFINE_UNQUOTED(PACKAGE_BIN_DIR, "${prefix}/bin", [Installation directory for user executables])
  fi
else
  AC_DEFINE_UNQUOTED(PACKAGE_BIN_DIR, "${bindir}", [Installation directory for user executables])
fi

dnl Set PACKAGE_LIB_DIR in config.h.
if test "x${libdir}" = 'xNONE'; then
  if test "x${prefix}" = "xNONE"; then
    AC_DEFINE_UNQUOTED(PACKAGE_LIB_DIR, "${ac_default_prefix}/lib", [Installation directory for libraries])
  else
    AC_DEFINE_UNQUOTED(PACKAGE_LIB_DIR, "${prefix}/lib", [Installation directory for libraries])
  fi
else
  AC_DEFINE_UNQUOTED(PACKAGE_LIB_DIR, "${libdir}", [Installation directory for libraries])
fi

dnl Set PACKAGE_DATA_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${ac_default_prefix}/share/${PACKAGE}", [Shared Data Directory])
else
  AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${prefix}/share/${PACKAGE}", [Shared Data Directory])
fi

dnl Set PACKAGE_SOURCE_DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`
AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}", [Source code directory])


edb=auto
AC_ARG_ENABLE(edb,
[  --disable-edb           disable building the edb loader],
[
 if test x$enableval = xno; then
   edb=no
 else
   edb=yes
 fi
])

eet=auto
AC_ARG_ENABLE(eet,
[  --disable-eet           disable building the eet loader],
[
 if test x$enableval = xno; then
   eet=no
 else
   eet=yes
 fi
])

AC_ARG_ENABLE(xcf,
[  --disable-xcf           disable building the xcf loader],
[
 if test x$enableval = xno; then
   xcf=no;
 fi
])


AC_ARG_WITH(imlib2-config, [  --with-imlib2-config=IMLIB2_CONFIG    use imlib2-config specified ],
[ IMLIB2_CONFIG=$withval;
  echo "using "$IMLIB2_CONFIG" for imlib2-config"; ],
[ if test -z "$IMLIB2_CONFIG"; then
    AC_PATH_PROG(IMLIB2_CONFIG, "imlib2-config", "", $PATH)
  fi
])
my_includes=`$IMLIB2_CONFIG --cflags`


found_edb=no
if test "x$edb" != "xno" ; then
        PKG_CHECK_MODULES(EDB, edb, found_edb=yes, found_edb=no)
fi

if test "x$edb$found_edb" = "xyesno" ; then
	AC_MSG_ERROR(Edb support was requested but not found)
elif test "x$found_edb" = "xno" ; then
	edb=no
	EDB_CFLAGS=""
	EDB_LIBS=""
fi

found_eet=no
if test "x$eet" != "xno" ; then
        PKG_CHECK_MODULES(EET, eet, found_eet=yes, found_eet=no)
fi
if test "x$eet$found_eet" = "xyesno" ; then
	AC_MSG_ERROR(Eet support was requested but not found)
elif test "x$found_eet" = "xno" ; then
	eet=no
	EET_CFLAGS=""
	EET_LIBS=""
fi

AC_SUBST(my_includes)
AC_SUBST(EDB_CFLAGS)
AC_SUBST(EDB_LIBS)
AC_SUBST(EET_CFLAGS)
AC_SUBST(EET_LIBS)

AM_CONDITIONAL(BUILD_EDB_LOADER, test x$edb != xno)
AM_CONDITIONAL(BUILD_EET_LOADER, test x$eet != xno)
AM_CONDITIONAL(BUILD_XCF_LOADER, test x$xcf != xno)

AC_OUTPUT([
Makefile
src/Makefile
src/modules/Makefile
src/modules/loaders/Makefile
README
imlib2_loaders.spec
],[
])

echo -e "\nBuild summary:"
echo    "------------------------"
echo -n " EDB: "
if test x$edb = xno ; then
  echo    "Disabled "
else
  echo    "Enabled "
fi
echo -n " EET: "
if test x$eet = xno ; then
  echo    "Disabled "
else
  echo    "Enabled "
fi
echo -n " XCF: "
if test x$xcf = xno ; then
  echo    "Disabled "
else
  echo    "Enabled "
fi
