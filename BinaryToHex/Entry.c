#include "BinaryToHex.h"

#include <DebugOutput.h>
#include <CommandLineUtilities.h>

#include <tchar.h>

#pragma comment(lib, "usermodeapi")

#define NUMBER_OF_OPTIONS	2

#define COMMAND_HELP        0
#define COMMAND_VERSION     1

int _tmain(int argc, TCHAR **argv)
{
    HRESULT Result;
    UINT NmOfOptions = NUMBER_OF_OPTIONS;
    UINT FoundOption;
    LONG OptionResult;
    COMMANDLINEOPTION Options[NUMBER_OF_OPTIONS];
    GETCMDLINEOPTION_STATE State;
    LPCTSTR OptionArgument;
    LPCTSTR FileName = NULL;

    Options[0].ShortName = _T("h");
    Options[0].LongName = _T("help");
    Options[0].ArgType = CMDLINEOPTARG_NONE;
    Options[0].Value = COMMAND_HELP;

    Options[1].ShortName = _T("v");
    Options[1].LongName = _T("version");
    Options[1].ArgType = CMDLINEOPTARG_NONE;
    Options[1].Value = COMMAND_VERSION;

    if (IS_ERROR(Result = CmdLineInitialize((UINT)(argc - 1), ++argv, NmOfOptions, Options, FALSE, &State)))
        _tprintf(_T("Call to CmdLineInitialize() failed (Result = 0x%08X).\n"), Result);

    Result = CmdLineGetNextOption(&State, &OptionResult, &FoundOption, &OptionArgument);
    while (MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_EMPTY) != Result)
    {
        switch (Result)
        {
        case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS):
            //_tprintf(_T("Main: next option = '%s', option argument = '%s'.\n"),
            //	State.Arguments[FoundOption], OptionArgument);
            switch (OptionResult)
            {
            case COMMAND_HELP:
                // print help and exit
                exit(0);
                break;
            case COMMAND_VERSION:
                // print version and exit
                exit(0);
                break;
            default:
                break;
            }
            break;
        case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_NOT_FOUND):
            FileName = State.Arguments[FoundOption];
            break;
        case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_NO_MATCH):
            _tprintf(_T("Main: no such option = '%s'.\n"), State.Arguments[FoundOption]);
            break;
        case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_INCORRECT_SIZE):
            _tprintf(_T("Main: option syntax is incorrect = '%s'.\n"),
                State.Arguments[FoundOption]);
            break;
        case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_BAD_ARGUMENTS):
            _tprintf(_T("Main: usage of COMMANDLINEOPTION is incorrect = '%s'.\n"),
                State.Arguments[FoundOption]);
            break;
        default:
            break;
        }

        Result = CmdLineGetNextOption(&State, &OptionResult, &FoundOption, &OptionArgument);
    }

    if (NULL != FileName)
    {
        Result = ProcessFile(FileName);
        if (MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS) != Result)
            return 1;
    }
    
    return 0;
}
