/*
 * vim:ts=8:sw=3:sts=8:noexpandtab:cino=>5n-3f0^-2{2
 */
#ifndef ELEMENTARY_H
#define ELEMENTARY_H

/* What is this?
 * 
 * This is a VERY SIMPLE toolkit - VERY SIMPLE. It is not meant for writing
 * extensive applications. Small simple ones with simple needs. It is meant
 * to make the programmers work almost brainless.
 * 
 * I'm toying with backing this with etk or ewl - or not. right now I am
 * unsure as some of the widgets will be faily complex edje creations. Do
 * not consider any choices permanent - but the API should stay unbroken.
 * 
 */

#include "elementary_config.h"

/* Standard headers for standard system calls etc. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/param.h>
#include <dlfcn.h>
#include <math.h>
#include <fnmatch.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <glob.h>
#include <locale.h>
#include <libintl.h>
#include <errno.h>
#include <signal.h>
#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

/* EFL headers */
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>
#include <Ecore_Job.h>
#include <Ecore_Txt.h>
#include <Ecore_File.h>
#include <Eet.h>
#include <Edje.h>

/* allow usage from c++ */
#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/   
   /* Objects */
   typedef enum _Elm_Win_Type
     {
	ELM_WIN_BASIC,
	  ELM_WIN_DIALOG_BASIC
     } Elm_Win_Type;
   
/**************************************************************************/   
   /* General calls */
   EAPI void         elm_init(int argc, char **argv);
   EAPI void         elm_shutdown(void);
   EAPI void         elm_run(void);
   EAPI void         elm_exit(void);
   
   EAPI Evas_Object *elm_win_add(Evas_Object *parent, const char *name, Elm_Win_Type type);
   EAPI void         elm_win_resize_object_add(Evas_Object *obj, Evas_Object *subobj);
   EAPI void         elm_win_resize_object_del(Evas_Object *obj, Evas_Object *subobj);
   EAPI void         elm_win_title_set(Evas_Object *obj, const char *title);
   EAPI void         elm_win_autodel_set(Evas_Object *obj, Evas_Bool autodel);
   EAPI void         elm_win_activate(Evas_Object *obj);
   EAPI void         elm_win_borderless_set(Evas_Object *obj, Evas_Bool borderless);
   EAPI void         elm_win_shaped_set(Evas_Object *obj, Evas_Bool shaped);
   EAPI void         elm_win_alpha_set(Evas_Object *obj, Evas_Bool alpha);
   EAPI void         elm_win_override_set(Evas_Object *obj, Evas_Bool override);
// FIXME: implement more of the above calls
   /* smart callbacks elm_win objects will call:
    * "delete-request" - the user requested to delete the window
    */

   EAPI Evas_Object *elm_bg_add(Evas_Object *parent);
   EAPI void         elm_bg_file_set(Evas_Object *obj, const char *file, const char *group);
   /* smart callbacks elm_win objects will call:
    */

   EAPI Evas_Object *elm_icon_add(Evas_Object *parent);
   EAPI void         elm_icon_file_set(Evas_Object *obj, const char *file, const char *group);
   EAPI void         elm_icon_smooth_set(Evas_Object *obj, Evas_Bool smooth);
   EAPI void         elm_icon_no_scale_set(Evas_Object *obj, Evas_Bool no_scale);
   EAPI void         elm_icon_scale_set(Evas_Object *obj, Evas_Bool scale_up, Evas_Bool scale_down);
   EAPI void         elm_icon_fill_outside_set(Evas_Object *obj, Evas_Bool fill_outside);
   /* smart callbacks elm_win objects will call:
    */

   EAPI Evas_Object *elm_box_add(Evas_Object *parent);
   EAPI void         elm_box_horizontal_set(Evas_Object *obj, Evas_Bool horizontal);
   EAPI void         elm_box_homogenous_set(Evas_Object *obj, Evas_Bool homogenous);
   EAPI void         elm_box_pack_start(Evas_Object *obj, Evas_Object *subobj);
   EAPI void         elm_box_pack_end(Evas_Object *obj, Evas_Object *subobj);
   EAPI void         elm_box_pack_before(Evas_Object *obj, Evas_Object *subobj, Evas_Object *before);
   EAPI void         elm_box_pack_after(Evas_Object *obj, Evas_Object *subobj, Evas_Object *after);
   /* smart callbacks elm_win objects will call:
    */
   
   EAPI Evas_Object *elm_button_add(Evas_Object *parent);
   EAPI void         elm_button_label_set(Evas_Object *obj, const char *label);
   EAPI void         elm_button_icon_set(Evas_Object *obj, Evas_Object *icon);
   /* smart callbacks elm_win objects will call:
    * "clicked" - the user clicked the button
    */
   
   EAPI Evas_Object *elm_scroller_add(Evas_Object *parent);
   EAPI void         elm_scroller_content_set(Evas_Object *obj, Evas_Object *child);
   /* smart callbacks elm_win objects will call:
    */

   EAPI Evas_Object *elm_label_add(Evas_Object *parent);
   EAPI void         elm_label_label_set(Evas_Object *obj, const char *label);
   /* smart callbacks elm_win objects will call:
    */
       
   EAPI Evas_Object *elm_toggle_add(Evas_Object *parent);
   EAPI void         elm_toggle_label_set(Evas_Object *obj, const char *label);
   EAPI void         elm_toggle_icon_set(Evas_Object *obj, Evas_Object *icon);
   EAPI void         elm_toggle_states_labels_set(Evas_Object *obj, const char *onlabel, const char *offlabel);
   EAPI void         elm_toggle_state_set(Evas_Object *obj, Evas_Bool state);
   EAPI void         elm_toggle_state_pointer_set(Evas_Object *obj, Evas_Bool *statep);
   /* smart callbacks elm_win objects will call:
    * "changed" - the user toggled the state
    */
   
   EAPI Evas_Object *elm_frame_add(Evas_Object *parent);
   EAPI void         elm_frame_label_set(Evas_Object *obj, const char *label);
   EAPI void         elm_frame_content_set(Evas_Object *obj, Evas_Object *content);
   /* smart callbacks elm_win objects will call:
    */
       
   EAPI Evas_Object *elm_table_add(Evas_Object *parent);
   EAPI void         elm_table_homogenous_set(Evas_Object *obj, Evas_Bool homogenous);
   EAPI void         elm_table_pack(Evas_Object *obj, Evas_Object *subobj, int x, int y, int w, int h);
   /* smart callbacks elm_win objects will call:
    */
   
   EAPI Evas_Object *elm_clock_add(Evas_Object *parent);
   EAPI void         elm_clock_time_set(Evas_Object *obj, int hrs, int min, int sec);
   EAPI void         elm_clock_time_get(Evas_Object *obj, int *hrs, int *min, int *sec);
   EAPI void         elm_clock_edit_set(Evas_Object *obj, Evas_Bool edit);
   EAPI void         elm_clock_show_am_pm_set(Evas_Object *obj, Evas_Bool am_pm);
   EAPI void         elm_clock_show_seconds_set(Evas_Object *obj, Evas_Bool seconds);
   /* smart callbacks elm_win objects will call:
    * "changed" - the user changed the time
    */

   EAPI Evas_Object *elm_layout_add(Evas_Object *parent);
   EAPI void         elm_layout_file_set(Evas_Object *obj, const char *file, const char *group);
   EAPI void         elm_layout_content_set(Evas_Object *obj, const char *swallow, Evas_Object *content);
   EAPI Evas_Object *elm_layout_edje_get(Evas_Object *obj);
   /* smart callbacks elm_win objects will call:
    */
   
   EAPI Evas_Object *elm_hover_add(Evas_Object *parent);
   EAPI void         elm_hover_target_set(Evas_Object *obj, Evas_Object *target);
   EAPI void         elm_hover_parent_set(Evas_Object *obj, Evas_Object *parent);
   /* smart callbacks elm_win objects will call:
    */
   
// FIXME: fixes to do
// * current sizing inefficient
// * scroller could do with page up/down/left/right buttons and and idea of
//   a page size
// * tab widget focusing (not useful for touchscreen tho...)
//    
// FIXME: immediate hitlist for widgets:
// HOVER: select a widget (any) and attach a hover to it - the over will hover
//        on top of the hover target and accept child widgets. haver may
//        optionally place something to cover the window (under the hover
//        widget, above everything else)
// POPSEL: a buttont that when pressed uses a hover to display a h/v box or
//         a table of options surrounding the button with scroll butons at
//         the end and the ability to drag and scroll
   
#endif
