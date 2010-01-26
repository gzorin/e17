/* EINA - EFL data type library
 * Copyright (C) 2007-2008 Jorge Luis Zapata Muga
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "eina_config.h"
#include "eina_private.h"
#include "eina_hash.h"
#include "eina_module.h"
#include "eina_log.h"
#include "eina_main.h"

/* undefs EINA_ARG_NONULL() so NULL checks are not compiled out! */
#include "eina_safety_checks.h"
#include "eina_mempool.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

static Eina_Hash *_backends;
static Eina_Array *_modules;
static int _eina_mempool_log_dom = -1;

#ifdef ERR
#undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_eina_mempool_log_dom, __VA_ARGS__)

#ifdef DBG
#undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_eina_mempool_log_dom, __VA_ARGS__)


static Eina_Mempool *
_new_va(const char *name, const char *context, const char *options, va_list args)
{
	Eina_Mempool_Backend *be;
	Eina_Mempool *mp;

	Eina_Error err = EINA_ERROR_NOT_MEMPOOL_MODULE;

	eina_error_set(0);
	be = eina_hash_find(_backends, name);
	if (!be)
		goto on_error;

	err = EINA_ERROR_OUT_OF_MEMORY;
	mp = malloc(sizeof(Eina_Mempool));
	if (!mp) goto on_error;

	/* FIXME why backend is not a pointer? */
	mp->backend = *be;
	mp->backend_data = mp->backend.init(context, options, args);

	return mp;

  on_error:
	eina_error_set(err);
	return NULL;
}

/* Built-in backend's prototypes */
#ifdef EINA_STATIC_BUILD_CHAINED_POOL
Eina_Bool chained_init(void);
void chained_shutdown(void);
#endif

#ifdef EINA_STATIC_BUILD_PASS_THROUGH
Eina_Bool pass_through_init(void);
void pass_through_shutdown(void);
#endif

#ifdef EINA_STATIC_BUILD_EMEMOA_UNKNOWN
Eina_Bool ememoa_unknown_init(void);
void ememoa_unknown_shutdown(void);
#endif

#ifdef EINA_STATIC_BUILD_EMEMOA_FIXED
Eina_Bool ememoa_fixed_init(void);
void ememoa_fixed_shutdown(void);
#endif

#ifdef EINA_STATIC_BUILD_FIXED_BITMAP
Eina_Bool fixed_bitmap_init(void);
void fixed_bitmap_shutdown(void);
#endif

/**
 * @endcond
 */

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

EAPI Eina_Error EINA_ERROR_NOT_MEMPOOL_MODULE = 0;

static const char EINA_ERROR_NOT_MEMPOOL_MODULE_STR[] = "Not a memory pool module.";

/**
 * @endcond
 */

EAPI Eina_Bool
eina_mempool_register(Eina_Mempool_Backend *be)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(be, 0);
   DBG("be=%p, name=%p", be, be->name);
   return eina_hash_add(_backends, be->name, be);
}

EAPI void
eina_mempool_unregister(Eina_Mempool_Backend *be)
{
   EINA_SAFETY_ON_NULL_RETURN(be);
   DBG("be=%p, name=%p", be, be->name);
   eina_hash_del(_backends, be->name, be);
}

Eina_Bool
eina_mempool_init(void)
{
   char *path;

   _eina_mempool_log_dom = eina_log_domain_register("eina_mempool", EINA_LOG_COLOR_DEFAULT);
   if (_eina_mempool_log_dom < 0)
     {
	EINA_LOG_ERR("Could not register log domain: eina_mempool");
	return 0;
     }

   EINA_ERROR_NOT_MEMPOOL_MODULE = eina_error_msg_static_register(EINA_ERROR_NOT_MEMPOOL_MODULE_STR);
   _backends = eina_hash_string_superfast_new(NULL);

   /* dynamic backends */
   _modules = eina_module_arch_list_get(NULL, PACKAGE_LIB_DIR "/eina/modules/mp", MODULE_ARCH);

   path = eina_module_environment_path_get("HOME", "/.eina/mp/modules/mp");
   _modules = eina_module_arch_list_get(_modules, path, MODULE_ARCH);
   if (path) free(path);

   path = eina_module_environment_path_get("EINA_MODULES_MEMPOOL_DIR", "/eina/modules/mp");
   _modules = eina_module_arch_list_get(_modules, path, MODULE_ARCH);
   if (path) free(path);

   path = eina_module_symbol_path_get(eina_init, "/eina/modules/mp");
   _modules = eina_module_arch_list_get(_modules, path, MODULE_ARCH);
   if (path) free(path);

   if (!_modules)
     {
	ERR("no mempool modules able to be loaded.");
	eina_hash_free(_backends);
	goto mempool_init_error;
     }
   eina_module_list_load(_modules);
   
   /* builtin backends */
#ifdef EINA_STATIC_BUILD_CHAINED_POOL
   chained_init();
#endif
#ifdef EINA_STATIC_BUILD_PASS_THROUGH
   pass_through_init();
#endif
#ifdef EINA_STATIC_BUILD_EMEMOA_UNKNOWN
   ememoa_unknown_init();
#endif
#ifdef EINA_STATIC_BUILD_EMEMOA_FIXED
   ememoa_fixed_init();
#endif
#ifdef EINA_STATIC_BUILD_FIXED_BITMAP
   fixed_bitmap_init();
#endif
#ifdef EINA_STATIC_BUILD_BUDDY
   buddy_init();
#endif

   return EINA_TRUE;

 mempool_init_error:
   eina_log_domain_unregister(_eina_mempool_log_dom);
   _eina_mempool_log_dom = -1;

   return EINA_FALSE;
}

Eina_Bool
eina_mempool_shutdown(void)
{
   /* builtin backends */
#ifdef EINA_STATIC_BUILD_CHAINED_POOL
   chained_shutdown();
#endif
#ifdef EINA_STATIC_BUILD_PASS_THROUGH
   pass_through_shutdown();
#endif
#ifdef EINA_STATIC_BUILD_EMEMOA_UNKNOWN
   ememoa_unknown_shutdown();
#endif
#ifdef EINA_STATIC_BUILD_EMEMOA_FIXED
   ememoa_fixed_shutdown();
#endif
#ifdef EINA_STATIC_BUILD_FIXED_BITMAP
   fixed_bitmap_shutdown();
#endif
#ifdef EINA_STATIC_BUILD_BUDDY
   buddy_shutdown();
#endif
   /* dynamic backends */
   eina_module_list_free(_modules);
   if (_modules)
     eina_array_free(_modules);

   if (_backends)
     eina_hash_free(_backends);

   eina_log_domain_unregister(_eina_mempool_log_dom);
   _eina_mempool_log_dom = -1;

   return EINA_TRUE;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

/**
 * @addtogroup Eina_Memory_Pool_Group Memory Pool
 *
 * @brief These functions provide memory pool management.
 *
 * @{
 */

EAPI Eina_Mempool *
eina_mempool_add(const char *name, const char *context, const char *options, ...)
{
	Eina_Mempool *mp;
	va_list args;

	EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);
	DBG("name=%s, context=%s, options=%s",
	    name, context ? context : "", options ? options : "");

	va_start(args, options);
	mp = _new_va(name, context, options, args);
	va_end(args);

	DBG("name=%s, context=%s, options=%s, mp=%p",
	    name, context ? context : "", options ? options : "", mp);

	return mp;
}

EAPI void eina_mempool_del(Eina_Mempool *mp)
{
        EINA_SAFETY_ON_NULL_RETURN(mp);
	EINA_SAFETY_ON_NULL_RETURN(mp->backend.shutdown);
	DBG("mp=%p", mp);
	mp->backend.shutdown(mp->backend_data);
	free(mp);
}

EAPI void eina_mempool_gc(Eina_Mempool *mp)
{
        EINA_SAFETY_ON_NULL_RETURN(mp);
        EINA_SAFETY_ON_NULL_RETURN(mp->backend.garbage_collect);
	DBG("mp=%p", mp);
	mp->backend.garbage_collect(mp->backend_data);
}

EAPI void eina_mempool_statistics(Eina_Mempool *mp)
{
        EINA_SAFETY_ON_NULL_RETURN(mp);
        EINA_SAFETY_ON_NULL_RETURN(mp->backend.statistics);
	DBG("mp=%p", mp);
	mp->backend.statistics(mp->backend_data);
}

/**
 * @}
 */
