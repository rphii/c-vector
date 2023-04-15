#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "vec.h"

int main(void)
{

    printf("evaluate vec\n");
    VecSize vec = {0};
    vec_size_free(&vec);
    if(vec_size_reserve(&vec, 1ULL)) printf("failed reserving\n");
    printf("reserved %zu bytes\n", vec_size_reserved(&vec));
    vec_size_push_back(&vec, (size_t *)10);

#if 0
    printf("evaluate vec2\n");
    Vec2Size vec2 = {0};
    vec2_size_free(&vec2);
    if(vec2_size_reserve(&vec2, 1ULL)) printf("failed reserving\n");
    printf("reserved %zu bytes\n", vec2_size_reserved(&vec2));
#endif


    printf("done\n");
    return 0;
}

