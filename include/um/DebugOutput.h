#pragma once

#include <Windows.h>
#include <tchar.h>

#if defined(_WINDLL_USERMODEAPILIBRARY)
#define LINK_USERMODEAPILIBRARY __declspec(dllexport)
#else
#define LINK_USERMODEAPILIBRARY __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	LINK_USERMODEAPILIBRARY HRESULT OutputDebugFmtString(LPCTSTR Format, ...);
	LINK_USERMODEAPILIBRARY HRESULT OutputDebugHexDump(LPVOID DataBuffer);

#ifdef __cplusplus
}
#endif
