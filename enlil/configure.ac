##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
m4_define([v_maj], [0])
m4_define([v_min], [6])
m4_define([v_mic], [0])
m4_define([v_rev], m4_esyscmd([(svnversion "${SVN_REPO_PATH:-.}" | grep -v export || echo 0) | awk -F : '{printf("%s\n", $1);}' | tr -d ' :MSP\n']))
m4_if(v_rev, [0], [m4_define([v_rev], m4_esyscmd([git log 2> /dev/null | (grep -m1 git-svn-id || echo 0) | sed -e 's/.*@\([0-9]*\).*/\1/' | tr -d '\n']))])
##--   When released, remove the dnl on the below line
dnl m4_undefine([v_rev])
##--   When doing snapshots - change soname. remove dnl on below line
dnl m4_define([relname], [ver-pre-svn-07])
dnl m4_define([v_rel], [-release relname])
##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
m4_ifdef([v_rev], [m4_define([v_ver], [v_maj.v_min.v_mic.v_rev])], [m4_define([v_ver], [v_maj.v_min.v_mic])])
m4_define([lt_cur], m4_eval(v_maj + v_min))
m4_define([lt_rev], v_mic)
m4_define([lt_age], v_min)
##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##

m4_define([v_ver], [v_maj.v_min.v_mic])

AC_INIT([enlil], [v_ver])
AC_PREREQ([2.52])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([1.6 dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AM_PROG_LIBTOOL

##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
m4_ifdef([v_rev], , [m4_define([v_rev], [0])])
m4_ifdef([v_rel], , [m4_define([v_rel], [])])
AC_DEFINE_UNQUOTED(VMAJ, [v_maj], [Major version])
AC_DEFINE_UNQUOTED(VMIN, [v_min], [Minor version])
AC_DEFINE_UNQUOTED(VMIC, [v_mic], [Micro version])
AC_DEFINE_UNQUOTED(VREV, [v_rev], [Revison])
version_info="lt_cur:lt_rev:lt_age"
release_info="v_rel"
AC_SUBST(version_info)
AC_SUBST(release_info)
##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
VMAJ=v_maj
AC_SUBST(VMAJ)

### Needed information

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

### Default options with respect to host


### Additional options to configure


### Checks for programs
AC_PROG_CC

# pkg-config
PKG_PROG_PKG_CONFIG

# Check whether pkg-config supports Requires.private
if $PKG_CONFIG --atleast-pkgconfig-version 0.22; then
   pkgconfig_requires_private="Requires.private"
else
   pkgconfig_requires_private="Requires"
fi
AC_SUBST(pkgconfig_requires_private)

# doxygen program for documentation building

EFL_CHECK_DOXYGEN([build_doc="yes"], [build_doc="no"])


### Checks for libraries

requirements="ethumb_client >= 0.1.0 ecore-file >= 1.0.0 ecore >= 1.0.0 evas >= 1.0.0 eet >= 1.2.2 eina >= 1.0.0 libexif >= 0.6.16 libiptcdata >= 1.0.2 libxml-2.0 >= 0.1.1 efreet >= 1.0.0 eio"

PKG_CHECK_EXISTS([azy >= 0.0.1],
   [
    requirements="azy >= 0.0.1 ${requirements}"
    have_azy="yes"
    AC_DEFINE_UNQUOTED(HAVE_AZY, [1], [have network synchronization (azy) support])
   ],
   [have_azy="no"])
AM_CONDITIONAL([HAVE_AZY], [test "x${have_azy}" = "xyes"])

PKG_CHECK_MODULES([ENLIL], [${requirements}])

EFL_CHECK_THREADS(
   [
    if test "x${_efl_have_posix_threads}" = "xyes" ; then
       have_threads="POSIX"
    else
       if test "x${_efl_have_win32_threads}" = "xyes" ; then
          have_threads="Win32"
       else
          have_threads="no"
       fi
    fi],
   [have_threads="no"])

if test "x${have_threads}" = "xno" ; then
   AC_MSG_ERROR([Thread support must be enabled])
fi


### Unit tests, coverage and benchmarking

EFL_CHECK_TESTS([enable_tests="yes"], [enable_tests="no"])

####

### Checks for compiler characteristics
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST
AM_PROG_CC_C_O
AC_C___ATTRIBUTE__

EFL_COMPILER_FLAG([-Wall])
EFL_COMPILER_FLAG([-W])
EFL_COMPILER_FLAG([-Wshadow])

EFL_ENLIL_BUILD=""
case "$host_os" in
   mingw*)
      EFL_ENLIL_BUILD="-DEFL_ENLIL_BUILD"
   ;;
esac
AC_SUBST(EFL_ENLIL_BUILD)

AC_SUBST(requirements)

AC_CONFIG_FILES([
Makefile
src/Makefile
src/lib/Makefile
src/enlil/Makefile
src/bin_load_sync/Makefile
src/bin_sync/Makefile
src/bin_print/Makefile
src/bin_geocaching_print/Makefile
src/bin_transformations/Makefile
src/tests/Makefile
enlil.pc
doc/Makefile
])

AC_OUTPUT

echo
echo
echo "--------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "--------------------------------------------------------"
echo
echo
echo "  Network synchronisation..: ${have_azy}"
echo "  Thread Support...........: ${have_threads}"
echo "  Tests....................: ${enable_tests}"
echo
echo "Installation...............: make install (as root if needed, with 'su' or 'sudo')"
echo "  prefix...................: $prefix"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE,"
echo "and then afterwards as root (or the user who will install this), type"
echo "'make install'. Change users with 'su' or 'sudo' appropriately."
echo
