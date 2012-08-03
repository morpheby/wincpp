SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

INCLUDES 						= $(default_includes)
EXTRA_DIST 						= Headers

noinst_LIBRARIES				= libwcppwindow.a

libwcppwindow_a_SOURCES			=						\
				 				  Window.cpp			\
				 				  Cursors.cpp			\
	 							  DPIScaler.cpp			\
	 							  Bitmap.cpp			\
	 							  CommonControlsInit.c

pkginclude_HEADERS				= Window.h		\
								  Cursors.cpp	\
								  DPIScaler.h	\
								  Bitmap.h		\
								  vsstyle.h		\
								  vssym32.h		\
								  CommonControlsInit.h
