#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "vec.h"

VEC_INCLUDE(VecSize, vec_size, size_t, BY_VAL);
VEC_INCLUDE(Vec2Size, vec2_size, VecSize, BY_REF);

VEC_IMPLEMENT(VecSize, vec_size, size_t, BY_VAL, 0);
VEC_IMPLEMENT(Vec2Size, vec2_size, VecSize, BY_REF, vec_size_free);

int main(void)
{
    printf("evaluate vec\n");
    VecSize vec = {0};
    vec_size_free(&vec);
    size_t val = 0;
    if(vec_size_reserve(&vec, 1ULL)) printf("failed reserving\n");
    printf("reserved %zu bytes\n", vec_size_reserved(&vec));
    vec_size_shrink(&vec);
    printf("reserved %zu bytes\n", vec_size_reserved(&vec));
#if 0
    for(size_t i = 0; i < 100; i++) {
        vec_size_push_back(&vec, i);
    }
    for(size_t i = 0; i < 100; i++) {
        val = vec_size_get_at(&vec, i);
        printf("at vec[%zu] : %zu\n", i, val);
    }
    for(size_t i = 0; i < 100; i++) {
        vec_size_pop_front(&vec, &val);
        printf("pop vec[%zu] : %zu\n", i, val);
    }
    printf("reserved %zu bytes\n", vec_size_reserved(&vec));
#endif

    printf("evaluate vec2\n");
    Vec2Size vec2 = {0};
    vec2_size_free(&vec2);
    if(vec2_size_reserve(&vec2, 1ULL)) printf("failed reserving\n");
    printf("reserved %zu bytes / sizeof item %zu\n", vec2_size_reserved(&vec2), sizeof(VecSize));


    printf("done\n");
    return 0;
}

