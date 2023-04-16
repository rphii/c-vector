//#define NDEBUG

#include "vec.h"

//VEC_IMPLEMENT(VecU8, vec_u8, unsigned char);
VEC_IMPLEMENT(VecSize, vec_size, size_t, BY_VAL, 0);
VEC_IMPLEMENT(Vec2Size, vec2_size, VecSize, BY_REF, vec_size_free);

