# Things may still change
# Very untested as of now

# c-vector
Compile Time Generic Dynamic Vector in C.

## First Things First
On it's own, no code is being compiled. You first have to include and implement a vector of your desires. For that, there are two macros.
```c
#include "vec.h"
VEC_INCLUDE(N, A, T, M);
VEC_IMPLEMENT(N, A, T, M, F);
```
1. `N` - **N**ame - the resulting name of the vector struct
2. `A` - **A**bbreviation - functions get prefixed with that
3. `T` - **T**ype - the type your elements within the vector
4. `M` - **M**ode - storage type, either `BY_VAL` (by value) or `BY_REF` (by reference)
5. `F` - **F**ree - provide a freeing function for your elements, if available

## Example

### Basic Vectors
```c
VEC_INCLUDE(VecU8, vec_u8, unsigned char, BY_VAL);
VEC_IMPLEMENT(VecU8, vec_u8, unsigned char, BY_VAL, 0);
```

```c
VEC_INCLUDE(VecSize, vec_size, size_t, BY_VAL);
VEC_IMPLEMENT(VecSize, vec_size, size_t, BY_VAL, 0);
```

### Struct Vectors
```c
VEC_INCLUDE(Vec2U8, vec2_u8, VecU8, BY_REF);
VEC_IMPLEMENT(Vec2U8, vec2_u8, VecU8, BY_REF, vec_u8_free);
```

## Design Considerations
- Both `BY_VAL` and `BY_REF` can be used on either basic types or (complex) structs.
- Generally speaking, I'd use the latter when dealing with structs where `sizeof struct > sizeof(void *)`.
- I went ahead and compared the very basic performance between the two approaches [here](https://github.com/rphii/vec_test).

## Metaprogramming
### Advantages
- Compiler optimized code
- Flexibility, reusability (generics / templates)
- Type safety
### Disadvantages
- Maintenance challenges
- Debugging difficulties
- Compilation overhead

## How to use it

### Available Functions
The `A##` means the `A` specified in the two macros.
- `A##_zero` set vector to zero without freeing
- `A##_clear` clear vector but keep capacity
- `A##_length` return length of of the vector in items
- `A##_resize` resize vector to optimal given length in items
- `A##_free` free all memory used
- `A##_reserved` return bytes allocated
- `A##_reserve` reserve more memory in items
- `A##_shrink` shrinks vector to minimal possible capacity
- `A##_set_at` overwrite item at index, free any previous item
- `A##_insert_at` insert item after index
- `A##_push_front` insert item in the front
- `A##_push_back` insert item at the back
- `A##_pop_front` pop item in the front
- `A##_pop_back` pop item at the back
- `A##_get_at` get item at index
- `A##_get_front` get item in the front
- `A##_get_back` get item at the back
- `A##iter_begin` return beginning iterator
- `A##iter_end` return end of iterator

# todo so I don't forget
- add `pop_at` pop item at index
- add `erase_at` clear and free item at
- add `capacity` return capacity of the vector
- add `is_empty` return if it's empty
- add `copy` make a copy of one vector...?
- add `emplace` insert item before index
- bundle the snippets I copied around used for freeing into it's own function
- consider removing `N.first` and anything relating to it... in order to reduce compile time...?

