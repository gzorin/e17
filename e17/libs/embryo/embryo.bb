DESCRIPTION = "Embryo is a tiny library designed to provide a virutal \
machine system for executing small pieces of code logic very quickly."
HOMEPAGE = "http://www.enlightenment.org"
MAINTAINER = "Carsten Haitzler (Rasterman) <raster@rasterman.com>"
SECTION = "e/libs"
PRIORITY = "optional"
DEPENDS = ""
PV = "0.9.1"
PR = "1"

do_prepsources () {
  make clean distclean || true
}
addtask prepsources after do_fetch before do_unpack
  
SRC_URI = "file://./"
S = "${WORKDIR}/embryo"
  
inherit autotools pkgconfig binconfig

do_stage () {
  oe_libinstall -C src/lib libembryo ${STAGING_LIBDIR}
  install -m 0644 ${S}/src/lib/Embryo.h ${STAGING_INCDIR}
}

PACKAGES += "embryo-tools emebryo-compiler"

FILES_${PN} = "${libdir}/libembryo*.so*"
FILES_${PN}-dev += "${bindir}/embryo-bin ${libdir}/pkgconfig"
FILES_${PN}-compiler += "${bindir}/embryo_cc ${datadir}"
FILES_${PN}-tools += "${bindir}/embryo"

