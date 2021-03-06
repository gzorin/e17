#include <stdlib.h>
#include <stdint.h>
#include <Evas.h>

#include "Elixir.h"

static const struct {
   const char*	name;
   int		value;
} evas_const_properties[] = {
   { "EVAS_CALLBACK_MOUSE_IN", EVAS_CALLBACK_MOUSE_IN },
   { "EVAS_CALLBACK_MOUSE_OUT", EVAS_CALLBACK_MOUSE_OUT },
   { "EVAS_CALLBACK_MOUSE_DOWN", EVAS_CALLBACK_MOUSE_DOWN },
   { "EVAS_CALLBACK_MOUSE_UP", EVAS_CALLBACK_MOUSE_UP },
   { "EVAS_CALLBACK_MOUSE_MOVE", EVAS_CALLBACK_MOUSE_MOVE },
   { "EVAS_CALLBACK_MOUSE_WHEEL", EVAS_CALLBACK_MOUSE_WHEEL },
   { "EVAS_CALLBACK_FREE", EVAS_CALLBACK_FREE },
   { "EVAS_CALLBACK_KEY_DOWN", EVAS_CALLBACK_KEY_DOWN },
   { "EVAS_CALLBACK_KEY_UP", EVAS_CALLBACK_KEY_UP },
   { "EVAS_CALLBACK_FOCUS_IN", EVAS_CALLBACK_FOCUS_IN },
   { "EVAS_CALLBACK_FOCUS_OUT", EVAS_CALLBACK_FOCUS_OUT },
   { "EVAS_CALLBACK_SHOW", EVAS_CALLBACK_SHOW },
   { "EVAS_CALLBACK_HIDE", EVAS_CALLBACK_HIDE },
   { "EVAS_CALLBACK_MOVE", EVAS_CALLBACK_MOVE },
   { "EVAS_CALLBACK_RESIZE", EVAS_CALLBACK_RESIZE },
   { "EVAS_CALLBACK_RESTACK", EVAS_CALLBACK_RESTACK },
   { "EVAS_CALLBACK_DEL", EVAS_CALLBACK_DEL },
   { "EVAS_CALLBACK_HOLD", EVAS_CALLBACK_HOLD },
   { "EVAS_CALLBACK_CHANGED_SIZE_HINTS", EVAS_CALLBACK_CHANGED_SIZE_HINTS },
   { "EVAS_CALLBACK_IMAGE_PRELOADED", EVAS_CALLBACK_IMAGE_PRELOADED },

   { "EVAS_CALLBACK_CANVAS_FOCUS_IN", EVAS_CALLBACK_CANVAS_FOCUS_IN },
   { "EVAS_CALLBACK_CANVAS_FOCUS_OUT", EVAS_CALLBACK_CANVAS_FOCUS_OUT },
   { "EVAS_CALLBACK_RENDER_FLUSH_PRE", EVAS_CALLBACK_RENDER_FLUSH_PRE },
   { "EVAS_CALLBACK_RENDER_FLUSH_POST", EVAS_CALLBACK_RENDER_FLUSH_POST },
   { "EVAS_CALLBACK_CANVAS_OBJECT_FOCUS_IN", EVAS_CALLBACK_CANVAS_OBJECT_FOCUS_IN },
   { "EVAS_CALLBACK_CANVAS_OBJECT_FOCUS_OUT", EVAS_CALLBACK_CANVAS_OBJECT_FOCUS_OUT },

   { "EVAS_BUTTON_NONE", EVAS_BUTTON_NONE },
   { "EVAS_BUTTON_DOUBLE_CLICK", EVAS_BUTTON_DOUBLE_CLICK },
   { "EVAS_BUTTON_TRIPLE_CLICK", EVAS_BUTTON_TRIPLE_CLICK },

   { "EVAS_FONT_HINTING_NONE", EVAS_FONT_HINTING_NONE },
   { "EVAS_FONT_HINTING_AUTO", EVAS_FONT_HINTING_AUTO },
   { "EVAS_FONT_HINTING_BYTECODE", EVAS_FONT_HINTING_BYTECODE },

   { "EVAS_COLORSPACE_ARGB8888", EVAS_COLORSPACE_ARGB8888 },
   { "EVAS_COLORSPACE_YCBCR422P601_PL", EVAS_COLORSPACE_YCBCR422P601_PL },
   { "EVAS_COLORSPACE_YCBCR422P709_PL", EVAS_COLORSPACE_YCBCR422P709_PL },
   { "EVAS_COLORSPACE_RGB565_A5P", EVAS_COLORSPACE_RGB565_A5P },

   { "EVAS_ASPECT_CONTROL_NONE", EVAS_ASPECT_CONTROL_NONE },
   { "EVAS_ASPECT_CONTROL_NEITHER", EVAS_ASPECT_CONTROL_NEITHER },
   { "EVAS_ASPECT_CONTROL_HORIZONTAL", EVAS_ASPECT_CONTROL_HORIZONTAL },
   { "EVAS_ASPECT_CONTROL_VERTICAL", EVAS_ASPECT_CONTROL_VERTICAL },
   { "EVAS_ASPECT_CONTROL_BOTH", EVAS_ASPECT_CONTROL_BOTH },

   { "EVAS_LOAD_ERROR_NONE", EVAS_LOAD_ERROR_NONE },
   { "EVAS_LOAD_ERROR_GENERIC", EVAS_LOAD_ERROR_GENERIC },
   { "EVAS_LOAD_ERROR_DOES_NOT_EXIST", EVAS_LOAD_ERROR_DOES_NOT_EXIST },
   { "EVAS_LOAD_ERROR_PERMISSION_DENIED", EVAS_LOAD_ERROR_PERMISSION_DENIED },
   { "EVAS_LOAD_ERROR_PERMISSION_DENIED", EVAS_LOAD_ERROR_PERMISSION_DENIED },
   { "EVAS_LOAD_ERROR_RESOURCE_ALLOCATION_FAILED", EVAS_LOAD_ERROR_RESOURCE_ALLOCATION_FAILED },
   { "EVAS_LOAD_ERROR_CORRUPT_FILE", EVAS_LOAD_ERROR_CORRUPT_FILE },
   { "EVAS_LOAD_ERROR_UNKNOWN_FORMAT", EVAS_LOAD_ERROR_UNKNOWN_FORMAT },

   { "EVAS_ALLOC_ERROR_NONE", EVAS_ALLOC_ERROR_NONE },
   { "EVAS_ALLOC_ERROR_FATAL", EVAS_ALLOC_ERROR_FATAL },
   { "EVAS_ALLOC_ERROR_RECOVERED", EVAS_ALLOC_ERROR_RECOVERED },

   { "EVAS_TEXT_INVALID", EVAS_TEXT_INVALID },
   { "EVAS_TEXT_SPECIAL", EVAS_TEXT_SPECIAL },

   { "EVAS_PIXEL_FORMAT_NONE", EVAS_PIXEL_FORMAT_NONE },
   { "EVAS_PIXEL_FORMAT_ARGB32", EVAS_PIXEL_FORMAT_ARGB32 },
   { "EVAS_PIXEL_FORMAT_YUV420P_601", EVAS_PIXEL_FORMAT_YUV420P_601 },

   { "EVAS_COLOR_SPACE_ARGB", EVAS_COLOR_SPACE_ARGB },
   { "EVAS_COLOR_SPACE_AHSV", EVAS_COLOR_SPACE_AHSV },

   { "EVAS_TEXTURE_REFLECT", EVAS_TEXTURE_REFLECT },
   { "EVAS_TEXTURE_REPEAT", EVAS_TEXTURE_REPEAT },
   { "EVAS_TEXTURE_RESTRICT", EVAS_TEXTURE_RESTRICT },
   { "EVAS_TEXTURE_RESTRICT_REFLECT", EVAS_TEXTURE_RESTRICT_REFLECT },
   { "EVAS_TEXTURE_RESTRICT_REPEAT", EVAS_TEXTURE_RESTRICT_REPEAT },
   { "EVAS_TEXTURE_PAD", EVAS_TEXTURE_PAD },

   { "EVAS_RENDER_BLEND", EVAS_RENDER_BLEND },
   { "EVAS_RENDER_BLEND_REL", EVAS_RENDER_BLEND_REL },
   { "EVAS_RENDER_COPY", EVAS_RENDER_COPY },
   { "EVAS_RENDER_COPY_REL", EVAS_RENDER_COPY_REL },
   { "EVAS_RENDER_ADD", EVAS_RENDER_ADD },
   { "EVAS_RENDER_ADD_REL", EVAS_RENDER_ADD_REL },
   { "EVAS_RENDER_SUB", EVAS_RENDER_SUB },
   { "EVAS_RENDER_SUB_REL", EVAS_RENDER_SUB_REL },
   { "EVAS_RENDER_TINT", EVAS_RENDER_TINT },
   { "EVAS_RENDER_TINT_REL", EVAS_RENDER_TINT_REL },
   { "EVAS_RENDER_MASK", EVAS_RENDER_MASK },
   { "EVAS_RENDER_MUL", EVAS_RENDER_MUL },

   { "EVAS_TEXT_STYLE_PLAIN", EVAS_TEXT_STYLE_PLAIN },
   { "EVAS_TEXT_STYLE_SHADOW", EVAS_TEXT_STYLE_SHADOW },
   { "EVAS_TEXT_STYLE_OUTLINE", EVAS_TEXT_STYLE_OUTLINE },
   { "EVAS_TEXT_STYLE_GLOW", EVAS_TEXT_STYLE_GLOW },
   { "EVAS_TEXT_STYLE_OUTLINE_SHADOW", EVAS_TEXT_STYLE_OUTLINE_SHADOW },
   { "EVAS_TEXT_STYLE_FAR_SHADOW", EVAS_TEXT_STYLE_FAR_SHADOW },
   { "EVAS_TEXT_STYLE_OUTLINE_SOFT_SHADOW", EVAS_TEXT_STYLE_OUTLINE_SOFT_SHADOW },
   { "EVAS_TEXT_STYLE_SOFT_SHADOW", EVAS_TEXT_STYLE_SOFT_SHADOW },
   { "EVAS_TEXT_STYLE_FAR_SOFT_SHADOW", EVAS_TEXT_STYLE_FAR_SOFT_SHADOW },
   { "EVAS_TEXTBLOCK_TEXT_RAW", EVAS_TEXTBLOCK_TEXT_RAW },
   { "EVAS_TEXTBLOCK_TEXT_PLAIN", EVAS_TEXTBLOCK_TEXT_PLAIN },

   { "EVAS_BORDER_FILL_NONE", EVAS_BORDER_FILL_NONE },
   { "EVAS_IMAGE_SCALE_HINT_DYNAMIC", EVAS_IMAGE_SCALE_HINT_DYNAMIC },
   { "EVAS_IMAGE_SCALE_HINT_STATIC", EVAS_IMAGE_SCALE_HINT_STATIC },

   { "EVAS_LAYER_MIN", EVAS_LAYER_MIN },
   { "EVAS_LAYER_MAX", EVAS_LAYER_MAX },

   { "EVAS_HINT_EXPAND", EVAS_HINT_EXPAND },
   { "EVAS_HINT_FILL", EVAS_HINT_FILL },

   { "EVAS_SMART_CLASS_VERSION", EVAS_SMART_CLASS_VERSION },

   { NULL, 0 }
};

Eina_Bool
load_evas_const_binding(JSContext *cx, JSObject *parent)
{
   unsigned int	i;

   for (i = 0; evas_const_properties[i].name; ++i)
     if (!elixir_add_int_prop(cx, parent,
                             evas_const_properties[i].name,
                             evas_const_properties[i].value))
       return EINA_FALSE;

   return EINA_TRUE;
}

Eina_Bool
unload_evas_const_binding(JSContext *cx, JSObject *parent)
{
   unsigned int i = 0;

   while (evas_const_properties[i].name)
     JS_DeleteProperty(cx, parent, evas_const_properties[i++].name);

   return EINA_TRUE;
}

