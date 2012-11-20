SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcppcommctl.a

libwcppcommctl_a_SOURCES		=					\
					CommCtlWnd.cpp					\
					Button/ButtonWnd.cpp			\
					ComboBox/SimpleCB.cpp			\
					ComboBox/DropDownCB.cpp			\
					ComboBox/DropDownListCB.cpp		\
					ComboBox/CBWndInternal.cpp		\
					ComboBox/ComboBoxWnd.cpp		\
					Editbox/EditboxWndInternal.cpp	\
					Editbox/EditboxWndInternal.h	\
					Editbox/EditboxWnd.cpp			\
					Editbox/EditorWnd.cpp			\
					Trackbar/TrackbarWnd.cpp		\
					Trackbar/TrackBarWndInternal.cpp\
					Trackbar/TrackBarWndInternal.h

libwcppcommctl_a_CPPFLAGS		=					\
					-I$(top_srcdir)/src/Window		\
					-I$(top_srcdir)/src/WCppUtils	\
					-I$(top_srcdir)/src/EventSystem


pkginclude_HEADERS				=			\
					CommCtlWnd.h			\
					ButtonWnd.h				\
					SimpleCB.h				\
					DropDownCB.h			\
					DropDownListCB.h		\
					CBWndInternal.h			\
					ComboBoxWnd.h			\
					ComboBox.h				\
					EditboxWnd.h			\
					EditorWnd.h				\
					TrackbarWnd.h
