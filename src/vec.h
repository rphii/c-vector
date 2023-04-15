#ifndef VEC_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

//#include "err.h"

//#define THROW(fmt, ...)      do { printf(F("[ERROR]", BOLD FG_RD_B)" "F("%s:%d:%s", FG_WT_B)" "fmt"\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); goto error; } while(0)
#define THROW(fmt, ...)     goto error

/*
 * N = name - used for the type name
 * A = abbreviation - used for the functions
 * T = type - the actual type to base the vector off of
 */

#define VEC_CONTEXT_FOREACH(T, F)     ((sizeof(T) > sizeof(size_t)) || ((void *)F != (void *)NULL)) /* hoping the casting gets rid of -Waddress */
#define VEC_DEFAULT_SIZE    4

#define ERR_VEC_POINTER     "expected pointer to vector struct"
#define ERR_VEC_VALUE       "expected pointer to value"
#define ERR_VEC_RESERVE     "failed to reserve memory"


#define VEC_INCLUDE(N, A, T, F) \
    typedef struct N { \
        union { \
            T **items_p; \
            T *items; \
        }; \
        size_t len; \
        size_t cap; \
    } N; \
    \
    void A##_free(N *vec); \
    void A##_recycle(N *vec); \
    size_t A##_reserved(N *vec); \
    int A##_reserve(N *vec, size_t cap); \
    int A##_push_front(N *vec, T *val); \
    int A##_push_back(N *vec, T *val); \
    int A##_pop_front(N *vec, T *val); \
    int A##_pop_back(N *vec, T *val); \
    int A##_get_at(N *vec, T *val, size_t index); \
    int A##_get_front(N *vec, T *val); \
    int A##_get_back(N *vec, T *val); \


#define VEC_IMPLEMENT(N, A, T, F) \
    inline void A##_free(N *vec) \
    { \
        assert(vec && ERR_VEC_POINTER); \
        if(VEC_CONTEXT_FOREACH(T, F)) { \
            for(size_t i = 0; i < vec->cap; i++) { \
                free(vec->items_p[i]); \
            } \
        } \
        free(vec->items); \
        vec->cap = 0; \
        vec->len = 0; \
    } \
    \
    inline void A##_recycle(N *vec) \
    { \
        assert(vec && ERR_VEC_POINTER); \
        vec->len = 0; \
    } \
    \
    inline size_t A##_reserved(N *vec) \
    { \
        assert(vec && ERR_VEC_POINTER); \
        size_t result = 0; \
        if(VEC_CONTEXT_FOREACH(T, F)) { \
            result += (sizeof(*vec->items_p) * vec->cap); \
        } \
        result += (sizeof(*vec->items) * vec->cap); \
        return result; \
    } \
    \
    inline int A##_reserve(N *vec, size_t cap) \
    { \
        assert(vec && ERR_VEC_POINTER); \
        size_t len = vec->cap; /* TODO make sure not using vec->len is okay (recycle) */ \
        size_t required = vec->cap ? vec->cap : VEC_DEFAULT_SIZE;\
        while(required < cap) required *= 2; \
        if(required > vec->cap) { \
            printf("required %zu\n", required); \
            if(VEC_CONTEXT_FOREACH(T, F)) { \
                void *temp = realloc(vec->items_p, sizeof(*vec->items_p) * required); \
                if(!temp) THROW("failed to allocate memory for table (%zu)", required); \
                vec->items_p = temp; \
                memset(&vec->items_p[len], 0, sizeof(vec->items_p[vec->cap]) * (required - len)); \
                for(size_t i = len; i < required; i++) { \
                    vec->items_p[len] = malloc(sizeof(**vec->items_p)); \
                    if(!vec->items_p[len]) THROW(""); \
                } \
            } else { \
                void *temp = realloc(vec->items, sizeof(*vec->items) * required); \
                if(!temp) THROW("failed to allocate memory for table (%zu)", required); \
                vec->items = temp; \
                memset(&vec->items[len], 0, sizeof(vec->items[vec->cap]) * (required - len)); \
            } \
            vec->cap = required; \
        } \
        return 0; \
    error: \
        return -1; \
    } \
    \
    inline int A##_push_back(N *vec, T *v) \
    { \
        assert(vec && ERR_VEC_POINTER); \
        if(A##_reserve(vec, vec->len + 1)) THROW(""); \
        if(VEC_CONTEXT_FOREACH(T, F)) { \
            assert(v && ERR_VEC_VALUE); \
        } \
        return 0; \
    error: \
        return -1; \
    } \
    \


//VEC_INCLUDE(VecU8, vec_u8, unsigned char, 0);
VEC_INCLUDE(VecSize, vec_size, size_t, 0);
VEC_INCLUDE(Vec2Size, vec2_size, VecSize, vec_size_free);

#define VEC_H
#endif
