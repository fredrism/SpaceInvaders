#pragma once
#include "glad/glad.h"
#include <string>
#include <iostream>
#include <regex>

inline void glCheckError(const char* file_cstr, const char* func, unsigned int line)
{
	unsigned int error = glGetError();
	while (error != GL_NO_ERROR)
	{
		std::string file = file_cstr;
		std::regex token("\\\\+");
		std::sregex_token_iterator iter(file.begin(), file.end(), token, -1);
		std::sregex_token_iterator end;
		std::vector<std::string> tokens(iter, end);

		std::string message;

		switch (error)
		{
		case GL_INVALID_ENUM: message = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: message = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: message = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW: message = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: message = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: message = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: message = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}

		std::cout << tokens.back() << " : " << line << " : " << func << " -> " << message << std::endl;
		std::cout << "Press any key to resume..." << std::endl;
		std::cin.ignore();
	}
}

#ifdef _DEBUG
	#define GLDEBUG(x) x; glCheckError(__FILE__, #x, __LINE__)
#else
	#define GLDEBUG(x) x
#endif