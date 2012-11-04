SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

INCLUDES 						= $(default_includes)
EXTRA_DIST 						= Headers

noinst_LIBRARIES				= libwcppwindow.a

libwcppwindow_a_SOURCES			=						\
				 				  Window.cpp			\
				 				  Cursors.cpp			\
				 				  DCFactory.cpp			\
				 				  DeviceContextBase.cpp	\
				 				  DeviceContext.cpp		\
	 							  DPIScaler.cpp			\
	 							  Bitmap.cpp			\
	 							  Drawer.cpp			\
	 							  ThemedDrawer.cpp		\
	 							  CommonControlsInit.c

pkginclude_HEADERS				= 				\
								  Window.h		\
								  Cursors.h		\
								  DPIScaler.h	\
								  Bitmap.h		\
								  vsstyle.h		\
								  vssym32.h		\
								  platform.h	\
								  DeviceContext.h		\
								  DeviceContextBase.h	\
								  DCFactory.h			\
								  Drawer.h				\
								  ThemedDrawer.h		\
								  CommonControlsInit.h
