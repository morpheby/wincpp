SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

INCLUDES	 					= $(default_includes)
EXTRA_DIST 						= Headers


noinst_LIBRARIES				= libwcppevents.a

libwcppevents_a_SOURCES			= Events.cpp

pkginclude_HEADERS				= Events.h
