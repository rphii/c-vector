# Things may still change
# Very untested as of now

# c-vector
Simple Compile Time Generic Dynamic Vector in C.

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
- `A##_zero` simply set to zero (unsafe)
- `A##_clear`
- `A##_length`
- `A##_resize`
- `A##_free`
- `A##_reserved`
- `A##_reserve`
- `A##_shrink`
- `A##_set_at`
- `A##_insert_at`
- `A##_push_front`
- `A##_push_back`
- `A##_pop_front`
- `A##_pop_back`
- `A##_get_at`
- `A##_get_front`
- `A##_get_back`

# todo so I don't forget
- add `pop_at`
- add `erase_at`
- add `capacity`
- add `empty`
- add `copy`
- bundle the snippets I copied around used for freeing into it's own function
- add `iter_begin`
- add `iter_end`

