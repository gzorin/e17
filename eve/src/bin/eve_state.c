/* This file has been automatically generated by geneet.py */
/*                      DO NOT MODIFY                      */

#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "eve_state.h"

struct _Config {
    unsigned char allow_popup;
    unsigned char enable_auto_load_images;
    unsigned char enable_auto_shrink_images;
    unsigned char enable_javascript;
    unsigned char enable_mouse_cursor;
    unsigned char enable_plugins;
    unsigned char enable_private_mode;
    unsigned char enable_touch_interface;
    const char * home_page;
    const char * proxy;
    unsigned char restore_state;
    const char * user_agent;
    unsigned char frame_flattening;
    int text_zoom;
    int minimum_font_size;
    const char *__eet_filename;
    int cookie_policy;
    int backing_store;
};

struct _Hist_Item {
    const char * title;
    const char * url;
    unsigned int visit_count;
    double last_visit;
};

struct _Hist {
    Eina_Hash * items;
    const char *__eet_filename;
};

struct _Fav_Item {
    const char * url;
    const char * title;
    unsigned int visit_count;
};

struct _Fav {
    Eina_Hash * items;
    const char *__eet_filename;
};

struct _Session_Item {
    const char * url;
    unsigned char focused;
    int scroll_x;
    int scroll_y;
};

struct _Session_Window {
    Eina_List * tabs;
    unsigned char focused;
};

struct _Session {
    Eina_List * windows;
    const char *__eet_filename;
};

static const char CONFIG_ENTRY[] = "config";
static const char HIST_ITEM_ENTRY[] = "hist_item";
static const char HIST_ENTRY[] = "hist";
static const char FAV_ITEM_ENTRY[] = "fav_item";
static const char FAV_ENTRY[] = "fav";
static const char SESSION_ITEM_ENTRY[] = "session_item";
static const char SESSION_WINDOW_ENTRY[] = "session_window";
static const char SESSION_ENTRY[] = "session";

static Eet_Data_Descriptor *_config_descriptor = NULL;
static Eet_Data_Descriptor *_hist_item_descriptor = NULL;
static Eet_Data_Descriptor *_hist_descriptor = NULL;
static Eet_Data_Descriptor *_fav_item_descriptor = NULL;
static Eet_Data_Descriptor *_fav_descriptor = NULL;
static Eet_Data_Descriptor *_session_item_descriptor = NULL;
static Eet_Data_Descriptor *_session_window_descriptor = NULL;
static Eet_Data_Descriptor *_session_descriptor = NULL;

static inline void
_config_init(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (_config_descriptor) return;

    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Config);
    _config_descriptor = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "allow_popup", allow_popup, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "enable_auto_load_images", enable_auto_load_images, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "enable_auto_shrink_images", enable_auto_shrink_images, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "enable_javascript", enable_javascript, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "enable_mouse_cursor", enable_mouse_cursor, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "enable_plugins", enable_plugins, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "enable_private_mode", enable_private_mode, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "enable_touch_interface", enable_touch_interface, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "home_page", home_page, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "proxy", proxy, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "restore_state", restore_state, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "user_agent", user_agent, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "frame_flattening", frame_flattening, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "text_zoom", text_zoom, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "minimum_font_size", minimum_font_size, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "cookie_policy", cookie_policy, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_config_descriptor, Config, "backing_store", backing_store, EET_T_INT);
}

static inline void
_config_shutdown(void)
{
    if (!_config_descriptor) return;
    eet_data_descriptor_free(_config_descriptor);
    _config_descriptor = NULL;
}

Config *
config_new(unsigned char allow_popup, unsigned char enable_auto_load_images, unsigned char enable_auto_shrink_images, unsigned char enable_javascript, unsigned char enable_mouse_cursor, unsigned char enable_plugins, unsigned char enable_private_mode, unsigned char enable_touch_interface, const char * home_page, const char * proxy, unsigned char restore_state, const char * user_agent, unsigned char frame_flattening, int text_zoom, int minimum_font_size, int cookie_policy, int backing_store)
{
    Config *config = calloc(1, sizeof(Config));

    if (!config)
       {
          fprintf(stderr, "ERROR: could not calloc Config\n");
          return NULL;
       }

    config->allow_popup = allow_popup;
    config->enable_auto_load_images = enable_auto_load_images;
    config->enable_auto_shrink_images = enable_auto_shrink_images;
    config->enable_javascript = enable_javascript;
    config->enable_mouse_cursor = enable_mouse_cursor;
    config->enable_plugins = enable_plugins;
    config->enable_private_mode = enable_private_mode;
    config->enable_touch_interface = enable_touch_interface;
    config->home_page = eina_stringshare_add(home_page ? home_page : "http://www.google.com");
    config->proxy = eina_stringshare_add(proxy);
    config->restore_state = restore_state;
    config->user_agent = eina_stringshare_add(user_agent ? user_agent : "eve");
    config->frame_flattening = frame_flattening;
    config->text_zoom = text_zoom;
    config->minimum_font_size = minimum_font_size;
    config->cookie_policy = cookie_policy;
    config->backing_store = backing_store;

    return config;
}

void
config_free(Config *config)
{
    eina_stringshare_del(config->home_page);
    eina_stringshare_del(config->proxy);
    eina_stringshare_del(config->user_agent);
    free(config);
}

inline unsigned char
config_allow_popup_get(const Config *config)
{
    return config->allow_popup;
}

inline void
config_allow_popup_set(Config *config, unsigned char allow_popup)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->allow_popup = allow_popup;
}
  
inline unsigned char
config_enable_auto_load_images_get(const Config *config)
{
    return config->enable_auto_load_images;
}

inline void
config_enable_auto_load_images_set(Config *config, unsigned char enable_auto_load_images)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->enable_auto_load_images = enable_auto_load_images;
}
  
inline unsigned char
config_enable_auto_shrink_images_get(const Config *config)
{
    return config->enable_auto_shrink_images;
}

inline void
config_enable_auto_shrink_images_set(Config *config, unsigned char enable_auto_shrink_images)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->enable_auto_shrink_images = enable_auto_shrink_images;
}
  
inline unsigned char
config_enable_javascript_get(const Config *config)
{
    return config->enable_javascript;
}

inline void
config_enable_javascript_set(Config *config, unsigned char enable_javascript)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->enable_javascript = enable_javascript;
}
  
inline unsigned char
config_enable_mouse_cursor_get(const Config *config)
{
    return config->enable_mouse_cursor;
}

inline void
config_enable_mouse_cursor_set(Config *config, unsigned char enable_mouse_cursor)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->enable_mouse_cursor = enable_mouse_cursor;
}
  
inline unsigned char
config_enable_plugins_get(const Config *config)
{
    return config->enable_plugins;
}

inline void
config_enable_plugins_set(Config *config, unsigned char enable_plugins)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->enable_plugins = enable_plugins;
}
  
inline unsigned char
config_enable_private_mode_get(const Config *config)
{
    return config->enable_private_mode;
}

inline void
config_enable_private_mode_set(Config *config, unsigned char enable_private_mode)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->enable_private_mode = enable_private_mode;
}
  
inline unsigned char
config_enable_touch_interface_get(const Config *config)
{
    return config->enable_touch_interface;
}

inline void
config_enable_touch_interface_set(Config *config, unsigned char enable_touch_interface)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->enable_touch_interface = enable_touch_interface;
}
  
inline const char *
config_home_page_get(const Config *config)
{
    return config->home_page;
}

inline void
config_home_page_set(Config *config, const char *home_page)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    eina_stringshare_replace(&(config->home_page), home_page);
}

inline const char *
config_proxy_get(const Config *config)
{
    return config->proxy;
}

inline void
config_proxy_set(Config *config, const char *proxy)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    eina_stringshare_replace(&(config->proxy), proxy);
}
  
inline unsigned char
config_restore_state_get(const Config *config)
{
    return config->restore_state;
}

inline void
config_restore_state_set(Config *config, unsigned char restore_state)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->restore_state = restore_state;
}
  
inline const char *
config_user_agent_get(const Config *config)
{
    return config->user_agent;
}

inline void
config_user_agent_set(Config *config, const char *user_agent)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    eina_stringshare_replace(&(config->user_agent), user_agent);
}
  
inline unsigned char
config_frame_flattening_get(const Config *config)
{
    return config->frame_flattening;
}

inline void
config_frame_flattening_set(Config *config, unsigned char frame_flattening)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->frame_flattening = frame_flattening;
}
  
inline int
config_text_zoom_get(const Config *config)
{
    return config->text_zoom;
}

inline void
config_text_zoom_set(Config *config, int text_zoom)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->text_zoom = text_zoom;
}
  
inline int
config_minimum_font_size_get(const Config *config)
{
    return config->minimum_font_size;
}

inline void
config_minimum_font_size_set(Config *config, int minimum_font_size)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->minimum_font_size = minimum_font_size;
}

inline int
config_cookie_policy_get(const Config *config)
{
    return config->cookie_policy;
}

inline void
config_cookie_policy_set(Config *config, int cookie_policy)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->cookie_policy = cookie_policy;
}

inline int
config_backing_store_get(const Config *config)
{
    return config->backing_store;
}

inline void
config_backing_store_set(Config *config, int backing_store)
{
    EINA_SAFETY_ON_NULL_RETURN(config);
    config->backing_store = backing_store;
}
  
Config *
config_load(const char *filename)
{
    Config *config = NULL;
    Eet_File *ef = eet_open(filename, EET_FILE_MODE_READ);
    if (!ef)
      {
        fprintf(stderr, "ERROR: could not open '%s' for read\n", filename);
        return NULL;
      }

    config = eet_data_read(ef, _config_descriptor, CONFIG_ENTRY);
    if (!config) goto end;
    config->__eet_filename = eina_stringshare_add(filename);

end:
    eet_close(ef);
    return config;
}

Eina_Bool
config_save(Config *config, const char *filename)
{
    Eet_File *ef;
    Eina_Bool ret;

    if (filename) config->__eet_filename = eina_stringshare_add(filename);
    else if (config->__eet_filename) filename = config->__eet_filename;
    else return EINA_FALSE;

    ef = eet_open(filename, EET_FILE_MODE_READ_WRITE);
    if (!ef)
       {
          fprintf(stderr, "ERROR: could not open '%s' for write\n", filename);
          return EINA_FALSE;
       }

    ret = !!eet_data_write(ef, _config_descriptor, CONFIG_ENTRY, config, EINA_TRUE);
    eet_close(ef);

    return ret;
}

static inline void
_hist_item_init(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (_hist_item_descriptor) return;

    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Hist_Item);
    _hist_item_descriptor = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_BASIC(_hist_item_descriptor, Hist_Item, "title", title, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_hist_item_descriptor, Hist_Item, "url", url, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_hist_item_descriptor, Hist_Item, "visit_count", visit_count, EET_T_UINT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_hist_item_descriptor, Hist_Item, "last_visit", last_visit, EET_T_DOUBLE);
}

static inline void
_hist_item_shutdown(void)
{
    if (!_hist_item_descriptor) return;
    eet_data_descriptor_free(_hist_item_descriptor);
    _hist_item_descriptor = NULL;
}

Hist_Item *
hist_item_new(const char * title, const char * url, unsigned int visit_count, double last_visit)
{
    Hist_Item *hist_item = calloc(1, sizeof(Hist_Item));

    if (!hist_item)
       {
          fprintf(stderr, "ERROR: could not calloc Hist_Item\n");
          return NULL;
       }

    hist_item->title = eina_stringshare_add(title ? title : "Untitled");
    hist_item->url = eina_stringshare_add(url ? url : "about:blank");
    hist_item->visit_count = visit_count;
    hist_item->last_visit = last_visit;

    return hist_item;
}

void
hist_item_free(Hist_Item *hist_item)
{
    eina_stringshare_del(hist_item->title);
    eina_stringshare_del(hist_item->url);
    free(hist_item);
}

inline const char *
hist_item_title_get(const Hist_Item *hist_item)
{
    return hist_item->title;
}

inline void
hist_item_title_set(Hist_Item *hist_item, const char *title)
{
    EINA_SAFETY_ON_NULL_RETURN(hist_item);
    eina_stringshare_replace(&(hist_item->title), title);
}
  
inline const char *
hist_item_url_get(const Hist_Item *hist_item)
{
    return hist_item->url;
}

inline void
hist_item_url_set(Hist_Item *hist_item, const char *url)
{
    EINA_SAFETY_ON_NULL_RETURN(hist_item);
    eina_stringshare_replace(&(hist_item->url), url);
}
  
inline unsigned int
hist_item_visit_count_get(const Hist_Item *hist_item)
{
    return hist_item->visit_count;
}

inline void
hist_item_visit_count_set(Hist_Item *hist_item, unsigned int visit_count)
{
    EINA_SAFETY_ON_NULL_RETURN(hist_item);
    hist_item->visit_count = visit_count;
}
  
inline double
hist_item_last_visit_get(const Hist_Item *hist_item)
{
    return hist_item->last_visit;
}

inline void
hist_item_last_visit_set(Hist_Item *hist_item, double last_visit)
{
    EINA_SAFETY_ON_NULL_RETURN(hist_item);
    hist_item->last_visit = last_visit;
}
  

static inline void
_hist_init(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (_hist_descriptor) return;

    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Hist);
    _hist_descriptor = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_HASH(_hist_descriptor, Hist, "items", items, _hist_item_descriptor);
}

static inline void
_hist_shutdown(void)
{
    if (!_hist_descriptor) return;
    eet_data_descriptor_free(_hist_descriptor);
    _hist_descriptor = NULL;
}

Hist *
hist_new()
{
    Hist *hist = calloc(1, sizeof(Hist));

    if (!hist)
       {
          fprintf(stderr, "ERROR: could not calloc Hist\n");
          return NULL;
       }

    hist->items = eina_hash_stringshared_new(EINA_FREE_CB(hist_item_free));

    return hist;
}

void
hist_free(Hist *hist)
{
    eina_hash_free(hist->items);
    free(hist);
}

void
hist_items_add(Hist *hist, const char * url, Hist_Item *hist_item)
{
    EINA_SAFETY_ON_NULL_RETURN(hist);
    eina_hash_add(hist->items, url, hist_item);
}

void
hist_items_del(Hist *hist, const char * url)
{
    EINA_SAFETY_ON_NULL_RETURN(hist);
    eina_hash_del(hist->items, url, NULL);
}

inline Hist_Item *
hist_items_get(const Hist *hist, const char * url)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(hist, NULL);
    return eina_hash_find(hist->items, url);
}

inline Eina_Hash *
hist_items_hash_get(const Hist *hist)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(hist, NULL);
    return hist->items;
}

void
hist_items_modify(Hist *hist, const char * key, void *value)
{
    EINA_SAFETY_ON_NULL_RETURN(hist);
    eina_hash_modify(hist->items, key, value);
}

Hist *
hist_load(const char *filename)
{
    Hist *hist = NULL;
    Eet_File *ef = eet_open(filename, EET_FILE_MODE_READ);
    if (!ef)
      {
        fprintf(stderr, "ERROR: could not open '%s' for read\n", filename);
        return NULL;
      }

    hist = eet_data_read(ef, _hist_descriptor, HIST_ENTRY);
    if (!hist) goto end;
    hist->__eet_filename = eina_stringshare_add(filename);

    if (!hist->items) hist->items = eina_hash_stringshared_new(EINA_FREE_CB(hist_item_free));

end:
    eet_close(ef);
    return hist;
}

Eina_Bool
hist_save(Hist *hist, const char *filename)
{
    Eet_File *ef;
    Eina_Bool ret;

    if (filename) hist->__eet_filename = eina_stringshare_add(filename);
    else if (hist->__eet_filename) filename = hist->__eet_filename;
    else return EINA_FALSE;

    ef = eet_open(filename, EET_FILE_MODE_READ_WRITE);
    if (!ef)
       {
          fprintf(stderr, "ERROR: could not open '%s' for write\n", filename);
          return EINA_FALSE;
       }

    ret = !!eet_data_write(ef, _hist_descriptor, HIST_ENTRY, hist, EINA_TRUE);
    eet_close(ef);

    return ret;
}

static inline void
_fav_item_init(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (_fav_item_descriptor) return;

    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Fav_Item);
    _fav_item_descriptor = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_BASIC(_fav_item_descriptor, Fav_Item, "url", url, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_fav_item_descriptor, Fav_Item, "title", title, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_fav_item_descriptor, Fav_Item, "visit_count", visit_count, EET_T_UINT);
}

static inline void
_fav_item_shutdown(void)
{
    if (!_fav_item_descriptor) return;
    eet_data_descriptor_free(_fav_item_descriptor);
    _fav_item_descriptor = NULL;
}

Fav_Item *
fav_item_new(const char * url, const char * title, unsigned int visit_count)
{
    Fav_Item *fav_item = calloc(1, sizeof(Fav_Item));

    if (!fav_item)
       {
          fprintf(stderr, "ERROR: could not calloc Fav_Item\n");
          return NULL;
       }

    fav_item->url = eina_stringshare_add(url);
    fav_item->title = eina_stringshare_add(title);
    fav_item->visit_count = visit_count;

    return fav_item;
}

void
fav_item_free(Fav_Item *fav_item)
{
    eina_stringshare_del(fav_item->url);
    eina_stringshare_del(fav_item->title);
    free(fav_item);
}

inline const char *
fav_item_url_get(const Fav_Item *fav_item)
{
    return fav_item->url;
}

inline void
fav_item_url_set(Fav_Item *fav_item, const char *url)
{
    EINA_SAFETY_ON_NULL_RETURN(fav_item);
    eina_stringshare_replace(&(fav_item->url), url);
}
  
inline const char *
fav_item_title_get(const Fav_Item *fav_item)
{
    return fav_item->title;
}

inline void
fav_item_title_set(Fav_Item *fav_item, const char *title)
{
    EINA_SAFETY_ON_NULL_RETURN(fav_item);
    eina_stringshare_replace(&(fav_item->title), title);
}
  
inline unsigned int
fav_item_visit_count_get(const Fav_Item *fav_item)
{
    return fav_item->visit_count;
}

inline void
fav_item_visit_count_set(Fav_Item *fav_item, unsigned int visit_count)
{
    EINA_SAFETY_ON_NULL_RETURN(fav_item);
    fav_item->visit_count = visit_count;
}
  

static inline void
_fav_init(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (_fav_descriptor) return;

    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Fav);
    _fav_descriptor = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_HASH(_fav_descriptor, Fav, "items", items, _fav_item_descriptor);
}

static inline void
_fav_shutdown(void)
{
    if (!_fav_descriptor) return;
    eet_data_descriptor_free(_fav_descriptor);
    _fav_descriptor = NULL;
}

Fav *
fav_new()
{
    Fav *fav = calloc(1, sizeof(Fav));

    if (!fav)
       {
          fprintf(stderr, "ERROR: could not calloc Fav\n");
          return NULL;
       }

    fav->items = eina_hash_stringshared_new(EINA_FREE_CB(fav_item_free));

    return fav;
}

void
fav_free(Fav *fav)
{
    eina_hash_free(fav->items);
    free(fav);
}

void
fav_items_add(Fav *fav, const char * url, Fav_Item *fav_item)
{
    EINA_SAFETY_ON_NULL_RETURN(fav);
    eina_hash_add(fav->items, url, fav_item);
}

void
fav_items_del(Fav *fav, const char * url)
{
    EINA_SAFETY_ON_NULL_RETURN(fav);
    eina_hash_del(fav->items, url, NULL);
}

inline Fav_Item *
fav_items_get(const Fav *fav, const char * url)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(fav, NULL);
    return eina_hash_find(fav->items, url);
}

inline Eina_Hash *
fav_items_hash_get(const Fav *fav)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(fav, NULL);
    return fav->items;
}

void
fav_items_modify(Fav *fav, const char * key, void *value)
{
    EINA_SAFETY_ON_NULL_RETURN(fav);
    eina_hash_modify(fav->items, key, value);
}

Fav *
fav_load(const char *filename)
{
    Fav *fav = NULL;
    Eet_File *ef = eet_open(filename, EET_FILE_MODE_READ);
    if (!ef)
      {
        fprintf(stderr, "ERROR: could not open '%s' for read\n", filename);
        return NULL;
      }

    fav = eet_data_read(ef, _fav_descriptor, FAV_ENTRY);
    if (!fav) goto end;
    fav->__eet_filename = eina_stringshare_add(filename);

    if (!fav->items) fav->items = eina_hash_stringshared_new(EINA_FREE_CB(fav_item_free));

end:
    eet_close(ef);
    return fav;
}

Eina_Bool
fav_save(Fav *fav, const char *filename)
{
    Eet_File *ef;
    Eina_Bool ret;

    if (filename) fav->__eet_filename = eina_stringshare_add(filename);
    else if (fav->__eet_filename) filename = fav->__eet_filename;
    else return EINA_FALSE;

    ef = eet_open(filename, EET_FILE_MODE_READ_WRITE);
    if (!ef)
       {
          fprintf(stderr, "ERROR: could not open '%s' for write\n", filename);
          return EINA_FALSE;
       }

    ret = !!eet_data_write(ef, _fav_descriptor, FAV_ENTRY, fav, EINA_TRUE);
    eet_close(ef);

    return ret;
}

static inline void
_session_item_init(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (_session_item_descriptor) return;

    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Session_Item);
    _session_item_descriptor = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_BASIC(_session_item_descriptor, Session_Item, "url", url, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_session_item_descriptor, Session_Item, "focused", focused, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_session_item_descriptor, Session_Item, "scroll_x", scroll_x, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_session_item_descriptor, Session_Item, "scroll_y", scroll_y, EET_T_INT);
}

static inline void
_session_item_shutdown(void)
{
    if (!_session_item_descriptor) return;
    eet_data_descriptor_free(_session_item_descriptor);
    _session_item_descriptor = NULL;
}

Session_Item *
session_item_new(const char * url, unsigned char focused, int scroll_x, int scroll_y)
{
    Session_Item *session_item = calloc(1, sizeof(Session_Item));

    if (!session_item)
       {
          fprintf(stderr, "ERROR: could not calloc Session_Item\n");
          return NULL;
       }

    session_item->url = eina_stringshare_add(url);
    session_item->focused = focused;
    session_item->scroll_x = scroll_x;
    session_item->scroll_y = scroll_y;

    return session_item;
}

void
session_item_free(Session_Item *session_item)
{
    eina_stringshare_del(session_item->url);
    free(session_item);
}

inline const char *
session_item_url_get(const Session_Item *session_item)
{
    return session_item->url;
}

inline void
session_item_url_set(Session_Item *session_item, const char *url)
{
    EINA_SAFETY_ON_NULL_RETURN(session_item);
    eina_stringshare_replace(&(session_item->url), url);
}
  
inline unsigned char
session_item_focused_get(const Session_Item *session_item)
{
    return session_item->focused;
}

inline void
session_item_focused_set(Session_Item *session_item, unsigned char focused)
{
    EINA_SAFETY_ON_NULL_RETURN(session_item);
    session_item->focused = focused;
}
  
inline int
session_item_scroll_x_get(const Session_Item *session_item)
{
    return session_item->scroll_x;
}

inline void
session_item_scroll_x_set(Session_Item *session_item, int scroll_x)
{
    EINA_SAFETY_ON_NULL_RETURN(session_item);
    session_item->scroll_x = scroll_x;
}
  
inline int
session_item_scroll_y_get(const Session_Item *session_item)
{
    return session_item->scroll_y;
}

inline void
session_item_scroll_y_set(Session_Item *session_item, int scroll_y)
{
    EINA_SAFETY_ON_NULL_RETURN(session_item);
    session_item->scroll_y = scroll_y;
}
  

static inline void
_session_window_init(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (_session_window_descriptor) return;

    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Session_Window);
    _session_window_descriptor = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_LIST(_session_window_descriptor, Session_Window, "tabs", tabs, _session_item_descriptor);
    EET_DATA_DESCRIPTOR_ADD_BASIC(_session_window_descriptor, Session_Window, "focused", focused, EET_T_UCHAR);
}

static inline void
_session_window_shutdown(void)
{
    if (!_session_window_descriptor) return;
    eet_data_descriptor_free(_session_window_descriptor);
    _session_window_descriptor = NULL;
}

Session_Window *
session_window_new(Eina_List * tabs, unsigned char focused)
{
    Session_Window *session_window = calloc(1, sizeof(Session_Window));

    if (!session_window)
       {
          fprintf(stderr, "ERROR: could not calloc Session_Window\n");
          return NULL;
       }

    session_window->tabs = tabs;
    session_window->focused = focused;

    return session_window;
}

void
session_window_free(Session_Window *session_window)
{
    if (session_window->tabs)
       {
          Session_Item *tabs_elem;
          EINA_LIST_FREE(session_window->tabs, tabs_elem)
             session_item_free(tabs_elem);
       }
    free(session_window);
}

inline void
session_window_tabs_add(Session_Window *session_window, Session_Item *session_item)
{
    EINA_SAFETY_ON_NULL_RETURN(session_window);
    session_window->tabs = eina_list_append(session_window->tabs, session_item);
}

inline void
session_window_tabs_del(Session_Window *session_window, Session_Item *session_item)
{
    EINA_SAFETY_ON_NULL_RETURN(session_window);
    session_window->tabs = eina_list_remove(session_window->tabs, session_item);
}

inline Session_Item *
session_window_tabs_get(const Session_Window *session_window, unsigned int nth)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(session_window, NULL);
    return eina_list_nth(session_window->tabs, nth);
}

inline unsigned int
session_window_tabs_count(const Session_Window *session_window)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(session_window, 0);
    return eina_list_count(session_window->tabs);
}

void
session_window_tabs_list_clear(Session_Window *session_window)
{
    EINA_SAFETY_ON_NULL_RETURN(session_window);
    Session_Item *data;
    EINA_LIST_FREE(session_window->tabs, data) session_item_free(data);
}

inline Eina_List *
session_window_tabs_list_get(const Session_Window *session_window)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(session_window, NULL);
    return session_window->tabs;
}

inline void
session_window_tabs_list_set(Session_Window *session_window, Eina_List *list)
{
    EINA_SAFETY_ON_NULL_RETURN(session_window);
    session_window->tabs = list;
}

inline unsigned char
session_window_focused_get(const Session_Window *session_window)
{
    return session_window->focused;
}

inline void
session_window_focused_set(Session_Window *session_window, unsigned char focused)
{
    EINA_SAFETY_ON_NULL_RETURN(session_window);
    session_window->focused = focused;
}
  

static inline void
_session_init(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (_session_descriptor) return;

    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, Session);
    _session_descriptor = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_LIST(_session_descriptor, Session, "windows", windows, _session_window_descriptor);
}

static inline void
_session_shutdown(void)
{
    if (!_session_descriptor) return;
    eet_data_descriptor_free(_session_descriptor);
    _session_descriptor = NULL;
}

Session *
session_new(Eina_List * windows)
{
    Session *session = calloc(1, sizeof(Session));

    if (!session)
       {
          fprintf(stderr, "ERROR: could not calloc Session\n");
          return NULL;
       }

    session->windows = windows;

    return session;
}

void
session_free(Session *session)
{
    if (session->windows)
       {
          Session_Window *windows_elem;
          EINA_LIST_FREE(session->windows, windows_elem)
             session_window_free(windows_elem);
       }
    free(session);
}

inline void
session_windows_add(Session *session, Session_Window *session_window)
{
    EINA_SAFETY_ON_NULL_RETURN(session);
    session->windows = eina_list_append(session->windows, session_window);
}

inline void
session_windows_del(Session *session, Session_Window *session_window)
{
    EINA_SAFETY_ON_NULL_RETURN(session);
    session->windows = eina_list_remove(session->windows, session_window);
}

inline Session_Window *
session_windows_get(const Session *session, unsigned int nth)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(session, NULL);
    return eina_list_nth(session->windows, nth);
}

inline unsigned int
session_windows_count(const Session *session)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(session, 0);
    return eina_list_count(session->windows);
}

void
session_windows_list_clear(Session *session)
{
    EINA_SAFETY_ON_NULL_RETURN(session);
    Session_Window *data;
    EINA_LIST_FREE(session->windows, data) session_window_free(data);
}

inline Eina_List *
session_windows_list_get(const Session *session)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(session, NULL);
    return session->windows;
}

inline void
session_windows_list_set(Session *session, Eina_List *list)
{
    EINA_SAFETY_ON_NULL_RETURN(session);
    session->windows = list;
}

Session *
session_load(const char *filename)
{
    Session *session = NULL;
    Eet_File *ef = eet_open(filename, EET_FILE_MODE_READ);
    if (!ef)
      {
        fprintf(stderr, "ERROR: could not open '%s' for read\n", filename);
        return NULL;
      }

    session = eet_data_read(ef, _session_descriptor, SESSION_ENTRY);
    if (!session) goto end;
    session->__eet_filename = eina_stringshare_add(filename);

end:
    eet_close(ef);
    return session;
}

Eina_Bool
session_save(Session *session, const char *filename)
{
    Eet_File *ef;
    Eina_Bool ret;

    if (filename) session->__eet_filename = eina_stringshare_add(filename);
    else if (session->__eet_filename) filename = session->__eet_filename;
    else return EINA_FALSE;

    ef = eet_open(filename, EET_FILE_MODE_READ_WRITE);
    if (!ef)
       {
          fprintf(stderr, "ERROR: could not open '%s' for write\n", filename);
          return EINA_FALSE;
       }

    ret = !!eet_data_write(ef, _session_descriptor, SESSION_ENTRY, session, EINA_TRUE);
    eet_close(ef);

    return ret;
}

void
eve_state_init(void)
{
    _config_init();
    _hist_item_init();
    _hist_init();
    _fav_item_init();
    _fav_init();
    _session_item_init();
    _session_window_init();
    _session_init();
}

void
eve_state_shutdown(void)
{
    _config_shutdown();
    _hist_item_shutdown();
    _hist_shutdown();
    _fav_item_shutdown();
    _fav_shutdown();
    _session_item_shutdown();
    _session_window_shutdown();
    _session_shutdown();
}

