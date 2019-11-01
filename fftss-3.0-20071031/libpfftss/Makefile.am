lib_LTLIBRARIES = libpfftss.la
noinst_PROGRAMS = testmpi2d

testmpi2d_SOURCES = testmpi2d.c ../include/pfftss.h 
testmpi2d_LDADD = libpfftss.la ../libfftss/libfftss.la

libpfftss_la_SOURCES = mpi2d.c ../include/pfftss.h
libpfftss_la_LDFLAGS = -version-info 1:0:0


