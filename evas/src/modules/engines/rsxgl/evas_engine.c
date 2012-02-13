#include "evas_common.h"
#include "evas_private.h"
#include "evas_engine.h"
#include "rsxutil.h"
#include "evas_gl_common.h"
#include "Evas_Engine_RSXGL.h"

#include <EGL/egl.h>

#define GL3_PROTOTYPES
#include <GL3/gl3.h>

#include <malloc.h>
#include <fcntl.h>

int _evas_engine_rsxgl_log_dom = -1;

/* function tables - filled in later (func and parent func) */
static Evas_Func func, pfunc;

/* engine struct data */
typedef struct _Render_Engine Render_Engine;

#define MAX_BUFFERS 2

struct _Render_Engine
{
#if 0
   Tilebuf        *tb;
   Tilebuf_Rect   *rects;
   Eina_Inlist    *cur_rect;

#if 0
   /* RSX device context */
   gcmContextData *context;
   void           *host_addr;
#endif

#if 0
   /* The buffers we will be drawing into. */
   rsxBuffer       buffers[MAX_BUFFERS];
   int             currentBuffer;
#endif

   int             width;
   int             height;
   RGBA_Image     *rgba_image;
   uint32_t        rgba_image_offset;

   int             end : 1;
#endif

   int			w, h;

   Evas_Engine_GL_Context *gl_context;
   struct {
      int              redraw : 1;
      int              drew : 1;
      int              x1, y1, x2, y2;
   } draw;

   EGLContext       egl_context[1];
   EGLSurface       egl_surface[1];
   EGLConfig        egl_config;
   EGLDisplay       egl_disp;
};

/* prototypes we will use here */
static void *_output_setup(int w, int h);

static void *eng_info(Evas *e);
static void
             eng_info_free(Evas *e, void *info);
static int
             eng_setup(Evas *e, void *info);
static void
             eng_output_free(void *data);
static void
             eng_output_resize(void *data, int w, int h);

#if 0
static void
             eng_output_tile_size_set(void *data, int w, int h);
static void
             eng_output_redraws_rect_add(void *data, int x, int y, int w, int h);
static void
             eng_output_redraws_rect_del(void *data, int x, int y, int w, int h);
static void
             eng_output_redraws_clear(void *data);
static void *eng_output_redraws_next_update_get(void *data, int *x, int *y, int *w, int *h, int *cx, int *cy, int *cw, int *ch);
static void
             eng_output_redraws_next_update_push(void *data, void *surface, int x, int y, int w, int h);
static void
             eng_output_flush(void *data);
static void
             eng_output_idle_flush(void *data);
#endif

/* RSX: */
static void eng_rectangle_draw(void *data, void *context, void *surface, int x, int y, int w, int h);
static void eng_line_draw(void *data, void *context, void *surface, int x1, int y1, int x2, int y2);
static void eng_polygon_draw(void *data __UNUSED__, void *context, void *surface, void *polygon, int x, int y);
static void eng_image_draw(void *data __UNUSED__, void *context, void *surface, void *image, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h, int smooth);
static void eng_image_map_draw(void *data __UNUSED__, void *context, void *surface, void *image, int npoints, RGBA_Map_Point *p, int smooth, int level);
static void * eng_image_map_surface_new(void *data __UNUSED__, int w, int h, int alpha);
static void eng_image_map_surface_free(void *data __UNUSED__, void *surface);
static void eng_font_draw(void *data __UNUSED__, void *context, void *surface, Evas_Font_Set *font, int x, int y, int w __UNUSED__, int h __UNUSED__, int ow __UNUSED__, int oh __UNUSED__, const Evas_Text_Props *text_props);

void rsxgl_printf(const char * fmt,...)
{
  static int fd = -1;
  if(fd == -1) {
    mkdir("/dev_hdd0/tmp/evas",0777);
    fd = open("/dev_hdd0/tmp/evas/log",O_WRONLY | O_APPEND);
  }

  if(fd != -1) {
    static char buffer[2048];
    
    va_list ap;
    va_start(ap,fmt);
    vsnprintf(buffer,2048,fmt,ap);
    va_end(ap);

    write(fd,buffer,strlen(buffer));
  }
}

/* internal engine routines */
static void *
_output_setup(int w, int h)
{
  rsxgl_printf("%s\n",__PRETTY_FUNCTION__);

   /* if we haven't initialized - init (automatic abort if already done) */
   evas_common_cpu_init();
   evas_common_blend_init();
   evas_common_image_init();
   evas_common_convert_init();
   evas_common_scale_init();
   evas_common_rectangle_init();
   evas_common_polygon_init();
   evas_common_line_init();
   evas_common_font_init();
   evas_common_draw_init();
   evas_common_tilebuf_init();

   Render_Engine *re;
   int i;
#if 0
   u16 width, height;
   DATA32 *image_data = NULL;
   int image_size;
#endif

   printf ("_output_setup called : %dx%d\n", w, h);
   re = calloc(1, sizeof(Render_Engine));
   if (!re)
     return NULL;

#if 0
   /* Allocate a 1Mb buffer, alligned to a 1Mb boundary
    * to be our shared IO memory with the RSX. */
   re->host_addr = memalign (1024 * 1024, HOST_SIZE);
   if (re->host_addr == NULL)
     {
        free (re);
        return NULL;
     }
   re->context = initScreen (re->host_addr, HOST_SIZE);
   if (re->context == NULL)
     {
        free (re->host_addr);
        free (re);
        return NULL;
     }
#endif

   EGLDisplay dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
   if(dpy == EGL_NO_DISPLAY) {
     free(re);
     return NULL;
   }

   EGLint version0 = 0,version1 = 0;
   EGLBoolean result;
   result = eglInitialize(dpy,&version0,&version1);
   if(!result) {
     eglTerminate(dpy);
     free(re);
     return NULL;
   }

   EGLint attribs[] = {
     EGL_RED_SIZE,8,
     EGL_BLUE_SIZE,8,
     EGL_GREEN_SIZE,8,
     EGL_ALPHA_SIZE,8,
     
     EGL_DEPTH_SIZE,16,
     EGL_NONE
   };
   EGLConfig config;
   EGLint nconfig = 0;
   result = eglChooseConfig(dpy,attribs,&config,1,&nconfig);
   if(nconfig == 0) {
     eglTerminate(dpy);
     free(re);
     return NULL;
   }

   EGLSurface surface = eglCreateWindowSurface(dpy,config,0,0);
   if(surface == EGL_NO_SURFACE) {
     eglTerminate(dpy);
     free(re);
     return NULL;
   }

   int width = 0, height = 0;
   eglQuerySurface(dpy,surface,EGL_WIDTH,&width);
   eglQuerySurface(dpy,surface,EGL_HEIGHT,&height);

   EGLContext ctx = eglCreateContext(dpy,config,0,0);
   if(ctx == EGL_NO_CONTEXT) {
     eglTerminate(dpy);
     free(re);
     return NULL;
   }

   result = eglMakeCurrent(dpy,surface,surface,ctx);
   if(!result) {
     eglDestroyContext(dpy,ctx);
     eglTerminate(dpy);
     free(re);
   }

   re->egl_context[0] = ctx;
   re->egl_surface[0] = surface;
   re->egl_config = config;
   re->egl_disp = dpy;

#if 0
   width = w;
   height = h;
   setResolution (re->context, &width, &height);
   re->currentBuffer = 0;
   re->width = width;
   re->height = height;

   for (i = 0; i < MAX_BUFFERS; i++)
     makeBuffer (&re->buffers[i], width, height, i);

   flipBuffer(re->context, MAX_BUFFERS - 1);
#endif

   re->gl_context = evas_gl_common_context_new();
   if (!re->gl_context)
     {
	free(re);
	return NULL;
     }
   evas_gl_common_context_use(re->gl_context);
   evas_gl_common_context_resize(re->gl_context, w, h, re->gl_context->rot);

   re->w = width;
   re->h = height;

#if 0
   re->tb = evas_common_tilebuf_new(w, h);

   /* in preliminary tests 16x16 gave highest framerates */
   evas_common_tilebuf_set_tile_size(re->tb, TILESIZE, TILESIZE);

   /* Allocate our memaligned backbuffer */
   image_size = ((w * h * sizeof(u32)) + 0xfffff) & - 0x100000;
   image_data = memalign (1024 * 1024, image_size);
   re->rgba_image = (RGBA_Image *)evas_cache_image_data(evas_common_image_cache_get(),
                                                        w, h, image_data, 1, EVAS_COLORSPACE_ARGB8888);
   gcmMapMainMemory(image_data, image_size, &re->rgba_image_offset);
#endif

   return re;
}

/* engine api this module provides */
static void *
eng_info(Evas *e)
{
   Evas_Engine_Info_RSXGL *info;

   printf ("eng_info called\n");
   info = calloc(1, sizeof(Evas_Engine_Info_RSXGL));
   if (!info)
     return NULL;

   info->magic.magic = rand();
   info->render_mode = EVAS_RENDER_MODE_BLOCKING;

   return info;
}

static void
eng_info_free(Evas *e __UNUSED__, void *info)
{
   Evas_Engine_Info_RSXGL *in;

   printf ("eng_info_free called\n");
   in = (Evas_Engine_Info_RSXGL *)info;
   free(in);
}

static int
eng_setup(Evas *e, void *in)
{
   Evas_Engine_Info_RSXGL *info;

   printf ("eng_setup called\n");
   info = (Evas_Engine_Info_RSXGL *)in;

   e->engine.data.output = _output_setup(e->output.w, e->output.h);
   if (!e->engine.data.output)
     return 0;

   e->engine.func = &func;
   e->engine.data.context = e->engine.func->context_new(e->engine.data.output);

   return 1;
}

static void
eng_output_free(void *data)
{
   Render_Engine *re;
   int i;

   printf ("eng_output_free called\n");
   re = (Render_Engine *)data;

#if 0
   gcmSetWaitFlip(re->context);
   for (i = 0; i < MAX_BUFFERS; i++)
     rsxFree (re->buffers[i].ptr);
#endif

#if 0
   if (re->rgba_image)
     {
        DATA32 *image_data;

        image_data = re->rgba_image->image.data;
        evas_cache_image_drop(&re->rgba_image->cache_entry);
        free (image_data);
     }
#endif

#if 0
   freeScreen (re->context);
   free (re->host_addr);
#endif

#if 0
   evas_common_tilebuf_free(re->tb);
   if (re->rects)
     evas_common_tilebuf_free_render_rects(re->rects);
#endif

   evas_gl_common_context_free(re->gl_context);
   free(re);

   evas_common_font_shutdown();
   evas_common_image_shutdown();
}

static void
eng_output_resize(void *data, int w, int h)
{
   Render_Engine *re;
   int i;
   u16 width, height;
   DATA32 *image_data;
   int image_size;

   printf ("eng_output_resize called : %dx%d\n", w, h);
   re = (Render_Engine *)data;

#if 0
   width = w;
   height = h;
   if (setResolution (re->context, &width, &height))
     {
        re->width = width;
        re->height = height;

        gcmSetWaitFlip(re->context);
        for (i = 0; i < MAX_BUFFERS; i++) {
             rsxFree (re->buffers[i].ptr);
             makeBuffer (&re->buffers[i], width, height, i);
          }

        flipBuffer(re->context, MAX_BUFFERS - 1);

        evas_common_tilebuf_free(re->tb);
        re->tb = evas_common_tilebuf_new(w, h);
        evas_common_tilebuf_set_tile_size(re->tb, TILESIZE, TILESIZE);

        /* Realloc our backbuf image */
        if (re->rgba_image)
          {
             image_data = re->rgba_image->image.data;
             evas_cache_image_drop(&re->rgba_image->cache_entry);
             free (image_data);
          }
        image_size = ((w * h * sizeof(u32)) + 0xfffff) & - 0x100000;
        image_data = memalign (1024 * 1024, image_size);
        re->rgba_image = (RGBA_Image *)evas_cache_image_data(evas_common_image_cache_get(),
                                                             w, h, image_data, 1, EVAS_COLORSPACE_ARGB8888);
        gcmMapMainMemory(image_data, image_size, &re->rgba_image_offset);
     }
#endif
}

/* From the PSL1GHT engine: */
#if 0
static void
eng_output_tile_size_set(void *data, int w, int h)
{
   Render_Engine *re;

   printf ("eng_output_tile_size_set called : %dx%d\n", w, h);
   re = (Render_Engine *)data;
   evas_common_tilebuf_set_tile_size(re->tb, w, h);
}

static void
eng_output_redraws_rect_add(void *data, int x, int y, int w, int h)
{
   Render_Engine *re;

   //printf ("eng_output_redraws_rect_add called : %d,%d %dx%d\n", x, y, w, h);
   re = (Render_Engine *)data;
   evas_common_tilebuf_add_redraw(re->tb, x, y, w, h);
}

static void
eng_output_redraws_rect_del(void *data, int x, int y, int w, int h)
{
   Render_Engine *re;

   //printf ("eng_output_redraws_rect_del called : %d,%d %dx%d\n", x, y, w, h);
   re = (Render_Engine *)data;
   evas_common_tilebuf_del_redraw(re->tb, x, y, w, h);
}

static void
eng_output_redraws_clear(void *data)
{
   Render_Engine *re;

   //printf ("eng_output_redraws_clear called\n");
   re = (Render_Engine *)data;
   evas_common_tilebuf_clear(re->tb);
}

static void *
eng_output_redraws_next_update_get(void *data, int *x, int *y, int *w, int *h, int *cx, int *cy, int *cw, int *ch)
{
   Render_Engine *re;
   Tilebuf_Rect *rect;
   int ux, uy, uw, uh;

   re = (Render_Engine *)data;
   /*printf ("eng_output_redraws_next_update_get called : %d,%d %dx%d -- %d,%d %dx%d\n",
    *x, *y, *w, *h, *cx, *cy, *cw, *ch);*/
   if (re->end)
     {
        re->end = 0;
        return NULL;
     }
   if (!re->rects)
     {
        re->rects = evas_common_tilebuf_get_render_rects(re->tb);
        re->cur_rect = EINA_INLIST_GET(re->rects);
     }
   if (!re->cur_rect)
     return NULL;

   rect = (Tilebuf_Rect *)re->cur_rect;
   ux = rect->x; uy = rect->y; uw = rect->w; uh = rect->h;
   re->cur_rect = re->cur_rect->next;
   if (!re->cur_rect)
     {
        evas_common_tilebuf_free_render_rects(re->rects);
        re->rects = NULL;
        re->end = 1;
     }

   *x = *cx = ux;
   *y = *cy = uy;
   *w = *cw = uw;
   *h = *ch = uh;
   /*printf ("eng_output_redraws_next_update_get returning : %d,%d %dx%d -- %d,%d %dx%d\n",
    *x, *y, *w, *h, *cx, *cy, *cw, *ch);*/

   return re->rgba_image;
}

static void
eng_output_redraws_next_update_push(void *data, void *surface, int x, int y, int w, int h)
{
   /* Don't do anything, we'll just coy the whole buffer when it's time to flush */
}

static void
eng_output_flush(void *data)
{
   Render_Engine *re;
#if 0
   rsxBuffer *buffer;
#endif
   int width;
   int height;

   //printf ("eng_output_flush called\n");
   re = (Render_Engine *)data;
#if 0
   buffer = &re->buffers[re->currentBuffer];
#endif
   width = re->rgba_image->cache_entry.w;
   height = re->rgba_image->cache_entry.h;

   glClearColor(1,0,0,1);
   glClear(GL_COLOR_BUFFER_BIT);

   eglSwapBuffers(re->egl_disp,re->egl_surface[0]);

#if 0
   /* Wait for the flip before copying the buffer */
   waitFlip ();

   if (re->width == width &&
       re->height == height)
     {
        /* DMA the back buffer into the rsx buffer */
         rsxSetTransferImage (re->context,
                              GCM_TRANSFER_MAIN_TO_LOCAL,
                              buffer->offset, buffer->width * sizeof(u32),
                              0, 0,
                              re->rgba_image_offset, re->width * sizeof(u32),
                              0, 0, re->width, re->height, sizeof(u32));
     }
   else
     {
        gcmTransferScale gcm_scale;
        gcmTransferSurface gcm_surface;

        gcm_surface.format = GCM_TRANSFER_SURFACE_FORMAT_A8R8G8B8;
        gcm_surface.pitch = buffer->width * sizeof(u32);
        gcm_surface._pad0[0] = gcm_surface._pad0[1] = 0;
        gcm_surface.offset = buffer->offset;

        gcm_scale.interp = GCM_TRANSFER_INTERPOLATOR_LINEAR;
        gcm_scale.conversion = GCM_TRANSFER_CONVERSION_TRUNCATE;
        gcm_scale.format = GCM_TRANSFER_SCALE_FORMAT_A8R8G8B8;
        gcm_scale.origin = GCM_TRANSFER_ORIGIN_CORNER;
        gcm_scale.operation = GCM_TRANSFER_OPERATION_SRCCOPY;
        gcm_scale.offset = re->rgba_image_offset;
        gcm_scale.clipX = 0;
        gcm_scale.clipY = 0;
        gcm_scale.clipW = re->width;
        gcm_scale.clipH = re->height;
        gcm_scale.outX = 0;
        gcm_scale.outY = 0;
        gcm_scale.outW = re->width;
        gcm_scale.outH = re->height;
        gcm_scale.ratioX = rsxGetFixedSint32 ((float)width / (float)re->width);
        gcm_scale.ratioY = rsxGetFixedSint32 ((float)height / (float)re->height);
        gcm_scale.inX = 0;
        gcm_scale.inY = 0;
        gcm_scale.inW = (width & ~1); // Width must be a multiple of 2
        gcm_scale.inH = height;
        if (gcm_scale.inW < 2) // Minimum inW value is 2
          gcm_scale.inW = 2;
        if (gcm_scale.inW > 2046) // Maximum inW value is 2046
          gcm_scale.inW = 2046;
        if (gcm_scale.inH < 1) // Minimum inH value is 1
          gcm_scale.inH = 1;
        if (gcm_scale.inH > 2047) // Maximum inW value is 2047
          gcm_scale.inH = 2047;
        gcm_scale.pitch = sizeof(u32) * width;

        rsxSetTransferScaleMode (re->context, GCM_TRANSFER_MAIN_TO_LOCAL, GCM_TRANSFER_SURFACE);
        rsxSetTransferScaleSurface (re->context, &gcm_scale, &gcm_surface);
     }
   /* Wait for the DMA to finish */
   flushRSX (re->context);

   /* Flip buffer onto screen */
   flipBuffer (re->context, re->currentBuffer);
   re->currentBuffer = (re->currentBuffer + 1) % MAX_BUFFERS;
#endif
}

static void
eng_output_idle_flush(void *data)
{
   Render_Engine *re;

   printf ("eng_output_idle_flush called\n");
   re = (Render_Engine *)data;
}

static Eina_Bool
eng_canvas_alpha_get(void *data, void *context __UNUSED__)
{
   Render_Engine *re;

   // printf ("eng_output_alpha_get called\n");
   re = (Render_Engine *)data;
   return EINA_TRUE;
}

/* RSX */
static void
eng_rectangle_draw(void *data __UNUSED__, void *context, void *surface, int x, int y, int w, int h)
{
#ifdef BUILD_PIPE_RENDER
   if ((cpunum > 1)
#ifdef EVAS_FRAME_QUEUING
        && evas_common_frameq_enabled()
#endif
        )
     evas_common_pipe_rectangle_draw(surface, context, x, y, w, h);
   else
#endif
     {
	evas_common_rectangle_draw(surface, context, x, y, w, h);
	evas_common_cpu_end_opt();
     }
}

static void
eng_line_draw(void *data __UNUSED__, void *context, void *surface, int x1, int y1, int x2, int y2)
{
#ifdef BUILD_PIPE_RENDER
   if ((cpunum > 1)
 #ifdef EVAS_FRAME_QUEUING
        && evas_common_frameq_enabled()
#endif
        )
    evas_common_pipe_line_draw(surface, context, x1, y1, x2, y2);
   else
#endif   
     {
	evas_common_line_draw(surface, context, x1, y1, x2, y2);
	evas_common_cpu_end_opt();
     }
}

static void
eng_polygon_draw(void *data __UNUSED__, void *context, void *surface, void *polygon, int x, int y)
{
#ifdef BUILD_PIPE_RENDER
   if ((cpunum > 1)
#ifdef EVAS_FRAME_QUEUING
        && evas_common_frameq_enabled()
#endif
        )
     evas_common_pipe_poly_draw(surface, context, polygon, x, y);
   else
#endif
     {
	evas_common_polygon_draw(surface, context, polygon, x, y);
	evas_common_cpu_end_opt();
     }
}

static void
eng_image_draw(void *data __UNUSED__, void *context, void *surface, void *image, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h, int smooth)
{
   RGBA_Image *im;

   if (!image) return;
   im = image;
#ifdef BUILD_PIPE_RENDER
   if ((cpunum > 1)
#ifdef EVAS_FRAME_QUEUING
        && evas_common_frameq_enabled()
#endif
        )
     {
        evas_common_rgba_image_scalecache_prepare((Image_Entry *)(im), 
                                                  surface, context, smooth,
                                                  src_x, src_y, src_w, src_h,
                                                  dst_x, dst_y, dst_w, dst_h);
        
        evas_common_pipe_image_draw(im, surface, context, smooth,
                                    src_x, src_y, src_w, src_h,
                                    dst_x, dst_y, dst_w, dst_h);
     }
   else
#endif
     {
//        if (im->cache_entry.space == EVAS_COLORSPACE_ARGB8888)
//          evas_cache_image_load_data(&im->cache_entry);
//        evas_common_image_colorspace_normalize(im);
        evas_common_rgba_image_scalecache_prepare(&im->cache_entry, surface, context, smooth,
                                                  src_x, src_y, src_w, src_h,
                                                  dst_x, dst_y, dst_w, dst_h);
        evas_common_rgba_image_scalecache_do(&im->cache_entry, surface, context, smooth,
                                             src_x, src_y, src_w, src_h,
                                             dst_x, dst_y, dst_w, dst_h);
/*        
	if (smooth)
	  evas_common_scale_rgba_in_to_out_clip_smooth(im, surface, context,
						       src_x, src_y, src_w, src_h,
						       dst_x, dst_y, dst_w, dst_h);
	else
	  evas_common_scale_rgba_in_to_out_clip_sample(im, surface, context,
						       src_x, src_y, src_w, src_h,
						       dst_x, dst_y, dst_w, dst_h);
 */
	evas_common_cpu_end_opt();
     }
}

static void
eng_image_map_draw(void *data __UNUSED__, void *context, void *surface, void *image, int npoints, RGBA_Map_Point *p, int smooth, int level)
{
   RGBA_Image *im;

   if (!image) return;
   if (npoints < 3) return;
   im = image;

   if ((p[0].x == p[3].x) &&
       (p[1].x == p[2].x) &&
       (p[0].y == p[1].y) &&
       (p[3].y == p[2].y) &&
       (p[0].x <= p[1].x) &&
       (p[0].y <= p[2].y) &&
       (p[0].u == 0) &&
       (p[0].v == 0) &&
       (p[1].u == (int)(im->cache_entry.w << FP)) &&
       (p[1].v == 0) &&
       (p[2].u == (int)(im->cache_entry.w << FP)) &&
       (p[2].v == (int)(im->cache_entry.h << FP)) &&
       (p[3].u == 0) &&
       (p[3].v == (int)(im->cache_entry.h << FP)) &&
       (p[0].col == 0xffffffff) &&
       (p[1].col == 0xffffffff) &&
       (p[2].col == 0xffffffff) &&
       (p[3].col == 0xffffffff))
     {
        int dx, dy, dw, dh;

        dx = p[0].x >> FP;
        dy = p[0].y >> FP;
        dw = (p[2].x >> FP) - dx;
        dh = (p[2].y >> FP) - dy;
        eng_image_draw
          (data, context, surface, image,
           0, 0, im->cache_entry.w, im->cache_entry.h,
           dx, dy, dw, dh, smooth);
     }
   else
     {
#ifdef BUILD_PIPE_RENDER
        if ((cpunum > 1)
# ifdef EVAS_FRAME_QUEUING
       && evas_common_frameq_enabled()
# endif
        )
          evas_common_pipe_map_draw(im, surface, context, npoints, p, smooth, level);
        else
#endif
          evas_common_map_rgba(im, surface, context, npoints, p, smooth, level);
     }
   evas_common_cpu_end_opt();

   if (npoints > 4)
     {
        eng_image_map_draw(data, context, surface, image, npoints - 2, p + 2,
                           smooth, level);
     }
}

static void *
eng_image_map_surface_new(void *data __UNUSED__, int w, int h, int alpha)
{
   void *surface;
   DATA32 *pixels;
   surface = evas_cache_image_copied_data(evas_common_image_cache_get(), 
                                          w, h, NULL, alpha, 
                                          EVAS_COLORSPACE_ARGB8888);
   pixels = evas_cache_image_pixels(surface);
   return surface;
}

static void
eng_image_map_surface_free(void *data __UNUSED__, void *surface)
{
   evas_cache_image_drop(surface);
}

/* module advertising code */
static int
module_open(Evas_Module *em)
{
   if (!em) return 0;
   /* get whatever engine module we inherit from */
   if (!_evas_module_engine_inherit(&pfunc, "software_generic")) return 0;
   _evas_engine_rsxgl_log_dom = eina_log_domain_register
       ("evas-rsxgl", EVAS_DEFAULT_LOG_COLOR);
   if (_evas_engine_rsxgl_log_dom < 0)
     {
        EINA_LOG_ERR("Can not create a module log domain.");
        return 0;
     }

   /* store it for later use */
   func = pfunc;
   /* now to override methods */
#define ORD(f) EVAS_API_OVERRIDE(f, &func, eng_)
   ORD(info);
   ORD(info_free);
   ORD(setup);
   ORD(canvas_alpha_get);
   ORD(output_free);
   ORD(output_resize);
   ORD(output_tile_size_set);
   ORD(output_redraws_rect_add);
   ORD(output_redraws_rect_del);
   ORD(output_redraws_clear);
   ORD(output_redraws_next_update_get);
   ORD(output_redraws_next_update_push);
   ORD(output_flush);
   ORD(output_idle_flush);

   /* now advertise out own api */
   em->functions = (void *)(&func);
   return 1;
}

static void
module_close(Evas_Module *em __UNUSED__)
{
   eina_log_domain_unregister(_evas_engine_rsxgl_log_dom);
}

static Evas_Module_Api evas_modapi =
{
   EVAS_MODULE_API_VERSION,
   "rsxgl",
   "none",
   {
      module_open,
      module_close
   }
};

EVAS_MODULE_DEFINE(EVAS_MODULE_TYPE_ENGINE, engine, rsxgl);

#ifndef EVAS_STATIC_BUILD_RSXGL
EVAS_EINA_MODULE_DEFINE(engine, rsxgl);
#endif

/* From the PSL1GHT engine */
#endif

/* From the gl_sdl engine */
static void
eng_output_tile_size_set(void *data, int w __UNUSED__, int h __UNUSED__)
{
//   Render_Engine *re;
//
//   re = (Render_Engine *)data;
}

static void
eng_output_redraws_rect_add(void *data, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_gl_common_context_resize(re->gl_context, re->w, re->h, re->gl_context->rot);
   /* smple bounding box */
   if (!re->draw.redraw)
     {
#if 0
	re->draw.x1 = x;
	re->draw.y1 = y;
	re->draw.x2 = x + w - 1;
	re->draw.y2 = y + h - 1;
#else
	re->draw.x1 = 0;
	re->draw.y1 = 0;
	re->draw.x2 = re->w - 1;
	re->draw.y2 = re->h - 1;
#endif
     }
   else
     {
	if (x < re->draw.x1) re->draw.x1 = x;
	if (y < re->draw.y1) re->draw.y1 = y;
	if ((x + w - 1) > re->draw.x2) re->draw.x2 = x + w - 1;
	if ((y + h - 1) > re->draw.y2) re->draw.y2 = y + h - 1;
     }
   re->draw.redraw = 1;
}

static void
eng_output_redraws_rect_del(void *data, int x __UNUSED__, int y __UNUSED__, int w __UNUSED__, int h __UNUSED__)
{
//   Render_Engine *re;
//
//   re = (Render_Engine *)data;
}

static void
eng_output_redraws_clear(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   re->draw.redraw = 0;
//   INF("GL: finish update cycle!");
}

/* at least the nvidia drivers are so abysmal that copying from the backbuffer
 * to the front using glCopyPixels() that you literally can WATCH it draw the
 * pixels slowly across the screen with a window update taking multiple
 * seconds - so workaround by doing a full buffer render as frankly GL isn't
 * up to doing anything that isn't done by quake (etc.)
 */
#define SLOW_GL_COPY_RECT 1
/* vsync games - not for now though */
//#define VSYNC_TO_SCREEN 1

static void *
eng_output_redraws_next_update_get(void *data, int *x, int *y, int *w, int *h, int *cx, int *cy, int *cw, int *ch)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_gl_common_context_flush(re->gl_context);
   evas_gl_common_context_newframe(re->gl_context);
   /* get the upate rect surface - return engine data as dummy */
   if (!re->draw.redraw)
     {
//	printf("GL: NO updates!\n");
	return NULL;
     }
//   printf("GL: update....!\n");
#ifdef SLOW_GL_COPY_RECT
   /* if any update - just return the whole canvas - works with swap
    * buffers then */
   if (x) *x = 0;
   if (y) *y = 0;
   if (w) *w = re->w;
   if (h) *h = re->h;
   if (cx) *cx = 0;
   if (cy) *cy = 0;
   if (cw) *cw = re->w;
   if (ch) *ch = re->h;
#else
   /* 1 update - INCREDIBLY SLOW if combined with swap_rect in flush. a gl
    * problem where there just is no hardware path for somethnig that
    * obviously SHOULD be there */
   /* only 1 update to minimise gl context games and rendering multiple update
    * regions as evas does with other engines
    */
   if (x) *x = re->draw.x1;
   if (y) *y = re->draw.y1;
   if (w) *w = re->draw.x2 - re->draw.x1 + 1;
   if (h) *h = re->draw.y2 - re->draw.y1 + 1;
   if (cx) *cx = re->draw.x1;
   if (cy) *cy = re->draw.y1;
   if (cw) *cw = re->draw.x2 - re->draw.x1 + 1;
   if (ch) *ch = re->draw.y2 - re->draw.y1 + 1;
#endif
// clear buffer. only needed for dest alpha
//   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//   glClear(GL_COLOR_BUFFER_BIT);
//x//   printf("frame -> new\n");
   return re->gl_context->def_surface;
}

static void
eng_output_redraws_next_update_push(void *data, void *surface __UNUSED__, int x __UNUSED__, int y __UNUSED__, int w __UNUSED__, int h __UNUSED__)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   /* put back update surface.. in this case just unflag redraw */
   re->draw.redraw = 0;
   re->draw.drew = 1;
   evas_gl_common_context_flush(re->gl_context);
//x//   printf("frame -> push\n");
}

static void
eng_output_flush(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   if (!re->draw.drew) return;
//x//   printf("frame -> flush\n");
   re->draw.drew = 0;

   glClearColor(1,0,0,1);
   glClear(GL_COLOR_BUFFER_BIT);

   eglSwapBuffers(re->egl_disp,re->egl_surface[0]);
}

static void
eng_output_idle_flush(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
}

static void
eng_output_dump(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_image_image_all_unload();
   evas_common_font_font_all_unload();
   evas_gl_common_image_all_unload(re->gl_context);
}

static void
eng_context_cutout_add(void *data, void *context, int x, int y, int w, int h)
{
//   Render_Engine *re;
//
//   re = (Render_Engine *)data;
//   re->gl_context->dc = context;
   evas_common_draw_context_add_cutout(context, x, y, w, h);
}

static void
eng_context_cutout_clear(void *data, void *context)
{
//   Render_Engine *re;
//
//   re = (Render_Engine *)data;
//   re->gl_context->dc = context;
   evas_common_draw_context_clear_cutouts(context);
}

static void
eng_rectangle_draw(void *data, void *context, void *surface, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_gl_common_context_target_surface_set(re->gl_context, surface);
   re->gl_context->dc = context;
   evas_gl_common_rect_draw(re->gl_context, x, y, w, h);
}

static void
eng_line_draw(void *data, void *context, void *surface, int x1, int y1, int x2, int y2)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_gl_common_context_target_surface_set(re->gl_context, surface);
   re->gl_context->dc = context;
   evas_gl_common_line_draw(re->gl_context, x1, y1, x2, y2);
}

static void *
eng_polygon_point_add(void *data, void *context __UNUSED__, void *polygon, int x, int y)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_gl_common_poly_point_add(polygon, x, y);
}

static void *
eng_polygon_points_clear(void *data, void *context __UNUSED__, void *polygon)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_gl_common_poly_points_clear(polygon);
}

static void
eng_polygon_draw(void *data, void *context, void *surface, void *polygon, int x, int y)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_gl_common_context_target_surface_set(re->gl_context, surface);
   re->gl_context->dc = context;
   evas_gl_common_poly_draw(re->gl_context, polygon, x, y);
}

static int
eng_image_alpha_get(void *data, void *image)
{
//   Render_Engine *re;
   Evas_GL_Image *im;

//   re = (Render_Engine *)data;
   if (!image) return 1;
   im = image;
   return im->alpha;
}

static int
eng_image_colorspace_get(void *data, void *image)
{
//   Render_Engine *re;
   Evas_GL_Image *im;

//   re = (Render_Engine *)data;
   if (!image) return EVAS_COLORSPACE_ARGB8888;
   im = image;
   return im->cs.space;
}

static void *
eng_image_alpha_set(void *data, void *image, int has_alpha)
{
   Render_Engine *re;
   Evas_GL_Image *im;

   re = (Render_Engine *)data;
   if (!image) return NULL;
   im = image;
   if (im->native.data)
     {
        im->alpha = has_alpha;
        return image;
     }
   /* FIXME: can move to gl_common */
   if (im->cs.space != EVAS_COLORSPACE_ARGB8888) return im;
   if ((has_alpha) && (im->im->cache_entry.flags.alpha)) return image;
   else if ((!has_alpha) && (!im->im->cache_entry.flags.alpha)) return image;
   if (im->references > 1)
     {
        Evas_GL_Image *im_new;
        
        im_new = evas_gl_common_image_new_from_copied_data(im->gc, im->im->cache_entry.w, im->im->cache_entry.h, im->im->image.data,
                                                           eng_image_alpha_get(data, image),
                                                           eng_image_colorspace_get(data, image));
        if (!im_new) return im;
        evas_gl_common_image_free(im);
        im = im_new;
     }
   else
     evas_gl_common_image_dirty(im, 0, 0, 0, 0);
   im->im->cache_entry.flags.alpha = has_alpha ? 1 : 0;
   return image;
}

static void *
eng_image_border_set(void *data, void *image, int l __UNUSED__, int r __UNUSED__, int t __UNUSED__, int b __UNUSED__)
{
//   Render_Engine *re;
//
//   re = (Render_Engine *)data;
   return image;
}

static void
eng_image_border_get(void *data, void *image __UNUSED__, int *l __UNUSED__, int *r __UNUSED__, int *t __UNUSED__, int *b __UNUSED__)
{
//   Render_Engine *re;
//
//   re = (Render_Engine *)data;
}

static char *
eng_image_comment_get(void *data, void *image, char *key __UNUSED__)
{
//   Render_Engine *re;
   Evas_GL_Image *im;

//   re = (Render_Engine *)data;
   if (!image) return NULL;
   im = image;
   if (!im->im) return NULL;
   return im->im->info.comment;
}

static char *
eng_image_format_get(void *data, void *image)
{
//   Render_Engine *re;
   Evas_GL_Image *im;

//   re = (Render_Engine *)data;
   im = image;
   return NULL;
}

static void
eng_image_colorspace_set(void *data, void *image, int cspace)
{
   Render_Engine *re;
   Evas_GL_Image *im;

   re = (Render_Engine *)data;
   if (!image) return;
   im = image;
   if (im->native.data) return;
   /* FIXME: can move to gl_common */
   if (im->cs.space == cspace) return;
   evas_cache_image_colorspace(&im->im->cache_entry, cspace);
   switch (cspace)
     {
      case EVAS_COLORSPACE_ARGB8888:
	if (im->cs.data)
	  {
	     if (!im->cs.no_free) free(im->cs.data);
	     im->cs.data = NULL;
	     im->cs.no_free = 0;
	  }
	break;
      case EVAS_COLORSPACE_YCBCR422P601_PL:
      case EVAS_COLORSPACE_YCBCR422P709_PL:
      case EVAS_COLORSPACE_YCBCR422601_PL:
      case EVAS_COLORSPACE_YCBCR420NV12601_PL:
      case EVAS_COLORSPACE_YCBCR420TM12601_PL:
        if (im->tex) evas_gl_common_texture_free(im->tex);
        im->tex = NULL;
	if (im->cs.data)
	  {
	     if (!im->cs.no_free) free(im->cs.data);
	  }
	im->cs.data = calloc(1, im->im->cache_entry.h * sizeof(unsigned char *) * 2);
	im->cs.no_free = 0;
	break;
      default:
	abort();
	break;
     }
   im->cs.space = cspace;
}

/////////////////////////////////////////////////////////////////////////
//
//
typedef struct _Native Native;

struct _Native
{
   Evas_Native_Surface ns;
   
#if defined (GLES_VARIETY_S3C6410) || defined (GLES_VARIETY_SGX)
   EGLSurface  egl_surface;
#endif
};

static void
_native_bind_cb(void *data, void *image)
{
}

static void
_native_unbind_cb(void *data, void *image)
{
}

static void
_native_free_cb(void *data, void *image)
{
}

static void *
eng_image_native_set(void *data, void *image, void *native)
{
   return NULL;
}

static void *
eng_image_native_get(void *data, void *image)
{
   return NULL;
}

//
//
/////////////////////////////////////////////////////////////////////////

static void *
eng_image_load(void *data, const char *file, const char *key, int *error, Evas_Image_Load_Opts *lo)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   *error = EVAS_LOAD_ERROR_NONE;
   return evas_gl_common_image_load(re->gl_context, file, key, lo, error);
}

static void *
eng_image_new_from_data(void *data, int w, int h, DATA32 *image_data, int alpha, int cspace)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_gl_common_image_new_from_data(re->gl_context, w, h, image_data, alpha, cspace);
}

static void *
eng_image_new_from_copied_data(void *data, int w, int h, DATA32 *image_data, int alpha, int cspace)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_gl_common_image_new_from_copied_data(re->gl_context, w, h, image_data, alpha, cspace);
}

static void
eng_image_free(void *data, void *image)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   if (!image) return;
   evas_gl_common_image_free(image);
}

static void
eng_image_size_get(void *data, void *image, int *w, int *h)
{
//   Render_Engine *re;
//
//   re = (Render_Engine *)data;
   if (!image)
     {
	*w = 0;
	*h = 0;
	return;
     }
   if (w) *w = ((Evas_GL_Image *)image)->w;
   if (h) *h = ((Evas_GL_Image *)image)->h;
}

static void *
eng_image_size_set(void *data, void *image, int w, int h)
{
   Render_Engine *re;
   Evas_GL_Image *im = image;
   Evas_GL_Image *im_old;
   
   re = (Render_Engine *)data;
   if (!im) return NULL;
   if (im->native.data)
     {
        im->w = w;
        im->h = h;
        return image;
     }
   im_old = image;

   switch (eng_image_colorspace_get(data, image))
     {
      case EVAS_COLORSPACE_YCBCR422P601_PL:
      case EVAS_COLORSPACE_YCBCR422P709_PL:
      case EVAS_COLORSPACE_YCBCR422601_PL:
      case EVAS_COLORSPACE_YCBCR420NV12601_PL:
      case EVAS_COLORSPACE_YCBCR420TM12601_PL:
         w &= ~0x1;
         break;
     }

   if ((im_old) && (im_old->im->cache_entry.w == w) && (im_old->im->cache_entry.h == h))
     return image;
   if (im_old)
     {
   	im = evas_gl_common_image_new(re->gl_context, w, h,
   				      eng_image_alpha_get(data, image),
   				      eng_image_colorspace_get(data, image));
/*
	evas_common_load_image_data_from_file(im_old->im);
	if (im_old->im->image->data)
	  {
	     evas_common_blit_rectangle(im_old->im, im->im, 0, 0, w, h, 0, 0);
	     evas_common_cpu_end_opt();
	  }
 */
        evas_gl_common_image_free(im_old);
     }
   else
     im = evas_gl_common_image_new(re->gl_context, w, h, 1, EVAS_COLORSPACE_ARGB8888);
   return im;
}

static void *
eng_image_dirty_region(void *data, void *image, int x, int y, int w, int h)
{
   Render_Engine *re;
   Evas_GL_Image *im = image;

   re = (Render_Engine *)data;
   if (!image) return NULL;
   if (im->native.data) return image;
   evas_gl_common_image_dirty(image, x, y, w, h);
   return image;
}

static void *
eng_image_data_get(void *data, void *image, int to_write, DATA32 **image_data, int *err)
{
   Render_Engine *re;
   Evas_GL_Image *im;
   int error;
   
   re = (Render_Engine *)data;
   if (!image)
     {
	*image_data = NULL;
        if (err) *err = EVAS_LOAD_ERROR_GENERIC;
	return NULL;
     }
   im = image;
   if (im->native.data)
     {
        *image_data = NULL;
        if (err) *err = EVAS_LOAD_ERROR_NONE;
        return im;
     }
   error = evas_cache_image_load_data(&im->im->cache_entry);
   switch (im->cs.space)
     {
      case EVAS_COLORSPACE_ARGB8888:
	if (to_write)
	  {
	     if (im->references > 1)
	       {
		  Evas_GL_Image *im_new;

   		  im_new = evas_gl_common_image_new_from_copied_data(im->gc, im->im->cache_entry.w, im->im->cache_entry.h, im->im->image.data,
   								     eng_image_alpha_get(data, image),
   								     eng_image_colorspace_get(data, image));
   		  if (!im_new)
   		    {
   		       *image_data = NULL;
                       if (err) *err = error;
                       return im;
   		    }
   		  evas_gl_common_image_free(im);
   		  im = im_new;
	       }
   	     else
   	       evas_gl_common_image_dirty(im, 0, 0, 0, 0);
	  }
	*image_data = im->im->image.data;
	break;
      case EVAS_COLORSPACE_YCBCR422P601_PL:
      case EVAS_COLORSPACE_YCBCR422P709_PL:
      case EVAS_COLORSPACE_YCBCR422601_PL:
      case EVAS_COLORSPACE_YCBCR420NV12601_PL:
      case EVAS_COLORSPACE_YCBCR420TM12601_PL:
	*image_data = im->cs.data;
	break;
      default:
	abort();
	break;
     }
   if (err) *err = error;
   return im;
}

static void *
eng_image_data_put(void *data, void *image, DATA32 *image_data)
{
   Render_Engine *re;
   Evas_GL_Image *im, *im2;

   re = (Render_Engine *)data;
   if (!image) return NULL;
   im = image;
   if (im->native.data) return image;
   switch (im->cs.space)
     {
      case EVAS_COLORSPACE_ARGB8888:
	if (image_data != im->im->image.data)
	  {
	     int w, h;

	     w = im->im->cache_entry.w;
	     h = im->im->cache_entry.h;
	     im2 = eng_image_new_from_data(data, w, h, image_data,
					   eng_image_alpha_get(data, image),
					   eng_image_colorspace_get(data, image));
   	     if (!im2) return im;
   	     evas_gl_common_image_free(im);
   	     im = im2;
	  }
        break;
      case EVAS_COLORSPACE_YCBCR422P601_PL:
      case EVAS_COLORSPACE_YCBCR422P709_PL:
      case EVAS_COLORSPACE_YCBCR422601_PL:
      case EVAS_COLORSPACE_YCBCR420NV12601_PL:
      case EVAS_COLORSPACE_YCBCR420TM12601_PL:
        if (image_data != im->cs.data)
	  {
	     if (im->cs.data)
	       {
		  if (!im->cs.no_free) free(im->cs.data);
	       }
	     im->cs.data = image_data;
	  }
	break;
      default:
	abort();
	break;
     }
   /* hmmm - but if we wrote... why bother? */
   evas_gl_common_image_dirty(im, 0, 0, 0, 0);
   return im;
}

static void
eng_image_data_preload_request(void *data __UNUSED__, void *image, const void *target)
{
   Evas_GL_Image *gim = image;
   RGBA_Image *im;

   if (!gim) return;
   if (gim->native.data) return;
   im = (RGBA_Image *)gim->im;
   if (!im) return;
   evas_cache_image_preload_data(&im->cache_entry, target);
}

static void
eng_image_data_preload_cancel(void *data __UNUSED__, void *image, const void *target)
{
   Evas_GL_Image *gim = image;
   RGBA_Image *im;

   if (!gim) return;
   if (gim->native.data) return;
   im = (RGBA_Image *)gim->im;
   if (!im) return;
   evas_cache_image_preload_cancel(&im->cache_entry, target);
}

static void
eng_image_draw(void *data, void *context, void *surface, void *image, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h, int smooth)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   if (!image) return;
   evas_gl_common_context_target_surface_set(re->gl_context, surface);
   re->gl_context->dc = context;
   evas_gl_common_image_draw(re->gl_context, image,
                             src_x, src_y, src_w, src_h,
                             dst_x, dst_y, dst_w, dst_h,
                             smooth);
}

static void
eng_image_scale_hint_set(void *data __UNUSED__, void *image, int hint)
{
}

static void
eng_image_map_draw(void *data __UNUSED__, void *context, void *surface, void *image, int npoints, RGBA_Map_Point *p, int smooth, int level)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_gl_common_context_target_surface_set(re->gl_context, surface);
   re->gl_context->dc = context;
   evas_gl_common_image_map_draw(re->gl_context, image, npoints, p, smooth, level);
}

static void *
eng_image_map_surface_new(void *data __UNUSED__, int w, int h, int alpha)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   return evas_gl_common_image_surface_new(re->gl_context, w, h, alpha);
}

static void
eng_image_map_surface_free(void *data __UNUSED__, void *surface)
{
   evas_gl_common_image_free(surface);
}

static int
eng_image_scale_hint_get(void *data __UNUSED__, void *image)
{
   return EVAS_IMAGE_SCALE_HINT_NONE;
}

static void
eng_font_draw(void *data, void *context, void *surface, Evas_Font_Set *font, int x, int y, int w __UNUSED__, int h __UNUSED__, int ow __UNUSED__, int oh __UNUSED__, const Evas_Text_Props *intl_props)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_gl_common_context_target_surface_set(re->gl_context, surface);
   re->gl_context->dc = context;
     {
        // FIXME: put im into context so we can free it
	static RGBA_Image *im = NULL;
        
        if (!im)
          im = (RGBA_Image *)evas_cache_image_empty(evas_common_image_cache_get());
        im->cache_entry.w = re->w;
        im->cache_entry.h = re->h;
        evas_common_draw_context_font_ext_set(context,
   					      re->gl_context,
   					      evas_gl_font_texture_new,
   					      evas_gl_font_texture_free,
   					      evas_gl_font_texture_draw);
	evas_common_font_draw(im, context, (RGBA_Font *) font, x, y,
              intl_props);
	evas_common_draw_context_font_ext_set(context,
					      NULL,
					      NULL,
					      NULL,
					      NULL);
     }
}

static Eina_Bool
eng_canvas_alpha_get(void *data __UNUSED__, void *info __UNUSED__)
{
   // FIXME: support ARGB gl targets!!!
   return EINA_FALSE;
}

static int
eng_image_load_error_get(void *data __UNUSED__, void *image)
{
   Evas_GL_Image *im;
   
   if (!image) return EVAS_LOAD_ERROR_NONE;
   im = image;
   return im->im->cache_entry.load_error;
}


static int
module_open(Evas_Module *em)
{
   if (!em) return 0;
   if (!evas_gl_common_module_open()) return 0;
   /* get whatever engine module we inherit from */
   if (!_evas_module_engine_inherit(&pfunc, "software_generic")) return 0;
   _evas_engine_rsxgl_log_dom = eina_log_domain_register
       ("evas-rsxgl", EVAS_DEFAULT_LOG_COLOR);
   if (_evas_engine_rsxgl_log_dom < 0)
     {
        EINA_LOG_ERR("Can not create a module log domain.");
        return 0;
     }

   /* store it for later use */
   func = pfunc;
   /* now to override methods */
   #define ORD(f) EVAS_API_OVERRIDE(f, &func, eng_)
   ORD(info);
   ORD(info_free);
   ORD(setup);
   ORD(canvas_alpha_get);
   ORD(output_free);
   ORD(output_resize);
   ORD(output_tile_size_set);
   ORD(output_redraws_rect_add);
   ORD(output_redraws_rect_del);
   ORD(output_redraws_clear);
   ORD(output_redraws_next_update_get);
   ORD(output_redraws_next_update_push);
   ORD(context_cutout_add);
   ORD(context_cutout_clear);
   ORD(output_flush);
   ORD(output_idle_flush);
   ORD(output_dump);
   ORD(rectangle_draw);
   ORD(line_draw);
   ORD(polygon_point_add);
   ORD(polygon_points_clear);
   ORD(polygon_draw);

   ORD(image_load);
   ORD(image_new_from_data);
   ORD(image_new_from_copied_data);
   ORD(image_free);
   ORD(image_size_get);
   ORD(image_size_set);
   ORD(image_dirty_region);
   ORD(image_data_get);
   ORD(image_data_put);
   ORD(image_data_preload_request);
   ORD(image_data_preload_cancel);
   ORD(image_alpha_set);
   ORD(image_alpha_get);
   ORD(image_border_set);
   ORD(image_border_get);
   ORD(image_draw);
   ORD(image_comment_get);
   ORD(image_format_get);
   ORD(image_colorspace_set);
   ORD(image_colorspace_get);
   ORD(image_native_set);
   ORD(image_native_get);
#if 0 // filtering disabled
//   ORD(image_draw_filtered);
//   ORD(image_filtered_get);
//   ORD(image_filtered_save);
//   ORD(image_filtered_free);
#endif
   ORD(font_draw);
   
   ORD(image_scale_hint_set);
   ORD(image_scale_hint_get);
   
   ORD(image_map_draw);
   ORD(image_map_surface_new);
   ORD(image_map_surface_free);

//   ORD(image_content_hint_set);
//   ORD(image_content_hint_get);
   
//   ORD(image_cache_flush);
//   ORD(image_cache_set);
//   ORD(image_cache_get);
   
//   ORD(gl_surface_create);
//   ORD(gl_surface_destroy);
//   ORD(gl_context_create);
//   ORD(gl_context_destroy);
//   ORD(gl_make_current);
//   ORD(gl_proc_address_get);
//   ORD(gl_native_surface_get);
   
//   ORD(gl_api_get);
   
   ORD(image_load_error_get);
   
   /* now advertise out own api */
   em->functions = (void *)(&func);
   return 1;
}

static void
module_close(Evas_Module *em)
{
    eina_log_domain_unregister(_evas_engine_rsxgl_log_dom);
    evas_gl_common_module_close();
}

static Evas_Module_Api evas_modapi =
{
   EVAS_MODULE_API_VERSION,
   "rsxgl",
   "none",
   {
     module_open,
     module_close
   }
};

EVAS_MODULE_DEFINE(EVAS_MODULE_TYPE_ENGINE, engine, rsxgl);

#ifndef EVAS_STATIC_BUILD_RSXGL
EVAS_EINA_MODULE_DEFINE(engine, rsxgl);
#endif
/* From the gl_sdl engine */
