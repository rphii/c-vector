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

typedef enum
{
    VEC_ERROR_NONE,
    /* errors below */
    VEC_ERROR_MALLOC,
    VEC_ERROR_REALLOC,
} VecError;

#define VEC_ITEM_BY_VAL(T)  T
#define VEC_ITEM_BY_REF(T)  T *
#define VEC_ITEM(T, M)  VEC_ITEM_##M(T)

#define VEC_INCLUDE(N, A, T, F, M) \
    typedef struct N { \
        VEC_ITEM(T, M)*items; \
        size_t len; \
        size_t cap; \
        size_t first; \
    } N; \
    \
    /* common implementation */ \
    void A##_recycle(N *vec); \
    size_t A##_length(N *vec); \
    int A##_resize(N *vec, size_t cap); \
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
    int A##_pop_front(N *vec, T *val); \
    int A##_pop_back(N *vec, T *val); \
    VEC_ITEM(T, M) A##_get_at(N *vec, size_t index); \
    VEC_ITEM(T, M) A##_get_front(N *vec); \
    VEC_ITEM(T, M) A##_get_back(N *vec); \

/**********************************************************/
/* IMPLEMENTATION *****************************************/
/**********************************************************/

#define VEC_IMPLEMENT(N, A, T, F, M)      VEC_IMPLEMENT_##M(N, A, T, F)

/* implementaion by value and by reference */
#define VEC_IMPLEMENT_COMMON(N, A, T, F)    \
    VEC_IMPLEMENT_COMMON_RECYCLE(N, A, T, F);        \
    VEC_IMPLEMENT_COMMON_LENGTH(N, A, T, F);         \
    VEC_IMPLEMENT_COMMON_RESIZE(N, A, T, F);         \
    VEC_IMPLEMENT_COMMON_POP_FRONT(N, A, T, F);      \
    VEC_IMPLEMENT_COMMON_POP_BACK(N, A, T, F);       \

/* implementation by value */
#define VEC_IMPLEMENT_BY_VAL(N, A, T, F);   \
    VEC_IMPLEMENT_BY_VAL_STATIC_GET(N, A, T, F);     \
    VEC_IMPLEMENT_COMMON(N, A, T, F);                \
    VEC_IMPLEMENT_BY_VAL_FREE(N, A, T, F);           \
    VEC_IMPLEMENT_BY_VAL_RESERVED(N, A, T, F);       \
    VEC_IMPLEMENT_BY_VAL_RESERVE(N, A, T, F);        \
    VEC_IMPLEMENT_BY_VAL_SHRINK(N, A, T, F);         \
    VEC_IMPLEMENT_BY_VAL_REALIGN(N, A, T, F);        \
    VEC_IMPLEMENT_BY_VAL_SET_AT(N, A, T, F);         \
    VEC_IMPLEMENT_BY_VAL_INSERT_AT(N, A, T, F);      \
    VEC_IMPLEMENT_BY_VAL_PUSH_FRONT(N, A, T, F);     \
    VEC_IMPLEMENT_BY_VAL_PUSH_BACK(N, A, T, F);      \
    VEC_IMPLEMENT_BY_VAL_GET_AT(N, A, T, F);         \
    VEC_IMPLEMENT_BY_VAL_GET_FRONT(N, A, T, F);      \
    VEC_IMPLEMENT_BY_VAL_GET_BACK(N, A, T, F);       \

/* implementation by reference */
#define VEC_IMPLEMENT_BY_REF(N, A, T, F);   \
    VEC_IMPLEMENT_BY_REF_STATIC_GET(N, A, T, F);     \
    VEC_IMPLEMENT_COMMON(N, A, T, F);                \
    VEC_IMPLEMENT_BY_REF_FREE(N, A, T, F);           \
    VEC_IMPLEMENT_BY_REF_RESERVED(N, A, T, F);       \
    VEC_IMPLEMENT_BY_REF_RESERVE(N, A, T, F);        \
    VEC_IMPLEMENT_BY_REF_SHRINK(N, A, T, F);         \
    VEC_IMPLEMENT_BY_REF_REALIGN(N, A, T, F);        \
    VEC_IMPLEMENT_BY_REF_SET_AT(N, A, T, F);         \
    VEC_IMPLEMENT_BY_REF_INSERT_AT(N, A, T, F);      \
    VEC_IMPLEMENT_BY_REF_PUSH_FRONT(N, A, T, F);     \
    VEC_IMPLEMENT_BY_REF_PUSH_BACK(N, A, T, F);      \
    VEC_IMPLEMENT_BY_REF_GET_AT(N, A, T, F);         \
    VEC_IMPLEMENT_BY_REF_GET_FRONT(N, A, T, F);      \
    VEC_IMPLEMENT_BY_REF_GET_BACK(N, A, T, F);       \

/**********************************************************/
/* PUBLIC FUNCTION IMPLEMENTATIONS ************************/
/**********************************************************/

/* implementation for both */
#define VEC_IMPLEMENT_COMMON_RECYCLE(N, A, T, F) \
    inline void A##_recycle(N *vec) \
    { \
        assert(vec); \
        vec->len = 0; \
    }

#define VEC_IMPLEMENT_COMMON_LENGTH(N, A, T, F) \
    inline size_t A##_length(N *vec) \
    { \
        assert(vec); \
        return vec->len; \
    }

#define VEC_IMPLEMENT_COMMON_RESIZE(N, A, T, F) \
    int A##_resize(N *vec, size_t cap) \
    { \
        assert(vec); \
        int result = VEC_ERROR_NONE; \
        result |= result ?: A##_reserve(vec, cap); \
        A##_realign(vec); \
        result |= result ?: A##_shrink(vec); \
        return result; \
    }

#define VEC_IMPLEMENT_COMMON_POP_FRONT(N, A, T, F) \
    inline int A##_pop_front(N *vec, T *val) \
    { \
        assert(vec); \
        assert(val); \
        assert(vec->len > 0); \
        size_t index = vec->first; \
        T *item = A##_static_get(vec, index); \
        vec->first++; \
        vec_memcpy(val, item, sizeof(T)); \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_COMMON_POP_BACK(N, A, T, F) \
    inline int A##_pop_back(N *vec, T *val) \
    { \
        assert(vec); \
        assert(val); \
        assert(vec->len > 0); \
        size_t index = vec->len - 1; \
        T *item = A##_static_get(vec, index); \
        vec->len--; \
        vec_memcpy(val, item, sizeof(T)); \
        return A##_shrink(vec); \
    }

/* implementation by value */
#define VEC_IMPLEMENT_BY_VAL_STATIC_GET(N, A, T, F) \
    static inline T *A##_static_get(N *vec, size_t index) \
    { \
        assert(vec); \
        assert(index < vec->len); \
        assert(index >= vec->first); \
        return &vec->items[index]; \
    }

#define VEC_IMPLEMENT_BY_VAL_FREE(N, A, T, F) \
    inline void A##_free(N *vec) \
    { \
        assert(vec); \
        free(vec->items); \
        vec->items = 0; \
        vec->cap = 0; \
        vec->len = 0; \
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
            vec_memset(&vec->items[len], 0, sizeof(vec->items[vec->cap]) * (required - len)); \
            vec->cap = required; \
        } \
        return VEC_ERROR_NONE; \
    }

#define VEC_IMPLEMENT_BY_VAL_SHRINK(N, A, T, F) \
    int A##_shrink(N *vec) \
    { \
        assert(vec); \
        size_t len = vec->len; \
        size_t required = vec->cap ? vec->cap : VEC_DEFAULT_SIZE;\
        while(required > len) required /= 2; \
        if(required * 2 < vec->cap) { \
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
        memmove(item + vec->first, item, sizeof(T) * (vec->len - vec->first)); \
        vec->len -= vec->first; \
    }

#define VEC_IMPLEMENT_BY_VAL_SET_AT(N, A, T, F) \
    void A##_set_at(N *vec, T val, size_t index) \
    { \
        assert(vec); \
        T *item = A##_static_get(vec, index); \
        memcpy(item, &val, sizeof(T)); \
    }

#define VEC_IMPLEMENT_BY_VAL_INSERT_AT(N, A, T, F) \
    int A##_insert_at(N *vec, T val, size_t index) \
    { \
        assert(vec); \
        int result = A##_reserve(vec, index + 1); \
        if(result) return result; \
        T *item = A##_static_get(vec, index); \
        vec_memmove(item + 1, item, sizeof(T) * (++vec->len - index)); \
        memcpy(item, &val, sizeof(T)); \
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
        T *item = A##_static_get(vec, index); \
        return *item; \
        /*return *A##_static_get(vec, index);*/ \
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
#define VEC_IMPLEMENT_BY_REF_STATIC_GET(N, A, T, F) \
    static inline T *A##_static_get(N *vec, size_t index) \
    { \
        assert(vec); \
        assert(index < vec->len); \
        assert(index >= vec->first); \
        return vec->items[index]; \
    }

#define VEC_IMPLEMENT_BY_REF_FREE(N, A, T, F) \
    inline void A##_free(N *vec) \
    { \
        assert(vec); \
        for(size_t i = 0; i < vec->cap; i++) { \
            free(vec->items[i]); \
        } \
        free(vec->items); \
        vec->cap = 0; \
        vec->len = 0; \
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
            vec_memset(&vec->items[len], 0, sizeof(vec->items[vec->cap]) * (required - len)); \
            for(size_t i = len; i < required; i++) { \
                vec->items[len] = vec_malloc(sizeof(**vec->items)); \
                if(!vec->items[len]) return VEC_ERROR_MALLOC; \
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
                free(vec->items[i]); \
            } \
            memset(&vec->items[required], 0, sizeof(*vec->items) * (cap - required)); \
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
        memmove(item + vec->first, item, sizeof(T *) * (vec->len - vec->first)); \
        vec->len -= vec->first; \
    }

#define VEC_IMPLEMENT_BY_REF_SET_AT(N, A, T, F) \
    void A##_set_at(N *vec, T *val, size_t index) \
    { \
        assert(vec); \
        T *item = A##_static_get(vec, index); \
        memcpy(item, val, sizeof(T)); \
    }

#define VEC_IMPLEMENT_BY_REF_INSERT_AT(N, A, T, F) \
    int A##_insert_at(N *vec, T *val, size_t index) \
    { \
        assert(vec); \
        assert(val); \
        int result = A##_reserve(vec, index + 1); \
        if(result) return result; \
        T *item = A##_static_get(vec, index); \
        vec_memmove(item + 1, item, sizeof(T *) * (++vec->len - index)); \
        memcpy(item, val, sizeof(T)); \
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
        return A##_static_get(vec, index); \
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


//VEC_INCLUDE(VecU8, vec_u8, unsigned char, 0);
VEC_INCLUDE(VecSize, vec_size, size_t, 0, BY_VAL);
VEC_INCLUDE(Vec2Size, vec2_size, VecSize, vec_size_free, BY_REF);

#define VEC_H
#endif
