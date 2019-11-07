AM_CPPFLAGS          = -I/usr/src/audioneex/src \
					   -I/usr/src/audioneex/src/index \
                       $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         	= aneexrecog.la

aneexrecog_la_SOURCES       = src/aneex_recog_engine.c
aneexrecog_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS)
apttest_LDADD        		= /usr/src/audioneex/lib/libaudioneex.so \
                       		  $(UNIMRCP_PLUGIN_LIBS)

include $(top_srcdir)/build/rules/uniplugin.am

#UNIMRCP_PLUGIN_INCLUDES += -I/usr/src/audioneex/src -I/usr/src/audioneex/src/index

 
