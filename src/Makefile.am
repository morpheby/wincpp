SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

SUBDIRS						= 					\
							  WCppUtils			\
							  EventSystem		\
							  Window			\
							  CommonControls	\
							  GeneralControls

INCLUDES 					= $(default_includes)
EXTRA_DIST 					= 


BUILT_LIBS					= 								\
						-Wl,WCppUtils/libwcpputils.a		\
						-Wl,EventSystem/libwcppevents.a		\
						-Wl,Window/libwcppwindow.a			\
						-Wl,CommonControls/libwcppcommctl.a	\
						-Wl,GeneralControls/libwcppgenctl.a

# Override EXEEXT to build dll
EXEEXT						=

### wincpp.dll ###
bin_PROGRAMS                = wincpp.dll

wincpp_dll_SOURCES			= 								\
							  resources/wincpp.rc			\
							  resources/Resource.h			\
							  resources/main.premf			\
							  main.cpp

wincpp_dll_LDFLAGS			= -shared -Wl,--out-implib=libwincpp.a -Wl,--export-all-symbols -Wl,--compat-implib \
							  -Wl,--whole-archive $(BUILT_LIBS) -Wl,--no-whole-archive
wincpp_dll_LDADD			= -luxtheme -lgdi32 -lole32

EXTRA_DIST				   += app.premf
BUILT_SOURCES 				= resources/main.manifest app.manifest
CLEANFILES					= resources/main.manifest app.manifest

# Wincpp.dll generates an implib, which shall be installed to the libdir
lib_LIBRARIES				= libwincpp.a
libwincpp_a_SOURCES			=

check_PROGRAMS				= eventtest.exe windowtest.exe editboxtest.exe \
							  labeltest.exe
TESTS						= $(check_PROGRAMS)

eventtest_exe_SOURCES		= test/eventtest.cpp
eventtest_exe_LDADD			= -lwincpp -L.

windowtest_exe_SOURCES		= test/windowtest.cpp test/windowtest.h
windowtest_exe_LDADD		= -lwincpp -L.
windowtest_exe_LDFLAGS		= -municode -mwindows
CLEANFILES				   += windowtest.exe.manifest
windowtest.$(OBJEXT): windowtest.exe.manifest
windowtest.exe.manifest: app.manifest
	$(LN_S) $< $@

editboxtest_exe_SOURCES		= test/editboxtest.cpp test/editboxtest.h
editboxtest_exe_LDADD		= -lwincpp -L.
editboxtest_exe_LDFLAGS		= -municode -mwindows
CLEANFILES				   += editboxtest.exe.manifest
editboxtest.$(OBJEXT): editboxtest.exe.manifest
editboxtest.exe.manifest: app.manifest
	$(LN_S) $< $@

labeltest_exe_SOURCES		= test/labeltest.cpp test/labeltest.h
labeltest_exe_LDADD			= -lwincpp -L.
labeltest_exe_LDFLAGS		= -municode -mwindows
CLEANFILES				   += labeltest.exe.manifest
labeltest.$(OBJEXT): labeltest.exe.manifest
labeltest.exe.manifest: app.manifest
	$(LN_S) $< $@

# dependencies for resource file
resources/wincpp.$(OBJEXT): $(srcdir)/resources/wincpp.rc \
				$(srcdir)/resources/Resource.h $(srcdir)/resources/main.manifest


