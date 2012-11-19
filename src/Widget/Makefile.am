SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

INCLUDES 						=	$(default_includes)			\
									-I$(srcdir)/../Window		\
									-I$(srcdir)/../WCppUtils	\
									-I$(srcdir)/../EventSystem

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcppwidget.a

libwcppwidget_a_SOURCES			=						\
	 							  Widget.cpp			\
	 							  WidgetEventParams.cpp

pkginclude_HEADERS				= 						\
								  Widget.h				\
								  WidgetEventParams.h
