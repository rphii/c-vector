/* MIT License

Copyright (c) 2023 rphii

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#ifndef VEC_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include <stdio.h>

#ifndef vec_malloc
#define vec_malloc  malloc
#endif
#ifndef vec_realloc
#define vec_realloc realloc
#endif
#ifndef vec_memset
#define vec_memset  memset
#endif
#ifndef vec_memcpy
#define vec_memcpy  memcpy
#endif
#ifndef vec_memmove
#define vec_memmove memmove
#endif

/*
 * N = name - used for the type name
 * A = abbreviation - used for the functions
 * T = type - the actual type to base the vector off of
 * F = free - function to use if your structure itself has a custom free routine
 * M = mode - either BY_VAL or BY_REF
 */

//#define VEC_CONTEXT_FOREACH(T, F)     ((sizeof(T) > sizeof(void *)) || ((void *)F != (void *)NULL)) /* hoping the casting gets rid of -Waddress */
#define VEC_DEFAULT_SIZE    4
#define VEC_TYPE_FREE(F)    (void (*)(void *))(F)

typedef enum
{
    VEC_ERROR_NONE,
    /* errors below */
    VEC_ERROR_MALLOC,
    VEC_ERROR_REALLOC,
} VecErrorList;

#define VEC_ITEM_BY_VAL(T)  T
#define VEC_ITEM_BY_REF(T)  T *
#define VEC_ITEM(T, M)  VEC_ITEM_##M(T)

#define VEC_INCLUDE(N, A, T, M) \
    typedef struct N { \
        VEC_ITEM(T, M)*items; \
        size_t len; \
        size_t cap; \
        size_t first; \
    } N; \
    \
    /* common implementation */ \
    void A##_zero(N *vec); \
    void A##_recycle(N *vec); \
    size_t A##_length(N *vec); \
    int A##_resize(N *vec, size_t cap); \
    int A##_pop_front(N *vec, T *val); \
    int A##_pop_back(N *vec, T *val); \
    /* split implementation */ \
    void A##_free(N *vec); \
    size_t A##_reserved(N *vec); \
    int A##_reserve(N *vec, size_t cap); \
    int A##_shrink(N *vec); \
    void A##_realign(N *vec); \
    void A##_set_at(N *vec, VEC_ITEM(T, M) val, size_t index); \
    int A##_insert_at(N *vec, VEC_ITEM(T, M) val, size_t index); \
    int A##_push_front(N *vec, VEC_ITEM(T, M) val); \
    int A##_push_back(N *vec, VEC_ITEM(T, M) val); \
    VEC_ITEM(T, M) A##_get_at(N *vec, size_t index); \
    VEC_ITEM(T, M) A##_get_front(N *vec); \
    VEC_ITEM(T, M) A##_get_back(N *vec); \

/**********************************************************/
/* IMPLEMENTATION *****************************************/
/**********************************************************/

#define VEC_IMPLEMENT(N, A, T, M, F) \
    /* private */ \
    VEC_IMPLEMENT_COMMON_STATIC_F(N, A, T, F);      \
    VEC_IMPLEMENT_##M##_STATIC_GET(N, A, T, F);     \
    /* public */ \
    VEC_IMPLEMENT_COMMON_ZERO(N, A, T, F);          \
    VEC_IMPLEMENT_COMMON_RECYCLE(N, A, T, F);       \
    VEC_IMPLEMENT_COMMON_LENGTH(N, A, T, F);        \
    VEC_IMPLEMENT_COMMON_RESIZE(N, A, T, F);        \
    VEC_IMPLEMENT_COMMON_POP_FRONT(N, A, T, F);     \
    VEC_IMPLEMENT_COMMON_POP_BACK(N, A, T, F);      \
    VEC_IMPLEMENT_##M##_FREE(N, A, T, F);           \
    VEC_IMPLEMENT_##M##_RESERVED(N, A, T, F);       \
    VEC_IMPLEMENT_##M##_RESERVE(N, A, T, F);        \
    VEC_IMPLEMENT_##M##_SHRINK(N, A, T, F);         \
    VEC_IMPLEMENT_##M##_REALIGN(N, A, T, F);        \
    VEC_IMPLEMENT_##M##_SET_AT(N, A, T, F);         \
    VEC_IMPLEMENT_##M##_INSERT_AT(N, A, T, F);      \
    VEC_IMPLEMENT_##M##_PUSH_FRONT(N, A, T, F);     \
    VEC_IMPLEMENT_##M##_PUSH_BACK(N, A, T, F);      \
    VEC_IMPLEMENT_##M##_GET_AT(N, A, T, F);         \
    VEC_IMPLEMENT_##M##_GET_FRONT(N, A, T, F);      \
    VEC_IMPLEMENT_##M##_GET_BACK(N, A, T, F);       \

/**********************************************************/
/* PRIVATE FUNCTION IMPLEMENTATIONS ***********************/
/**********************************************************/

/* implementation for both */
#define VEC_IMPLEMENT_COMMON_STATIC_F(N, A, T, F) \
    static void (*A##_static_f)(void *) = F != 0 ? VEC_TYPE_FREE(F) : 0; \

/* implementation by val */
#define VEC_IMPLEMENT_BY_VAL_STATIC_GET(N, A, T, F) \
    static inline T *A##_static_get(N *vec, size_t index) \
    { \
        assert(vec); \
        assert(index < vec->len); \
        assert(index >= vec->first); \
        return &vec->items[index]; \
    }

/* implementation by ref */
#define VEC_IMPLEMENT_BY_REF_STATIC_GET(N, A, T, F) \
    static inline T *A##_static_get(N *vec, size_t index) \
    { \
        assert(vec); \
        assert(index < vec->len); \
        assert(index >= vec->first); \
        return vec->items[index]; \
    }

/**********************************************************/
/* PUBLIC FUNCTION IMPLEMENTATIONS ************************/
/**********************************************************/

/* implementation for both */
#define VEC_IMPLEMENT_COMMON_ZERO(N, A, T, F) \
    inline void A##_zero(N *vec) \
    { \
        assert(vec); \
        vec_memset(vec, 0, sizeof(*vec)); \
    }

#define VEC_IMPLEMENT_COMMON_RECYCLE(N, A, T, F) \
    inline void A##_recycle(N *vec) \
    { \
        assert(vec); \
        vec->first = 0; \
        vec->len = 0; \
    }

#define VEC_IMPLEMENT_COMMON_LENGTH(N, A, T, F) \
    inline size_t A##_length(N *vec) \
    { \
        assert(vec); \
        return vec->len - vec->first; \
    }

#define VEC_IMPLEMENT_COMMON_RESIZE(N, A, T, F) \
    int A##_resize(N *vec, size_t cap) \
    { \
        assert(vec); \
        int result = VEC_ERROR_NONE; \
        A##_realign(vec); \
        result |= result ?: A##_reserve(vec, cap); \
        result |= result ?: A##_shrink(vec); \
        return result; \
    }

#define VEC_IMPLEMENT_COMMON_POP_FRONT(N, A, T, F) \
    inline int A##_pop_front(N *vec, T *val) \
    { \
        assert(vec); \
        assert(vec->len > vec->first); \
        if(val) { \
            size_t index = vec->first; \
            T *item = A##_static_get(vec, index); \
            vec_memcpy(val, item, sizeof(T)); \
        } \
        vec->first++; \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_COMMON_POP_BACK(N, A, T, F) \
    inline int A##_pop_back(N *vec, T *val) \
    { \
        assert(vec); \
        assert(vec->len > vec->first); \
        if(val) { \
            size_t index = vec->len - 1; \
            T *item = A##_static_get(vec, index); \
            vec_memcpy(val, item, sizeof(T)); \
        } \
        vec->len--; \
        return A##_shrink(vec); \
    }

/* implementation by value */
#define VEC_IMPLEMENT_BY_VAL_FREE(N, A, T, F) \
    inline void A##_free(N *vec) \
    { \
        assert(vec); \
        if(F != 0) { \
            for(size_t i = 0; i < vec->cap; i++) { \
                A##_static_f(VEC_TYPE_FREE(&vec->items[i])); \
            } \
        } \
        free(vec->items); \
        A##_zero(vec); \
    }

#define VEC_IMPLEMENT_BY_VAL_RESERVED(N, A, T, F) \
    inline size_t A##_reserved(N *vec) \
    { \
        assert(vec); \
        size_t result = 0; \
        result += (sizeof(*vec->items) * vec->cap); \
        return result; \
    }

#define VEC_IMPLEMENT_BY_VAL_RESERVE(N, A, T, F) \
    inline int A##_reserve(N *vec, size_t cap) \
    { \
        assert(vec); \
        size_t len = vec->cap; \
        size_t required = vec->cap ? vec->cap : VEC_DEFAULT_SIZE;\
        while(required < cap) required *= 2; \
        if(required > vec->cap) { \
            void *temp = vec_realloc(vec->items, sizeof(*vec->items) * required); \
            if(!temp) return VEC_ERROR_REALLOC; \
            vec->items = temp; \
            vec_memset(&vec->items[len], 0, sizeof(*vec->items) * (required - len)); \
            vec->cap = required; \
        } \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_VAL_SHRINK(N, A, T, F) \
    int A##_shrink(N *vec) \
    { \
        assert(vec); \
        size_t cap = vec->cap; \
        size_t len = vec->len; \
        size_t required = vec->cap ? vec->cap : VEC_DEFAULT_SIZE;\
        while(required > len) required /= 2; \
        if(required * 2 < vec->cap) { \
            if(F != 0) { \
                for(size_t i = required; i < cap; i++) { \
                    A##_static_f(VEC_TYPE_FREE(&vec->items[i])); \
                } \
            } \
            void *temp = vec_realloc(vec->items, sizeof(*vec->items) * required); \
            if(!temp) return VEC_ERROR_REALLOC; \
            vec->items = temp; \
        } \
        vec->cap = required; \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_VAL_REALIGN(N, A, T, F) \
    void A##_realign(N *vec) \
    { \
        assert(vec); \
        T *item = A##_static_get(vec, 0); \
        if(F != 0) { \
            for(size_t i = 0; i < vec->first; i++) { \
                A##_static_f(VEC_TYPE_FREE(&vec->items[i])); \
            } \
        } \
        vec_memmove(item + vec->first, item, sizeof(T) * (vec->len - vec->first)); \
        vec->len -= vec->first; \
        vec->first = 0; \
    }

#define VEC_IMPLEMENT_BY_VAL_SET_AT(N, A, T, F) \
    void A##_set_at(N *vec, T val, size_t index) \
    { \
        assert(vec); \
        T *item = A##_static_get(vec, index + vec->first); \
        vec_memcpy(item, &val, sizeof(T)); \
    }

#define VEC_IMPLEMENT_BY_VAL_INSERT_AT(N, A, T, F) \
    int A##_insert_at(N *vec, T val, size_t index) \
    { \
        assert(vec); \
        int result = A##_reserve(vec, index + 1); \
        if(result) return result; \
        vec->len++; \
        T *item = A##_static_get(vec, index + vec->first); \
        vec_memmove(item + 1, item, sizeof(T) * (++vec->len - index)); \
        vec_memcpy(item, &val, sizeof(T)); \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_VAL_PUSH_FRONT(N, A, T, F) \
    inline int A##_push_front(N *vec, T val) \
    { \
        assert(vec); \
        int result = A##_reserve(vec, vec->len + 1); \
        if(result) return result; \
        size_t len = vec->len++ + vec->first; \
        T *item = A##_static_get(vec, vec->first); \
        vec_memmove(item + 1, item, sizeof(T) * len); \
        vec_memcpy(item, &val, sizeof(T)); \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_VAL_PUSH_BACK(N, A, T, F) \
    inline int A##_push_back(N *vec, T val) \
    { \
        assert(vec); \
        int result = A##_reserve(vec, vec->len + 1); \
        if(result) return result; \
        size_t index = vec->len++; \
        T *item = A##_static_get(vec, index); \
        vec_memcpy(item, &val, sizeof(T)); \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_VAL_GET_AT(N, A, T, F) \
    inline T A##_get_at(N *vec, size_t index) \
    { \
        assert(vec); \
        return *A##_static_get(vec, index + vec->first); \
    }

#define VEC_IMPLEMENT_BY_VAL_GET_FRONT(N, A, T, F) \
    inline T A##_get_front(N *vec) \
    { \
        assert(vec); \
        return *A##_static_get(vec, vec->first); \
    }

#define VEC_IMPLEMENT_BY_VAL_GET_BACK(N, A, T, F) \
    inline T A##_get_back(N *vec) \
    { \
        assert(vec); \
        return *A##_static_get(vec, vec->len - 1); \
    }

/* implementaiton by reference */
#define VEC_IMPLEMENT_BY_REF_FREE(N, A, T, F) \
    inline void A##_free(N *vec) \
    { \
        assert(vec); \
        for(size_t i = 0; i < vec->cap; i++) { \
            if(F != 0) { \
                A##_static_f(VEC_TYPE_FREE(vec->items[i])); \
            } \
            free(vec->items[i]); \
        } \
        free(vec->items); \
        A##_zero(vec); \
    }

#define VEC_IMPLEMENT_BY_REF_RESERVED(N, A, T, F) \
    inline size_t A##_reserved(N *vec) \
    { \
        assert(vec); \
        size_t result = 0; \
        result += (sizeof(**vec->items) * vec->cap); \
        result += (sizeof(*vec->items) * vec->cap); \
        return result; \
    }

#define VEC_IMPLEMENT_BY_REF_RESERVE(N, A, T, F) \
    inline int A##_reserve(N *vec, size_t cap) \
    { \
        assert(vec); \
        size_t len = vec->cap; \
        size_t required = vec->cap ? vec->cap : VEC_DEFAULT_SIZE;\
        while(required < cap) required *= 2; \
        if(required > vec->cap) { \
            void *temp = vec_realloc(vec->items, sizeof(*vec->items) * required); \
            if(!temp) return VEC_ERROR_REALLOC; \
            vec->items = temp; \
            vec_memset(&vec->items[len], 0, sizeof(*vec->items) * (required - len)); \
            for(size_t i = len; i < required; i++) { \
                vec->items[i] = vec_malloc(sizeof(**vec->items)); \
                if(!vec->items[i]) return VEC_ERROR_MALLOC; \
            } \
            vec->cap = required; \
        } \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_REF_SHRINK(N, A, T, F) \
    int A##_shrink(N *vec) \
    { \
        assert(vec); \
        size_t cap = vec->cap; \
        size_t len = vec->len; \
        size_t required = vec->cap ? vec->cap : VEC_DEFAULT_SIZE;\
        while(required > len) required /= 2; \
        if(required * 2 < vec->cap) { \
            for(size_t i = required; i < cap; i++) { \
                if(F != 0) { \
                    A##_static_f(VEC_TYPE_FREE(vec->items[i])); \
                } \
                free(vec->items[i]); \
            } \
            vec_memset(&vec->items[required], 0, sizeof(*vec->items) * (cap - required)); \
            void *temp = vec_realloc(vec->items, sizeof(*vec->items) * required); \
            if(!temp) return VEC_ERROR_REALLOC; \
            vec->items = temp; \
        } \
        vec->cap = required; \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_REF_REALIGN(N, A, T, F) \
    void A##_realign(N *vec) \
    { \
        assert(vec); \
        T *item = A##_static_get(vec, 0); \
        if(F != 0) { \
            for(size_t i = 0; i < vec->first; i++) { \
                A##_static_f(VEC_TYPE_FREE(vec->items[i])); \
            } \
        } \
        vec_memmove(item + vec->first, item, sizeof(T *) * (vec->len - vec->first)); \
        vec->len -= vec->first; \
        vec->first = 0; \
    }

#define VEC_IMPLEMENT_BY_REF_SET_AT(N, A, T, F) \
    void A##_set_at(N *vec, T *val, size_t index) \
    { \
        assert(vec); \
        T *item = A##_static_get(vec, index + vec->first); \
        vec_memcpy(item, val, sizeof(T)); \
    }

#define VEC_IMPLEMENT_BY_REF_INSERT_AT(N, A, T, F) \
    int A##_insert_at(N *vec, T *val, size_t index) \
    { \
        assert(vec); \
        assert(val); \
        int result = A##_reserve(vec, index + 1); \
        if(result) return result; \
        vec->len++; \
        T *item = A##_static_get(vec, index + vec->first); \
        vec_memmove(item + 1, item, sizeof(T *) * (++vec->len - index)); \
        vec_memcpy(item, val, sizeof(T)); \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_REF_PUSH_FRONT(N, A, T, F) \
    inline int A##_push_front(N *vec, T *val) \
    { \
        assert(vec); \
        int result = A##_reserve(vec, vec->len + 1); \
        if(result) return result; \
        size_t len = vec->len++ - vec->first; \
        T *item = A##_static_get(vec, vec->first); \
        vec_memmove(item + 1, item, sizeof(T *) * len); \
        vec_memcpy(item, val, sizeof(T)); \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_REF_PUSH_BACK(N, A, T, F) \
    inline int A##_push_back(N *vec, T *val) \
    { \
        assert(vec); \
        assert(val); \
        int result = A##_reserve(vec, vec->len + 1); \
        if(result) return result; \
        size_t index = vec->len++; \
        T *item = A##_static_get(vec, index); \
        vec_memcpy(item, val, sizeof(T)); \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_REF_GET_AT(N, A, T, F) \
    inline T *A##_get_at(N *vec, size_t index) \
    { \
        assert(vec); \
        return A##_static_get(vec, index + vec->first); \
    }

#define VEC_IMPLEMENT_BY_REF_GET_FRONT(N, A, T, F) \
    inline T *A##_get_front(N *vec) \
    { \
        assert(vec); \
        return A##_static_get(vec, vec->first); \
    }

#define VEC_IMPLEMENT_BY_REF_GET_BACK(N, A, T, F) \
    inline T *A##_get_back(N *vec) \
    { \
        assert(vec); \
        return A##_static_get(vec, vec->len - 1); \
    }

#define VEC_H
#endif

