AM_CPPFLAGS                = $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         = aneexrecog.la

aneexrecog_la_SOURCES       = src/aneex_recog_engine.c
aneexrecog_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS) \
                              -L$(top_srcdir)/plugins/third-party/aneex/libs/x64 \
                              -lmsc -ldl -lpthread -lrt
aneexrecog_ladir            = $(libdir)
aneexrecog_la_DATA          = $(top_srcdir)/plugins/third-party/aneex/libs/x64/libmsc.so



include $(top_srcdir)/build/rules/uniplugin.am

UNIMRCP_PLUGIN_INCLUDES     += -I$(top_srcdir)/plugins/third-party/aneex/include
 