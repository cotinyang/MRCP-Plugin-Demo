AM_CPPFLAGS          = -I/usr/src/audioneex/src \
					   -I/usr/src/audioneex/src/index \
                       $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         	= aneexrecog.la

aneexrecog_la_SOURCES       = src/aneex_recog_engine.c
aneexrecog_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS) -L/usr/src/audioneex/lib -laudioneex 

include $(top_srcdir)/build/rules/uniplugin.am

 
