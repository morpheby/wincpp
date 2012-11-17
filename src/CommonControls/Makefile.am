SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

INCLUDES 						=	$(default_includes)			\
									-I$(srcdir)/../Window		\
									-I$(srcdir)/../WCppUtils	\
									-I$(srcdir)/../EventSystem

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
					Editbox/EditboxWnd.cpp			\
					Editbox/EditorWnd.cpp			\
					Trackbar/TrackbarWnd.cpp		\
					Trackbar/TrackBarWndInternal.cpp


pkginclude_HEADERS				=					\
					CommCtlWnd.h					\
					Button/ButtonWnd.h				\
					ComboBox/SimpleCB.h				\
					ComboBox/DropDownCB.h			\
					ComboBox/DropDownListCB.h		\
					ComboBox/CBWndInternal.h		\
					ComboBox/ComboBoxWnd.h			\
					ComboBox/ComboBox.h				\
					Editbox/EditboxWndInternal.h	\
					Editbox/EditboxWnd.h			\
					Editbox/EditorWnd.h				\
					Trackbar/TrackBarWndInternal.h	\
					Trackbar/TrackbarWnd.h
