#ifndef UTIL_H
#define UTIL_H

#include <gc/gc.h>

#define FLOOR(x) ((int) (x))

#define LEN(a) (sizeof (a) / sizeof *(a))

#define NEW_N(v, n) ((v) = GC_MALLOC((n) * sizeof *(v)))
#define NEW(v) (NEW_N((v), 1))

#endif
