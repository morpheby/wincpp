/*
 * main.cpp
 *
 *  Created on: 13.06.2012
 *      Author: Morphe
 */

/**
 * Initializes library
 * and defines all export functions
 */

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#if HAVE_WINDOWS_H
#	include <windows.h>
#endif

#include <CommonControlsInit.h>

BOOLEAN DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved) {
	BOOLEAN bSuccess = 1;
//	ShowAppError(wideStr("test error"), L"error", MB_OK);
	switch (nReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( hDllHandle );
		InitCommCtl();
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return bSuccess;

}
