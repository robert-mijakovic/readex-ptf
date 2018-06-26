AM_CXXFLAGS += ${autotune_search_base_cxxflags} \
               -I$(top_srcdir)/autotune/searchalgorithms/activeharmony/include
AM_CFLAGS = ${AM_CXXFLAGS}

lib_LTLIBRARIES = libptfactiveharmony.la
libptfactiveharmony_la_SOURCES = ActiveHarmony.cc \
                                 hclient.c \
                                 hsockutil.c \
                                 hutil.c \
                                 hmesgs.c \
                                 hclientf.c

libptfactiveharmony_la_LDFLAGS = ${autotune_search_base_ldflags} -version-info 1:0:0
libdir = ${searchdir}/activeharmony/
