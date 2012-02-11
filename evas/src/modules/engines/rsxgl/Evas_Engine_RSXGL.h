#ifndef _EVAS_ENGINE_RSXGL_H
#define _EVAS_ENGINE_RSXGL_H

typedef struct _Evas_Engine_Info_RSXGL Evas_Engine_Info_RSXGL;

struct _Evas_Engine_Info_RSXGL
{
   /* PRIVATE - don't mess with this baby or evas will poke its tongue out */
   /* at you and make nasty noises */
     Evas_Engine_Info        magic;

     /* non-blocking or blocking mode */
     Evas_Engine_Render_Mode render_mode;
};
#endif

