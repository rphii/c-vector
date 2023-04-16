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
    printf("%zu\n", test(0));


    printf("evaluate vec\n");
    VecSize vec = {0};
    vec_size_free(&vec);
    if(vec_size_reserve(&vec, 1ULL)) printf("failed reserving\n");
    printf("reserved %zu bytes\n", vec_size_reserved(&vec));
    vec_size_push_back(&vec, (void *)10);
    size_t val = 0;
    vec_size_get_at(&vec, &val, 0);
    printf("value is %zu\n", val);

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

