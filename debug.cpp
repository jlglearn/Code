#include <string>
#include <cstdlib>
#include <iostream>
#include "debug.h"

int ThrowAssert( std::string filename, int linenumber, std::string msg )
{         
    std::cerr << "Assertion failure in " << filename << " line: " << linenumber << ": " << msg << "\n";
    abort();
    return 0;
}

int DebugMsg( std::string msg )
{
    if (msg != "")
    {
        std::cerr << msg << "\n";
    }
    return 0;
}