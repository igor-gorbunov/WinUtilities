#include "BinaryToHex.h"

HRESULT ProcessFile(LPCTSTR FileName)
{
    HRESULT Result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_CALL_NOT_IMPLEMENTED);

    if (NULL == FileName)
        return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_INVALID_PARAMETER);

    return Result;
}