SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcpputils.a

libwcpputils_a_SOURCES			=					\
								  errorParse.cpp	\
	 							  sharedPtr.cpp		\
	 							  utf8.cpp			\
	 							  Serializable.cpp	\
								  serialized_io.cpp
libwcpputils_a_CPPFLAGS			=									\
								  -I$(top_srcdir)/src/EventSystem


pkginclude_HEADERS				= errorParse.h sharedPtr.h utf8.h Serializable.h Serializable.hpp serialized_io.h serialized_io.hpp
