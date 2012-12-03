SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

									-I$(srcdir)/../Widget		\
EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcppgenctl.a

libwcppgenctl_a_SOURCES			=						\
								Label/LabelWnd.cpp		\
								Label/UrlWnd.cpp		\
								NotifyWnd/NotifyWnd.cpp	\
								Tabs/TabController.cpp	\
								Tabs/TabWidget.cpp		\
								Tabs/TabButton.cpp		\
								Tabs/TabButton.h		\
								Tabs/TabCloseButton.cpp	\
								Tabs/TabCloseButton.h	\
								XEditor/XEditorWindow.cpp
								
libwcppgenctl_a_CPPFLAGS		=								\
								-I$(top_srcdir)/src/Window		\
								-I$(top_srcdir)/src/Widget		\
								-I$(top_srcdir)/src/WCppUtils	\
								-I$(top_srcdir)/src/EventSystem	\
								-I$(top_srcdir)/src/CommonControls


pkginclude_HEADERS				=						\
								LabelWnd.h				\
								UrlWnd.h				\
								NotifyWnd.h				\
								TabController.h			\
								TabWidget.h				\
								XEditorWindow.h


