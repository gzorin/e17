MAINTAINERCLEANFILES = Makefile.in

.PHONY: screenshots

examplesdir = $(datadir)/$(PACKAGE)/examples
filesdir = $(datadir)/$(PACKAGE)/examples
files_DATA =

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib \
-I$(top_builddir)/src/lib \
-DPACKAGE_DATA_DIR="\"$(datadir)/elementary\"" \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
@ELEMENTARY_EDBUS_CFLAGS@ \
@ELEMENTARY_EFREET_CFLAGS@ \
@ELEMENTARY_ETHUMB_CFLAGS@ \
@ELEMENTARY_EMAP_CFLAGS@ \
@ELEMENTARY_X_CFLAGS@ \
@ELEMENTARY_CFLAGS@

LDADD = \
	@ELEMENTARY_LIBS@ \
	@ELEMENTARY_ETHUMB_LIBS@ \
	@ELEMENTARY_X_LIBS@ \
	$(top_builddir)/src/lib/libelementary.la

EDJE_CC = @edje_cc@
EDJE_FLAGS_VERBOSE_ =
EDJE_FLAGS_VERBOSE_0 =
EDJE_FLAGS_VERBOSE_1 = -v
EDJE_FLAGS = $(EDJE_FLAGS_$(V))

SRCS = \
	actionslider_example_01.c \
	anchorblock_example_01.c \
	bubble_example_01.c \
	button_example_01.c \
	check_example_01.c \
	colorselector_example_01.c \
	frame_example_01.c \
	transit_example_01.c \
	transit_example_02.c \
	transit_example_03.c \
	transit_example_04.c \
	bg_example_01.c \
	bg_example_02.c \
	bg_example_03.c \
	box_example_02.c \
	flip_example_01.c \
	general_funcs_example.c \
	hover_example_01.c \
	hoversel_example_01.c \
	label_example_01.c \
	theme_example_01.c \
	theme_example_02.c \
	calendar_example_01.c \
	calendar_example_02.c \
	calendar_example_03.c \
	calendar_example_04.c \
	calendar_example_05.c \
	calendar_example_06.c \
	map_example_01.c \
	map_example_02.c \
	map_example_03.c \
	toolbar_example_01.c \
	toolbar_example_02.c \
	toolbar_example_03.c \
	spinner_example.c \
	clock_example.c \
	mapbuf_example.c \
	conformant_example_01.c \
	conformant_example_02.c \
	image_example_01.c \
	icon_example_01.c \
	layout_example_01.c \
	layout_example_02.c \
	layout_example_03.c \
	list_example_01.c \
	list_example_02.c \
	list_example_03.c \
	segment_control_example.c \
	flipselector_example.c \
	fileselector_example.c \
	fileselector_button_example.c \
	fileselector_entry_example.c \
	diskselector_example_01.c \
	diskselector_example_02.c \
	index_example_01.c \
	index_example_02.c \
	genlist_example_01.c \
	genlist_example_02.c \
	genlist_example_03.c \
	genlist_example_04.c \
	genlist_example_05.c \
	theme_example.edc \
	layout_example.edc \
	slider_example.c \
	panes_example.c \
	ctxpopup_example_01.c \
	pager_example_01.c \
	separator_example_01.c \
	radio_example_01.c \
	panel_example_01.c \
	gengrid_example.c \
	entry_example.c \
	slideshow_example.c \
	progressbar_example.c \
	notify_example_01.c \
	photocam_example_01.c \
	inwin_example.c \
	scroller_example_01.c \
	table_example_01.c \
	table_example_02.c \
	menu_example_01.c \
	thumb_example_01.c \
	web_example.c \
	win_example.c \
        efl_thread_1.c \
        efl_thread_2.c \
        efl_thread_3.c \
        efl_thread_4.c \
        efl_thread_5.c \
        efl_thread_6.c

examples_PROGRAMS =

.edc.edj:
	$(EDJE_CC) $(EDJE_FLAGS) $< $@

if EFL_INSTALL_EXAMPLES
files_DATA += $(SRCS) \
	dict.txt
endif

if EFL_BUILD_EXAMPLES
files_DATA += \
	theme_example.edj \
	layout_example.edj

clean-local:
	rm -f *.edj

examples_PROGRAMS += \
	actionslider_example_01 \
	anchorblock_example_01 \
	bubble_example_01 \
	button_example_01 \
	check_example_01 \
	colorselector_example_01 \
	frame_example_01 \
	transit_example_01 \
	transit_example_02 \
	transit_example_03 \
	transit_example_04 \
	bg_example_01 \
	bg_example_02 \
	bg_example_03 \
	box_example_02 \
	flip_example_01 \
	general_funcs_example \
	hover_example_01 \
	hoversel_example_01 \
	label_example_01 \
	theme_example_01 \
	theme_example_02 \
	calendar_example_01 \
	calendar_example_02 \
	calendar_example_03 \
	calendar_example_04 \
	calendar_example_05 \
	calendar_example_06 \
	map_example_01 \
	map_example_02 \
	map_example_03 \
	toolbar_example_01 \
	toolbar_example_02 \
	toolbar_example_03 \
	spinner_example \
	clock_example \
	mapbuf_example \
	conformant_example_01 \
	conformant_example_02 \
	image_example_01 \
	diskselector_example_01 \
	diskselector_example_02 \
	icon_example_01 \
	layout_example_01 \
	layout_example_02 \
	layout_example_03 \
	list_example_01 \
	list_example_02 \
	list_example_03 \
	segment_control_example \
	slider_example \
	panes_example \
	ctxpopup_example_01 \
	flipselector_example \
	fileselector_example \
	fileselector_button_example \
	fileselector_entry_example \
	index_example_01 \
	index_example_02 \
	pager_example_01 \
	separator_example_01 \
	radio_example_01 \
	toggle_example_01 \
	panel_example_01 \
	gengrid_example \
	genlist_example_01 \
	genlist_example_02 \
	genlist_example_03 \
	genlist_example_04 \
	genlist_example_05 \
	entry_example \
	slideshow_example \
	progressbar_example \
	notify_example_01 \
	photocam_example_01 \
	inwin_example \
	scroller_example_01 \
	table_example_01 \
	table_example_02 \
	menu_example_01 \
	thumb_example_01 \
	web_example \
	win_example \
        efl_thread_1 \
        efl_thread_2 \
        efl_thread_3 \
        efl_thread_4 \
        efl_thread_5 \
        efl_thread_6

# This variable will hold the list of screenshots that will be made
# by "make screenshots". Each item in the list is of the form:
# <example executable>:<screenshot filename>:<delay in seconds>
SCREENSHOTS = \
	actionslider_example_01:actionslider_01.png:0.0 \
	bg_example_02:bg_01.png:0.0 \
	box_example_02:box_example_02.png:1.3 \
	bubble_example_01:bubble_example_01.png:0.0 \
	button_example_01:button_01.png:0.0 \
	check_example_01:check_example_01.png:0.0 \
	colorselector_example_01:colorselector_example_01.png:0.0 \
	animator_example_01:animator_example_01.png:0.2 \
	animator_example_01:animator_example_02.png:0.5 \
	animator_example_01:animator_example_03.png:0.9 \
	flip_example_01:flip_example_01.png:0.0 \
	frame_example_01:frame_example_01.png:0.0 \
	hover_example_01:hover_example_01.png:0.0 \
	hoversel_example_01:hoversel_example_01.png:0.0 \
	label_example_01:label_example_01.png:0.0 \
	theme_example_01:theme_example_01.png:0.0 \
	conformant_example_01:conformant_example_01.png:0.0 \
	conformant_example_02:conformant_example_02.png:0.0 \
	calendar_example_01:calendar_example_01.png:0.0 \
	calendar_example_02:calendar_example_02.png:0.0 \
	calendar_example_03:calendar_example_03.png:0.0 \
	calendar_example_04:calendar_example_04.png:0.0 \
	calendar_example_05:calendar_example_05.png:0.0 \
	calendar_example_06:calendar_example_06.png:0.0 \
	map_example_01:map_example_01.png:2 \
	map_example_02:map_example_02.png:2.5 \
	map_example_03:map_example_03.png:2 \
	toolbar_example_01:toolbar_example_01.png:0.0 \
	toolbar_example_02:toolbar_example_02.png:1 \
	toolbar_example_03:toolbar_example_03.png:1 \
	spinner_example:spinner_example.png:0.0 \
	clock_example:clock_example.png:0.5 \
	mapbuf_example:mapbuf_example.png:0.0 \
	image_example_01:image_example_01.png:0.0 \
	diskselector_example_01:diskselector_example_01.png:0.2 \
	diskselector_example_02:diskselector_example_02.png:0.2 \
	icon_example_01:icon_example_01.png:0.0 \
	layout_example_01:layout_example_01.png:0.0 \
	layout_example_02:layout_example_02.png:0.0 \
	layout_example_03:layout_example_03.png:0.0 \
	list_example_01:list_example_01.png:0.0 \
	list_example_02:list_example_02.png:0.0 \
	list_example_03:list_example_03.png:0.0 \
	segment_control_example:segment_control_example.png:0.0 \
	flipselector_example:flipselector_example.png:0.0 \
	fileselector_example:fileselector_example.png:0.0 \
	index_example_02:index_example_03.png:0.3 \
	slider_example:slider_example.png:0.0 \
	panes_example:panes_example.png:0.0 \
	ctxpopup_example_01:ctxpopup_example_01.png:0.0 \
	pager_example_01:pager_example_01.png:0.0 \
	separator_example_01:separator_example_01.png:0.0 \
	radio_example_01:radio_example_01.png:0.0 \
	toggle_example_01:toggle_example_01.png:0.0 \
	panel_example_01:panel_example_01.png:0.0 \
	gengrid_example:gengrid_example.png:0.0 \
	genlist_example_01:genlist_example_01.png:0.1 \
	genlist_example_02:genlist_example_02.png:0.1 \
	genlist_example_03:genlist_example_03.png:0.1 \
	genlist_example_04:genlist_example_04.png:0.1 \
	genlist_example_05:genlist_example_05.png:0.1 \
	thumb_example_01:thumb_example_01.png:0.5 \
	entry_example:entry_example.png:0.0 \
	progressbar_example:progressbar_example.png:0.0 \
	notify_example_01:notify_example_01.png:0.0 \
	notify_example_01:notify_example_01_a.png:6.0 \
	slideshow_example:slideshow_example.png:1.0 \
	photocam_example_01:photocam_example_01.png:3 \
	scroller_example_01:scroller_example_01.png:0.0 \
	inwin_example:inwin_example.png:0.0 \
	inwin_example:inwin_example_a.png:0.2 \
	table_example_01:table_example_01.png:0.0 \
	table_example_02:table_example_02.png:0.0 \
	menu_example_01:menu_example_01.png:0.5

HTML_SS_DIR=$(top_builddir)/doc/html/screenshots
LATEX_SS_DIR=$(top_builddir)/doc/latex/screenshots
screenshots: all
	@mkdir -p $(HTML_SS_DIR)
	@mkdir -p $(LATEX_SS_DIR)
	@for ss in $(SCREENSHOTS); do \
		SS_ENTRY=($${ss//:/ }) ; \
		EXAMPLE=$${SS_ENTRY[0]} ; \
		SS_FILE=$${SS_ENTRY[1]} ; \
		SS_DELAY=$${SS_ENTRY[2]} ; \
		ILLUME_KBD=0,0,240,123 ILLUME_IND=0,0,240,32 ILLUME_STK=0,288,240,32 ELM_ENGINE="shot:delay=$${SS_DELAY}:file=$(HTML_SS_DIR)/$${SS_FILE}" ./$${EXAMPLE} ; \
		convert $(HTML_SS_DIR)/$${SS_FILE} $(LATEX_SS_DIR)/$${SS_FILE/.png/.eps} ; \
	done

else

screenshots:
	@echo "Examples are not built. Run \"./configure --enable-build-examples\" first."

endif
