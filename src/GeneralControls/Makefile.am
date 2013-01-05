SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcppgenctl.a

libwcppgenctl_a_SOURCES			=							\
								Label/LabelWnd.cpp			\
								Label/UrlWnd.cpp			\
								NotifyWnd/NotifyWnd.cpp		\
								XEditor/UndoOperation.h		\
								XEditor/UndoOperation.cpp	\
								XEditor/UndoMaintainer.h	\
								XEditor/UndoMaintainer.cpp	\
								XEditor/XEditorWindow.cpp
								
libwcppgenctl_a_CPPFLAGS		=								\
								-I$(top_srcdir)/src/Window		\
								-I$(top_srcdir)/src/WCppUtils	\
								-I$(top_srcdir)/src/EventSystem	\
								-I$(top_srcdir)/src/CommonControls


pkginclude_HEADERS				=						\
								LabelWnd.h				\
								UrlWnd.h				\
								NotifyWnd.h				\
								XEditorWindow.h


