#pragma once

#include <fstream>
#include <stdarg.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"

struct Log {
	static const char* filename;
	static std::ofstream logfile;

	static void info(const char* message, ...);

	static void error(const char* message, ...);
	static void close();
};