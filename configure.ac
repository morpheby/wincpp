#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([WinCpp],[0.0.2.0],[morpheby@gmail.com])

AC_SUBST([VERSION_4D], ["0.0.2.0"])
AC_DEFINE([PACKAGE_VERSION_NUM], [0,0,2,0],
	[Define to the version number (acceptable by rc or windres) of this package.])
AC_DEFINE([VERSION_NUM], PACKAGE_VERSION_NUM,
	[Version number (acceptable by rc or windres) of this package.])
AC_DEFINE([PACKAGE_IS_RELEASE], [0], [Define to 1 if package is for release.])

AC_DEFINE([PACKAGE_AUTHOR], ["Morpheby"], [Author of the package])

AC_DEFINE([PACKAGE_COPYRIGHT], ["Copyright (C) 2012 " PACKAGE_AUTHOR ". Licensed under GNU GPL"], [Copyright])

AC_DEFINE([DEBUG], [], [Use for building library with debug information])
AC_DEFINE([_DEBUG], [], [Use for building library with debug information])
AC_DEFINE([DBG_OUT(str)], [OutputDebugStringW(str)], [Outputs debugging data])

AC_DEFINE([UNICODE], [], [Necessary for library to be unicode])
AC_DEFINE([_UNICODE], [], [Necessary for library to be unicode])

AC_DEFINE([ISOLATION_AWARE_ENABLED], [0], [Necessary for isolating module dependencies])

AC_DEFINE([WINVER], [0x0501], [Define to the target windows version])
AC_DEFINE([WIN32_IE], [0x600], [Define to the target windows IE version])

AC_CANONICAL_TARGET
if [[ ${target_cpu} == x86_64 ]]; then
	rescomp_target=-Fpe-x86-64
	cxx_target=-m64
	manifest_arch=AMD64
else
	rescomp_target=-Fpe-i386
	cxx_target=-m32
	manifest_arch=x86
fi

AC_SUBST([MANIFEST_ARCH], "${manifest_arch}")

AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB
AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_PROG_SED
AM_PROG_AR
AC_CHECK_PROG(RESCOMP, windres, windres ${rescomp_target})
AC_PROG_INSTALL

CFLAGS="${CFLAGS} ${cxx_target}"
LDFLAGS="${LDFLAGS} ${cxx_target}"


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
AC_LANG([C++])
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



AC_CONFIG_FILES([	Makefile
					src/Makefile
					src/WCppUtils/Makefile
					src/EventSystem/Makefile
					src/Window/Makefile
					src/CommonControls/Makefile
					src/GeneralControls/Makefile
				])
AC_OUTPUT
