#ifndef EVAS_ENGINE_H
#define EVAS_ENGINE_H

#include <X11/XCB/xcb.h>

typedef struct _Outbuf                Outbuf;
typedef struct _Outbuf_Perf           Outbuf_Perf;
typedef struct _Xcb_Output_Buffer     Xcb_Output_Buffer;

typedef enum   _Outbuf_Depth          Outbuf_Depth;

enum _Outbuf_Depth
{
   OUTBUF_DEPTH_NONE,
   OUTBUF_DEPTH_INHERIT,
   OUTBUF_DEPTH_RGB_16BPP_565_565_DITHERED,
   OUTBUF_DEPTH_RGB_16BPP_555_555_DITHERED,
   OUTBUF_DEPTH_RGB_16BPP_444_444_DITHERED,
   OUTBUF_DEPTH_RGB_16BPP_565_444_DITHERED,
   OUTBUF_DEPTH_RGB_32BPP_888_8888,
   OUTBUF_DEPTH_LAST
};

struct _Outbuf
{
   Outbuf_Depth    depth;
   int             w, h;
   int             rot;
   Outbuf_Perf    *perf;
   
   struct {
      Convert_Pal *pal;
      struct {
         XCBConnection  *conn;
	 XCBDRAWABLE     win;
	 XCBDRAWABLE     mask;
	 XCBVISUALTYPE  *vis;
	 XCBCOLORMAP     cmap;
	 int             depth;
	 int             shm;
	 XCBGCONTEXT     gc;
	 XCBGCONTEXT     gcm;
	 int             swap : 1;
      } x;
      struct {
	 DATA32    r, g, b;
      } mask;
      RGBA_Image  *back_buf;
      
      int          mask_dither : 1;
      
      int          debug : 1;
   } priv;
};

struct _Outbuf_Perf
{
   struct {
      XCBConnection  *conn;
      XCBDRAWABLE     root;
      
      char *display;
      char *vendor;
      int   version;
      int   revision;
      int   release;
      int   w, h;
      int   screen_count;
      int   depth;
      int   screen_num;
   } x;
   struct{
      char *name;
      char *version;
      char *machine;
   } os;
   struct {
      char *info;
   } cpu;
   
   int   min_shm_image_pixel_count;
};

/****/
/* main */
void               evas_software_x11_x_software_xcb_init                    (void);

/* buffer */
void               evas_software_x11_x_software_xcb_write_mask_line         (Xcb_Output_Buffer *xcbob,
									     DATA32            *src,
									     int                w,
									     int                y);
int                evas_software_x11_x_software_xcb_can_do_shm              (XCBConnection *c);
Xcb_Output_Buffer *evas_software_x11_x_software_xcb_output_buffer_new       (XCBConnection *c,
									     int            depth,
									     int            w,
									     int            h,
									     int            try_shm,
									     void          *data);
void               evas_software_x11_x_software_xcb_output_buffer_free      (Xcb_Output_Buffer *xcbob);
void               evas_software_x11_x_software_xcb_output_buffer_paste     (Xcb_Output_Buffer *xcbob,
									     XCBDRAWABLE        d,
									     XCBGCONTEXT        gc,
									     int                x,
									     int                y);
DATA8             *evas_software_x11_x_software_xcb_output_buffer_data      (Xcb_Output_Buffer *xcbob,
									     int               *bytes_per_line_ret);
int                evas_software_x11_x_software_xcb_output_buffer_depth     (Xcb_Output_Buffer *xcbob);
int                evas_software_x11_x_software_xcb_output_buffer_byte_order(Xcb_Output_Buffer *xcbob);

/* color */
void               x_software_xcb_color_init              (void);
Convert_Pal       *evas_software_x11_x_software_xcb_color_allocate          (XCBConnection   *conn,
									     XCBCOLORMAP      cmap,
									     XCBVISUALTYPE   *vis,
									     Convert_Pal_Mode colors);
void               evas_software_x11_x_software_xcb_color_deallocate        (XCBConnection *conn,
									     XCBCOLORMAP    cmap,
									     XCBVISUALTYPE *vis, 
									     Convert_Pal   *pal);

/* outbuf */    
void           evas_software_x11_outbuf_software_xcb_init                   (void);
void           evas_software_x11_outbuf_software_xcb_free                   (Outbuf *buf);
Outbuf        *evas_software_x11_outbuf_software_xcb_setup_x                (int            w,
									     int            h,
									     int            rot,
									     Outbuf_Depth   depth,
									     XCBConnection *conn,
									     XCBDRAWABLE    draw,
									     XCBVISUALTYPE *vis,
									     XCBCOLORMAP    cmap,
									     int            x_depth,
									     Outbuf_Perf   *perf,
									     int            grayscale,
									     int            max_colors,
									     XCBDRAWABLE    mask,
									     int            shape_dither);

char          *evas_software_x11_outbuf_software_xcb_perf_serialize_x       (Outbuf_Perf *perf);
void           evas_software_x11_outbuf_software_xcb_perf_deserialize_x     (Outbuf_Perf *perf,
									     const char *data);
Outbuf_Perf   *evas_software_x11_outbuf_software_xcb_perf_new_x             (XCBConnection *conn,
									     XCBDRAWABLE    draw,
									     XCBVISUALTYPE *vis,
									     XCBCOLORMAP    cmap,
									     int      	     x_depth);

char          *evas_software_x11_outbuf_software_xcb_perf_serialize_info_x  (Outbuf_Perf *perf);
void           evas_software_x11_outbuf_software_xcb_perf_store_x           (Outbuf_Perf *perf);
Outbuf_Perf   *evas_software_x11_outbuf_software_xcb_perf_restore_x         (XCBConnection *conn,
									     XCBDRAWABLE    draw,
									     XCBVISUALTYPE *vis,
									     XCBCOLORMAP    cmap,
									     int            x_depth);
void           evas_software_x11_outbuf_software_xcb_perf_free              (Outbuf_Perf *perf);
Outbuf_Perf   *evas_software_x11_outbuf_software_xcb_perf_x                 (XCBConnection *conn,
									     XCBDRAWABLE    draw,
									     XCBVISUALTYPE *vis,
									     XCBCOLORMAP    cmap,
									     int            x_depth);

void           evas_software_x11_outbuf_software_xcb_blit                   (Outbuf *buf,
									     int     src_x,
									     int     src_y,
									     int     w,
									     int     h,
									     int     dst_x,
									     int     dst_y);
void           evas_software_x11_outbuf_software_xcb_update                 (Outbuf *buf,
									     int     x,
									     int     y,
									     int     w,
									     int     h);
RGBA_Image    *evas_software_x11_outbuf_software_xcb_new_region_for_update  (Outbuf *buf,
									     int     x,
									     int     y,
									     int     w,
									     int     h,
									     int    *cx,
									     int    *cy,
									     int    *cw,
									     int    *ch);
void           evas_software_x11_outbuf_software_xcb_free_region_for_update (Outbuf    *buf,
									     RGBA_Image *update);
void           evas_software_x11_outbuf_software_xcb_push_updated_region    (Outbuf     *buf,
									     RGBA_Image *update,
									     int         x,
									     int         y,
									     int         w,
									     int         h);
void           evas_software_x11_outbuf_software_xcb_reconfigure            (Outbuf      *buf,
									     int          w,
									     int          h,
									     int          rot,
									     Outbuf_Depth depth);
int            evas_software_x11_outbuf_software_xcb_get_width              (Outbuf *buf);
int            evas_software_x11_outbuf_software_xcb_get_height             (Outbuf *buf);
Outbuf_Depth   evas_software_x11_outbuf_software_xcb_get_depth              (Outbuf *buf);
int            evas_software_x11_outbuf_software_xcb_get_rot                (Outbuf *buf);
int            evas_software_x11_outbuf_software_xcb_get_have_backbuf       (Outbuf *buf);
void           evas_software_x11_outbuf_software_xcb_set_have_backbuf       (Outbuf *buf, int have_backbuf);
void           evas_software_x11_outbuf_software_xcb_drawable_set           (Outbuf *buf, XCBDRAWABLE draw);
void           evas_software_x11_outbuf_software_xcb_mask_set               (Outbuf *buf, XCBDRAWABLE mask);
void           evas_software_x11_outbuf_software_xcb_rotation_set           (Outbuf *buf, int rot);

void           evas_software_x11_outbuf_software_xcb_debug_set              (Outbuf *buf, int debug);
void           evas_software_x11_outbuf_software_xcb_debug_show             (Outbuf     *buf,
									     XCBDRAWABLE draw,
									     int         x,
									     int         y,
									     int         w,
									     int         h);
    
#endif /* EVAS_ENGINE_H */