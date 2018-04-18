AM_CPPFLAGS                = $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         = xfyunrecog.la

xfyunrecog_la_SOURCES       = src/xfyun_recog_engine.c
xfyunrecog_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS) \
                              -L$(top_srcdir)/plugins/third-party/xfyun/libs/x64 \
                              -lmsc -ldl -lpthread -lrt
xfyunrecog_ladir            = $(libdir)
xfyunrecog_la_DATA          = $(top_srcdir)/plugins/third-party/xfyun/libs/x64/libmsc.so



include $(top_srcdir)/build/rules/uniplugin.am

UNIMRCP_PLUGIN_INCLUDES     += -I$(top_srcdir)/plugins/third-party/xfyun/include
 