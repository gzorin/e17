#include <Eina.h>
#include <Eet.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <Evas.h>
#include "tsuite_file_data.h"

static data_desc *desc = NULL; /* this struct holds descs (alloc on init) */

static eet_event_type_mapping eet_mapping[] = {
       { TSUITE_EVENT_MOUSE_IN, TSUITE_EVENT_MOUSE_IN_STR },
       { TSUITE_EVENT_MOUSE_OUT, TSUITE_EVENT_MOUSE_OUT_STR },
       { TSUITE_EVENT_MOUSE_DOWN, TSUITE_EVENT_MOUSE_DOWN_STR },
       { TSUITE_EVENT_MOUSE_UP, TSUITE_EVENT_MOUSE_UP_STR },
       { TSUITE_EVENT_MOUSE_MOVE, TSUITE_EVENT_MOUSE_MOVE_STR },
       { TSUITE_EVENT_MOUSE_WHEEL, TSUITE_EVENT_MOUSE_WHEEL_STR },
       { TSUITE_EVENT_MULTI_DOWN, TSUITE_EVENT_MULTI_DOWN_STR },
       { TSUITE_EVENT_MULTI_UP, TSUITE_EVENT_MULTI_UP_STR },
       { TSUITE_EVENT_MULTI_MOVE, TSUITE_EVENT_MULTI_MOVE_STR },
       { TSUITE_EVENT_KEY_DOWN, TSUITE_EVENT_KEY_DOWN_STR },
       { TSUITE_EVENT_KEY_UP, TSUITE_EVENT_KEY_UP_STR },
       { TSUITE_EVENT_TAKE_SHOT, TSUITE_EVENT_TAKE_SHOT_STR },
       { TSUITE_EVENT_NOT_SUPPORTED, NULL }
};

Tsuite_Event_Type
tsuite_event_mapping_type_get(const char *name)
{
   int i;
   for (i = 0; eet_mapping[i].name != NULL; ++i)
     if (strcmp(name, eet_mapping[i].name) == 0)
       return eet_mapping[i].t;

   return TSUITE_EVENT_NOT_SUPPORTED;
}

const char *
tsuite_event_mapping_type_str_get(Tsuite_Event_Type t)
{
   int i;
   for (i = 0; eet_mapping[i].name != NULL; ++i)
     if (t == eet_mapping[i].t)
       return eet_mapping[i].name;

   return NULL;
}

void
write_events(const char *filename, Lists_st *vr_list)
{
   _data_descriptors_init();
   Eet_File *fp = eet_open(filename, EET_FILE_MODE_WRITE);
   if (fp)
     {
        eet_data_write(fp, desc->_lists_descriptor, CACHE_FILE_ENTRY, vr_list,
              EINA_TRUE);

        eet_close(fp);
     }
   else
     {
        printf("Failed to create record file <%s>.\n", filename);
     }

   _data_descriptors_shutdown();
}

Lists_st *
read_events(char *filename, Evas *e, Timer_Data *td)
{
   Lists_st *vr_list;
   td->e = e;
   td->fp = eet_open(filename, EET_FILE_MODE_READ);
   if (!td->fp)
     {
        printf("Failed to open input file <%s>.\n", filename);
        return NULL;
     }

   /* Read events list */
   _data_descriptors_init();
   vr_list = eet_data_read(td->fp, desc->_lists_descriptor, CACHE_FILE_ENTRY);
   eet_close(td->fp);
   _data_descriptors_shutdown();
   if (!vr_list->variant_list)
        return NULL;

   td->current_event = eina_list_nth_list(vr_list->variant_list, 0);
#ifdef DEBUG_TSUITE
   printf("%s list size is <%d>\n", __func__, eina_list_count(vr_list->variant_list));
#endif

   return vr_list;
}

const char *
_variant_type_get(const void *data, Eina_Bool  *unknow)
{
   const Variant_Type_st *type = data;
   int i;

   if (unknow)
     *unknow = type->unknow;

   for (i = 0; eet_mapping[i].name != NULL; ++i)
     if (strcmp(type->type, eet_mapping[i].name) == 0)
       return eet_mapping[i].name;

   if (unknow)
     *unknow = EINA_FALSE;

   return type->type;
} /* _variant_type_get */

Eina_Bool
_variant_type_set(const char *type,
                  void       *data,
                  Eina_Bool   unknow)
{
   Variant_Type_st *vt = data;

   vt->type = type;
   vt->unknow = unknow;
   return EINA_TRUE;
} /* _variant_type_set */

/* START - Allocating and setting variant structs */
mouse_in_mouse_out *mouse_in_mouse_out_set(unsigned int timestamp)
{
   mouse_in_mouse_out *st = calloc(1, sizeof(mouse_in_mouse_out));
   if (st)
     {
        st->timestamp = timestamp;
     }

   return st;
}

Variant_st *mouse_in_mouse_out_new(Tsuite_Event_Type type,
      unsigned int timestamp)
{
   Variant_st *va = calloc(1, sizeof(Variant_st));
   va->t.type = eet_mapping[type].name;
   va->data = mouse_in_mouse_out_set(timestamp);

   return va;
}

mouse_down_mouse_up *mouse_down_mouse_up_set(int b, Evas_Button_Flags flags,
      unsigned int timestamp)
{
   mouse_down_mouse_up *st = calloc(1, sizeof(mouse_down_mouse_up));
   if (st)
     {
        st->b = b;
        st->flags = flags;
        st->timestamp = timestamp;
     }

   return st;
}

Variant_st *mouse_down_mouse_up_new(Tsuite_Event_Type type, int b,
      Evas_Button_Flags flags, unsigned int timestamp)
{
   Variant_st *va = calloc(1, sizeof(Variant_st));
   va->t.type = eet_mapping[type].name;
   va->data = mouse_down_mouse_up_set(b, flags, timestamp);

   return va;
}

mouse_move *mouse_move_set(int x, int y, unsigned int timestamp)
{
   mouse_move *st = calloc(1, sizeof(mouse_move));
   if (st)
     {
        st->x = x;
        st->y = y;
        st->timestamp = timestamp;
     }

   return st;
}

Variant_st *mouse_move_new(int x, int y, unsigned int timestamp)
{
   Variant_st *va = calloc(1, sizeof(Variant_st));
   va->t.type = eet_mapping[TSUITE_EVENT_MOUSE_MOVE].name;
   va->data = mouse_move_set(x, y, timestamp);

   return va;
}

mouse_wheel *mouse_wheel_set(int direction, int z, unsigned int timestamp)
{
   mouse_wheel *st = calloc(1, sizeof(mouse_wheel));
   if (st)
     {
        st->direction = direction;
        st->z = z;
        st->timestamp = timestamp;
     }

   return st;
}

Variant_st *mouse_wheel_new(int direction, int z, unsigned int timestamp)
{
   Variant_st *va = calloc(1, sizeof(Variant_st));
   va->t.type = eet_mapping[TSUITE_EVENT_MOUSE_WHEEL].name;
   va->data = mouse_wheel_set(direction, z, timestamp);

   return va;
}

multi_event *multi_event_set(int d, int x, int y, double rad,
      double radx, double rady, double pres, double ang,
      double fx, double fy, Evas_Button_Flags flags, unsigned int timestamp)
{
   multi_event *st = calloc(1, sizeof(multi_event));
   if (st)
     {
        st->d = d;
        st->x = x;
        st->y = y;
        st->rad = rad;
        st->radx = radx;
        st->rady = rady;
        st->pres = pres;
        st->ang = ang;
        st->fx = fx;
        st->fy = fy;
        st->flags = flags;
        st->timestamp = timestamp;
     }

   return st;
}

Variant_st *multi_event_new(Tsuite_Event_Type type, int d, int x, int y,
      double rad, double radx, double rady, double pres, double ang,
      double fx, double fy, Evas_Button_Flags flags, unsigned int timestamp)
{
   Variant_st *va = calloc(1, sizeof(Variant_st));
   va->t.type = eet_mapping[type].name;
   va->data = multi_event_set(d, x, y, rad, radx, rady, pres, ang, fx, fy,
         flags, timestamp);

   return va;
}

multi_move *multi_move_set(int d, int x, int y, double rad, double radx,
      double rady, double pres, double ang, double fx, double fy,
      unsigned int timestamp)
{
   multi_move *st = calloc(1, sizeof(multi_move));
   if (st)
     {
        st->d = d;
        st->x = x;
        st->y = y;
        st->rad = rad;
        st->radx = radx;
        st->rady = rady;
        st->pres = pres;
        st->ang = ang;
        st->fx = fx;
        st->fy = fy;
        st->timestamp = timestamp;
     }

   return st;
}

Variant_st *multi_move_new(int d, int x, int y,
      double rad, double radx,double rady, double pres, double ang,
      double fx, double fy, unsigned int timestamp)
{
   Variant_st *va = calloc(1, sizeof(Variant_st));
   va->t.type = eet_mapping[TSUITE_EVENT_MULTI_MOVE].name;
   va->data = multi_move_set(d, x, y, rad, radx, rady, pres, ang,
         fx, fy,timestamp);

   return va;
}

key_down_key_up *key_down_key_up_set(unsigned int timestamp,
      const char *keyname, const char *key, const char *string,
      const char *compose)
{
   key_down_key_up *st = calloc(1, sizeof(key_down_key_up));
   if (st)
     {
        st->timestamp = timestamp;
        st->keyname = keyname;
        st->key = key;
        st->string = string;
        st->compose = compose;
     }

   return st;
}

Variant_st *key_down_key_up_new(Tsuite_Event_Type type,
      unsigned int timestamp, const char *keyname, const char *key,
      const char *string, const char *compose)
{
   Variant_st *va = calloc(1, sizeof(Variant_st));
   va->t.type = eet_mapping[type].name;
   va->data = key_down_key_up_set(timestamp, keyname, key, string, compose);

   return va;
}

take_screenshot *take_screenshot_set(unsigned int timestamp)
{
   take_screenshot *st = calloc(1, sizeof(take_screenshot));
   if (st)
     {
        st->timestamp = timestamp;
     }

   return st;
}

Variant_st *take_screenshot_new(unsigned int timestamp)
{
   Variant_st *va = calloc(1, sizeof(Variant_st));
   va->t.type = eet_mapping[TSUITE_EVENT_TAKE_SHOT].name;
   va->data = take_screenshot_set(timestamp);

   return va;
}
/* END   - Allocating and setting variant structs */

/* START Event struct descriptors */
Eet_Data_Descriptor *
take_screenshot_desc_make(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor *desc;
   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, take_screenshot);
   desc = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, take_screenshot, "timestamp",
         timestamp, EET_T_UINT);

   return desc;
}

Eet_Data_Descriptor *
mouse_in_mouse_out_desc_make(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor *desc;
   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, mouse_in_mouse_out);
   desc = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_in_mouse_out, "timestamp",
         timestamp, EET_T_UINT);

   return desc;
}

Eet_Data_Descriptor *
mouse_down_mouse_up_desc_make(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor *desc;
   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, mouse_down_mouse_up);
   desc = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_down_mouse_up, "b", b, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_down_mouse_up, "flags",
         flags, EET_T_INT);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_down_mouse_up, "timestamp",
         timestamp, EET_T_UINT);

   return desc;
}

Eet_Data_Descriptor *
mouse_move_desc_make(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor *desc;
   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, mouse_move);
   desc = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_move, "x", x, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_move, "y", y, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_move, "timestamp",
         timestamp, EET_T_UINT);

   return desc;
}

Eet_Data_Descriptor *
mouse_wheel_desc_make(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor *desc;
   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, mouse_wheel);
   desc = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_wheel, "direction",
         direction, EET_T_INT);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_wheel, "z", z, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, mouse_wheel, "timestamp",
         timestamp, EET_T_UINT);

   return desc;
}

Eet_Data_Descriptor *
key_down_key_up_desc_make(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor *desc;
   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, key_down_key_up);
   desc = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, key_down_key_up, "timestamp",
         timestamp, EET_T_UINT);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, key_down_key_up, "keyname",
         keyname, EET_T_STRING);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, key_down_key_up, "key",
         key, EET_T_STRING);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, key_down_key_up, "string",
         string, EET_T_STRING);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, key_down_key_up, "compose",
         compose, EET_T_STRING);

   return desc;
}

Eet_Data_Descriptor *
multi_event_desc_make(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor *desc;
   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, multi_event);
   desc = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "d", d, EET_T_UINT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "x", x, EET_T_UINT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "y", y, EET_T_UINT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "rad", rad, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "radx", radx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "rady", rady, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "pres", pres, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "ang", ang, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "fx", fx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "fy", fy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "flags", flags, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_event, "timestamp",
         timestamp, EET_T_UINT);

   return desc;
}

Eet_Data_Descriptor *
multi_move_desc_make(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor *desc;
   EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, multi_move);
   desc = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "d", d, EET_T_UINT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "x", x, EET_T_UINT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "y", y, EET_T_UINT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "rad", rad, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "radx", radx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "rady", rady, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "pres", pres, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "ang", ang, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "fx", fx, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "fy", fy, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, multi_move, "timestamp",
         timestamp, EET_T_UINT);

   return desc;
}

/* declaring types */
data_desc *_data_descriptors_init(void)
{
   if (desc)  /* Was allocated */
     return desc;

   desc = calloc(1, sizeof(data_desc));

   Eet_Data_Descriptor_Class eddc;

   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Lists_st);
   desc->_lists_descriptor = eet_data_descriptor_file_new(&eddc);

   desc->take_screenshot = take_screenshot_desc_make();
   desc->mouse_in_mouse_out = mouse_in_mouse_out_desc_make();
   desc->mouse_down_mouse_up = mouse_down_mouse_up_desc_make();
   desc->mouse_move = mouse_move_desc_make();
   desc->mouse_wheel = mouse_wheel_desc_make();
   desc->multi_event = multi_event_desc_make();
   desc->multi_move = multi_move_desc_make();
   desc->key_down_key_up =  key_down_key_up_desc_make();

   /* for variant */
   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Variant_st);
   desc->_variant_descriptor = eet_data_descriptor_file_new(&eddc);

   eddc.version = EET_DATA_DESCRIPTOR_CLASS_VERSION;
   eddc.func.type_get = _variant_type_get;
   eddc.func.type_set = _variant_type_set;
   desc->_variant_unified_descriptor = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MOUSE_IN_STR, desc->mouse_in_mouse_out);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MOUSE_OUT_STR, desc->mouse_in_mouse_out);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MOUSE_DOWN_STR, desc->mouse_down_mouse_up);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MOUSE_UP_STR, desc->mouse_down_mouse_up);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MOUSE_MOVE_STR, desc->mouse_move);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MOUSE_WHEEL_STR, desc->mouse_wheel);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MULTI_DOWN_STR, desc->multi_event);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MULTI_UP_STR, desc->multi_event);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_MULTI_MOVE_STR, desc->multi_move);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_KEY_DOWN_STR, desc->key_down_key_up);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_KEY_UP_STR, desc->key_down_key_up);

   EET_DATA_DESCRIPTOR_ADD_MAPPING(desc->_variant_unified_descriptor,
         TSUITE_EVENT_TAKE_SHOT_STR, desc->take_screenshot);


   EET_DATA_DESCRIPTOR_ADD_VARIANT(desc->_variant_descriptor,
         Variant_st, "data", data, t, desc->_variant_unified_descriptor);

   EET_DATA_DESCRIPTOR_ADD_LIST(desc->_lists_descriptor,
         Lists_st, "variant_list", variant_list, desc->_variant_descriptor);

   return desc;
}

void _data_descriptors_shutdown(void)
{
   if (desc)
     {
        eet_data_descriptor_free(desc->mouse_in_mouse_out);
        eet_data_descriptor_free(desc->mouse_down_mouse_up);
        eet_data_descriptor_free(desc->mouse_move);
        eet_data_descriptor_free(desc->mouse_wheel);
        eet_data_descriptor_free(desc->multi_event);
        eet_data_descriptor_free(desc->multi_move);
        eet_data_descriptor_free(desc->key_down_key_up);
        eet_data_descriptor_free(desc->take_screenshot);
        eet_data_descriptor_free(desc->_lists_descriptor);
        eet_data_descriptor_free(desc->_variant_descriptor);
        eet_data_descriptor_free(desc->_variant_unified_descriptor);

        free(desc);
        desc = NULL;
        /* FIXME: Should probably only init and shutdown once */
     }
}
/* END   Event struct descriptors */
