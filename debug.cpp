#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <iostream>
#include "debug.h"

int ThrowAssert( std::string filename, int linenumber, std::string msg )
{         
    std::cerr << "Assertion failure in " << filename << " line: " << linenumber << ": " << msg << "\n";
    abort();
    return 0;
}

int DebugMsg(const char * msg, ... )
{
	char buf[512];
	
    va_list arg_list;                                                           
    va_start(arg_list, msg);                                                 
    vsnprintf(buf, sizeof(buf), msg, arg_list) + 1;
    
    std::cerr << buf << "\n";
    return 0;
}