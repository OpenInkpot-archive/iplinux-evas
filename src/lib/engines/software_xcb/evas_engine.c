#include "evas_common.h"
#include "evas_private.h"
#include "evas_engine.h"
#include "evas_engine_api_software_xcb.h"
#include "Evas.h"
#include "Evas_Engine_Software_Xcb.h"

static void *evas_engine_software_xcb_info(Evas *e);
static void evas_engine_software_xcb_info_free(Evas *e, void *info);
static void evas_engine_software_xcb_setup(Evas *e, void *info);
static void *evas_engine_software_xcb_output_setup(int w, int h, int rot, XCBConnection *conn, XCBDRAWABLE draw, XCBVISUALTYPE *vis, XCBCOLORMAP cmap, int depth, int debug, int grayscale, int max_colors, XCBDRAWABLE mask, int shape_dither);
static void evas_engine_software_xcb_output_free(void *data);
static void evas_engine_software_xcb_output_resize(void *data, int w, int h);
static void evas_engine_software_xcb_output_tile_size_set(void *data, int w, int h);
static void evas_engine_software_xcb_output_redraws_rect_add(void *data, int x, int y, int w, int h);
static void evas_engine_software_xcb_output_redraws_rect_del(void *data, int x, int y, int w, int h);
static void evas_engine_software_xcb_output_redraws_clear(void *data);
static void *evas_engine_software_xcb_output_redraws_next_update_get(void *data, int *x, int *y, int *w, int *h, int *cx, int *cy, int *cw, int *ch);
static void evas_engine_software_xcb_output_redraws_next_update_push(void *data, void *surface, int x, int y, int w, int h);
static void evas_engine_software_xcb_output_flush(void *data);
static void *evas_engine_software_xcb_context_new(void *data);
static void evas_engine_software_xcb_context_free(void *data, void *context);
static void evas_engine_software_xcb_context_clip_set(void *data, void *context, int x, int y, int w, int h);
static void evas_engine_software_xcb_context_clip_clip(void *data, void *context, int x, int y, int w, int h);
static void evas_engine_software_xcb_context_clip_unset(void *data, void *context);
static int evas_engine_software_xcb_context_clip_get(void *data, void *context, int *x, int *y, int *w, int *h);
static void evas_engine_software_xcb_context_color_set(void *data, void *context, int r, int g, int b, int a);
static int evas_engine_software_xcb_context_color_get(void *data, void *context, int *r, int *g, int *b, int *a);
static void evas_engine_software_xcb_context_multiplier_set(void *data, void *context, int r, int g, int b, int a);
static void evas_engine_software_xcb_context_multiplier_unset(void *data, void *context);
static int evas_engine_software_xcb_context_multiplier_get(void *data, void *context, int *r, int *g, int *b, int *a);
static void evas_engine_software_xcb_context_cutout_add(void *data, void *context, int x, int y, int w, int h);
static void evas_engine_software_xcb_context_cutout_clear(void *data, void *context);
static void evas_engine_software_xcb_rectangle_draw(void *data, void *context, void *surface, int x, int y, int w, int h);
static void evas_engine_software_xcb_line_draw(void *data, void *context, void *surface, int x1, int y1, int x2, int y2);
static void *evas_engine_software_xcb_polygon_point_add(void *data, void *context, void *polygon, int x, int y);
static void *evas_engine_software_xcb_polygon_points_clear(void *data, void *context, void *polygon);
static void evas_engine_software_xcb_polygon_draw(void *data, void *context, void *surface, void *polygon);
static void *evas_engine_software_xcb_gradient_color_add(void *data, void *context, void *gradient, int r, int g, int b, int a, int distance);
static void *evas_engine_software_xcb_gradient_colors_clear(void *data, void *context, void *gradient);
static void evas_engine_software_xcb_gradient_draw(void *data, void *context, void *surface, void *gradient, int x, int y, int w, int h, double angle);
static void *evas_engine_software_xcb_image_load(void *data, char *file, char *key, int *error);
static void *evas_engine_software_xcb_image_new_from_data(void *data, int w, int h, DATA32 *image_data);
static void *evas_engine_software_xcb_image_new_from_copied_data(void *data, int w, int h, DATA32 *image_data);
static void evas_engine_software_xcb_image_free(void *data, void *image);
static void evas_engine_software_xcb_image_size_get(void *data, void *image, int *w, int *h);
static void *evas_engine_software_xcb_image_size_set(void *data, void *image, int w, int h);
static void *evas_engine_software_xcb_image_dirty_region(void *data, void *image, int x, int y, int w, int h);
static void *evas_engine_software_xcb_image_data_get(void *data, void *image, int to_write, DATA32 **image_data);
static void *evas_engine_software_xcb_image_data_put(void *data, void *image, DATA32 *image_data);
static void *evas_engine_software_xcb_image_alpha_set(void *data, void *image, int has_alpha);
static int evas_engine_software_xcb_image_alpha_get(void *data, void *image);
static void evas_engine_software_xcb_image_draw(void *data, void *context, void *surface, void *image, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h, int smooth);
static char *evas_engine_software_xcb_image_comment_get(void *data, void *image, char *key);
static char *evas_engine_software_xcb_image_format_get(void *data, void *image);
static void evas_engine_software_xcb_image_cache_flush(void *data);
static void evas_engine_software_xcb_image_cache_set(void *data, int bytes);
static int evas_engine_software_xcb_image_cache_get(void *data);
static void *evas_engine_software_xcb_font_load(void *data, char *name, int size);
static void *evas_engine_software_xcb_font_memory_load(void *data, char *name, int size, const void *fdata, int fdata_size);
static void *evas_engine_software_xcb_font_add(void *data, void *font, char *name, int size);
static void *evas_engine_software_xcb_font_memory_add(void *data, void *font, char *name, int size, const void *fdata, int fdata_size);
static void evas_engine_software_xcb_font_free(void *data, void *font);
static int evas_engine_software_xcb_font_ascent_get(void *data, void *font);
static int evas_engine_software_xcb_font_descent_get(void *data, void *font);
static int evas_engine_software_xcb_font_max_ascent_get(void *data, void *font);
static int evas_engine_software_xcb_font_max_descent_get(void *data, void *font);
static void evas_engine_software_xcb_font_string_size_get(void *data, void *font, char *text, int *w, int *h);
static int evas_engine_software_xcb_font_inset_get(void *data, void *font, char *text);
static int evas_engine_software_xcb_font_h_advance_get(void *data, void *font, char *text);
static int evas_engine_software_xcb_font_v_advance_get(void *data, void *font, char *text);
static int evas_engine_software_xcb_font_char_coords_get(void *data, void *font, char *text, int pos, int *cx, int *cy, int *cw, int *ch);
static int evas_engine_software_xcb_font_char_at_coords_get(void *data, void *font, char *text, int x, int y, int *cx, int *cy, int *cw, int *ch);
static void evas_engine_software_xcb_font_draw(void *data, void *context, void *surface, void *font, int x, int y, int w, int h, int ow, int oh, char *text);
static void evas_engine_software_xcb_font_cache_flush(void *data);
static void evas_engine_software_xcb_font_cache_set(void *data, int bytes);
static int evas_engine_software_xcb_font_cache_get(void *data);


static XCBVISUALTYPE *evas_engine_software_xcb_best_visual_get(XCBConnection *conn, int screen);
static XCBCOLORMAP evas_engine_software_xcb_best_colormap_get(XCBConnection *conn, int screen);
static int evas_engine_software_xcb_best_depth_get(XCBConnection *conn, int screen);
static Evas_Performance *evas_engine_software_xcb_output_perf_new(Evas *e, XCBConnection *conn, XCBVISUALTYPE *vis, XCBCOLORMAP cmap, XCBDRAWABLE draw, int depth);
static Evas_Performance *evas_engine_software_xcb_output_perf_test(Evas *e, XCBConnection *conn, XCBVISUALTYPE *vis, XCBCOLORMAP cmap, XCBDRAWABLE draw, int depth);
static char *evas_engine_software_xcb_output_perf_data(Evas_Performance *perf);
static char *evas_engine_software_xcb_output_perf_key(Evas_Performance *perf);
static void evas_engine_software_xcb_output_perf_free(Evas_Performance *perf);
static void evas_engine_software_xcb_output_perf_build(Evas_Performance *perf, const char *data);
static void evas_engine_software_xcb_output_perf_device_store(Evas_Performance *perf);

typedef struct _Render_Engine Render_Engine;

struct _Render_Engine
{
   Tilebuf          *tb;
   Outbuf           *ob;
   Tilebuf_Rect     *rects;
   Evas_Object_List *cur_rect;
   int               end : 1;
};

Evas_Func evas_engine_software_xcb_func =
{
   evas_engine_software_xcb_info,
   evas_engine_software_xcb_info_free,
   evas_engine_software_xcb_setup,
   evas_engine_software_xcb_output_free,
   evas_engine_software_xcb_output_resize,
   evas_engine_software_xcb_output_tile_size_set,
   evas_engine_software_xcb_output_redraws_rect_add,
   evas_engine_software_xcb_output_redraws_rect_del,
   evas_engine_software_xcb_output_redraws_clear,
   evas_engine_software_xcb_output_redraws_next_update_get,
   evas_engine_software_xcb_output_redraws_next_update_push,
   evas_engine_software_xcb_output_flush,
   /* draw context virtual methods */
   evas_engine_software_xcb_context_new,
   evas_engine_software_xcb_context_free,
   evas_engine_software_xcb_context_clip_set,
   evas_engine_software_xcb_context_clip_clip,
   evas_engine_software_xcb_context_clip_unset,
   evas_engine_software_xcb_context_clip_get,
   evas_engine_software_xcb_context_color_set,
   evas_engine_software_xcb_context_color_get,
   evas_engine_software_xcb_context_multiplier_set,
   evas_engine_software_xcb_context_multiplier_unset,
   evas_engine_software_xcb_context_multiplier_get,
   evas_engine_software_xcb_context_cutout_add,
   evas_engine_software_xcb_context_cutout_clear,
   /* rectangle draw funcs */
   evas_engine_software_xcb_rectangle_draw,
   /* line draw funcs */
   evas_engine_software_xcb_line_draw,
   /* polygon draw funcs */
   evas_engine_software_xcb_polygon_point_add,
   evas_engine_software_xcb_polygon_points_clear,
   evas_engine_software_xcb_polygon_draw,
   /* gradient draw funcs */
   evas_engine_software_xcb_gradient_color_add,
   evas_engine_software_xcb_gradient_colors_clear,
   evas_engine_software_xcb_gradient_draw,
   /* image draw funcs */
   evas_engine_software_xcb_image_load,
   evas_engine_software_xcb_image_new_from_data,
   evas_engine_software_xcb_image_new_from_copied_data,
   evas_engine_software_xcb_image_free,
   evas_engine_software_xcb_image_size_get,
   evas_engine_software_xcb_image_size_set,
   evas_engine_software_xcb_image_dirty_region,
   evas_engine_software_xcb_image_data_get,
   evas_engine_software_xcb_image_data_put,
   evas_engine_software_xcb_image_alpha_set,
   evas_engine_software_xcb_image_alpha_get,
   evas_engine_software_xcb_image_draw,
   evas_engine_software_xcb_image_comment_get,
   evas_engine_software_xcb_image_format_get,
   /* image cache funcs */
   evas_engine_software_xcb_image_cache_flush,
   evas_engine_software_xcb_image_cache_set,
   evas_engine_software_xcb_image_cache_get,
   /* font draw functions */
   evas_engine_software_xcb_font_load,
   evas_engine_software_xcb_font_memory_load,
   evas_engine_software_xcb_font_add,
   evas_engine_software_xcb_font_memory_add,
   evas_engine_software_xcb_font_free,
   evas_engine_software_xcb_font_ascent_get,
   evas_engine_software_xcb_font_descent_get,
   evas_engine_software_xcb_font_max_ascent_get,
   evas_engine_software_xcb_font_max_descent_get,
   evas_engine_software_xcb_font_string_size_get,
   evas_engine_software_xcb_font_inset_get,
   evas_engine_software_xcb_font_h_advance_get,
   evas_engine_software_xcb_font_v_advance_get,
   evas_engine_software_xcb_font_char_coords_get,
   evas_engine_software_xcb_font_char_at_coords_get,     
   evas_engine_software_xcb_font_draw,
   /* font cache functions */
   evas_engine_software_xcb_font_cache_flush,
   evas_engine_software_xcb_font_cache_set,
   evas_engine_software_xcb_font_cache_get
};

static void *
evas_engine_software_xcb_info(Evas *e)
{
   Evas_Engine_Info_Software_Xcb *info;
   
   info = calloc(1, sizeof(Evas_Engine_Info_Software_Xcb));
   if (!info) return NULL;
   info->magic.magic = rand();
   info->info.debug = 0;
   info->info.alloc_grayscale = 0;
   info->info.alloc_colors_max = 216;
   info->func.best_visual_get = evas_engine_software_xcb_best_visual_get;
   info->func.best_colormap_get = evas_engine_software_xcb_best_colormap_get;
   info->func.best_depth_get = evas_engine_software_xcb_best_depth_get;
   info->func.performance_test = evas_engine_software_xcb_output_perf_test;
   info->func.performance_free = evas_engine_software_xcb_output_perf_free;
   info->func.performance_data_get = evas_engine_software_xcb_output_perf_data;
   info->func.performance_key_get = evas_engine_software_xcb_output_perf_key;
   info->func.performance_new = evas_engine_software_xcb_output_perf_new;
   info->func.performance_build = evas_engine_software_xcb_output_perf_build;
   info->func.performance_device_store = evas_engine_software_xcb_output_perf_device_store;
   return info;
   e = NULL;
}

static void
evas_engine_software_xcb_info_free(Evas *e, void *info)
{
   Evas_Engine_Info_Software_Xcb *in;
   
   in = (Evas_Engine_Info_Software_Xcb *)info;
   free(in);
}

static void
evas_engine_software_xcb_setup(Evas *e, void *in)
{
   Render_Engine *re;
   Evas_Engine_Info_Software_Xcb *info;
   
   info = (Evas_Engine_Info_Software_Xcb *)in;
   if (!e->engine.data.output)
     e->engine.data.output = 
     evas_engine_software_xcb_output_setup(e->output.w,
					   e->output.h,
					   info->info.rotation,
					   info->info.conn, 
					   info->info.drawable,
					   info->info.visual, 
					   info->info.colormap,
					   info->info.depth,
					   info->info.debug,
					   info->info.alloc_grayscale,
					   info->info.alloc_colors_max,
					   info->info.mask,
					   info->info.shape_dither);
   if (!e->engine.data.output) return;
   if (!e->engine.data.context)
     e->engine.data.context = 
     e->engine.func->context_new(e->engine.data.output);   
   
   re = e->engine.data.output;
   evas_software_x11_outbuf_software_xcb_drawable_set(re->ob, info->info.drawable);
   evas_software_x11_outbuf_software_xcb_mask_set(re->ob, info->info.mask);
   evas_software_x11_outbuf_software_xcb_rotation_set(re->ob, info->info.rotation);
}

static void *
evas_engine_software_xcb_output_setup(int            w,
				      int            h,
				      int            rot,
				      XCBConnection *conn,
				      XCBDRAWABLE    draw,
				      XCBVISUALTYPE *vis,
				      XCBCOLORMAP    cmap,
				      int            depth,
				      int            debug,
				      int            grayscale,
				      int            max_colors,
				      XCBDRAWABLE    mask,
				      int            shape_dither)
{
   Render_Engine *re;
   Outbuf_Perf   *perf;
   
   re = calloc(1, sizeof(Render_Engine));
   /* if we haven't initialized - init (automatic abort if already done) */
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
   
   evas_software_x11_x_software_xcb_init();
   x_software_xcb_color_init();
   evas_software_x11_outbuf_software_xcb_init();   
   
   /* get any stored performance metrics from device (xserver) */
   perf = evas_software_x11_outbuf_software_xcb_perf_restore_x(conn, draw, vis, cmap, depth);
   re->ob = evas_software_x11_outbuf_software_xcb_setup_x(w, h, rot,
							  OUTBUF_DEPTH_INHERIT,
							  conn,
							  draw,
							  vis,
							  cmap,
							  depth,
							  perf,
							  grayscale,
							  max_colors,
							  mask, shape_dither);
   if (!re->ob)
     {
	evas_software_x11_outbuf_software_xcb_perf_free(perf);
	free(re);
	return NULL;
     }
   evas_software_x11_outbuf_software_xcb_debug_set(re->ob, debug);
   evas_software_x11_outbuf_software_xcb_set_have_backbuf(re->ob, 0);
   re->tb = evas_common_tilebuf_new(w, h);
   if (!re->tb)
     {
	evas_software_x11_outbuf_software_xcb_free(re->ob);
	free(re);
	return NULL;
     }
   /* in preliminary tests 16x16 gave highest framerates */
   evas_common_tilebuf_set_tile_size(re->tb, TILESIZE, TILESIZE);
   return re;
}

static void
evas_engine_software_xcb_output_free(void *data)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_software_x11_outbuf_software_xcb_free(re->ob);
   evas_common_tilebuf_free(re->tb);
   if (re->rects) evas_common_tilebuf_free_render_rects(re->rects);
   free(re);

   evas_common_font_shutdown();
   evas_common_image_shutdown();
}

static void
evas_engine_software_xcb_output_resize(void *data, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_software_x11_outbuf_software_xcb_reconfigure(re->ob, w, h, 
				   evas_software_x11_outbuf_software_xcb_get_rot(re->ob),
				   OUTBUF_DEPTH_INHERIT);
   evas_software_x11_outbuf_software_xcb_set_have_backbuf(re->ob, 0);
   evas_common_tilebuf_free(re->tb);
   re->tb = evas_common_tilebuf_new(w, h);
   if (re->tb)
     evas_common_tilebuf_set_tile_size(re->tb, TILESIZE, TILESIZE);
}

static void
evas_engine_software_xcb_output_tile_size_set(void *data, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_tilebuf_set_tile_size(re->tb, w, h);   
}

static void
evas_engine_software_xcb_output_redraws_rect_add(void *data, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_tilebuf_add_redraw(re->tb, x, y, w, h);
}

static void
evas_engine_software_xcb_output_redraws_rect_del(void *data, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_tilebuf_del_redraw(re->tb, x, y, w, h);
}

static void
evas_engine_software_xcb_output_redraws_clear(void *data)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_tilebuf_clear(re->tb);
}

static void *
evas_engine_software_xcb_output_redraws_next_update_get(void *data, int *x, int *y, int *w, int *h, int *cx, int *cy, int *cw, int *ch)
{
   Render_Engine *re;
   RGBA_Image *surface;
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
   
   surface = evas_software_x11_outbuf_software_xcb_new_region_for_update(re->ob, 
					  ux, uy, uw, uh, 
					  cx, cy, cw, ch);
   *x = ux; *y = uy; *w = uw; *h = uh;
   return surface;
}

static void
evas_engine_software_xcb_output_redraws_next_update_push(void *data, void *surface, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_software_x11_outbuf_software_xcb_push_updated_region(re->ob, surface, x, y, w, h);
   evas_software_x11_outbuf_software_xcb_free_region_for_update(re->ob, surface);
   evas_common_cpu_end_opt();
}

static void
evas_engine_software_xcb_output_flush(void *data)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   XCBSync(re->ob->priv.x.conn, 0);
}

static void *
evas_engine_software_xcb_context_new(void *data)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   return evas_common_draw_context_new();
}

static void
evas_engine_software_xcb_context_free(void *data, void *context)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_free(context);
}

static void
evas_engine_software_xcb_context_clip_set(void *data, void *context, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_set_clip(context, x, y, w, h);
}

static void
evas_engine_software_xcb_context_clip_clip(void *data, void *context, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_clip_clip(context, x, y, w, h);
}

static void
evas_engine_software_xcb_context_clip_unset(void *data, void *context)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_unset_clip(context);
}

static int
evas_engine_software_xcb_context_clip_get(void *data, void *context, int *x, int *y, int *w, int *h)
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
evas_engine_software_xcb_context_color_set(void *data, void *context, int r, int g, int b, int a)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_set_color(context, r, g, b, a);
}

static int
evas_engine_software_xcb_context_color_get(void *data, void *context, int *r, int *g, int *b, int *a)
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
evas_engine_software_xcb_context_multiplier_set(void *data, void *context, int r, int g, int b, int a)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_set_multiplier(context, r, g, b, a);
}

static void
evas_engine_software_xcb_context_multiplier_unset(void *data, void *context)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_unset_multiplier(context);
}

static int
evas_engine_software_xcb_context_multiplier_get(void *data, void *context, int *r, int *g, int *b, int *a)
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
evas_engine_software_xcb_context_cutout_add(void *data, void *context, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_add_cutout(context, x, y, w, h);
}

static void
evas_engine_software_xcb_context_cutout_clear(void *data, void *context)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_draw_context_clear_cutouts(context);
}






static void
evas_engine_software_xcb_rectangle_draw(void *data, void *context, void *surface, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
#ifdef IMGONLY   
   return;
#endif   
   evas_common_rectangle_draw(surface, context, x, y, w, h);
   evas_common_cpu_end_opt();
}

static void
evas_engine_software_xcb_line_draw(void *data, void *context, void *surface, int x1, int y1, int x2, int y2)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
#ifdef IMGONLY   
   return;
#endif   
   evas_common_line_draw(surface, context, x1, y1, x2, y2);
   evas_common_cpu_end_opt();
}

static void *
evas_engine_software_xcb_polygon_point_add(void *data, void *context, void *polygon, int x, int y)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   return evas_common_polygon_point_add(polygon, x, y);
   context = NULL;
}

static void *
evas_engine_software_xcb_polygon_points_clear(void *data, void *context, void *polygon)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   return evas_common_polygon_points_clear(polygon);
   context = NULL;
}

static void
evas_engine_software_xcb_polygon_draw(void *data, void *context, void *surface, void *polygon)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
#ifdef IMGONLY   
   return;
#endif   
   evas_common_polygon_draw(surface, context, polygon);
   evas_common_cpu_end_opt();
}

static void *
evas_engine_software_xcb_gradient_color_add(void *data, void *context, void *gradient, int r, int g, int b, int a, int distance)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (!gradient)
     gradient = evas_common_gradient_new();
   evas_common_gradient_color_add(gradient, r, g, b, a, distance);
   return gradient;
   context = NULL;
}

static void *
evas_engine_software_xcb_gradient_colors_clear(void *data, void *context, void *gradient)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   if (gradient) evas_common_gradient_free(gradient);
   return NULL;
   context = NULL;
}

static void
evas_engine_software_xcb_gradient_draw(void *data, void *context, void *surface, void *gradient, int x, int y, int w, int h, double angle)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
#ifdef IMGONLY   
   return;
#endif   
   evas_common_gradient_draw(surface, context, x, y, w, h, gradient, angle);
   evas_common_cpu_end_opt();
}

static void *
evas_engine_software_xcb_image_load(void *data, char *file, char *key, int *error)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   *error = 0;
   return evas_common_load_image_from_file(file, key);
}

static void *
evas_engine_software_xcb_image_new_from_data(void *data, int w, int h, DATA32 *image_data)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = evas_common_image_new();
   im->image = evas_common_image_surface_new(im);
   if (!im->image)
     {
	evas_common_image_free(im);
	return NULL;
     }
   im->image->w = w;
   im->image->h = h;
   im->image->data = image_data;
   im->image->no_free = 1;
   return im;
}

static void *
evas_engine_software_xcb_image_new_from_copied_data(void *data, int w, int h, DATA32 *image_data)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = evas_common_image_create(w, h);
   if (!im) return NULL;
   if (image_data)
     memcpy(im->image->data, image_data, w * h * sizeof(DATA32));
   return im;
}

static void
evas_engine_software_xcb_image_free(void *data, void *image)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_image_unref(image);
}

static void
evas_engine_software_xcb_image_size_get(void *data, void *image, int *w, int *h)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = image;
   if (w) *w = im->image->w;
   if (h) *h = im->image->h;
}

static void *
evas_engine_software_xcb_image_size_set(void *data, void *image, int w, int h)
{
   Render_Engine *re;
   RGBA_Image *im, *im_old;
   
   re = (Render_Engine *)data;
   im_old = image;
   im = evas_common_image_create(w, h);
   if (!im) return im_old;
   if (im_old)
     {
	evas_common_load_image_data_from_file(im_old);
	if (im_old->image->data)
	  {
	     evas_common_blit_rectangle(im_old, im, 0, 0, w, h, 0, 0);
	     evas_common_cpu_end_opt();
	  }
	evas_common_image_unref(im_old);
     }
   return im;
}

static void *
evas_engine_software_xcb_image_dirty_region(void *data, void *image, int x, int y, int w, int h)
{
   Render_Engine *re;
   
   re = (Render_Engine *)data;
   evas_common_image_dirty(image);
   return image;
   x = 0;
   y = 0;
   w = 0;
   h = 0;
}

static void *
evas_engine_software_xcb_image_data_get(void *data, void *image, int to_write, DATA32 **image_data)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = image;
   evas_common_load_image_data_from_file(im);
   if (to_write)
     {
	if (im->references > 1)
	  {
	     RGBA_Image *im_new;
	     
	     im_new = evas_common_image_create(im->image->w, im->image->h);
	     if (!im_new) return im;
	     evas_common_blit_rectangle(im, im_new, 0, 0, im->image->w, im->image->h, 0, 0);
	     evas_common_cpu_end_opt();
	     evas_common_image_unref(im);
	     im = im_new;
	  }
	else
	  evas_common_image_dirty(im);
     }
   *image_data = im->image->data;
   return im;
}

static void *
evas_engine_software_xcb_image_data_put(void *data, void *image, DATA32 *image_data)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = image;
   if (image_data != im->image->data)
     {
	int w, h;
	
	w = im->image->w;
	h = im->image->h;
	evas_common_image_unref(im);
	return evas_engine_software_xcb_image_new_from_data(data, w, h, image_data);
     }
   /* hmmm - but if we wrote... why bother? */
/*   evas_common_image_dirty(im); */
   return im;
}

static void *
evas_engine_software_xcb_image_alpha_set(void *data, void *image, int has_alpha)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = image;
   if (im->references > 1)
     {
	RGBA_Image *im_new;
	
	im_new = evas_common_image_create(im->image->w, im->image->h);
	if (!im_new) return im;
	evas_common_load_image_data_from_file(im);
	evas_common_blit_rectangle(im, im_new, 0, 0, im->image->w, im->image->h, 0, 0);
	evas_common_cpu_end_opt();
	evas_common_image_unref(im);
	im = im_new;
     }
   else
     evas_common_image_dirty(im);
   if (has_alpha)
     im->flags |= RGBA_IMAGE_HAS_ALPHA;
   else
     im->flags &= ~RGBA_IMAGE_HAS_ALPHA;
   return im;
}


static int
evas_engine_software_xcb_image_alpha_get(void *data, void *image)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = image;
   if (im->flags & RGBA_IMAGE_HAS_ALPHA) return 1;
   return 0;
}

static void
evas_engine_software_xcb_image_draw(void *data, void *context, void *surface, void *image, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h, int smooth)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_load_image_data_from_file(image);
   if (smooth)
     evas_common_scale_rgba_in_to_out_clip_smooth(image, surface, context, 
				      src_x, src_y, src_w, src_h, 
				      dst_x, dst_y, dst_w, dst_h);
   else
     evas_common_scale_rgba_in_to_out_clip_sample(image, surface, context, 
				      src_x, src_y, src_w, src_h, 
				      dst_x, dst_y, dst_w, dst_h);     
   evas_common_cpu_end_opt();
}

static char *
evas_engine_software_xcb_image_comment_get(void *data, void *image, char *key)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = image;
   return im->info.comment;
   key = 0;
}

static char *
evas_engine_software_xcb_image_format_get(void *data, void *image)
{
   Render_Engine *re;
   RGBA_Image *im;
   
   re = (Render_Engine *)data;
   im = image;
   if (im->info.format == 1) return "png";
   return NULL;
}

static void
evas_engine_software_xcb_image_cache_flush(void *data)
{
   Render_Engine *re;
   int tmp_size;
   
   re = (Render_Engine *)data;
   tmp_size = evas_common_image_get_cache();
   evas_common_image_set_cache(0);
   evas_common_image_set_cache(tmp_size);
}

static void
evas_engine_software_xcb_image_cache_set(void *data, int bytes)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_image_set_cache(bytes);
}

static int
evas_engine_software_xcb_image_cache_get(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_image_get_cache();
}

static void *
evas_engine_software_xcb_font_load(void *data, char *name, int size)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_load(name, size);
}

static void *
evas_engine_software_xcb_font_memory_load(void *data, char *name, int size, const void *fdata, int fdata_size)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_memory_load(name, size, fdata, fdata_size);
}

static void *
evas_engine_software_xcb_font_add(void *data, void *font, char *name, int size)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_add(font, name, size);
}

static void *
evas_engine_software_xcb_font_memory_add(void *data, void *font, char *name, int size, const void *fdata, int fdata_size)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_memory_add(font, name, size, fdata, fdata_size);
}

static void
evas_engine_software_xcb_font_free(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_free(font);
}

static int
evas_engine_software_xcb_font_ascent_get(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_ascent_get(font);
}

static int 
evas_engine_software_xcb_font_descent_get(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_descent_get(font);
}

static int
evas_engine_software_xcb_font_max_ascent_get(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_max_ascent_get(font);
}

static int
evas_engine_software_xcb_font_max_descent_get(void *data, void *font)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_max_descent_get(font);
}

static void
evas_engine_software_xcb_font_string_size_get(void *data, void *font, char *text, int *w, int *h)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_query_size(font, text, w, h);
}

static int
evas_engine_software_xcb_font_inset_get(void *data, void *font, char *text)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_query_inset(font, text);
}

static int
evas_engine_software_xcb_font_h_advance_get(void *data, void *font, char *text)
{
   Render_Engine *re;
   int h, v;
   
   re = (Render_Engine *)data;
   evas_common_font_query_advance(font, text, &h, &v);
   return h;
}

static int
evas_engine_software_xcb_font_v_advance_get(void *data, void *font, char *text)
{
   Render_Engine *re;
   int h, v;

   re = (Render_Engine *)data;
   evas_common_font_query_advance(font, text, &h, &v);
   return v;
}

static int
evas_engine_software_xcb_font_char_coords_get(void *data, void *font, char *text, int pos, int *cx, int *cy, int *cw, int *ch)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_query_char_coords(font, text, pos, cx, cy, cw, ch);
}

static int
evas_engine_software_xcb_font_char_at_coords_get(void *data, void *font, char *text, int x, int y, int *cx, int *cy, int *cw, int *ch)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_query_text_at_pos(font, text, x, y, cx, cy, cw, ch);
}

static void
evas_engine_software_xcb_font_draw(void *data, void *context, void *surface, void *font, int x, int y, int w, int h, int ow, int oh, char *text)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
#ifdef IMGONLY   
   return;
#endif   
   if ((w == ow) && (h == oh))
     evas_common_font_draw(surface, context, font, x, y, text);
   else
     {
	/* create output surface size ow x oh and scale to w x h */
	RGBA_Draw_Context *dc, *dc_in;

	dc_in = context;
	dc = evas_common_draw_context_new();
	if (dc)
	  {
	     RGBA_Image *im;
	     int inset;
	     
	     dc->col.col = dc_in->col.col;
	     inset = evas_common_font_query_inset( font, text);
	     im = evas_common_image_create(ow+inset, oh);
	     if (im)
	       {
		  int max_ascent;
		  int j;
		  
		  im->flags |= RGBA_IMAGE_HAS_ALPHA;
		  j = (ow+inset) * oh;
		  memset(im->image->data, 0, j * sizeof(DATA32));
		  
		  max_ascent = evas_common_font_max_ascent_get(font);
		  
		  evas_common_font_draw(im, dc, font, 0, max_ascent, text);
		  evas_common_cpu_end_opt();
		  evas_common_scale_rgba_in_to_out_clip_smooth(im, surface, context, 
						   inset, 0, ow, oh, 
						   x + ((inset * w) / ow), y - ((max_ascent * h) / oh), 
						   w, h);
		  evas_common_image_free(im);
	       }
	     evas_common_draw_context_free(dc);
	  }
     }
   evas_common_cpu_end_opt();
}

static void
evas_engine_software_xcb_font_cache_flush(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_flush();
}

static void
evas_engine_software_xcb_font_cache_set(void *data, int bytes)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   evas_common_font_cache_set(bytes);
}

static int
evas_engine_software_xcb_font_cache_get(void *data)
{
   Render_Engine *re;

   re = (Render_Engine *)data;
   return evas_common_font_cache_get();
}









/* private engine functions the calling prog can use */

/* Convenient function for getting the best visual, colormap and depth */
static XCBSCREEN *
evas_engine_software_xcb_screen_get(XCBConnection *conn, int screen)
{
   XCBSCREENIter i;
   int           cur;

   if (!conn) return NULL;
   
   i = XCBConnSetupSuccessRepRootsIter(XCBGetSetup(conn));
   if (screen > i.rem - 1) return NULL; /* screen must be between 0 and i.rem - 1 */
   for (cur = 0; cur <= screen; XCBSCREENNext(&i), ++cur) {}
     
   return i.data;
}

static XCBVISUALTYPE *
evas_engine_software_xcb_best_visual_get(XCBConnection *conn, int screen)
{
   XCBSCREEN        *scr;
   XCBDEPTH         *d;
   XCBVISUALTYPEIter iter;
   int               cur;

   if (!conn) return NULL;
   scr = evas_engine_software_xcb_screen_get(conn, screen);
   if (!scr) return NULL;
   d = XCBSCREENAllowedDepthsIter(scr).data;
   if (!d) return NULL;
   
   iter = XCBDEPTHVisualsIter(d);
   for (cur = 0 ; cur < iter.rem ; XCBVISUALTYPENext(&iter), ++cur)
      if (scr->root_visual.id == iter.data->visual_id.id)
	 return iter.data;
   
   return NULL;
}

static XCBCOLORMAP
evas_engine_software_xcb_best_colormap_get(XCBConnection *conn, int screen)
{
   XCBSCREEN *scr;
   XCBCOLORMAP c;
   
   c.xid = 0;
   if (!conn) return c;
   scr = evas_engine_software_xcb_screen_get(conn, screen);
   if (!scr) return c;

   return scr->default_colormap;
}

static int
evas_engine_software_xcb_best_depth_get(XCBConnection *conn, int screen)
{
   XCBSCREEN *scr;

   if (!conn) return 0;
   scr = evas_engine_software_xcb_screen_get(conn, screen);
   if (!scr) return 0;

   return scr->root_depth;
}

static Evas_Performance *
evas_engine_software_xcb_output_perf_new(Evas *e, XCBConnection *conn, XCBVISUALTYPE *vis, XCBCOLORMAP cmap, XCBDRAWABLE draw, int depth)
{
   return evas_software_x11_outbuf_software_xcb_perf_new_x(conn, draw, vis, cmap, depth);
   e = NULL;
}

static Evas_Performance *
evas_engine_software_xcb_output_perf_test(Evas *e, XCBConnection *conn, XCBVISUALTYPE *vis, XCBCOLORMAP cmap, XCBDRAWABLE draw, int depth)
{
   return evas_software_x11_outbuf_software_xcb_perf_x(conn, draw, vis, cmap, depth);
   e = NULL;
}

static char *
evas_engine_software_xcb_output_perf_data(Evas_Performance *perf)    
{
   return evas_software_x11_outbuf_software_xcb_perf_serialize_x(perf);
}

static char *
evas_engine_software_xcb_output_perf_key(Evas_Performance *perf)    
{
   return evas_software_x11_outbuf_software_xcb_perf_serialize_info_x(perf);
}

static void
evas_engine_software_xcb_output_perf_free(Evas_Performance *perf)    
{
   evas_software_x11_outbuf_software_xcb_perf_free(perf);
}

static void
evas_engine_software_xcb_output_perf_build(Evas_Performance *perf, const char *data)
{
   evas_software_x11_outbuf_software_xcb_perf_deserialize_x(perf, data);
}

static void
evas_engine_software_xcb_output_perf_device_store(Evas_Performance *perf)    
{
   evas_software_x11_outbuf_software_xcb_perf_store_x(perf);
}