#pragma once



#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct RTSARG {
	RTSARG(std::string cinResult) {
		str2argv((cinResult + "\n").c_str());
	}

	char** GetArgv() noexcept {
		return _argv;
	}

	int GetArgc() const noexcept {
		return _argc;
	}

private:
	static const short ARGV_MAX = 255;
	static const short ARGV_TOKEN_MAX = 255;

	int    _argc;
	char  *_argv[ARGV_MAX];
	char  *_argv_token;

	void argv_init()
	{
		_argc = 0;
		if ((_argv_token = (char*)calloc(ARGV_TOKEN_MAX, sizeof(char))) == NULL) {
			printf("argv_init: failed to calloc\n");
			return;
		}
		memset(_argv_token, 0, ARGV_TOKEN_MAX * sizeof(char));
	}

	
	void argv_token_addch(int c)
	{
		auto n = strlen(_argv_token);
		if (n == ARGV_TOKEN_MAX - 1) {
			printf("argv_token_addch: reached max token length (%i)\n", ARGV_TOKEN_MAX);
			return;
		}

		_argv_token[n] = c;
	}

	
	void argv_token_finish()
	{
		if (_argc == ARGV_MAX) {
			printf("argv_token_finish: reached max argv length (%i)\n", ARGV_MAX);
			return;
		}

		
		_argv[_argc++] = _argv_token;
		if ((_argv_token = (char*)calloc(ARGV_TOKEN_MAX, sizeof(char))) == NULL) {
			printf("argv_token_finish: failed to calloc\n");
		}
		memset(_argv_token, 0, ARGV_TOKEN_MAX * sizeof(char));
	}

	
	void str2argv(const char *s)
	{
		bool in_token;
		bool in_container;
		bool escaped;
		char container_start;
		char c;
		int  i;

		container_start = 0;
		in_token = false;
		in_container = false;
		escaped = false;

		auto len = strlen(s);

		argv_init();
		for (i = 0; i < len; i++) {
			c = s[i];

			switch (c) {
				
			case ' ':
			case '\t':
			case '\n':
				if (!in_token)
					continue;

				if (in_container) {
					argv_token_addch(c);
					continue;
				}

				if (escaped) {
					escaped = false;
					argv_token_addch(c);
					continue;
				}

				
				in_token = false;
				argv_token_finish();
				break;

				
			case '\'':
			case '\"':

				if (escaped) {
					argv_token_addch(c);
					escaped = false;
					continue;
				}

				if (!in_token) {
					in_token = true;
					in_container = true;
					container_start = c;
					continue;
				}

				if (in_container) {
					if (c == container_start) {
						in_container = false;
						in_token = false;
						argv_token_finish();
						continue;
					}
					else {
						argv_token_addch(c);
						continue;
					}
				}

				/* XXX in this case, we:
				 *    1. have a quote
				 *    2. are in a token
				 *    3. and not in a container
				 * e.g.
				 *    hell"o
				 *
				 * what's done here appears shell-dependent,
				 * but overall, it's an error.... i *think*
				 */
				printf("Parse Error! Bad quotes\n");
				break;

			case '\\':

				if (in_container && s[i + 1] != container_start) {
					argv_token_addch(c);
					continue;
				}

				if (escaped) {
					argv_token_addch(c);
					continue;
				}

				escaped = true;
				break;

			default:
				if (!in_token) {
					in_token = true;
				}

				argv_token_addch(c);
			}
		}

		if (in_container)
			printf("Parse Error! Still in container\n");

		if (escaped)
			printf("Parse Error! Unused escape (\\)\n");
	}
};
