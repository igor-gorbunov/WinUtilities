#include "DriverInstallUtility.h"
#include <DebugOutput.h>
#include <CommandLineUtilities.h>

#pragma warning(push)
#pragma warning(disable: 4820)

#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#pragma warning(pop)

#pragma comment(lib, "usermodeapi")

int WINAPI _tWinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PrevInstance, _In_ LPTSTR CmdLine, _In_ int ShowCmd)
{
	HRESULT Result;
	HANDLE StdInputHandle, StdOutputHandle, StdErrorHandle;
	LPWSTR *Arguments;
	UINT NmOfArguments, NmOfOptions = 3;
	UINT FoundOption;
	LONG OptionResult;
	COMMANDLINEOPTION Options[3];
	GETCMDLINEOPTION_STATE State;
	LPCTSTR OptionArgument;

	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(PrevInstance);
	UNREFERENCED_PARAMETER(CmdLine);
	UNREFERENCED_PARAMETER(ShowCmd);

	StdInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	StdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	StdErrorHandle = GetStdHandle(STD_ERROR_HANDLE);

	if (NULL == StdOutputHandle || NULL == StdErrorHandle)
	{
		if (TRUE == AttachConsole(ATTACH_PARENT_PROCESS))
		{
			if (NULL == StdOutputHandle)
			{
				if (NULL != (StdOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE)))
				{
					int OutputConHandle;

					OutputConHandle = _open_osfhandle((intptr_t)StdOutputHandle, _O_TEXT);
					*stdout = *_tfdopen(OutputConHandle, _T("w"));
					setvbuf(stdout, NULL, _IONBF, 0);
				}
			}
			if (NULL == StdErrorHandle)
			{
				if (NULL != (StdErrorHandle = GetStdHandle(STD_ERROR_HANDLE)))
				{
					int ErrorConHandle;

					ErrorConHandle = _open_osfhandle((intptr_t)StdErrorHandle, _O_TEXT);
					*stderr = *_tfdopen(ErrorConHandle, _T("w"));
					setvbuf(stderr, NULL, _IONBF, 0);
				}
			}
		}
	}

	if (FALSE == SetConsoleOutputCP(1200))
		_ftprintf(stderr, _T("DriverInstallUtility: Call to SetConsoleOutputCP() failed.\n"));
	if (FALSE == SetConsoleCP(1200))
		_ftprintf(stderr, _T("DriverInstallUtility: Call to SetConsoleCP() failed.\n"));

	_tprintf(_T("Command line string: %s\n"), GetCommandLine());
	Arguments = CommandLineToArgvW(GetCommandLine(), (int *)&NmOfArguments);
	for (UINT i = 0; i < NmOfArguments; i++)
		_tprintf(_T("Command line argument [%u]: %s\n"), i, Arguments[i]);
	
	Options[0].ShortName = _T("l");
	Options[0].LongName = _T("long");
	Options[0].ArgType = CMDLINEOPTARG_NONE;
	Options[0].Value = -1;

	Options[1].ShortName = NULL;
	Options[1].LongName = _T("unsigned");
	Options[1].ArgType = CMDLINEOPTARG_OPTIONAL;
	Options[1].Value = 0;

	Options[2].ShortName = _T("z");
	Options[2].LongName = _T("zephyr");
	Options[2].ArgType = CMDLINEOPTARG_REQUIRED;
	Options[2].Value = 1;

	if (IS_ERROR(Result = CmdLineInitialize(NmOfArguments, Arguments, NmOfOptions, Options, FALSE, &State)))
		_tprintf(_T("Call to CmdLineInitialize() failed (Result = 0x%08X).\n"), Result);

	Result = CmdLineGetNextOption(&State, &OptionResult, &FoundOption, &OptionArgument);
	while (MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_EMPTY) != Result)
	{
		switch (Result)
		{
		case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS):
			_tprintf(_T("Main: next option = '%s', option argument = '%s'.\n"),
				State.Arguments[FoundOption], OptionArgument);
			switch (OptionResult)
			{
			case -1:
				_tprintf(_T("\tOption value: LONG, argument not permitted.\n"));
				break;
			case 0:
				_tprintf(_T("\tOption value: UNSIGNED, argument is optional"));
				if (NULL == OptionArgument)
				{
					_tprintf(_T(", not supplied.\n"));
				}
				else
				{
					_tprintf(_T(" = '%s'.\n"), OptionArgument);
				}
				break;
			case 1:
				_tprintf(_T("\tOption value: ZEPHYR, argument is required = '%s'.\n"), OptionArgument);
				break;
			default:
				break;
			}
			break;
		case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_NOT_FOUND):
			_tprintf(_T("Main: not an option = '%s'.\n"), State.Arguments[FoundOption]);
			break;
		case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_NO_MATCH):
			_tprintf(_T("Main: option is not supported = '%s'.\n"), State.Arguments[FoundOption]);
			break;
		case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_INCORRECT_SIZE):
			_tprintf(_T("Main: option syntax is incorrect = '%s'.\n"), State.Arguments[FoundOption]);
			break;
		case MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_BAD_ARGUMENTS):
			_tprintf(_T("Main: usage of COMMANDLINEOPTION is incorrect = '%s'.\n"), State.Arguments[FoundOption]);
			break;
		default:
			break;
		}

		Result = CmdLineGetNextOption(&State, &OptionResult, &FoundOption, &OptionArgument);
	}

	//if (IS_ERROR(DrvInstall_Create(_T("DummyDevice"), NULL, _T("DummyDevice.sys"))))
	//{
	//	_ftprintf(stderr, _T("DriverInstallUtility: Call to DrvInstall_Create() failed.\n"));
	//	return 1;
	//}
	//if (IS_ERROR(DrvInstall_Start(_T("DummyDevice"))))
	//{
	//	_ftprintf(stderr, _T("DriverInstallUtility: Call to DrvInstall_Start() failed.\n"));
	//	return 1;
	//}
	if (IS_ERROR(DrvInstall_Stop(_T("DummyDevice"))))
	{
		_ftprintf(stderr, _T("DriverInstallUtility: Call to DrvInstall_Stop() failed.\n"));
		return 1;
	}
	if (IS_ERROR(DrvInstall_Remove(_T("DummyDevice"))))
	{
		_ftprintf(stderr, _T("DriverInstallUtility: Call to DrvInstall_Remove() failed.\n"));
		return 1;
	}

	if (FALSE == FreeConsole())
		_ftprintf(stderr, _T("DriverInstallUtility: Cannot free console.\n"));

	return 0;
}
