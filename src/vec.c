#include "vec.h"

//VEC_IMPLEMENT(VecU8, vec_u8, unsigned char);
VEC_IMPLEMENT(VecSize, vec_size, size_t, 0);
VEC_IMPLEMENT(Vec2Size, vec2_size, VecSize, vec_size_free);

