#include <CommandLineUtilities.h>
#include <tchar.h>

static HRESULT CheckLongOption(IN GETCMDLINEOPTION_STATE *State, IN UINT Index,
	OUT LONG *Value, OUT OPTIONAL LPCTSTR *OptionArgument);
static HRESULT CheckShortOption(IN GETCMDLINEOPTION_STATE *State, IN UINT Index,
	OUT LONG *Value, OUT OPTIONAL LPCTSTR *OptionArgument);

__declspec(dllexport) HRESULT CmdLineInitialize(IN UINT NmOfArguments, IN TCHAR **Arguments, IN UINT NmOfOptions,
	IN COMMANDLINEOPTION *Options, IN BOOL PrintOnError, OUT GETCMDLINEOPTION_STATE *State)
{
	if (0 == NmOfArguments || NULL == Arguments)
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_INVALID_PARAMETER);
	if (0 == NmOfOptions || NULL == Options)
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_INVALID_PARAMETER);
	if (NULL == State)
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_INVALID_PARAMETER);

	State->NmOfArguments = NmOfArguments;
	State->Arguments = Arguments;
	State->NmOfOptions = NmOfOptions;
	State->Options = Options;
	State->PrintOnError = PrintOnError;
	State->NextArgumentIdx = 0;
	State->OptionArgument = NULL;

	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS);
}

__declspec(dllexport) HRESULT CmdLineGetNextOption(IN GETCMDLINEOPTION_STATE *State, OUT LONG *Value,
	OUT OPTIONAL UINT *ArgumentIndex, OUT OPTIONAL LPCTSTR *OptionArgument)
{
	UINT Index;
	HRESULT Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS);

	if (NULL == State || NULL == Value)
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, ERROR_INVALID_PARAMETER);

	if (State->NextArgumentIdx >= State->NmOfArguments)
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_EMPTY);

	//_tprintf(_T("CmdLineGetNextOption(): Next option is \"%s\".\n"), State->Arguments[State->NextOptionIdx]);

	if (NULL != ArgumentIndex)
		*ArgumentIndex = State->NextArgumentIdx;

	Index = 0;
	while (_istspace(State->Arguments[State->NextArgumentIdx][Index]))
		++Index;

	//_tprintf(_T("CmdLineGetNextOption(): Despaced argument \"%s\".\n"), State->Arguments[State->NextOptionIdx]);

	if (2 > _tcslen(&State->Arguments[State->NextArgumentIdx][Index]))
	{
		++State->NextArgumentIdx;
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_NOT_FOUND);
	}

	if (_T('-') == State->Arguments[State->NextArgumentIdx][Index]
		&& _T('-') == State->Arguments[State->NextArgumentIdx][Index + 1])
	{	// seems to be a long option
		Index += 2;
		Result = CheckLongOption(State, Index, Value, OptionArgument);
	}
	else if (_T('-') == State->Arguments[State->NextArgumentIdx][Index])
	{	// seems to be a short option
		Index += 1;
		Result = CheckShortOption(State, Index, Value, OptionArgument);
	}
	else
	{
		// seems not to be an option
		Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_NOT_FOUND);
	}

	++State->NextArgumentIdx;
	return Result;
}

static HRESULT CheckLongOption(IN GETCMDLINEOPTION_STATE *State, IN UINT Index,
	OUT LONG *Value, OUT OPTIONAL LPCTSTR *OptionArgument)
{
	BOOL OptionFound = FALSE;
	HRESULT Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS);

	for (UINT i = 0; i < State->NmOfOptions; i++)
	{
		size_t OptionLength;

		if (NULL == State->Options[i].LongName)
			continue;

		OptionLength = _tcslen(State->Options[i].LongName);
		if (0 < OptionLength && 0 == _tcsnccmp(&State->Arguments[State->NextArgumentIdx][Index], State->Options[i].LongName, OptionLength))
		{	// Option found, check the syntax
			//_tprintf(_T("CmdLineGetNextOption(): Found option \"%s\".\n"), State->Arguments[State->NextOptionIdx]);
			OptionFound = TRUE;
			*Value = State->Options[i].Value;
			Index += OptionLength;

			if (CMDLINEOPTARG_REQUIRED == State->Options[i].ArgType)
			{
				if (_T('=') == State->Arguments[State->NextArgumentIdx][Index])
				{
					if (NULL != OptionArgument)
						*OptionArgument = &State->Arguments[State->NextArgumentIdx][++Index];
				}
				else
				{
					Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_INCORRECT_SIZE);
				}
				break;
			}

			if (CMDLINEOPTARG_NONE == State->Options[i].ArgType)
			{
				if (_T('\0') == State->Arguments[State->NextArgumentIdx][Index])
				{
					if (NULL != OptionArgument)
						*OptionArgument = NULL;
				}
				else
				{
					Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_INCORRECT_SIZE);
				}
				break;
			}

			if (CMDLINEOPTARG_OPTIONAL == State->Options[i].ArgType)
			{
				if (_T('\0') == State->Arguments[State->NextArgumentIdx][Index])
				{
					if (NULL != OptionArgument)
						*OptionArgument = NULL;
				}
				else if (_T('=') == State->Arguments[State->NextArgumentIdx][Index])
				{
					if (NULL != OptionArgument)
						*OptionArgument = &State->Arguments[State->NextArgumentIdx][++Index];
				}
				else
				{
					Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_INCORRECT_SIZE);
				}
				break;
			}
		}
	}
	if (FALSE == OptionFound)
	{	// Option not found, user misspelled
		Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_NO_MATCH);
	}

	return Result;
}

static HRESULT CheckShortOption(IN GETCMDLINEOPTION_STATE *State, IN UINT Index,
	OUT LONG *Value, OUT OPTIONAL LPCTSTR *OptionArgument)
{
	BOOL OptionFound = FALSE;
	HRESULT Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_SUCCESS);

	for (UINT i = 0; i < State->NmOfOptions; i++)
	{
		size_t OptionLength;

		if (NULL == State->Options[i].ShortName)
			continue;

		OptionLength = _tcslen(State->Options[i].ShortName);
		if (0 < OptionLength && 0 == _tcsnccmp(&State->Arguments[State->NextArgumentIdx][Index], State->Options[i].ShortName, OptionLength))
		{	// Option found, check the syntax
			//_tprintf(_T("CmdLineGetNextOption(): Found option \"%s\".\n"), State->Arguments[State->NextOptionIdx]);
			OptionFound = TRUE;
			*Value = State->Options[i].Value;
			Index += OptionLength;

			if (CMDLINEOPTARG_REQUIRED == State->Options[i].ArgType)
			{
				if (_T('\0') == State->Arguments[State->NextArgumentIdx][Index])
				{
					++State->NextArgumentIdx;
					if (State->NextArgumentIdx >= State->NmOfArguments)
					{
						if (NULL != OptionArgument)
							*OptionArgument = NULL;
						Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_INCORRECT_SIZE);
					}
					else
					{
						if (NULL != OptionArgument)
							*OptionArgument = State->Arguments[State->NextArgumentIdx];
					}
				}
				else
				{
					Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_INCORRECT_SIZE);
				}
				break;
			}

			if (CMDLINEOPTARG_NONE == State->Options[i].ArgType)
			{
				if (_T('\0') == State->Arguments[State->NextArgumentIdx][Index])
				{
					if (NULL != OptionArgument)
						*OptionArgument = NULL;
				}
				else
				{
					Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_INCORRECT_SIZE);
				}
				break;
			}

			if (CMDLINEOPTARG_OPTIONAL == State->Options[i].ArgType)
			{	// Short option cannot have an optional argument - this would be ambigous
				Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_BAD_ARGUMENTS);
				break;
			}
		}
	}
	if (FALSE == OptionFound)
	{	// Option not found, user misspelled
		Result = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, ERROR_NO_MATCH);
	}

	return Result;
}

//#include <assert.h>
//#include <errno.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#ifndef __GETOPT_H__
//#define __GETOPT_H__
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//	int opterr;		/* if error message should be printed */
//	int optind;		/* index into parent argv vector */
//	int optopt;		/* character checked for validity */
//	int optreset;		/* reset getopt */
//	char *optarg;		/* argument associated with option */
//
//	struct option
//	{
//		const char *name;
//		int has_arg;
//		int *flag;
//		int val;
//	};
//
//#define no_argument       0
//#define required_argument 1
//#define optional_argument 2
//
//	int getopt(int, char**, char*);
//	int getopt_long(int, char**, char*, struct option*, int*);
//
//#ifdef __cplusplus
//}
//#endif
//
//#endif /* __GETOPT_H__ */
//
//#define __P(x) x
//#define _DIAGASSERT(x) assert(x)
//
//static char * __progname __P((char *));
//int getopt_internal __P((int, char * const *, const char *));
//
//static char * __progname(char * nargv0)
//{
//	char * tmp;
//
//	_DIAGASSERT(nargv0 != NULL);
//
//	tmp = strrchr(nargv0, '/');
//	if (tmp)
//		tmp++;
//	else
//		tmp = nargv0;
//	return(tmp);
//}
//
//#define	BADCH	(int)'?'
//#define	BADARG	(int)':'
//#define	EMSG	""
//
///*
//* getopt --
//*	Parse argc/argv argument vector.
//*/
//int getopt_internal(int nargc, char * const *nargv, const char *ostr)
//{
//	static char *place = EMSG;		/* option letter processing */
//	char *oli;				/* option letter list index */
//
//	_DIAGASSERT(nargv != NULL);
//	_DIAGASSERT(ostr != NULL);
//
//	if (optreset || !*place)
//	{		/* update scanning pointer */
//		optreset = 0;
//		if (optind >= nargc || *(place = nargv[optind]) != '-')
//		{
//			place = EMSG;
//			return (-1);
//		}
//		if (place[1] && *++place == '-')
//		{	/* found "--" */
//			/* ++optind; */
//			place = EMSG;
//			return (-2);
//		}
//	}					/* option letter okay? */
//	if ((optopt = (int)*place++) == (int)':' || !(oli = strchr(ostr, optopt)))
//	{
//		/*
//		* if the user didn't specify '-' as an option,
//		* assume it means -1.
//		*/
//		if (optopt == (int)'-')
//			return (-1);
//		if (!*place)
//			++optind;
//		if (opterr && *ostr != ':')
//			(void)fprintf(stderr, "%s: illegal option -- %c\n", __progname(nargv[0]), optopt);
//		return (BADCH);
//	}
//	if (*++oli != ':')
//	{			/* don't need argument */
//		optarg = NULL;
//		if (!*place)
//			++optind;
//	}
//	else
//	{				/* need an argument */
//		if (*place)			/* no white space */
//			optarg = place;
//		else if (nargc <= ++optind)
//		{	/* no arg */
//			place = EMSG;
//			if ((opterr) && (*ostr != ':'))
//				(void)fprintf(stderr, "%s: option requires an argument -- %c\n", __progname(nargv[0]), optopt);
//			return (BADARG);
//		}
//		else				/* white space */
//			optarg = nargv[optind];
//		place = EMSG;
//		++optind;
//	}
//	return (optopt);			/* dump back option letter */
//}
//
//#if 0
///*
//* getopt --
//*	Parse argc/argv argument vector.
//*/
//int getopt2(int nargc, char * const *nargv, const char *ostr)
//{
//	int retval;
//
//	if ((retval = getopt_internal(nargc, nargv, ostr)) == -2)
//	{
//		retval = -1;
//		++optind;
//	}
//	return(retval);
//}
//#endif
//
///*
//* getopt_long --
//*	Parse argc/argv argument vector.
//*/
//int getopt_long(int nargc, char ** nargv, char * options, struct option * long_options, int * index)
//{
//	int retval;
//
//	_DIAGASSERT(nargv != NULL);
//	_DIAGASSERT(options != NULL);
//	_DIAGASSERT(long_options != NULL);
//	/* index may be NULL */
//
//	if ((retval = getopt_internal(nargc, nargv, options)) == -2)
//	{
//		char *current_argv = nargv[optind++] + 2, *has_equal;
//		int i, current_argv_len, match = -1;
//
//		if (*current_argv == '\0')
//		{
//			return(-1);
//		}
//		if ((has_equal = strchr(current_argv, '=')) != NULL)
//		{
//			current_argv_len = has_equal - current_argv;
//			has_equal++;
//		}
//		else
//			current_argv_len = strlen(current_argv);
//
//		for (i = 0; long_options[i].name; i++)
//		{
//			if (strncmp(current_argv, long_options[i].name, current_argv_len))
//				continue;
//
//			if (strlen(long_options[i].name) == (unsigned)current_argv_len)
//			{
//				match = i;
//				break;
//			}
//			if (match == -1)
//				match = i;
//		}
//		if (match != -1)
//		{
//			if (long_options[match].has_arg == required_argument || long_options[match].has_arg == optional_argument)
//			{
//				if (has_equal)
//					optarg = has_equal;
//				else
//					optarg = nargv[optind++];
//			}
//			if ((long_options[match].has_arg == required_argument) && (optarg == NULL))
//			{
//				/*
//				* Missing argument, leading :
//				* indicates no error should be generated
//				*/
//				if ((opterr) && (*options != ':'))
//					(void)fprintf(stderr, "%s: option requires an argument -- %s\n", __progname(nargv[0]), current_argv);
//				return (BADARG);
//			}
//		}
//		else
//		{ /* No matching argument */
//			if ((opterr) && (*options != ':'))
//				(void)fprintf(stderr, "%s: illegal option -- %s\n", __progname(nargv[0]), current_argv);
//			return (BADCH);
//		}
//		if (long_options[match].flag)
//		{
//			*long_options[match].flag = long_options[match].val;
//			retval = 0;
//		}
//		else
//			retval = long_options[match].val;
//		if (index)
//			*index = match;
//	}
//	return(retval);
//}
