#include <Windows.h>

BOOL WINAPI DllMain(_In_ HINSTANCE DllInstance, _In_ DWORD Reason, _In_ LPVOID Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		return TRUE;
	case DLL_PROCESS_DETACH:
		return TRUE;
	case DLL_THREAD_ATTACH:
		return TRUE;
	case DLL_THREAD_DETACH:
		return TRUE;
	default:
		return FALSE;
	}
}
