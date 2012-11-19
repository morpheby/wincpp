SUFFIXES =
include $(top_srcdir)/src/rc-build.mk
include $(top_srcdir)/src/lib-build.mk

SUBDIRS						= 					\
							  WCppUtils			\
							  EventSystem		\
							  Window			\
							  Widget			\
							  CommonControls	\
							  GeneralControls

INCLUDES 						=	$(default_includes)			\
									-I$(srcdir)/Window			\
									-I$(srcdir)/Widget			\
									-I$(srcdir)/WCppUtils		\
									-I$(srcdir)/EventSystem		\
									-I$(srcdir)/CommonControls	\
									-I$(srcdir)/GeneralControls

EXTRA_DIST 						= 


BUILT_IMP_LIBS				= 								\
						-Wl,WCppUtils/libwcpputils.a		\
						-Wl,EventSystem/libwcppevents.a		\
						-Wl,Window/libwcppwindow.a			\
						-Wl,Widget/libwcppwidget.a			\
						-Wl,CommonControls/libwcppcommctl.a	\
						-Wl,GeneralControls/libwcppgenctl.a

BUILT_LIBS					= WCppUtils/libwcpputils.a			\
							  EventSystem/libwcppevents.a		\
							  Window/libwcppwindow.a			\
							  Widget/libwcppwidget.a			\
							  CommonControls/libwcppcommctl.a	\
							  GeneralControls/libwcppgenctl.a

# Override EXEEXT to build dll
EXEEXT						=

bins						= $(top_builddir)/bin

### wincpp.dll ###
bin_PROGRAMS                = $(bins)/wincpp.dll

__bins__wincpp_dll_SOURCES	= 								\
							  resources/wincpp.rc			\
							  resources/Resource.h			\
							  resources/main.premf			\
							  main.cpp

__bins__wincpp_dll_LDFLAGS	= -shared -Wl,--out-implib=$(bins)/libwincpp.a -Wl,--export-all-symbols \
							  -Wl,--compat-implib \
							  -Wl,--whole-archive $(BUILT_IMP_LIBS) -Wl,--no-whole-archive
__bins__wincpp_dll_LDADD	= -luxtheme -lgdi32 -lole32
__bins__wincpp_dll_DEPENDENCIES = $(BUILT_LIBS)
							  

EXTRA_DIST				   += app.premf
BUILT_SOURCES 				= app.manifest
CLEANFILES					= resources/main.manifest app.manifest

# Wincpp.dll generates an implib, which shall be installed to the libdir
lib_LIBRARIES				= $(bins)/libwincpp.a
__bins__libwincpp_a_SOURCES	=

check_PROGRAMS				= $(bins)/eventtest.exe		\
							  $(bins)/windowtest.exe	\
							  $(bins)/widgettest.exe	\
							  $(bins)/editboxtest.exe 	\
							  $(bins)/labeltest.exe		\
							  $(bins)/btntest.exe		\
							  $(bins)/urltest.exe		\
							  $(bins)/cbtest.exe		\
							  $(bins)/tabstest.exe

TESTS						= $(check_PROGRAMS)


__bins__eventtest_exe_SOURCES	= test/eventtest.cpp
__bins__eventtest_exe_LDADD		= -lwincpp -L$(bins)

__bins__windowtest_exe_SOURCES	= test/windowtest.cpp test/windowtest.h
__bins__windowtest_exe_LDADD	= -lwincpp -L$(bins)
__bins__windowtest_exe_LDFLAGS	= -municode -mwindows
CLEANFILES					   += $(bins)/windowtest.exe.manifest
windowtest.$(OBJEXT): $(bins)/windowtest.exe.manifest
$(bins)/windowtest.exe.manifest: app.manifest
	$(LN_S) $< $@

__bins__widgettest_exe_SOURCES	= test/widgettest.cpp
__bins__widgettest_exe_LDADD	= -lwincpp -L$(bins)
__bins__widgettest_exe_LDFLAGS	= -municode -mwindows
CLEANFILES					   += $(bins)/widgettest.exe.manifest
widgettest.$(OBJEXT): $(bins)/widgettest.exe.manifest
$(bins)/widgettest.exe.manifest: app.manifest
	$(LN_S) $< $@

__bins__editboxtest_exe_SOURCES	= test/editboxtest.cpp
__bins__editboxtest_exe_LDADD	= -lwincpp -L$(bins)
__bins__editboxtest_exe_LDFLAGS	= -municode -mwindows
CLEANFILES					   += $(bins)/editboxtest.exe.manifest
editboxtest.$(OBJEXT): $(bins)/editboxtest.exe.manifest
$(bins)/editboxtest.exe.manifest: app.manifest
	$(LN_S) $< $@

__bins__labeltest_exe_SOURCES	= test/labeltest.cpp test/labeltest.h
__bins__labeltest_exe_LDADD		= -lwincpp -L$(bins)
__bins__labeltest_exe_LDFLAGS	= -municode -mwindows
CLEANFILES					   += $(bins)/labeltest.exe.manifest
labeltest.$(OBJEXT): $(bins)/labeltest.exe.manifest
$(bins)/labeltest.exe.manifest: app.manifest
	$(LN_S) $< $@

__bins__btntest_exe_SOURCES		= test/btntest.cpp test/btntest.h
__bins__btntest_exe_LDADD		= -lwincpp -L$(bins)
__bins__btntest_exe_LDFLAGS		= -municode -mwindows
CLEANFILES					   += $(bins)/btntest.exe.manifest
btntest.$(OBJEXT): $(bins)/btntest.exe.manifest
$(bins)/btntest.exe.manifest: app.manifest
	$(LN_S) $< $@

__bins__cbtest_exe_SOURCES		= test/cbtest.cpp test/cbtest.h
__bins__cbtest_exe_LDADD		= -lwincpp -L$(bins)
__bins__cbtest_exe_LDFLAGS		= -municode -mwindows
CLEANFILES					   += $(bins)/cbtest.exe.manifest
cbtest.$(OBJEXT): $(bins)/cbtest.exe.manifest
$(bins)/cbtest.exe.manifest: app.manifest
	$(LN_S) $< $@

__bins__urltest_exe_SOURCES		= test/urltest.cpp test/urltest.h
__bins__urltest_exe_LDADD		= -lwincpp -L$(bins)
__bins__urltest_exe_LDFLAGS		= -municode -mwindows
CLEANFILES					   += $(bins)/urltest.exe.manifest
urltest.$(OBJEXT): $(bins)/urltest.exe.manifest
$(bins)/urltest.exe.manifest: app.manifest
	$(LN_S) $< $@

__bins__tabstest_exe_SOURCES		= test/tabstest.cpp
__bins__tabstest_exe_LDADD			= -lwincpp -L$(bins)
__bins__tabstest_exe_LDFLAGS		= -municode -mwindows
CLEANFILES						   += $(bins)/tabstest.exe.manifest
tabstest.$(OBJEXT): $(bins)/tabstest.exe.manifest
$(bins)/tabstest.exe.manifest: app.manifest
	$(LN_S) $< $@

# dependencies for resource file
resources/wincpp.$(OBJEXT): $(srcdir)/resources/wincpp.rc \
				$(srcdir)/resources/Resource.h resources/main.manifest


