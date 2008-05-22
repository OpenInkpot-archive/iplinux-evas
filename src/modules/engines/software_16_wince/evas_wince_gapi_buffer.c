#include "evas_common.h"
#include "evas_engine.h"


typedef int (*evas_engine_wince_close_display)();
typedef int (*evas_engine_wince_close_input)();

typedef struct Evas_Engine_WinCE_GAPI_Priv Evas_Engine_WinCE_GAPI_Priv;


#define GETGXINFO 0x00020000

typedef struct
{
    long Version;               //00 (should filled with 100 before calling ExtEscape)
    void *pvFrameBuffer;        //04
    unsigned long cbStride;     //08
    unsigned long cxWidth;      //0c
    unsigned long cyHeight;     //10
    unsigned long cBPP;         //14
    unsigned long ffFormat;     //18
    char Unused[0x84 - 7 * 4];
} _GXDeviceInfo;


#define LINK(type,name,import) \
  name = (gapi_##type)GetProcAddress (gapi_lib, import)

#define GX_FULLSCREEN 0x01
#define GX_NORMALKEYS 0x02

#define kfDirect555   0x40
#define kfDirect565   0x80


typedef struct
{
   DWORD cxWidth;
   DWORD cyHeight;
   LONG  cbxPitch;
   LONG  cbyPitch;
   LONG  cBPP;
   DWORD ffFormat;
} _GAPI_Display_Properties;

typedef struct
{
  short vkUp;        // key for up
  POINT ptUp;        // x,y position of key/button.  Not on screen but in screen coordinates.
  short vkDown;
  POINT ptDown;
  short vkLeft;
  POINT ptLeft;
  short vkRight;
  POINT ptRight;
  short vkA;
  POINT ptA;
  short vkB;
  POINT ptB;
  short vkC;
  POINT ptC;
  short vkStart;
  POINT ptStart;
} _GAPI_Key_List;

typedef int                      (*gapi_display_open)(HWND hWnd, DWORD dwFlags);
typedef int                      (*gapi_display_close)();
typedef _GAPI_Display_Properties (*gapi_display_properties_get)(void);
typedef void*                    (*gapi_draw_begin)(void);
typedef int                      (*gapi_draw_end)(void);
typedef int                      (*gapi_input_open)(void);
typedef int                      (*gapi_input_close)(void);
typedef _GAPI_Key_List           (*gapi_default_keys_get)(int iOptions);
typedef int                      (*gapi_suspend)(void);
typedef int                      (*gapi_resume)(void);

gapi_default_keys_get default_keys_get = NULL;
gapi_suspend          suspend = NULL;
gapi_resume           resume = NULL;
_GAPI_Key_List       *default_keys = NULL;

int
evas_software_wince_gapi_suspend(void)
{
   if (suspend)
     return suspend();
   else
     return 0;
}

int
evas_software_wince_gapi_resume(void)
{
   if (resume)
     return resume();
   else
     return 0;
}

void *
evas_software_wince_gapi_default_keys(void)
{
   return default_keys;
}


struct Evas_Engine_WinCE_GAPI_Priv
{
   HMODULE            lib;
   gapi_display_close close_display;
   gapi_input_close   close_input;
   gapi_draw_begin    draw_begin;
   gapi_draw_end      draw_end;
   void              *buffer;
   int                width;
   int                height;
   int                stride;
};

void *
evas_software_wince_gapi_init (HWND window)
{
    WCHAR                       oemstr[100];
   _GAPI_Display_Properties     prop;
   _GAPI_Key_List               key_list;
   HMODULE                      gapi_lib;
   Evas_Engine_WinCE_GAPI_Priv *priv;

   gapi_display_open            display_open = NULL;
   gapi_display_close           display_close = NULL;
   gapi_display_properties_get  display_properties_get = NULL;
   gapi_draw_begin              draw_begin = NULL;
   gapi_draw_end                draw_end = NULL;
   gapi_input_open              input_open = NULL;
   gapi_input_close             input_close = NULL;

   priv = (Evas_Engine_WinCE_GAPI_Priv *)malloc(sizeof(Evas_Engine_WinCE_GAPI_Priv));
   if (!priv)
     return NULL;

   gapi_lib = LoadLibrary(L"\\Windows\\gx.dll");
   if (!gapi_lib)
     {
        gapi_lib = LoadLibrary(L"gx.dll");
        if (!gapi_lib) {
           printf ("error : LoadLibrary\n");
           goto free_priv;
        }
     }

   LINK(display_open, display_open, L"?GXOpenDisplay@@YAHPAUHWND__@@K@Z");
   LINK(display_close, display_close, L"?GXCloseDisplay@@YAHXZ");
   LINK(display_properties_get, display_properties_get, L"?GXGetDisplayProperties@@YA?AUGXDisplayProperties@@XZ");
   LINK(draw_begin, draw_begin, L"?GXBeginDraw@@YAPAXXZ");
   LINK(draw_end, draw_end, L"?GXEndDraw@@YAHXZ");
   LINK(input_open, input_open, L"?GXOpenInput@@YAHXZ" );
   LINK(input_close, input_close, L"?GXCloseInput@@YAHXZ" );
   LINK(default_keys_get, default_keys_get, L"?GXGetDefaultKeys@@YA?AUGXKeyList@@H@Z");
   LINK(suspend, suspend, L"?GXSuspend@@YAHXZ" );
   LINK(resume, resume, L"?GXResume@@YAHXZ" );

   if (!display_open ||
       !display_close ||
       !display_properties_get ||
       !draw_begin ||
       !draw_end ||
       !input_open ||
       !input_close ||
       !default_keys_get ||
       !suspend ||
       !resume)
     {
        printf ("error : no valid symbols\n");
        goto free_lib;
     }

   if (!display_open(window, GX_FULLSCREEN))
     {
        printf ("error : GXOpenDisplay\n");
        goto free_lib;
     }

   prop = display_properties_get();

   // verify pixel format
   if(!(prop.ffFormat & kfDirect565) || (prop.cBPP != 16))
     {
        printf ("error : GAPI format mismatch\n");
        goto close_display;
     }

   // verify we have a vga device
   if ((GetSystemMetrics(SM_CXSCREEN) != (int)prop.cxWidth) ||
       (GetSystemMetrics(SM_CYSCREEN) != (int)prop.cyHeight))
     {
        printf ("error : GAPI format mismatch\n");
        goto close_display;
     }

   if (!input_open())
     {
        printf ("error : GXOpenInput\n");
        goto close_display;
     }

   priv->lib = gapi_lib;
   priv->close_display = display_close;
   priv->close_input = input_close;
   priv->draw_begin = draw_begin;
   priv->draw_end = draw_end;

   key_list = default_keys_get(GX_NORMALKEYS);
   default_keys = (_GAPI_Key_List *)malloc(sizeof(_GAPI_Key_List));
   if (!default_keys)
     {
        printf ("error : GXOpenInput\n");
        goto close_input;
     }
   memcpy(default_keys, &key_list, sizeof(_GAPI_Key_List));

   /* GAPI on Ipaq H38** and H39** is completely buggy */
   /* They are detected as portrait device (width = 240 and height = 320) */
   /* but the framebuffer is managed like a landscape device : */
   /*
     240
 +---------+
 |         |
 |         |
 |         |
 |         |
 |         | 320
 | ^^^     |
 | |||     |
 | |||     |
 | |||     |
 +---------+
  ---->

   */
   /* So these devices are considered as landscape devices */
   /* and width and height are switched. */
   /* Other devices are managed normally : */
   /*
     240
  +---------+
| |--->     |
| |--->     |
| |--->     |
v |         |
  |         | 320
  |         |
  |         |
  |         |
  |         |
  +---------+

    */

   SystemParametersInfo (SPI_GETOEMINFO, sizeof (oemstr), oemstr, 0);

   if (((oemstr[12] == 'H') &&
        (oemstr[13] == '3') &&
        (oemstr[14] == '8')) ||
       ((oemstr[12] == 'H') &&
        (oemstr[13] == '3') &&
        (oemstr[14] == '9')))
     {
        _GXDeviceInfo gxInfo = { 0 };
        HDC           dc;
        int           result;

        priv->width = prop.cyHeight;
        priv->height = prop.cxWidth;
        priv->stride = prop.cbxPitch;
        default_keys->vkA = 193;
        default_keys->vkB = 194;
        default_keys->vkC = 195;
        default_keys->vkStart = 196;

        dc = GetDC (window);
        if (!dc)
          goto free_keys;

        gxInfo.Version = 100;
        result = ExtEscape(dc, GETGXINFO, 0, NULL, sizeof(gxInfo),
                           (char *) &gxInfo);
        if (result <= 0)
          {
             ReleaseDC(window, dc);
             goto free_keys;
          }

        priv->buffer = gxInfo.pvFrameBuffer;
        ReleaseDC(window, dc);
     }
   else
     {
        priv->width = prop.cxWidth;
        priv->height = prop.cyHeight;
        priv->stride = prop.cbyPitch;
        priv->buffer = NULL;
     }

   return priv;

 free_keys:
   free(default_keys);
 close_input:
   input_close();
 close_display:
   display_close();
 free_lib:
   FreeLibrary(gapi_lib);
 free_priv:
   free(priv);
   return NULL;
}

void
evas_software_wince_gapi_shutdown(void *priv)
{
   Evas_Engine_WinCE_GAPI_Priv *p;

   p = (Evas_Engine_WinCE_GAPI_Priv *)priv;
   p->close_input();
   p->close_display();
   suspend = NULL;
   resume = NULL;
   FreeLibrary(p->lib);
   free(p);
}


FB_Output_Buffer *
evas_software_wince_gapi_output_buffer_new(void *priv,
                                           int   width,
                                           int   height)
{
   FB_Output_Buffer *fbob;
   void             *buffer;

   fbob = calloc(1, sizeof(FB_Output_Buffer));
   if (!fbob) return NULL;

   buffer = malloc (width * height * 2); /* we are sure to have 16bpp */
   if (!buffer)
     {
        free(fbob);
        return NULL;
     }

   fbob->priv = priv;

   fbob->im = (Soft16_Image *) evas_cache_image_data(evas_common_soft16_image_cache_get(), width, height, (DATA32 *)buffer, 0, EVAS_COLORSPACE_RGB565_A5P);
   if (fbob->im)
     fbob->im->stride = ((Evas_Engine_WinCE_GAPI_Priv *)priv)->stride >> 1;

   return fbob;
}

void
evas_software_wince_gapi_output_buffer_free(FB_Output_Buffer *fbob)
{
   free(fbob->im->pixels);
   free(fbob);
}

void
evas_software_wince_gapi_output_buffer_paste(FB_Output_Buffer *fbob)
{
   Evas_Engine_WinCE_GAPI_Priv *priv;
   void                        *buffer;

   priv = (Evas_Engine_WinCE_GAPI_Priv *)fbob->priv;

   buffer = priv->draw_begin();
   if (!buffer)
     return;

   printf ("buffers : %p %p\n", buffer, priv->buffer);
   if (priv->buffer) buffer = priv->buffer;

   if ((fbob->im->cache_entry.w == priv->width) &&
       (fbob->im->cache_entry.h == priv->height))
     memcpy(buffer, fbob->im->pixels,
            priv->width * priv->height * 2);

   priv->draw_end();
}

void
evas_software_wince_gapi_surface_resize(FB_Output_Buffer *ddob)
{
}
