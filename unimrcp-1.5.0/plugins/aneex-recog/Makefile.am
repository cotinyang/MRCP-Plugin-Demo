AM_CPPFLAGS                	= $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         	= aneexrecog.la

aneexrecog_la_SOURCES       = src/aneex_recog_engine.c
aneexrecog_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS)
aneexrecog_la_LDLIBS        = -laudioneex
aneexrecog_la_DATA 			= /usr/src/audioneex/lib/libaudioneex.so

include $(top_srcdir)/build/rules/uniplugin.am

UNIMRCP_PLUGIN_INCLUDES += -I/usr/src/audioneex/src -I/usr/src/audioneex/src/index

 
