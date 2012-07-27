/*
 * CommonControls.c
 *
 *  Created on: 9.4.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include <windows.h>
#include <tchar.h>

#include "CommonControlsInit.h"

typedef struct tagINITCOMMONCONTROLSEX {
  DWORD dwSize;
  DWORD dwICC;
} INITCOMMONCONTROLSEX, *LPINITCOMMONCONTROLSEX;

typedef BOOL (*_InitCommonControlsEx) (const LPINITCOMMONCONTROLSEX lpInitCtrls);

void InitCommCtl() {
	HMODULE hCommCtl = LoadLibrary(TEXT("commctl32.dll"));
	INITCOMMONCONTROLSEX iccex;

	_InitCommonControlsEx InitCommonControlsEx =
			(_InitCommonControlsEx) GetProcAddress(hCommCtl, "InitCommonControlsEx");

	if(InitCommonControlsEx) {
		iccex.dwICC = 0xffffffff; // Load everything
		iccex.dwSize = sizeof(iccex);
		InitCommonControlsEx(&iccex);
	}
}


