/*
 * utf8.cpp
 *
 *  Created on: 24.05.2012
 *      Author: Morphe
 */

#include "utf8.h"
#include <sstream>
#include <stdlib.h>

using namespace std;

string utf8Str(const wstring& str) {
	size_t sz = (str.size() + 1) * sizeof(wchar_t);
	char *utf8str = new char[sz];
	if(wcstombs(utf8str, str.c_str(), sz) == sz)
		utf8str[sz-1] = 0;
	string retStr(utf8str);
	delete[] utf8str;

	return retStr;
}

wstring wideStr(const string& str) {
	size_t sz = str.size() + 1;
	wchar_t *buf = new wchar_t [sz];
	sz = mbstowcs (buf, str.c_str(), sz);

	wstring retStr(buf);
	delete[] buf;

	return retStr;
}
