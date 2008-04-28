
MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/include \
-I$(top_srcdir)/src/modules/engines \
-I$(top_srcdir)/src/modules/engines/software_16 \
@FREETYPE_CFLAGS@

if BUILD_ENGINE_SOFTWARE_16_WINCE

pkgdir = $(libdir)/evas/modules/engines/software_16_wince/$(MODULE_ARCH)

pkg_LTLIBRARIES	= module.la

module_la_SOURCES = \
evas_engine.h \
evas_engine.c \
evas_wince_fb_buffer.c \
evas_wince_gapi_buffer.c

module_la_CFLAGS = @WIN32_CFLAGS@
module_la_LIBADD = $(top_builddir)/src/lib/libevas.la
module_la_LDFLAGS = @lt_no_undefined@ -module -avoid-version
module_la_LIBTOOLFLAGS = --tag=disable-static
module_la_DEPENDENCIES = $(top_builddir)/config.h

include_HEADERS = Evas_Engine_Software_16_WinCE.h

install-exec-hook:
	mv $(libdir)/evas/modules/engines/software_16_wince/$(MODULE_ARCH)/module.dll $(bindir)/engine_buffer.dll

endif