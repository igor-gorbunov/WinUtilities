#pragma once

#include <Windows.h>

#pragma warning(push)
#pragma warning(disable: 4820)

typedef enum _CMDLINEOPT_ARGTYPE
{
	CMDLINEOPTARG_NONE = 0, CMDLINEOPTARG_REQUIRED, CMDLINEOPTARG_OPTIONAL
} CMDLINEOPT_ARGTYPE;

typedef struct _COMMANDLINEOPTION
{
	LPCTSTR ShortName;
	LPCTSTR LongName;
	CMDLINEOPT_ARGTYPE ArgType;
	LONG Value;
} COMMANDLINEOPTION;

typedef struct _GETCMDLINEOPTION_STATE
{
	UINT NmOfArguments;
	TCHAR **Arguments;
	UINT NmOfOptions;
	COMMANDLINEOPTION *Options;
	BOOL PrintOnError;
	UINT NextArgumentIdx;
	TCHAR *OptionArgument;
} GETCMDLINEOPTION_STATE;

#pragma warning(pop)

#if defined(_WINDLL_USERMODEAPILIBRARY)
#define LINK_USERMODEAPILIBRARY __declspec(dllexport)
#else
#define LINK_USERMODEAPILIBRARY __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	LINK_USERMODEAPILIBRARY HRESULT CmdLineInitialize(IN UINT NmOfArguments, IN TCHAR **Arguments, IN UINT NmOfOptions,
		IN COMMANDLINEOPTION *Options, IN BOOL PrintOnError, OUT GETCMDLINEOPTION_STATE *State);
	LINK_USERMODEAPILIBRARY HRESULT CmdLineGetNextOption(IN GETCMDLINEOPTION_STATE *State, OUT LONG *Value,
		OUT OPTIONAL UINT *ArgumentIndex, OUT OPTIONAL LPCTSTR *OptionArgument);

#ifdef __cplusplus
}
#endif
