#include <DebugOutput.h>

__declspec(dllexport) HRESULT OutputDebugFmtString(LPCTSTR Format, ...)
{
	static TCHAR OutputString[2048];	// TODO Dynamic allocation and expansion
	int BufferLength, StringLength;
	va_list _args;

	HRESULT Result;

	va_start(_args, Format);
	BufferLength = sizeof(OutputString) / sizeof(TCHAR);
	StringLength = _vsntprintf_s(OutputString, BufferLength, _TRUNCATE, Format, _args);
	if (-1 != StringLength)
	{
		OutputDebugString(OutputString);
		Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS);
	}
	else
	{
		Result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_BUFFER_OVERFLOW);
	}
	va_end(_args);

	return Result;
}

__declspec(dllexport) HRESULT OutputDebugHexDump(LPVOID DataBuffer)
{
	OutputDebugString(_T("OutputDebugHexDump(): not implemented.\n"));
	return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_CALL_NOT_IMPLEMENTED);
}
