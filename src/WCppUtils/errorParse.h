
#include <string>

#define __WIDEN2(x) L##x
#define __WIDEN(x) __WIDEN2(x)
#define __WFILE__ __WIDEN(__FILE__)

/*
int ShowErrorInfoEx (DWORD dwErrorCode, BOOL bException, HWND hOwner, DWORD dwMBStyle, LPCWSTR lpString, ...);
#define ShowErrorInfo(dwErrorCode) ShowErrorInfoEx(dwErrorCode, FALSE, 0, MB_OK, L"Error!")
#define ShowExceptionInfo(dwErrorCode) ShowErrorInfoEx(dwErrorCode, TRUE, NULL, MB_OK, L"Error!")
void __CriticalErrorEx (DWORD dwErrorCode, DWORD dwSourceLine, LPWSTR lpSourceFile, BOOL bException, HWND hOwner); // Use ThrowCriticalErrorEx instead or specify __LINE__, __FILE__ for dwSourceLine and lpSourceFile
#define ThrowCriticalErrorEx(dwErrorCode, bException, hWndOwner) __CriticalErrorEx((dwErrorCode), (__LINE__), (__WFILE__), (bException), (hWndOwner))
#define ThrowCriticalError(dwErrorCode) ThrowCriticalErrorEx(dwErrorCode, FALSE, NULL)
#define ThrowCriticalException(dwErrorCode) ThrowCriticalErrorEx(dwErrorCode, TRUE, NULL)
*/

int _ShowAppError(const std::wstring& str, const std::wstring& name,
		int style, int line, const std::wstring& file);
#define ShowAppError(str, name, style) \
	_ShowAppError((str), (name), (style), (__LINE__), (__WFILE__))
