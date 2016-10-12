#include "BinaryToHex.h"
#include <stdio.h>

HRESULT ProcessFile(LPCTSTR FileName)
{
    HANDLE InputFile, OutputFile;
    HRESULT Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS);
    BOOL ReadResult;
    TCHAR OutputFileName[MAX_PATH];
    UCHAR CharString[1024], ByteArray[16];
    DWORD Offset = 0, NmOfBytesToRead, NmOfBytesRead, NmOfBytesToWrite, NmOfBytesWritten;


    if (NULL == FileName)
        return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_INVALID_PARAMETER);

    // Open input file
    if (INVALID_HANDLE_VALUE == (InputFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, GetLastError());
    }

    // Create output file name and open the file
    _stprintf(OutputFileName, _T("%s.txt"), FileName);
    if (INVALID_HANDLE_VALUE == (OutputFile = CreateFile(OutputFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, GetLastError());
    }
        
    // Read input file into memory
    NmOfBytesToRead = 16, NmOfBytesRead = 0;
    ReadResult = ReadFile(InputFile, ByteArray, NmOfBytesToRead, &NmOfBytesRead, NULL);
    while (TRUE == ReadResult && 0 != NmOfBytesRead)
    {
        // While there is data available
        //      Print next 16 bytes into string buffer
        sprintf(CharString,
            "/* %08x */ 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x,"
            " 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x,\n",
            Offset, ByteArray[0], ByteArray[1], ByteArray[2], ByteArray[3], ByteArray[4],
            ByteArray[5], ByteArray[6], ByteArray[7], ByteArray[8], ByteArray[9], ByteArray[10],
            ByteArray[11], ByteArray[12], ByteArray[13], ByteArray[14], ByteArray[15]);
        //      Output to file
        NmOfBytesToWrite = (DWORD)strlen(CharString), NmOfBytesWritten = 0;
        if (FALSE == WriteFile(OutputFile, CharString, NmOfBytesToWrite, &NmOfBytesWritten, NULL))
            OutputDebugString(_T("ProcessFile: Call to WriteFile() failed.\n"));
        Offset += NmOfBytesToRead;
        ReadResult = ReadFile(InputFile, ByteArray, NmOfBytesToRead, &NmOfBytesRead, NULL);
    }

    if (FALSE == CloseHandle(InputFile))
        OutputDebugString(_T("ProcessFile: Call to CloseHandle() failed.\n"));
    if (FALSE == CloseHandle(OutputFile))
        OutputDebugString(_T("ProcessFile: Call to CloseHandle() failed.\n"));

    return Result;
}
