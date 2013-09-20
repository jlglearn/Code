#ifndef _QSORT3_H_
#define _QSORT3_H_
void QSort(int a[], int start, int end);
void QSortX(void *P, int size, int start, int end, float fn(void *P, int i));
#endif