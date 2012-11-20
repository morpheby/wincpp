SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcppwidget.a

libwcppwidget_a_SOURCES			=						\
	 							  Widget.cpp			\
	 							  WidgetEventParams.cpp
								
libwcppwidget_a_CPPFLAGS		=									\
								  -I$(top_srcdir)/src/Window		\
								  -I$(top_srcdir)/src/WCppUtils		\
								  -I$(top_srcdir)/src/EventSystem

pkginclude_HEADERS				= 						\
								  Widget.h				\
								  WidgetEventParams.h
