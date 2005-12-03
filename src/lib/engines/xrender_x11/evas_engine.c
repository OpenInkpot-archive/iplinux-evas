#include "evas_common.h"
#include "evas_private.h"
#include "evas_engine.h"
#include "evas_engine_api_xrender_x11.h"
#include "Evas_Engine_XRender_X11.h"

static void *evas_engine_xrender_x11_info(Evas *e);
static void evas_engine_xrender_x11_info_free(Evas *e, void *info);
static void evas_engine_xrender_x11_setup(Evas *e, void *info);
static void evas_engine_xrender_x11_output_free(void *data);
static void evas_engine_xrender_x11_output_resize(void *data, int w, int h);
static void evas_engine_xrender_x11_output_tile_size_set(void *data, int w, int h);
static void evas_engine_xrender_x11_output_redraws_rect_add(void *data, int x, int y, int w, int h);
static void evas_engine_xrender_x11_output_redraws_rect_del(void *data, int x, int y, int w, int h);
static void evas_engine_xrender_x11_output_redraws_clear(void *data);
static void *evas_engine_xrender_x11_output_redraws_next_update_get(void *data, int *x, int *y, int *w, int *h, int *cx, int *cy, int *cw, int *ch);
static void evas_engine_xrender_x11_output_redraws_next_update_push(void *data, void *surface, int x, int y, int w, int h);
static void evas_engine_xrender_x11_output_flush(void *data);
static void *evas_engine_xrender_x11_context_new(void *data);
static void evas_engine_xrender_x11_context_free(void *data, void *context);
static void evas_engine_xrender_x11_context_clip_set(void *data, void *context, int x, int y, int w, int h);
static void evas_engine_xrender_x11_context_clip_clip(void *data, void *context, int x, int y, int w, int h);
static void evas_engine_xrender_x11_context_clip_unset(void *data, void *context);
static int evas_engine_xrender_x11_context_clip_get(void *data, void *context, int *x, int *y, int *w, int *h);
static void evas_engine_xrender_x11_context_color_set(void *data, void *context, int r, int g, int b, int a);
static int evas_engine_xrender_x11_context_color_get(void *data, void *context, int *r, int *g, int *b, int *a);
static void evas_engine_xrender_x11_context_multiplier_set(void *data, void *context, int r, int g, int b, int a);
static void evas_engine_xrender_x11_context_multiplier_unset(void *data, void *context);
static int evas_engine_xrender_x11_context_multiplier_get(void *data, void *context, int *r, int *g, int *b, int *a);
static void evas_engine_xrender_x11_context_cutout_add(void *data, void *context, int x, int y, int w, int h);
static void evas_engine_xrender_x11_context_cutout_clear(void *data, void *context);
static void evas_engine_xrender_x11_context_anti_alias_set(void *data, void *context, unsigned char aa);
static unsigned char evas_engine_xrender_x11_context_anti_alias_get(void *data, void *context);
static void evas_engine_xrender_x11_context_color_interpolation_set(void *data, void *context, int color_space);
static int evas_engine_xrender_x11_context_color_interpolation_get(void *data, void *context);
static void evas_engine_xrender_x11_rectangle_draw(void *data, void *context, void *surface, int x, int y, int w, int h);
static void evas_engine_xrender_x11_line_draw(void *data, void *context, void *surface, int x1, int y1, int x2, int y2);
static void *evas_engine_xrender_x11_polygon_point_add(void *data, void *context, void *polygon, int x, int y);
static void *evas_engine_xrender_x11_polygon_points_clear(void *data, void *context, void *polygon);
static void evas_engine_xrender_x11_polygon_draw(void *data, void *context, void *surface, void *polygon);
static void *evas_engine_xrender_x11_gradient_color_add(void *data, void *context, void *gradient, int r, int g, int b, int a, int distance);
static void *evas_engine_xrender_x11_gradient_colors_clear(void *data, void *context, void *gradient);
static void evas_engine_xrender_x11_gradient_free(void *data, void *gradient);
static void evas_engine_xrender_x11_gradient_fill_set(void *data, void *gradient, int x, int y, int w, int h);
static void evas_engine_xrender_x11_gradient_type_set(void *data, void *gradient, char *name);
static void evas_engine_xrender_x11_gradient_type_params_set(void *data, void *gradient, char *params);
static void *evas_engine_xrender_x11_gradient_geometry_init(void *data, void *gradient, int spread);
static int  evas_engine_xrender_x11_gradient_alpha_get(void *data, void *gradient, int spread);
static void evas_engine_xrender_x11_gradient_map(void *data, void *context, void *gradient, int spread);
static void evas_engine_xrender_x11_gradient_draw(void *data, void *context, void *surface, void *gradient, int x, int y, int w, int h, double angle, int spread);
static void *evas_engine_xrender_x11_image_load(void *data, char *file, char *key, int *error);
static void *evas_engine_xrender_x11_image_new_from_data(void *data, int w, int h, DATA32 *image_data);
static void *evas_engine_xrender_x11_image_new_from_copied_data(void *data, int w, int h, DATA32 *image_data);
static void evas_engine_xrender_x11_image_free(void *data, void *image);
static void evas_engine_xrender_x11_image_size_get(void *data, void *image, int *w, int *h);
static void *evas_engine_xrender_x11_image_size_set(void *data, void *image, int w, int h);
static void *evas_engine_xrender_x11_image_dirty_region(void *data, void *image, int x, int y, int w, int h);
static void *evas_engine_xrender_x11_image_data_get(void *data, void *image, int to_write, DATA32 **image_data);
static void *evas_engine_xrender_x11_image_data_put(void *data, void *image, DATA32 *image_data);
static void *evas_engine_xrender_x11_image_alpha_set(void *data, void *image, int has_alpha);
static int evas_engine_xrender_x11_image_alpha_get(void *data, void *image);
static void evas_engine_xrender_x11_image_draw(void *data, void *context, void *surface, void *image, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h, int smooth);
static char *evas_engine_xrender_x11_image_comment_get(void *data, void *image, char *key);
static char *evas_engine_xrender_x11_image_format_get(void *data, void *image);
static void evas_engine_xrender_x11_image_cache_flush(void *data);
static void evas_engine_xrender_x11_image_cache_set(void *data, int bytes);
static int evas_engine_xrender_x11_image_cache_get(void *data);
static void *evas_engine_xrender_x11_font_load(void *data, char *name, int size);
static void *evas_engine_xrender_x11_font_memory_load(void *data, char *name, int size, const void *fdata, int fdata_size);
static void *evas_engine_xrender_x11_font_add(void *data, void *font, char *name, int size);
static void *evas_engine_xrender_x11_font_memory_add(void *data, void *font, char *name, int size, const void *fdata, int fdata_size);
static void evas_engine_xrender_x11_font_free(void *data, void *font);
static int evas_engine_xrender_x11_font_ascent_get(void *data, void *font);
static int evas_engine_xrender_x11_font_descent_get(void *data, void *font);
static int evas_engine_xrender_x11_font_max_ascent_get(void *data, void *font);
static int evas_engine_xrender_x11_font_max_descent_get(void *data, void *font);
static void evas_engine_xrender_x11_font_string_size_get(void *data, void *font, char *text, int *w, int *h);
static int evas_engine_xrender_x11_font_inset_get(void *data, void *font, char *text);
static int evas_engine_xrender_x11_font_h_advance_get(void *data, void *font, char *text);
static int evas_engine_xrender_x11_font_v_advance_get(void *data, void *font, char *text);
static int evas_engine_xrender_x11_font_char_coords_get(void *data, void *font, char *text, int pos, int *cx, int *cy, int *cw, int *ch);
static int evas_engine_xrender_x11_font_char_at_coords_get(void *data, void *font, char *text, int x, int y, int *cx, int *cy, int *cw, int *ch);
static void evas_engine_xrender_x11_font_draw(void *data, void *context, void *surface, void *font, int x, int y, int w, int h, int ow, int oh, char *text);
static void evas_engine_xrender_x11_font_cache_flush(void *data);
static void evas_engine_xrender_x11_font_cache_set(void *data, int bytes);
static int evas_engine_xrender_x11_font_cache_get(void *data);

typedef struct _Render_Engine        Render_Engine;
typedef struct _Render_Engine_Update Render_Engine_Update;

struct _Render_Engine_Update
{
   int x, y, w, h;
   Xrender_Surface *surface;
};

struct _Render_Engine
{
   Display              *disp;
   Visual               *vis;
   Drawable              win;
   Pixmap                mask;
   unsigned char         destination_alpha : 1;
   
   Ximage_Info          *xinf;
   Xrender_Surface      *output;
   Xrender_Surface      *mask_output;
   
   Tilebuf              *tb;
   Tilebuf_Rect         *rects;
   Evas_Object_List     *cur_rect;
   int                   end : 1;
   
   Evas_List            *updates;
};

Evas_Func evas_engine_xrender_x11_func =
{
   evas_engine_xrender_x11_info,
     evas_engine_xrender_x11_info_free,
     evas_engine_xrender_x11_setup,
     evas_engine_xrender_x11_output_free,
     evas_engine_xrender_x11_output_resize,
     evas_engine_xrender_x11_output_tile_size_set,
     evas_engine_xrender_x11_output_redraws_rect_add,
     evas_engine_xrender_x11_output_redraws_rect_del,
     evas_engine_xrender_x11_output_redraws_clear,
     evas_engine_xrender_x11_output_redraws_next_update_get,
     evas_engine_xrender_x11_output_redraws_next_update_push,
     evas_engine_xrender_x11_output_flush,
     /* draw context virtual methods */
     evas_engine_xrender_x11_context_new,
     evas_engine_xrender_x11_context_free,
     evas_engine_xrender_x11_context_clip_set,
     evas_engine_xrender_x11_context_clip_clip,
     evas_engine_xrender_x11_context_clip_unset,
     evas_engine_xrender_x11_context_clip_get,
     evas_engine_xrender_x11_context_color_set,
     evas_engine_xrender_x11_context_color_get,
     evas_engine_xrender_x11_context_multiplier_set,
     evas_engine_xrender_x11_context_multiplier_unset,
     evas_engine_xrender_x11_context_multiplier_get,
     evas_engine_xrender_x11_context_cutout_add,
     evas_engine_xrender_x11_context_cutout_clear,
     evas_engine_xrender_x11_context_anti_alias_set,
     evas_engine_xrender_x11_context_anti_alias_get,
     evas_engine_xrender_x11_context_color_interpolation_set,
     evas_engine_xrender_x11_context_color_interpolation_get,
     /* rectangle draw funcs */
     evas_engine_xrender_x11_rectangle_draw,
     /* line draw funcs */
     evas_engine_xrender_x11_line_draw,
     /* polygon draw funcs */
     evas_engine_xrender_x11_polygon_point_add,
     evas_engine_xrender_x11_polygon_points_clear,
     evas_engine_xrender_x11_polygon_draw,
     /* gradient draw funcs */
     evas_engine_xrender_x11_gradient_color_add,
     evas_engine_xrender_x11_gradient_colors_clear,
     evas_engine_xrender_x11_gradient_free,
     evas_engine_xrender_x11_gradient_fill_set,
     evas_engine_xrender_x11_gradient_type_set,
     evas_engine_xrender_x11_gradient_type_params_set,
     evas_engine_xrender_x11_gradient_geometry_init,
     evas_engine_xrender_x11_gradient_alpha_get,
     evas_engine_xrender_x11_gradient_map,
     evas_engine_xrender_x11_gradient_draw,
     /* image draw funcs */
     evas_engine_xrender_x11_image_load,
     evas_engine_xrender_x11_image_new_from_data,
     evas_engine_xrender_x11_image_new_from_copied_data,
     evas_engine_xrender_x11_image_free,
     evas_engine_xrender_x11_image_size_get,
     evas_engine_xrender_x11_image_size_set,
     evas_engine_xrender_x11_image_dirty_region,
     evas_engine_xrender_x11_image_data_get,
     evas_engine_xrender_x11_image_data_put,
     evas_engine_xrender_x11_image_alpha_set,
     evas_engine_xrender_x11_image_alpha_get,
     evas_engine_xrender_x11_image_draw,
     evas_engine_xrender_x11_image_comment_get,
     evas_engine_xrender_x11_image_format_get,
     /* image cache funcs */
     evas_engine_xrender_x11_image_cache_flush,
     evas_engine_xrender_x11_image_cache_set,
     evas_engine_xrender_x11_image_cache_get,
     /* font draw functions */
     evas_engine_xrender_x11_font_load,
     evas_engine_xrender_x11_font_memory_load,
     evas_engine_xrender_x11_font_add,
     evas_engine_xrender_x11_font_memory_add,
     evas_engine_xrender_x11_font_free,
     evas_engine_xrender_x11_font_ascent_get,
     evas_engine_xrender_x11_font_descent_get,
     evas_engine_xrender_x11_font_max_ascent_get,
     evas_engine_xrender_x11_font_max_descent_get,
     evas_engine_xrender_x11_font_string_size_get,
     evas_engine_xrender_x11_font_inset_get,
     evas_engine_xrender_x11_font_h_advance_get,
     evas_engine_xrender_x11_font_v_advance_get,
     evas_engine_xrender_x11_font_char_coords_get,
     evas_engine_xrender_x11_font_char_at_coords_get,
     evas_engine_xrender_x11_font_draw,
     /* font cache functions */
     evas_engine_xrender_x11_font_cache_flush,
     evas_engine_xrender_x11_font_cache_set,
     evas_engine_xrender_x11_font_cache_get
};

static void *
evas_engine_xrender_x11_info(Evas *e)
{
   Evas_Engine_Info_XRender_X11 *info;

   info = calloc(1, sizeof(Evas_Engine_Info_XRender_X11));
   if (!info) return NULL;
   info->magic.magic = rand();
   return info;
   e = NULL;
}

static void
evas_engine_xrender_x11_info_free(Evas *e, void *info)
{
   Evas_Engine_Info_XRender_X11 *in;

   in = (Evas_Engine_Info_XRender_X11 *)info;
   free(in);
}

static void
evas_engine_xrender_x11_setup(Evas *e, void *in)
{
   Render_Engine *re;
   Evas_Engine_Info_XRender_X11 *info;

   info = (Evas_Engine_Info_XRender_X11 *)in;
   if (!e->engine.data.output)
     {
	re = calloc(1, sizeof(Render_Engine));
	evas_common_cpu_init();
	evas_common_blend_init();
	evas_common_image_init();
	evas_common_convert_init();
	evas_common_scale_init();
	evas_common_rectangle_init();
	evas_common_gradient_init();
	evas_common_polygon_init();
	evas_common_line_init();
	evas_common_font_init();
	evas_common_draw_init();
	evas_common_tilebuf_init();
	re->tb = evas_common_tilebuf_new(e->output.w, e->output.h);
	if (re->tb)
	  evas_common_tilebuf_set_tile_size(re->tb, TILESIZE, TILESIZE);
	e->engine.data.output = re;
     }
   re = e->engine.data.output;
   if (!re) return;
   
   if (!e->engine.data.context) e->engine.data.context = e->engine.func->context_new(e->engine.data.output);
   
   re->disp = info->info.display;
   re->vis = info->info.visual;
   re->win = info->info.drawable;
   re->mask = info->info.mask;
   re->destination_alpha = info->info.destination_alpha;
   
   if (re->xinf) _xr_image_info_free(re->xinf);
   re->xinf = _xr_image_info_get(re->disp, re->win, re->vis);

   if (re->output) _xr_render_surface_free(re->output);
   re->output = _xr_render_surface_adopt(re->xinf, re->win, e->output.w, e->output.h, 0);
   if (re->mask)
     {
	if (re->mask_output) _xr_render_surface_free(re->mask_output);
	re->mask_output = _xr_render_surface_format_adopt(re->xinf, re->mask, 
							  e->output.w, e->output.h,
							  re->xinf->fmt1, 1);
     }
}

static void
evas_engine_xrender_x11_output_free(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_shutdown();
   evas_common_image_shutdown();
   while (re->updates)
     {
	Render_Engine_Update *reu;
	
	reu = re->updates->data;
	re->updates = evas_list_remove_list(re->updates, re->updates);
	_xr_render_surface_free(reu->surface);
	free(reu);
     }
   if (re->tb) evas_common_tilebuf_free(re->tb);
   if (re->output) _xr_render_surface_free(re->output);
   if (re->mask_output) _xr_render_surface_free(re->mask_output);
   if (re->rects) evas_common_tilebuf_free_render_rects(re->rects);
   if (re->xinf) _xr_image_info_free(re->xinf);
   free(re);
}

static void
evas_engine_xrender_x11_output_resize(void *data, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   if (re->output)
     {
	if ((re->output->w == w) && (re->output->h ==h)) return;
	if (re->output) _xr_render_surface_free(re->output);
     }
   re->output = _xr_render_surface_adopt(re->xinf, re->win, w, h, 0);
   if (re->mask_output)
     {
	if (re->mask_output) _xr_render_surface_free(re->mask_output);
	re->mask_output = _xr_render_surface_format_adopt(re->xinf, re->mask, 
							  w, h, 
							  re->xinf->fmt1, 1);
     }
   evas_common_tilebuf_free(re->tb);
   re->tb = evas_common_tilebuf_new(w, h);
   if (re->tb) evas_common_tilebuf_set_tile_size(re->tb, TILESIZE, TILESIZE);
}

static void
evas_engine_xrender_x11_output_tile_size_set(void *data, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_tilebuf_set_tile_size(re->tb, w, h);
}

static void
evas_engine_xrender_x11_output_redraws_rect_add(void *data, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_tilebuf_add_redraw(re->tb, x, y, w, h);
}

static void
evas_engine_xrender_x11_output_redraws_rect_del(void *data, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_tilebuf_del_redraw(re->tb, x, y, w, h);
}

static void
evas_engine_xrender_x11_output_redraws_clear(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_tilebuf_clear(re->tb);
}

static void *
evas_engine_xrender_x11_output_redraws_next_update_get(void *data, int *x, int *y, int *w, int *h, int *cx, int *cy, int *cw, int *ch)
{
   Render_Engine *re;
   Tilebuf_Rect *rect;
   int ux, uy, uw, uh;

   re = (Render_Engine *)data;
   if (re->end)
     {
	re->end = 0;
	return NULL;
     }
   if (!re->rects)
     {
	re->rects = evas_common_tilebuf_get_render_rects(re->tb);
	re->cur_rect = (Evas_Object_List *)re->rects;
     }
   if (!re->cur_rect) return NULL;
   rect = (Tilebuf_Rect *)re->cur_rect;
   ux = rect->x; uy = rect->y; uw = rect->w; uh = rect->h;
   re->cur_rect = re->cur_rect->next;
   if (!re->cur_rect)
     {
	evas_common_tilebuf_free_render_rects(re->rects);
	re->rects = NULL;
	re->end = 1;
     }

   *x = ux; *y = uy; *w = uw; *h = uh;
   *cx = 0; *cy = 0; *cw = uw; *ch = uh;
   if ((re->destination_alpha) || (re->mask))
     {
	Xrender_Surface *surface;
	
	surface = _xr_render_surface_new(re->xinf, uw, uh, re->xinf->fmt32, 1);
	_xr_render_surface_solid_rectangle_set(surface, 0, 0, 0, 0, 0, 0, uw, uh);
	return surface;
     }
   return _xr_render_surface_new(re->xinf, uw, uh, re->xinf->fmt24, 0);
}

static void
evas_engine_xrender_x11_output_redraws_next_update_push(void *data, void *surface, int x, int y, int w, int h)
{
   Render_Engine *re;
   Render_Engine_Update *reu;
   
   re = (Render_Engine *)data;
   reu = malloc(sizeof(Render_Engine_Update));
   if (!reu) return;
   reu->x = x;
   reu->y = y;
   reu->w = w;
   reu->h = h;
   reu->surface = (Xrender_Surface *)surface;
   re->updates = evas_list_append(re->updates, reu);
}

static void
evas_engine_xrender_x11_output_flush(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   while (re->updates)
     {
	Render_Engine_Update *reu;
	
	reu = re->updates->data;
	re->updates = evas_list_remove_list(re->updates, re->updates);
	if (re->mask_output)
	  {
	     Xrender_Surface *tsurf;
	     
	     _xr_render_surface_copy(reu->surface, re->output, 0, 0,
				     reu->x, reu->y, reu->w, reu->h);
	     tsurf = _xr_render_surface_new(re->xinf, reu->w, reu->h, re->xinf->fmt1, 1);
	     if (tsurf)
	       {
		  _xr_render_surface_copy(reu->surface, tsurf, 0, 0,
					  0, 0, reu->w, reu->h);
		  _xr_render_surface_copy(tsurf, re->mask_output, 0, 0,
					  reu->x, reu->y, reu->w, reu->h);
		  _xr_render_surface_free(tsurf);
	       }
	  }
	else
	  {
	     _xr_render_surface_copy(reu->surface, re->output, 0, 0,
				     reu->x, reu->y, reu->w, reu->h);
	  }
	_xr_render_surface_free(reu->surface);
	free(reu);
     }
   XSync(re->disp, False);
   _xr_image_info_pool_flush(re->xinf, 0, 0);
}

static void *
evas_engine_xrender_x11_context_new(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_draw_context_new();
}

static void
evas_engine_xrender_x11_context_free(void *data, void *context)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_free(context);
}

static void
evas_engine_xrender_x11_context_clip_set(void *data, void *context, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_set_clip(context, x, y, w, h);
}

static void
evas_engine_xrender_x11_context_clip_clip(void *data, void *context, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_clip_clip(context, x, y, w, h);
}

static void
evas_engine_xrender_x11_context_clip_unset(void *data, void *context)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_unset_clip(context);
}

static int
evas_engine_xrender_x11_context_clip_get(void *data, void *context, int *x, int *y, int *w, int *h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   *x = ((RGBA_Draw_Context *)context)->clip.x;
   *y = ((RGBA_Draw_Context *)context)->clip.y;
   *w = ((RGBA_Draw_Context *)context)->clip.w;
   *h = ((RGBA_Draw_Context *)context)->clip.h;
   return ((RGBA_Draw_Context *)context)->clip.use;
}

static void
evas_engine_xrender_x11_context_color_set(void *data, void *context, int r, int g, int b, int a)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_set_color(context, r, g, b, a);
}

static int
evas_engine_xrender_x11_context_color_get(void *data, void *context, int *r, int *g, int *b, int *a)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   *r = (int)(R_VAL(&((RGBA_Draw_Context *)context)->col.col));
   *g = (int)(G_VAL(&((RGBA_Draw_Context *)context)->col.col));
   *b = (int)(B_VAL(&((RGBA_Draw_Context *)context)->col.col));
   *a = (int)(A_VAL(&((RGBA_Draw_Context *)context)->col.col));
   return 1;
}

static void
evas_engine_xrender_x11_context_multiplier_set(void *data, void *context, int r, int g, int b, int a)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_set_multiplier(context, r, g, b, a);
}

static void
evas_engine_xrender_x11_context_multiplier_unset(void *data, void *context)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_unset_multiplier(context);
}

static int
evas_engine_xrender_x11_context_multiplier_get(void *data, void *context, int *r, int *g, int *b, int *a)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   *r = (int)(R_VAL(&((RGBA_Draw_Context *)context)->mul.col));
   *g = (int)(G_VAL(&((RGBA_Draw_Context *)context)->mul.col));
   *b = (int)(B_VAL(&((RGBA_Draw_Context *)context)->mul.col));
   *a = (int)(A_VAL(&((RGBA_Draw_Context *)context)->mul.col));
   return ((RGBA_Draw_Context *)context)->mul.use;
}

static void
evas_engine_xrender_x11_context_cutout_add(void *data, void *context, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_add_cutout(context, x, y, w, h);
}

static void
evas_engine_xrender_x11_context_cutout_clear(void *data, void *context)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_clear_cutouts(context);
}

static void
evas_engine_xrender_x11_context_anti_alias_set(void *data, void *context, unsigned char aa)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_set_anti_alias(context, aa);
}

static unsigned char
evas_engine_xrender_x11_context_anti_alias_get(void *data, void *context)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return ((RGBA_Draw_Context *)context)->anti_alias;
}

static void
evas_engine_xrender_x11_context_color_interpolation_set(void *data, void *context, int color_space)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_draw_context_set_color_interpolation(context, color_space);
}

static int
evas_engine_xrender_x11_context_color_interpolation_get(void *data, void *context)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return ((RGBA_Draw_Context *)context)->interpolation.color_space;
}





static void
evas_engine_xrender_x11_rectangle_draw(void *data, void *context, void *surface, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xr_render_surface_rectangle_draw((Xrender_Surface *)surface,
				     (RGBA_Draw_Context *)context,
				     x, y, w, h);
}

static void
evas_engine_xrender_x11_line_draw(void *data, void *context, void *surface, int x1, int y1, int x2, int y2)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xr_render_surface_line_draw((Xrender_Surface *)surface, (RGBA_Draw_Context *)context, x1, y1, x2, y2);
}

static void *
evas_engine_xrender_x11_polygon_point_add(void *data, void *context, void *polygon, int x, int y)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_polygon_point_add(polygon, x, y);
}

static void *
evas_engine_xrender_x11_polygon_points_clear(void *data, void *context, void *polygon)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_polygon_points_clear(polygon);
}

static void
evas_engine_xrender_x11_polygon_draw(void *data, void *context, void *surface, void *polygon)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xre_poly_draw((Xrender_Surface *)surface, (RGBA_Draw_Context *)context, (RGBA_Polygon_Point *)polygon);
}

static void *
evas_engine_xrender_x11_gradient_color_add(void *data, void *context, void *gradient, int r, int g, int b, int a, int distance)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return _xre_gradient_color_add(re->xinf, (XR_Gradient *)gradient, r, g, b, a, distance);
}

static void *
evas_engine_xrender_x11_gradient_colors_clear(void *data, void *context, void *gradient)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return _xre_gradient_colors_clear((XR_Gradient *)gradient);
}

static void
evas_engine_xrender_x11_gradient_free(void *data, void *gradient)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xre_gradient_free((XR_Gradient *)gradient);
}

static void
evas_engine_xrender_x11_gradient_fill_set(void *data, void *gradient, int x, int y, int w, int h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xre_gradient_fill_set((XR_Gradient *)gradient, x, y, w, h);
}

static void
evas_engine_xrender_x11_gradient_type_set(void *data, void *gradient, char *name)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xre_gradient_type_set((XR_Gradient *)gradient, name);
}

static void
evas_engine_xrender_x11_gradient_type_params_set(void *data, void *gradient, char *params)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xre_gradient_type_params_set((XR_Gradient *)gradient, params);
}

static void *
evas_engine_xrender_x11_gradient_geometry_init(void *data, void *gradient, int spread)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return _xre_gradient_geometry_init((XR_Gradient *)gradient, spread);
}

static int
evas_engine_xrender_x11_gradient_alpha_get(void *data, void *gradient, int spread)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return _xre_gradient_alpha_get((XR_Gradient *)gradient, spread);
}

static void
evas_engine_xrender_x11_gradient_map(void *data, void *context, void *gradient, int spread)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xre_gradient_map((RGBA_Draw_Context *)context, (XR_Gradient *)gradient, spread);
}

static void
evas_engine_xrender_x11_gradient_draw(void *data, void *context, void *surface, void *gradient, int x, int y, int w, int h, double angle, int spread)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   _xre_gradient_draw((Xrender_Surface *)surface,
		      (RGBA_Draw_Context *)context,
		      (XR_Gradient *)gradient, x, y, w, h, angle, spread);
}

static void *
evas_engine_xrender_x11_image_load(void *data, char *file, char *key, int *error)
{
   Render_Engine *re;
   XR_Image *im;
   
   re = (Render_Engine *)data;
   *error = 0;
   im = _xre_image_load(re->xinf, file, key);
   return im;
}

static void *
evas_engine_xrender_x11_image_new_from_data(void *data, int w, int h, DATA32 *image_data)
{
   Render_Engine *re;
   XR_Image *im;
   
   re = (Render_Engine *)data;
   im = _xre_image_new_from_data(re->xinf, w, h, image_data);
   return im;
}

static void *
evas_engine_xrender_x11_image_new_from_copied_data(void *data, int w, int h, DATA32 *image_data)
{
   Render_Engine *re;
   XR_Image *im;
   
   re = (Render_Engine *)data;
   im = _xre_image_new_from_copied_data(re->xinf, w, h, image_data);
   return im;
}

static void
evas_engine_xrender_x11_image_free(void *data, void *image)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   if (!image) return;
   _xre_image_free((XR_Image *)image);
}

static void
evas_engine_xrender_x11_image_size_get(void *data, void *image, int *w, int *h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return;
   if (w) *w = ((XR_Image *)image)->w;
   if (h) *h = ((XR_Image *)image)->h;
}

static void *
evas_engine_xrender_x11_image_size_set(void *data, void *image, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return image;
   if ((w <= 0) || (h <= 0))
     {
	_xre_image_free((XR_Image *)image);
	return NULL;
     }
   if (((XR_Image *)image)->references > 1)
     {
	XR_Image *old_image;

	old_image = (XR_Image *)image;
	image = _xre_image_copy((XR_Image *)old_image);
	if (image)
	  {
	     ((XR_Image *)image)->alpha = old_image->alpha;
	     _xre_image_free(old_image);
	  }
	else
	  image = old_image;
     }
   else
     _xre_image_dirty((XR_Image *)image);
   _xre_image_resize((XR_Image *)image, w, h);
   return image;
}

static void *
evas_engine_xrender_x11_image_dirty_region(void *data, void *image, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return image;
   _xre_image_dirty((XR_Image *)image);
   _xre_image_region_dirty((XR_Image *)image, x, y, w, h);
   return image;
}

static void *
evas_engine_xrender_x11_image_data_get(void *data, void *image, int to_write, DATA32 **image_data)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return image;
   if (to_write)
     {
	if (((XR_Image *)image)->references > 1)
	  {
	     XR_Image *old_image;
	     
	     old_image = (XR_Image *)image;
	     image = _xre_image_copy((XR_Image *)old_image);
	     if (image)
	       {
		  ((XR_Image *)image)->alpha = old_image->alpha;
		  _xre_image_free(old_image);
	       }
	     else
	       image = old_image;
	  }
	else
	  _xre_image_dirty((XR_Image *)image);
     }
   if (image_data) *image_data = _xre_image_data_get((XR_Image *)image);
   return image;
}

static void *
evas_engine_xrender_x11_image_data_put(void *data, void *image, DATA32 *image_data)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return image;
   if (_xre_image_data_get((XR_Image *)image) != image_data)
     {
	XR_Image *old_image;

	old_image = (XR_Image *)image;
	image = _xre_image_data_find(image_data);
	if (image != old_image)
	  {
	     if (!image)
	       {
		  image = _xre_image_new_from_data(old_image->xinf, old_image->w, old_image->h, image_data);
		  if (image)
		    {
		       ((XR_Image *)image)->alpha = old_image->alpha;
		       _xre_image_free(old_image);
		    }
		  else
		    image = old_image;
	       }
	     else
	       {
		  _xre_image_free(old_image);
	       }
	  }
	else
	  {
	     _xre_image_free(image);
	  }
     }
   return image;
}

static void *
evas_engine_xrender_x11_image_alpha_set(void *data, void *image, int has_alpha)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return image;
   if (((((XR_Image *)image)->alpha) && (has_alpha)) ||
       ((!((XR_Image *)image)->alpha) && (!has_alpha))) 
     return image;
   if (((XR_Image *)image)->references > 1)
     {
	XR_Image *old_image;
	
	old_image = (XR_Image *)image;
	image = _xre_image_copy((XR_Image *)old_image);
	if (image)
	  {
	     ((XR_Image *)image)->alpha = old_image->alpha;
	     _xre_image_free(old_image);
	  }
	else
	  image = old_image;
     }
   else
     _xre_image_dirty((XR_Image *)image);
   _xre_image_alpha_set((XR_Image *)image, has_alpha);
   return image;
}

static int
evas_engine_xrender_x11_image_alpha_get(void *data, void *image)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return 0;
   return _xre_image_alpha_get((XR_Image *)image);
}

static void
evas_engine_xrender_x11_image_draw(void *data, void *context, void *surface, void *image, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h, int smooth)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   if ((!image) || (!surface)) return;
   _xre_image_surface_gen((XR_Image *)image);
   if (((XR_Image *)image)->surface)
     _xr_render_surface_composite(((XR_Image *)image)->surface,
				  (Xrender_Surface *)surface,
				  (RGBA_Draw_Context *)context,
				  src_x, src_y, src_w, src_h,
				  dst_x, dst_y, dst_w, dst_h,
				  smooth);
}

static char *
evas_engine_xrender_x11_image_comment_get(void *data, void *image, char *key)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return NULL;
   return ((XR_Image *)image)->comment;
}

static char *
evas_engine_xrender_x11_image_format_get(void *data, void *image)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!image) return NULL;
   return ((XR_Image *)image)->format;
}

static void
evas_engine_xrender_x11_image_cache_flush(void *data)
{
   Render_Engine *re;
   int tmp_size;

   re = (Render_Engine *)data;
   tmp_size = evas_common_image_get_cache();
   evas_common_image_set_cache(0);
   evas_common_image_set_cache(tmp_size);
   _xre_image_cache_set(0);
   _xre_image_cache_set(tmp_size);
}

static void
evas_engine_xrender_x11_image_cache_set(void *data, int bytes)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_image_set_cache(bytes);
   _xre_image_cache_set(bytes);
}

static int
evas_engine_xrender_x11_image_cache_get(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_image_get_cache();
}

static void *
evas_engine_xrender_x11_font_load(void *data, char *name, int size)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_load(name, size);
}

static void *
evas_engine_xrender_x11_font_memory_load(void *data, char *name, int size, const void *fdata, int fdata_size)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_memory_load(name, size, fdata, fdata_size);
}

static void *
evas_engine_xrender_x11_font_add(void *data, void *font, char *name, int size)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_add(font, name, size);
}

static void *
evas_engine_xrender_x11_font_memory_add(void *data, void *font, char *name, int size, const void *fdata, int fdata_size)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_memory_add(font, name, size, fdata, fdata_size);
}

static void
evas_engine_xrender_x11_font_free(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_free(font);
}

static int
evas_engine_xrender_x11_font_ascent_get(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_ascent_get(font);
}

static int
evas_engine_xrender_x11_font_descent_get(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_descent_get(font);
}

static int
evas_engine_xrender_x11_font_max_ascent_get(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_max_ascent_get(font);
}

static int
evas_engine_xrender_x11_font_max_descent_get(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_max_descent_get(font);
}

static void
evas_engine_xrender_x11_font_string_size_get(void *data, void *font, char *text, int *w, int *h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_query_size(font, text, w, h);
}

static int
evas_engine_xrender_x11_font_inset_get(void *data, void *font, char *text)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_query_inset(font, text);
}

static int
evas_engine_xrender_x11_font_h_advance_get(void *data, void *font, char *text)
{
   Render_Engine *re;
   int h, v;

   re = (Render_Engine *)data;
   evas_common_font_query_advance(font, text, &h, &v);
   return h;
}

static int
evas_engine_xrender_x11_font_v_advance_get(void *data, void *font, char *text)
{
   Render_Engine *re;
   int h, v;

   re = (Render_Engine *)data;
   evas_common_font_query_advance(font, text, &h, &v);
   return v;
}

static int
evas_engine_xrender_x11_font_char_coords_get(void *data, void *font, char *text, int pos, int *cx, int *cy, int *cw, int *ch)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_query_char_coords(font, text, pos, cx, cy, cw, ch);
}

static int
evas_engine_xrender_x11_font_char_at_coords_get(void *data, void *font, char *text, int x, int y, int *cx, int *cy, int *cw, int *ch)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_query_text_at_pos(font, text, x, y, cx, cy, cw, ch);
}

static void
evas_engine_xrender_x11_font_draw(void *data, void *context, void *surface, void *font, int x, int y, int w, int h, int ow, int oh, char *text)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
     {
	static RGBA_Image *im = NULL;
	
	if (!im)
	  {
	     im = evas_common_image_new();
	     im->image = evas_common_image_surface_new(im);
	     im->image->no_free = 1;
	  }
	im->image->w = ((Xrender_Surface *)surface)->w;
	im->image->h = ((Xrender_Surface *)surface)->h;
	_xr_render_surface_clips_set((Xrender_Surface *)surface, (RGBA_Draw_Context *)context, x, y, w, h);
	im->image->data = surface;
	evas_common_draw_context_font_ext_set(context,
					      re->xinf,
					      _xre_font_surface_new,
					      _xre_font_surface_free,
					      _xre_font_surface_draw);
	evas_common_font_draw(im, context, font, x, y, text);
	evas_common_draw_context_font_ext_set(context,
					      NULL,
					      NULL,
					      NULL,
					      NULL);
	evas_common_cpu_end_opt();
     }
}

static void
evas_engine_xrender_x11_font_cache_flush(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_flush();
}

static void
evas_engine_xrender_x11_font_cache_set(void *data, int bytes)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_cache_set(bytes);
}

static int
evas_engine_xrender_x11_font_cache_get(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_cache_get();
}
