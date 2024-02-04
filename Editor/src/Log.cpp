#include "Log.h"

void Log::info(const char* message, ...) {
		va_list arglist;
		va_start( arglist, message );
		char buffer[1024];
		vsnprintf(buffer, 1024, message, arglist);
		Log::logfile << buffer << std::endl;
	}

void Log::error(const char* message, ...) {
    va_list arglist;
    va_start( arglist, message );
    char buffer[1024];
    vsnprintf(buffer, 1024, message, arglist);
    Log::logfile << RED << buffer << RESET << std::endl;
}

void Log::close() {
    Log::logfile.close();
}


const char* Log::filename("./log/editor.log");
std::ofstream Log::logfile{Log::filename, std::ios::app};