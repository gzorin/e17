MAINTAINERCLEANFILES = Makefile.in default.edj

EDJE_CC = @edje_cc@
EDJ = default.edj

default.edj:  *.edc \
../images/* \
../images/netsync/* \
../default_elementary/*.edc \
../default_elementary/images/* 
	$(EDJE_CC) default.edc -id ../images/ -id ../default_elementary/

icondir = $(pkgdatadir)/themes
icon_DATA =  $(EDJ)

clean-local:
	rm -rf *.edj *~

uninstall:
	rm -rf $(DESTDIR)$(pkgdatadir)/themes

