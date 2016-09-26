#pragma once

#pragma warning(push)
#pragma warning(disable: 4255 4668 4820)

#include <Windows.h>
#include <tchar.h>

#pragma warning(pop)

HRESULT DrvInstall_Create(IN LPCTSTR DriverName, IN OPTIONAL LPCTSTR DisplayName, IN LPCTSTR DriverFilePath);
HRESULT DrvInstall_Start(IN LPCTSTR DriverName);
HRESULT DrvInstall_Stop(IN LPCTSTR DriverName);
HRESULT DrvInstall_Remove(IN LPCTSTR DriverName);
