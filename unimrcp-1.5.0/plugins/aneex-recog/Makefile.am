AM_CPPFLAGS                = $(UNIMRCP_PLUGIN_INCLUDES)

plugin_LTLIBRARIES         = aneexrecog.la

aneexrecog_la_SOURCES       = src/aneex_recog_engine.c ${AX_SRC_ROOT}/DAO/${DATASTORE_T}.cpp ${AX_SRC_ROOT}/audio/AudioSource.cpp
aneexrecog_la_LDFLAGS       = $(UNIMRCP_PLUGIN_OPTS)

include $(top_srcdir)/build/rules/uniplugin.am
 