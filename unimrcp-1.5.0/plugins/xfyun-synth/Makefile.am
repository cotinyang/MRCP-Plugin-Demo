AM_CPPFLAGS                = $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         = xfyunsynth.la

xfyunsynth_la_SOURCES       = src/xfyun_synth_engine.c

xfyunsynth_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS) \
                              -L$(top_srcdir)/plugins/third-party/xfyun/libs/x64 \
                              -lmsc -ldl -lpthread -lrt
xfyunsynth_ladir            = $(libdir)

include $(top_srcdir)/build/rules/uniplugin.am

UNIMRCP_PLUGIN_INCLUDES     += -I$(top_srcdir)/plugins/third-party/xfyun/include