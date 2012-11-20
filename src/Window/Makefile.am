SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

EXTRA_DIST 						= 

noinst_LIBRARIES				= libwcppwindow.a

libwcppwindow_a_SOURCES			=						\
				 				  Window.cpp			\
				 				  Cursors.cpp			\
	 							  DPIScaler.cpp			\
	 							  Bitmap.cpp			\
	 							  CommonControlsInit.c
								
libwcppwindow_a_CPPFLAGS		=								\
								-I$(top_srcdir)/src/WCppUtils	\
								-I$(top_srcdir)/src/EventSystem

pkginclude_HEADERS				= Window.h		\
								  Cursors.h	\
								  DPIScaler.h	\
								  Bitmap.h		\
								  vsstyle.h		\
								  vssym32.h		\
								  CommonControlsInit.h
