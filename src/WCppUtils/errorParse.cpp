
#include "errorParse.h"

#if HAVE_WINDOWS_H
#include <windows.h>
#endif

#include <sstream>

using namespace std;

const wchar_t * errorFormat = L"%s\nError code: %lX\n%s%s",
    * errorString = L"Error",
    * errorPrefix = L"An error occurred!",
    * critErrorFmt = L"Critical error. Line %d,\nFile %s.\nError code: %lX\nAdd. info:\n%s";

int ShowErrorInfoEx (DWORD dwErrorCode, BOOL bException, HWND hOwner,  DWORD dwMBStyle, LPCWSTR formatStr, ...)
{
	// TODO write C++ streams error parser
	/*
	LPWSTR errDescription = 0;
	FormatMessage((bException?FORMAT_MESSAGE_FROM_HMODULE:FORMAT_MESSAGE_FROM_SYSTEM)|FORMAT_MESSAGE_ALLOCATE_BUFFER, GetModuleHandle(0), dwErrorCode, 0, (LPWSTR) &errDescription, 0, 0);
	if(!formatStr)
		formatStr = errorPrefix;

	va_list args;
	va_start(args, formatStr);
	int userFmtStrSz = 0;
	userFmtStrSz = _vscwprintf(formatStr, args);
	va_end(args);

	int fmtSz = snwprintf(0, 0, errorFormat, formatStr, dwErrorCode, errDescription, L"") + 1 + userFmtStrSz + 1;

	LPWSTR formattedResult = (LPWSTR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, fmtSz*2);

	wchar_t *formattedErr = (wchar_t*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, userFmtStrSz*2+2);
	va_start(args, formatStr);
	vsnwprintf(formattedErr, userFmtStrSz, formatStr, args);
	va_end(args);
	snwprintf(formattedResult, fmtSz, errorFormat, formattedErr, dwErrorCode, errDescription, L"");
	HeapFree(GetProcessHeap(), 0, formattedErr);
	LocalFree(errDescription);

	MessageBeep (MB_ICONERROR);
	fmtSz = MessageBox(hOwner ? : GetActiveWindow(), formattedResult, errorString, dwMBStyle);
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, formattedResult);
	return fmtSz;
	*/
}

void __CriticalErrorEx (DWORD dwErrorCode, DWORD dwSourceLine, LPWSTR lpSourceFile, BOOL bException, HWND hOwner) //WARNING: Not usable directly in code. Use CriticalErrorEx instead
{
/*
	LPWSTR errDescription = 0;
	FormatMessage((bException?FORMAT_MESSAGE_FROM_HMODULE:FORMAT_MESSAGE_FROM_SYSTEM)|FORMAT_MESSAGE_ALLOCATE_BUFFER, GetModuleHandle(0), dwErrorCode, 0, (LPWSTR) &errDescription, 0, 0);
	int size = snwprintf(0, 0, critErrorFmt, dwSourceLine, lpSourceFile, dwErrorCode, errDescription) + 1;

	LPWSTR formattedErr = (LPWSTR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size*2);
	_snwprintf(formattedErr, size, critErrorFmt, dwSourceLine, lpSourceFile, dwErrorCode, errDescription);
	MessageBeep (MB_ICONERROR);
	LocalFree(errDescription);
	FatalAppExit(0, formattedErr);
	return;
	*/
}

int _ShowAppError(const std::wstring& str, const std::wstring& name,
		int style, int line, const std::wstring& file) {
	std::wstringstream ss;
	ss	<< L"Application error" << endl
#ifdef DEBUG
		<< L"Line: " << line << endl
		<< L"File: " << file << endl
#endif
		<< str;
	MessageBeep(style);
	return MessageBox(GetActiveWindow(), ss.str().c_str(), name.c_str(), style);
}

