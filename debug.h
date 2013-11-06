#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifndef RELEASE
#include <string>

int ThrowAssert(std::string, int, std::string);
int DebugMsg(const char*, ...);

#define ASSERT(x,m)   ((x)||ThrowAssert(__FILE__,__LINE__,(m)))
#define DEBUGMSG      DebugMsg

#else

#define ASSERT(x,m)
#define DEBUGMSG(...)

#endif

#endif // _DEBUG_H_ 