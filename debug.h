#ifndef _DEBUG_H_
#define _DEBUG_H_

#if defined(DEBUG_ON)
#include <string>

int ThrowAssert(std::string, int, std::string);
int DebugMsg(std::string);

#define ASSERT(x,m)   ((x)||ThrowAssert(__FILE__,__LINE__,(m)))
#define DEBUGMSG(m)   DebugMsg((m))
#else
#define ASSERT(x,m)
#define DEBUGMSG(m)
#endif

#endif // _DEBUG_H_ 