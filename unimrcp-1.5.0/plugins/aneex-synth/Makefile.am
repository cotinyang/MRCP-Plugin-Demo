AM_CPPFLAGS                = $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         = aneexsynth.la

aneexsynth_la_SOURCES       = src/aneex_synth_engine.c

aneexsynth_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS) \
                              -L$(top_srcdir)/plugins/third-party/aneex/libs/x64 \
                              -lmsc -ldl -lpthread -lrt
aneexsynth_ladir            = $(libdir)

include $(top_srcdir)/build/rules/uniplugin.am

UNIMRCP_PLUGIN_INCLUDES     += -I$(top_srcdir)/plugins/third-party/aneex/include