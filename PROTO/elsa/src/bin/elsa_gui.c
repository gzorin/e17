#include "elsa_client.h"
#include "Ecore_X.h"

#define ELM_INTERNAL_API_ARGESFSDFEFC
#include <elm_widget.h>


#define ELSA_GUI_GET(edj, name) edje_object_part_external_object_get(elm_layout_edje_get(edj), name)

typedef struct Elsa_Gui_ Elsa_Gui;
typedef struct Elsa_Screen_ Elsa_Screen;
typedef struct Elsa_Gui_Item_ Elsa_Gui_Item;

struct Elsa_Gui_
{
   Eina_List *screens;
   Eina_List *xsessions;
   Eina_List *users;
   Eina_List *actions;
   Elsa_Xsession *selected_session;
   const char *theme;
};

struct Elsa_Screen_
{
   Evas_Object *win;
   Evas_Object *bg;
   Evas_Object *edj;
};

typedef char *(*ElsaItemLabelGetFunc) (void *data, Evas_Object *obj, const char *part);
typedef Evas_Object *(*ElsaItemIconGetFunc) (void *data, Evas_Object *obj, const char *part);
typedef Eina_Bool (*ElsaItemStateGetFunc) (void *data, Evas_Object *obj, const char *part);
typedef void (*ElsaItemDelFunc) (void *data, Evas_Object *obj);

struct Elsa_Gui_Item_
{
   const char *item_style; //maybee need to be provided by theme ?
   struct
     {
        ElsaItemLabelGetFunc text_get;
        ElsaItemIconGetFunc  content_get;
        ElsaItemStateGetFunc state_get;
        ElsaItemDelFunc      del;
     } func;
};

static Evas_Object *_elsa_gui_theme_get(Evas_Object *win, const char *group, const char *theme);
static void _elsa_gui_hostname_activated_cb(void *data, Evas_Object *obj, void *event_info);
static void _elsa_gui_password_activated_cb(void *data, Evas_Object *obj, void *event_info);
static void _elsa_gui_shutdown(void *data, Evas_Object *obj, void *event_info);
static void _elsa_gui_focus(void *data, Evas_Object *obj, void *event_info);
static void _elsa_gui_session_update(Elsa_Xsession *xsession);
static void _elsa_gui_users_list_set(Evas_Object *obj, Eina_List *users);
static void _elsa_gui_users_genlist_set(Evas_Object *obj, Eina_List *users);
static void _elsa_gui_users_gengrid_set(Evas_Object *obj, Eina_List *users);
static void _elsa_gui_user_sel_cb(void *data, Evas_Object *obj, void *event_info);
static void _elsa_gui_user_sel(Elsa_User *ou);

static void _elsa_gui_action_clicked_cb(void *data, Evas_Object *obj, void *event_info);
static Elsa_Gui *_gui;

static Evas_Object *
_elsa_gui_theme_get (Evas_Object *win, const char *group, const char *theme)
{
   Evas_Object *edje = NULL;

   edje = elm_layout_add(win);
   if (theme)
     {
        char buf[PATH_MAX];
        snprintf(buf, sizeof(buf), PACKAGE_DATA_DIR"/themes/%s.edj", theme);
        if (!elm_layout_file_set(edje, buf, group))
          {
             fprintf(stderr, PACKAGE": can't load %s theme fallback to default\n", theme);
             elm_layout_file_set(edje, PACKAGE_DATA_DIR"/themes/default.edj", group);
          }
     }
   else
     elm_layout_file_set(edje, PACKAGE_DATA_DIR"/themes/default.edj", group);
   return edje;
}

static void
_elsa_gui_hostname_activated_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info __UNUSED__)
{
   char *txt;
   Eina_List *l, *ll;
   Elsa_Xsession *xsess;
   Elsa_User *eu = NULL;
   Elsa_Screen *screen;

   txt = elm_entry_markup_to_utf8(elm_entry_entry_get(obj));
   if (!txt || !strcmp(txt, ""))
     {
        free(txt);
        return;
     }
   EINA_LIST_FOREACH(_gui->users, ll, eu)
      if (!strcmp(txt, eu->login)) break;
   free(txt);

   if (eu && eu->lsess)
     {
        EINA_LIST_FOREACH(_gui->xsessions, l, xsess)
          {
             if (!strcmp(xsess->name, eu->lsess))
               {
                  _elsa_gui_session_update(xsess);
                  break;
               }
          }
     }
   else if (_gui->xsessions)
     _elsa_gui_session_update(_gui->xsessions->data);
   elm_object_focus_set(data, EINA_TRUE);
   EINA_LIST_FOREACH(_gui->screens, l, screen)
      edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                              "elsa.auth.enable", "");
}

static void
_elsa_gui_login_cb(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *sig __UNUSED__, const char *src __UNUSED__)
{
   elm_exit();
}

static void
_elsa_gui_shutdown(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   elm_exit();
}

static void
_elsa_gui_focus(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Eina_List *l;
   Elsa_Screen *screen;

   EINA_LIST_FOREACH(_gui->screens, l, screen)
      elm_object_focus_set(ELSA_GUI_GET(screen->edj, "hostname"), EINA_TRUE);
}

static void
_elsa_gui_login_cancel_cb(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *sig __UNUSED__, const char *src __UNUSED__)
{
   Evas_Object *o;
   Elsa_Screen *screen;
   Eina_List *l;

   EINA_LIST_FOREACH(_gui->screens, l, screen)
     {
        o = ELSA_GUI_GET(screen->edj, "hostname");
        elm_entry_entry_set(o, "");
        elm_object_focus_set(o, EINA_TRUE);
        o = ELSA_GUI_GET(screen->edj, "password");
        elm_entry_entry_set(o, "");
        edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                                "elsa.auth.disable", "");
     }
}

static void
_elsa_gui_login(Elsa_Screen *screen)
{
   Eina_List *l;
   char *h, *s;
   h = elsa_gui_user_get(screen);
   s = elsa_gui_password_get(screen);
   if (h && s)
     {
        if (strcmp(h, "") && strcmp(s, ""))
          {
             if (_gui->selected_session)
                  elsa_connect_auth_send(h, s, _gui->selected_session->name);
             else
                  elsa_connect_auth_send(h, s, NULL);
          }
     }
   if (h) free(h);
   if (s) free(s);
   EINA_LIST_FOREACH(_gui->screens, l, screen)
     {
        elm_object_focus_set(ELSA_GUI_GET(screen->edj, "password"), EINA_TRUE);
        edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                                "elsa.auth.enable", "");
     }
}

static void
_elsa_gui_login_request_cb(void *data, Evas_Object *obj __UNUSED__, const char *sig __UNUSED__, const char *src __UNUSED__)
{
   _elsa_gui_login(data);
}

static void
_elsa_gui_password_activated_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   _elsa_gui_login(data);
}

static void
_elsa_gui_xsessions_clicked_cb(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Evas_Object *icon;
   Eina_List *l;
   Elsa_Screen *screen;

   _gui->selected_session = data;
   EINA_LIST_FOREACH(_gui->screens, l, screen)
     {
        elm_object_text_set(obj, _gui->selected_session->name);
        icon = elm_icon_add(screen->win);
        elm_icon_file_set(icon, _gui->selected_session->icon, NULL);
        elm_hoversel_icon_set(obj, icon);
        evas_object_show(icon);
     }
}

static void
_elsa_gui_action_clicked_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Elsa_Action *ea;
   ea = data;
   if (ea) elsa_connect_action_send(ea->id);
}



static void
_elsa_gui_callback_add(Elsa_Screen *screen)
{
   Evas_Object *host, *pwd;
   Evas_Object *edj;

   evas_object_smart_callback_add(screen->win, "delete,request",
                                  _elsa_gui_shutdown, NULL);
   evas_object_smart_callback_add(screen->win, "focus,in",
                                  _elsa_gui_focus, NULL);

   edj = elm_layout_edje_get(screen->edj);
   host = ELSA_GUI_GET(screen->edj, "hostname");
   pwd = ELSA_GUI_GET(screen->edj, "password");
   evas_object_smart_callback_add(host, "activated",
                                  _elsa_gui_hostname_activated_cb, pwd);
   evas_object_smart_callback_add(pwd, "activated",
                                  _elsa_gui_password_activated_cb, screen);
   edje_object_signal_callback_add(edj, "elsa.auth.cancel", "",
                                   _elsa_gui_login_cancel_cb, NULL);
   edje_object_signal_callback_add(edj, "elsa.auth.request", "",
                                   _elsa_gui_login_request_cb, screen);
   edje_object_signal_callback_add(edj, "elsa.auth.end", "",
                                   _elsa_gui_login_cb, screen);
   elm_entry_single_line_set(host, EINA_TRUE);
   elm_entry_single_line_set(pwd, EINA_TRUE);
}

static void
_elsa_gui_session_update(Elsa_Xsession *xsession)
{
   Evas_Object *o, *icon;
   Eina_List *l;
   Elsa_Screen *screen;

   if (!xsession) return;
   EINA_LIST_FOREACH(_gui->screens, l, screen)
     {
        o = ELSA_GUI_GET(screen->edj, "xsessions");
        _gui->selected_session = xsession;
        elm_object_text_set(o, _gui->selected_session->name);
        icon = elm_icon_add(screen->win);
        elm_icon_file_set(icon, _gui->selected_session->icon, NULL);
        elm_hoversel_icon_set(o, icon);
     }
}

static void
_elsa_gui_sessions_populate()
{
   Evas_Object *o;

   Elsa_Xsession *xsession;
   Eina_List *l, *ll;
   Elsa_Screen *screen;

   EINA_LIST_FOREACH(_gui->screens, ll, screen)
     {
        o = ELSA_GUI_GET(screen->edj, "xsessions");

        EINA_LIST_FOREACH(_gui->xsessions, l, xsession)
          {
             elm_hoversel_item_add(o, xsession->name, xsession->icon,
                                   ELM_ICON_FILE,
                                   _elsa_gui_xsessions_clicked_cb, xsession);
          }
        edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                                "elsa.xsession.enabled", "");
     }
   if (_gui->xsessions)
     _elsa_gui_session_update(_gui->xsessions->data);
}

void
elsa_gui_xsession_set(Eina_List *xsessions)
{
   if (!xsessions) return;
   _gui->xsessions = xsessions;
   _elsa_gui_sessions_populate();
}

static void
_elsa_gui_actions_populate()
{
   Evas_Object *o;

   Elsa_Action *action;
   Eina_List *l, *ll;
   Elsa_Screen *screen;

   EINA_LIST_FOREACH(_gui->screens, ll, screen)
     {
        o = ELSA_GUI_GET(screen->edj, "actions");
        EINA_LIST_FOREACH(_gui->actions, l, action)
          {
             elm_hoversel_item_add(o, action->label, NULL,
                                   ELM_ICON_FILE,
                                   _elsa_gui_action_clicked_cb, action);
          }
        edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                                "elsa.action.enabled", "");
     }
}


void
elsa_gui_actions_set(Eina_List *actions)
{
   if (!actions) return;
   fprintf(stderr, PACKAGE": Action set\n");
   _gui->actions = actions;
   _elsa_gui_actions_populate();
}

int
elsa_gui_init(const char *theme)
{

   Ecore_X_Window xw;
   Elsa_Screen *screen;
   int ii, i;
   int x, y, w, h;

   fprintf(stderr, PACKAGE": client Gui init\n");
   _gui = calloc(1, sizeof(Elsa_Gui));
   if (!_gui)
     {
        fprintf(stderr, PACKAGE": client Not Enough memory\n");
        return 1;
     }

#ifdef XNEST_DEBUG
   char *tmp = getenv("DISPLAY");
   if (tmp && *tmp)
     {
        fprintf(stderr, PACKAGE": client Using display name %s", tmp);
     }
#endif


   i = ecore_x_xinerama_screen_count_get();
   for(ii = 0; ii < i; ++ii)
     {
        screen = calloc(1, sizeof(Elsa_Screen));
        if (!screen) return 1;

        _gui->screens = eina_list_append(_gui->screens, screen);
        ecore_x_xinerama_screen_geometry_get(ii, &x, &y, &w, &h);
        screen->win = elm_win_add(NULL, "main", ELM_WIN_BASIC);
        elm_win_fullscreen_set(screen->win, EINA_TRUE);
        elm_win_title_set(screen->win, PACKAGE);

        _gui->theme = eina_stringshare_add(theme);
        screen->edj = _elsa_gui_theme_get(screen->win, "elsa", theme);

        if (!screen->edj)
          {
             fprintf(stderr, PACKAGE": client Tut Tut Tut no theme\n");
//             return 2;
          }
        evas_object_size_hint_weight_set(screen->edj,
                                         EVAS_HINT_EXPAND,
                                         EVAS_HINT_EXPAND);
        elm_win_resize_object_add(screen->win, screen->edj);
        _elsa_gui_callback_add(screen);
        evas_object_show(screen->edj);

        xw = elm_win_xwindow_get(screen->win);
        evas_object_resize(screen->win, w, h);
        ecore_x_window_move(xw, x, y);
        evas_object_show(screen->win);
     }
   if (_gui->screens)
     {
        /* tricky situation. we are not normally running with a wm and thus
         * have to set focus to our window so things work right */
        screen = _gui->screens->data;
        ecore_evas_focus_set
           (ecore_evas_ecore_evas_get(evas_object_evas_get(screen->win)), 1);
     }
   return 0;

}


void
elsa_gui_shutdown()
{
   Elsa_Xsession *xsession;
   Elsa_Screen *screen;
   fprintf(stderr, PACKAGE": Gui shutdown\n");
   EINA_LIST_FREE(_gui->screens, screen)
     {
        evas_object_del(screen->win);
        free(screen);
     }
   eina_stringshare_del(_gui->theme);
   EINA_LIST_FREE(_gui->xsessions, xsession)
     {
        eina_stringshare_del(xsession->name);
        eina_stringshare_del(xsession->command);
        if (xsession->icon) eina_stringshare_del(xsession->icon);
     }
   if (_gui) free(_gui);
}

char *
elsa_gui_user_get(Elsa_Screen *screen)
{
   Evas_Object *o;
   o = ELSA_GUI_GET(screen->edj, "hostname");
   if (o) return elm_entry_markup_to_utf8(elm_entry_entry_get(o));
   return NULL;
}

char *
elsa_gui_password_get(Elsa_Screen *screen)
{
   Evas_Object *o;
   o = ELSA_GUI_GET(screen->edj, "password");
   if (o) return elm_entry_markup_to_utf8(elm_entry_entry_get(o));
   return NULL;
}

void
elsa_gui_auth_error()
{
   Evas_Object *o;
   Eina_List *l;
   Elsa_Screen *screen;

   EINA_LIST_FOREACH(_gui->screens, l, screen)
     {
        o = ELSA_GUI_GET(screen->edj, "password");
        elm_entry_entry_set(o, "");
        edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                                "elsa.auth.error", "");
     }
}

void
elsa_gui_auth_valid()
{
   Eina_List *l;
   Elsa_Screen *screen;
   EINA_LIST_FOREACH(_gui->screens, l, screen)
     {
        edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                                "elsa.auth.valid", "");
     }
}
///////////////// USER ////////////////////////////
void
elsa_gui_users_set(Eina_List *users)
{
   Evas_Object *ol;
   const char *type;
   Elsa_Screen *screen;
   Eina_List *l;

   EINA_LIST_FOREACH(_gui->screens, l, screen)
     {
        ol = ELSA_GUI_GET(screen->edj, "elsa_users");
        if ((ol) && ((type = elm_widget_type_get(ol))))
          {
             if (!strcmp(type, "list"))
               _elsa_gui_users_list_set(ol, users);
             else if (!strcmp(type, "genlist"))
               _elsa_gui_users_genlist_set(ol, users);
             else if (!strcmp(type, "gengrid"))
               _elsa_gui_users_gengrid_set(ol, users);

             edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                                     "elsa.users.enabled", "");
             _gui->users = users;
          }
     }
}



static void
_elsa_gui_user_sel(Elsa_User *eu)
{
   Evas_Object *o;
   Elsa_Xsession *xsess;
   Eina_List *l;
   Elsa_Screen *screen;

   EINA_LIST_FOREACH(_gui->screens, l, screen)
     {
        o = ELSA_GUI_GET(screen->edj, "hostname");
        elm_entry_entry_set(o, eu->login);
        elm_object_focus_set(ELSA_GUI_GET(screen->edj, "password"), EINA_TRUE);
        edje_object_signal_emit(elm_layout_edje_get(screen->edj),
                                "elsa.auth.enable", "");
     }
   if (eu->lsess)
     {
        EINA_LIST_FOREACH(_gui->xsessions, l, xsess)
          {
             if (!strcmp(xsess->name, eu->lsess))
               {
                  _elsa_gui_session_update(xsess);
                  break;
               }
          }
     }
   else if (_gui->xsessions)
     _elsa_gui_session_update(_gui->xsessions->data);
}

static void
_elsa_gui_user_sel_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   _elsa_gui_user_sel(data);
}

static char *
_elsa_gui_user_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   Elsa_User *eu;
   eu = data;
   return strdup(eu->login);
}

static Evas_Object *
_elsa_gui_user_content_get(void *data __UNUSED__, Evas_Object *obj, const char *part)
{
   Evas_Object *ic = NULL;
   Elsa_User *eu;
   eu = data;

   if (eu && !strcmp(part, "elm.swallow.icon"))
     {
        if (eu->image)
          {
             ic = elm_icon_add(obj);
             elm_icon_file_set(ic, eu->image, "elsa/user/icon");
          }
        else
          {
             ic = _elsa_gui_theme_get(obj, "elsa/user/default",
                                      _gui->theme);
          }
        evas_object_size_hint_weight_set(ic, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_show(ic);
     }
   return ic;
}

static Eina_Bool
_elsa_gui_user_state_get(void *data __UNUSED__, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   return EINA_FALSE;
}

static void
_elsa_gui_user_del(void *data __UNUSED__, Evas_Object *obj __UNUSED__)
{
}

///////////////// LIST ///////////////////////////////
static void
_elsa_gui_users_list_set(Evas_Object *obj, Eina_List *users)
{
   Elsa_User *eu;
   Eina_List *l;

   EINA_LIST_FOREACH(users, l, eu)
      elm_list_item_append(obj, eu->login, NULL, NULL,
                           _elsa_gui_user_sel_cb, eu);
   elm_list_go(obj);
}

///////////////// USER GENLIST /////////////////////////////
static Elm_Genlist_Item_Class _elsa_glc;
static void
_elsa_gui_users_genlist_set(Evas_Object *obj, Eina_List *users)
{
   Elsa_User *eu;
   Eina_List *l;

   _elsa_glc.item_style = "default";
   _elsa_glc.func.text_get = _elsa_gui_user_text_get;
   _elsa_glc.func.content_get = _elsa_gui_user_content_get;
   _elsa_glc.func.state_get = _elsa_gui_user_state_get;
   _elsa_glc.func.del = _elsa_gui_user_del;


   EINA_LIST_FOREACH(users, l, eu)
      elm_genlist_item_append(obj, &_elsa_glc,
                              eu, NULL, ELM_GENLIST_ITEM_NONE,
                              _elsa_gui_user_sel_cb, eu);
}

///////////////// USER GENGRID /////////////////////////////
static Elm_Gengrid_Item_Class _elsa_ggc;
static void
_elsa_gui_users_gengrid_set(Evas_Object *obj, Eina_List *users)
{
   Elsa_User *eu;
   Eina_List *l;

   _elsa_ggc.item_style = "default";
   _elsa_ggc.func.text_get = _elsa_gui_user_text_get;
   _elsa_ggc.func.content_get = _elsa_gui_user_content_get;
   _elsa_ggc.func.state_get = _elsa_gui_user_state_get;
   _elsa_ggc.func.del = _elsa_gui_user_del;

   evas_object_size_hint_weight_set(obj, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   EINA_LIST_FOREACH(users, l, eu)
      elm_gengrid_item_append(obj, &_elsa_ggc,
                              eu, _elsa_gui_user_sel_cb, eu);
   evas_object_show(obj);

}



////////////////////////////////////////////////////////////////////////////////
///////////////// LIST ///////////////////////////////
static void
_elsa_gui_list_fill(Evas_Object *obj, Elsa_Gui_Item egi, Eina_List *users, Evas_Smart_Cb func, void *data)
{
   Elsa_User *eu;
   Eina_List *l;

   EINA_LIST_FOREACH(users, l, eu)
      elm_list_item_append(obj, egi.func.text_get(eu, NULL, NULL), NULL, NULL,
                           func, data);
   elm_list_go(obj);
}

///////////////// GENLIST /////////////////////////////
static void
_elsa_gui_genlist_fill(Evas_Object *obj, Elsa_Gui_Item egi, Eina_List *users, Evas_Smart_Cb func, void *data)
{
   Elsa_User *eu;
   Eina_List *l;
   Elm_Genlist_Item_Class glc;

   glc.item_style = egi.item_style;
   glc.func.text_get = egi.func.text_get;
   glc.func.content_get = egi.func.content_get;
   glc.func.state_get = egi.func.state_get;
   glc.func.del = egi.func.del;


   EINA_LIST_FOREACH(users, l, eu)
      elm_genlist_item_append(obj, &glc,
                              eu, NULL, ELM_GENLIST_ITEM_NONE,
                              func, data);
}

///////////////// GENGRID /////////////////////////////
static void
_elsa_gui_gengrid_fill(Evas_Object *obj, Elsa_Gui_Item egi, Eina_List *users, Evas_Smart_Cb func, void *data)
{
   Elsa_User *eu;
   Eina_List *l;
   Elm_Gengrid_Item_Class ggc;

   ggc.item_style = egi.item_style;
   ggc.func.text_get = egi.func.text_get;
   ggc.func.content_get = egi.func.content_get;
   ggc.func.state_get = egi.func.state_get;
   ggc.func.del = egi.func.del;

   EINA_LIST_FOREACH(users, l, eu)
      elm_gengrid_item_append(obj, &ggc,
                              eu, func, data);
}

static void
_elsa_gui_cont_fill(Evas_Object *obj, Elsa_Gui_Item egi, Eina_List *users, Evas_Smart_Cb func, void *data)
{
   const char *type;
   if ((type = elm_widget_type_get(obj)))
     {
        if (!strcmp(type, "list"))
          _elsa_gui_list_fill(obj, egi, users, func, data);
        else if (!strcmp(type, "genlist"))
          _elsa_gui_genlist_fill(obj, egi, users, func, data);
        else if (!strcmp(type, "gengrid"))
          _elsa_gui_gengrid_fill(obj, egi, users, func, data);
     }
}

