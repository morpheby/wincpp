SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcpputils.a

libwcpputils_a_SOURCES			=					\
								  errorParse.cpp	\
	 							  sharedPtr.cpp		\
	 							  utf8.cpp				
libwcpputils_a_CPPFLAGS			=


pkginclude_HEADERS				= errorParse.h sharedPtr.h utf8.h
