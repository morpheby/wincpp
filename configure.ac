#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT([WinCpp], [0.1], [morpheby@gmail.com])

AC_SUBST([VERSION_4D], ["0.0.1.0"])
AC_DEFINE([PACKAGE_VERSION_NUM], [0,0,1,0],
	[Define to the version number (acceptable by rc or windres) of this package.])
AC_DEFINE([VERSION_NUM], PACKAGE_VERSION_NUM,
	[Version number (acceptable by rc or windres) of this package.])
AC_DEFINE([PACKAGE_IS_RELEASE], [0], [Define to 1 if package is for release.])

AC_DEFINE([PACKAGE_AUTHOR], ["Morpheby"], [Author of the package])

AC_DEFINE([PACKAGE_COPYRIGHT], ["Copyright (C) 2012 " PACKAGE_AUTHOR ". Licensed under GNU GPL"], [Copyright])

AC_DEFINE([DEBUG], [], [Use for building library with debug information])
AC_DEFINE([_DEBUG], [], [Use for building library with debug information])

AC_DEFINE([UNICODE], [], [Necessary for library to be unicode])
AC_DEFINE([_UNICODE], [], [Necessary for library to be unicode])

AC_DEFINE([ISOLATION_AWARE_ENABLED], [0], [Necessary for isolating module dependencies])

AC_DEFINE([WINVER], [0x0501], [Define to the target windows version])
AC_DEFINE([WIN32_IE], [0x600], [Define to the target windows IE version])

AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_PROG_SED
AC_CHECK_PROG(RESCOMP, windres, windres)


# Checks for libraries.

# there shuold be checks for uxtheme, gdi32 and ole32,
# but neither of them has cdecl functions....

# Checks for header files.
AC_CHECK_HEADERS([memory.h stdlib.h string.h tchar.h \
	windows.h windowsx.h])

# MinGW commctrl.h may be non-self-compilable and
# demands inclusion of windows.h
AC_CHECK_HEADERS([commctrl.h uxtheme.h], [], [], [
	#if HAVE_WINDOWS_H
	#	include <windows.h>
	#endif
	])

# Check for c++11 compatible compiler
AC_LANG_CPLUSPLUS
AX_CXX_COMPILE_STDCXX_11([], \
                         [
                          AC_MSG_WARN([
****************************************************************
****** This library is designed for C++11. You may still *******
****** use it, however you will lack some usefool tools. *******
****** See library documentation or README to find which *******
****** tools depend on C++11.                            *******
****************************************************************
]) ])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])


LF_LINK_HEADERS([	src/WCppUtils		\
					src/EventSystem		\
					src/Window			\
					src/CommonControls	\
					src/GeneralControls	\
				])
LF_SET_INCLUDES


AC_CONFIG_FILES([	Makefile
					src/Makefile
					src/WCppUtils/Makefile
					src/EventSystem/Makefile
					src/Window/Makefile
					src/CommonControls/Makefile
					src/GeneralControls/Makefile
				])
AC_OUTPUT
