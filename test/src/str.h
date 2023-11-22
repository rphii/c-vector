#ifndef STR_H

#define STR_DEFAULT_SIZE 32

/* configuration, inclusion and de-configuration of vector */

#define VEC_SETTINGS_DEFAULT_SIZE STR_DEFAULT_SIZE
#define VEC_SETTINGS_KEEP_ZERO_END 1
#define VEC_SETTINGS_STRUCT_ITEMS s

#include "vec.h"
VEC_INCLUDE(Str, str, char, BY_VAL);

#undef VEC_SETTINGS_STRUCT_ITEMS
#undef VEC_SETTINGS_KEEP_ZERO_END
#undef VEC_SETTINGS_DEFAULT_SIZE

/* other functions */

int str_fmt(Str *str, char *format, ...);

#define STR_H
#endif

