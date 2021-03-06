#ifndef EVAS_ENGINE_SDL_16_H
#define EVAS_ENGINE_SDL_16_H

#include "evas_common.h"
#include "evas_private.h"
#include "evas_common_soft16.h"
#include "Evas_Engine_SDL_16.h"
extern int _evas_engine_soft16_sdl_log_dom ;
#ifdef ERR
# undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_evas_engine_soft16_sdl_log_dom, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_evas_engine_soft16_sdl_log_dom, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...) EINA_LOG_DOM_INFO(_evas_engine_soft16_sdl_log_dom, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...) EINA_LOG_DOM_WARN(_evas_engine_soft16_sdl_log_dom, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(_evas_engine_soft16_sdl_log_dom, __VA_ARGS__)

typedef struct _SDL_Engine_Image_Entry SDL_Engine_Image_Entry;
struct _SDL_Engine_Image_Entry
{
  Engine_Image_Entry     cache_entry;

  SDL_Surface           *surface;

  struct
  {
    unsigned int         engine_surface : 1;
  } flags;
};


typedef struct _Render_Engine Render_Engine;
struct _Render_Engine
{
  SDL_Engine_Image_Entry        *soft16_engine_image;

  Evas_Cache_Engine_Image       *cache;

  Soft16_Image                  *tmp_out;

  int                            w;
  int                            h;
  int                            rot;

  Tilebuf                       *tb;
  Tilebuf_Rect                  *rects;
  Tilebuf_Rect                  *cur_rect;

  SDL_Rect                      *update_rects;
  int                            update_rects_count;
  int                            update_rects_limit;

  struct
  {
    unsigned int                 fullscreen : 1;
    unsigned int                 noframe : 1;
    unsigned int                 alpha : 1;
    unsigned int                 hwsurface : 1;
    unsigned int                 end : 1;
  } flags;
};

#endif
