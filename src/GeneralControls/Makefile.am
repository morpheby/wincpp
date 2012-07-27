SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

INCLUDES 						= $(default_includes)
EXTRA_DIST 						= Headers

noinst_LIBRARIES				= libwcppgenctl.a

libwcppgenctl_a_SOURCES			=						\
								Label/LabelWnd.cpp		\
								Label/UrlWnd.cpp		\
								NotifyWnd/NotifyWnd.cpp	\
								RenameWnd/RenameWnd.cpp		


pkginclude_HEADERS				=						\
								Label/LabelWnd.h		\
								Label/UrlWnd.h			\
								NotifyWnd/NotifyWnd.h	\
								RenameWnd/RenameWnd.h


