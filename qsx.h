#ifndef _QSX_H_
#define _QSX_H_

#ifdef INT64
#include <cstdlib>
#define INT int64_t
#else
#define INT int
#endif


void QSx(INT a[], INT start, INT end, double (*fnVal)(INT));

#endif