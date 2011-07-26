/*
 * Copyright 2010-2011 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__

extern int tokenizeCommandLine(const char* _commandLine, char* _outBuffer, size_t& _outBufferSize, char** _outArgv, int _outMaxArgvs);

class CommandLine
{
public:
#if !defined(__linux__)
	CommandLine()
		: m_argc(__argc)
		, m_argv(__argv)
	{
	}
#endif // !defined(__linux__)

	CommandLine(int _argc, char const* const* _argv)
		: m_argc(_argc)
		, m_argv(_argv)
	{
	}

	const char* findOption(const char _short, const char* _long = NULL, int _numParams = 1)
	{
		const char* result = _findOption(_short, _long, _numParams);
//		printf("findArg '%c' \"%s\" \"%s\"\n", _short, _long?_long:"NULL", result?result:"NULL");
		return result;
	}

	bool hasArg(const char _short, const char* _long = NULL)
	{
		const char* arg = findOption(_short, _long, 0);
		return NULL != arg;
	}

	bool hasArg(const char* _long)
	{
		const char* arg = findOption('\0', _long, 0);
		return NULL != arg;
	}

	bool hasArg(const char*& _value, const char _short, const char* _long = NULL)
	{
		const char* arg = findOption(_short, _long, 1);
		_value = arg;
		return NULL != arg;
	}

	bool hasArg(int& _value, const char _short, const char* _long = NULL)
	{
		const char* arg = findOption(_short, _long, 1);
		if (NULL != arg)
		{
			_value = atoi(arg);
			return true;
		}
		
		return false;
	}

	bool hasArg(unsigned int& _value, const char _short, const char* _long = NULL)
	{
		const char* arg = findOption(_short, _long, 1);
		if (NULL != arg)
		{
			_value = atoi(arg);
			return true;
		}
		
		return false;
	}
	
	bool hasArg(bool& _value, const char _short, const char* _long = NULL)
	{
		const char* arg = findOption(_short, _long, 1);
		if (NULL != arg)
		{
			if ('0' == *arg || _stricmp(arg, "false") )
			{
				_value = false;
			}
			else if ('0' != *arg || _stricmp(arg, "true") )
			{
				_value = true;
			}
			
			return true;
		}
		
		return false;
	}

private:
	const char* _findOption(const char _short, const char* _long, int _numParams)
	{
		for (int ii = 0; ii < m_argc; ++ii)
		{
			const char* arg = m_argv[ii];
			if ('-' == *arg)
			{
				++arg;
				if (_short == *arg)
				{
					if (1 == strlen(arg) )
					{
						if (0 == _numParams)
						{
							return "";
						}
						else if (ii+_numParams < m_argc
							 && '-' != *m_argv[ii+1] )
						{
							return m_argv[ii+1];
						}

						return NULL;
					}
				}
				else if (NULL != _long
					 &&  '-' == *arg
					 &&  0 == _stricmp(arg+1, _long) )
				{
					if (0 == _numParams)
					{
						return "";
					}
					else if (ii+_numParams < m_argc
						 &&  '-' != *m_argv[ii+1] )
					{
						return m_argv[ii+1];
					}

					return NULL;
				}
			}
		}
		
		return NULL;
	}
	
	int m_argc;
	char const* const* m_argv;
};

#endif /// __COMMANDLINE_H__
