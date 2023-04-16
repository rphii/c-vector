#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "vec.h"

size_t test_get(size_t *arr, size_t *val, size_t index)
{
    *val = arr[index];
    return 0;
}

size_t test(size_t index)
{
    size_t *testarr = malloc(sizeof(size_t) * 10);
    testarr[index] = 10;
    size_t val = 0;
    test_get(testarr, &val, 0);
    return val;
}


int main(void)
{
    printf("evaluate vec\n");
    VecSize vec = {0};
    vec_size_free(&vec);
    size_t val = 0;
    if(vec_size_reserve(&vec, 100ULL)) printf("failed reserving\n");
    printf("reserved %zu bytes\n", vec_size_reserved(&vec));
    vec_size_shrink(&vec);
    printf("reserved %zu bytes\n", vec_size_reserved(&vec));
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

