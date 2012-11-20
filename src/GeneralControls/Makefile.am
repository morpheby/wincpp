SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcppgenctl.a

libwcppgenctl_a_SOURCES			=						\
								Label/LabelWnd.cpp		\
								Label/UrlWnd.cpp		\
								NotifyWnd/NotifyWnd.cpp
								
libwcppgenctl_a_CPPFLAGS		=								\
								-I$(top_srcdir)/src/Window		\
								-I$(top_srcdir)/src/WCppUtils	\
								-I$(top_srcdir)/src/EventSystem


pkginclude_HEADERS				=						\
								LabelWnd.h				\
								UrlWnd.h				\
								NotifyWnd.h


