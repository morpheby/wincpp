SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

INCLUDES 						=	$(default_includes)			\
									-I$(srcdir)/../Window		\
									-I$(srcdir)/../WCppUtils	\
									-I$(srcdir)/../EventSystem

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcppgenctl.a

libwcppgenctl_a_SOURCES			=						\
								Label/LabelWnd.cpp		\
								Label/UrlWnd.cpp		\
								NotifyWnd/NotifyWnd.cpp


pkginclude_HEADERS				=						\
								Label/LabelWnd.h		\
								Label/UrlWnd.h			\
								NotifyWnd/NotifyWnd.h


