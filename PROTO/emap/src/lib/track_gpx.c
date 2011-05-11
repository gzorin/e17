
#include "emap_private.h"


#define GPX_NAME "name "
#define GPX_COORDINATES "trkpt "
#define GPX_LON "lon"
#define GPX_LAT "lat"

static  void             _parse(EMap_Track *track);
static  Eina_Bool        _parser_cb(void *data, Eina_Simple_XML_Type type,
                            const char *content, unsigned offset, unsigned length);
static  Eina_Bool        _parser_attributes_cb(void *data, const char *key, const char *value);

EMap_Track *emap_track_gpx_new(const char *file)
{
   if(!file)
   {
      ERR("File is null");
      return NULL;
   }

   EMap_Track *track = emap_track_new(EMAP_TRACK_TYPE_GPX);
   track->gpx.file = eina_stringshare_add(file);

   _parse(track);

   return track;
}

static void _parse(EMap_Track *track)
{
   if(!track) return;
   if(!track->type == EMAP_TRACK_TYPE_GPX) return;

   DBG("[Start] Load GPX file %s", track->gpx.file);

   FILE *f = fopen(track->gpx.file, "rb");
   if (f)
   {
      long sz;

      fseek(f, 0, SEEK_END);
      sz = ftell(f);
      if (sz > 0)
      {
         char *buf;

         fseek(f, 0, SEEK_SET);
         buf = malloc(sz);
         if (buf)
         {
            int n = fread(buf, 1, sz, f);
            eina_simple_xml_parse(buf, sz, EINA_TRUE, _parser_cb, track);

            free(buf);
         }
      }
      fclose(f);
   }
   else
   {
      ERR("can't open the file %s", track->gpx.file);
      return ;
   }

   DBG("[Done] Load GPX file %s", track->gpx.file);
}

static Eina_Bool _parser_cb(void *data, Eina_Simple_XML_Type type,
                            const char *content, unsigned offset, unsigned length)
{
   EMap_Track *track = data;

   if(type == EINA_SIMPLE_XML_OPEN)
   {
      if(!strncmp(GPX_NAME, content, strlen(GPX_NAME)))
      {
         track->gpx.xml_is_name = EINA_TRUE;
      }
      else if(!strncmp(GPX_COORDINATES, content, strlen(GPX_COORDINATES)))
      {
         EMap_Track_Node *node = emap_track_node_new(eina_list_count(track->nodes));
         emap_track_node_add(track, node);

         const char *tags = eina_simple_xml_tag_attributes_find(content, length);
         eina_simple_xml_attributes_parse(tags, length - (tags - content), _parser_attributes_cb, node);
      }
   }
   else if(type == EINA_SIMPLE_XML_DATA
            && track->gpx.xml_is_name)
   {
      emap_track_name_set(track, content);
      track->gpx.xml_is_name = EINA_FALSE;
   }

   return EINA_TRUE;
}

static Eina_Bool _parser_attributes_cb(void *data, const char *key, const char *value)
{
   EMap_Track_Node *node = data;
   double d;

   if(!strcmp(GPX_LAT, key))
   {
      sscanf(value, "%lf", &d);
      emap_track_node_lat_set(node, d);
   }
   else if(!strcmp(GPX_LON, key))
   {
      sscanf(value, "%lf", &d);
      emap_track_node_lon_set(node, d);
   }

   return EINA_TRUE;
}