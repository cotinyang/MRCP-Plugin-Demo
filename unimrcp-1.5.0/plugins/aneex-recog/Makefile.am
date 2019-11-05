AM_CPPFLAGS                	= $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         	= aneexrecog.la

aneexrecog_la_SOURCES       = src/aneex_recog_engine.c
aneexrecog_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS) \
								-L/usr/local/lib \ 
								-lmsc -ldl -lpthread -lrt
aneexrecog_ladir 			= $(libdir)
aneexrecog_la_DATA 			= /usr/local/lib/libaudioneex.so

include $(top_srcdir)/build/rules/uniplugin.am



 