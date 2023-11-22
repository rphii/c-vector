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
3. `T` - **T**ype - the type of your elements within the vector
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
VEC_INCLUDE(Vec2U8, vec2_u8, VecU8, BY_VAL);
VEC_IMPLEMENT(Vec2U8, vec2_u8, VecU8, BY_VAL, vec_u8_free);
```

```c
VEC_INCLUDE(Vec2U8, vec2_u8, VecU8, BY_REF);
VEC_IMPLEMENT(Vec2U8, vec2_u8, VecU8, BY_REF, vec_u8_free);
```

## Design Considerations
- Both `BY_VAL` and `BY_REF` can be used on either basic types or (complex) structs. Either way, if
  you (e.g.) push back a value, its content is copied
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
### Examples
- `$ cd examples && make` (binaries in subfolder "bin") -> WIP, I want to add more examples

### Tests
- `$ cd test && make` (binaries in subfolder "bin") -> WIP, I want to add more tests to make the vector bug free
- Ignore the `compile_flags.txt` files -> I added those only so that my LSP knows what's up.

### Available Functions
The `A##` means the `A` specified in the two macros.
- `A##_zero` set vector to zero without freeing
- `A##_clear` clear vector but keep capacity
- `A##_length` return length of of the vector in items
- `A##_capacity` return capacity of the vector
- `A##_empty` return if it's empty
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
- `A##_swap` swap two items by index
- `A##_reverse` reverse the vector
- `A##_iter_begin` return beginning iterator
- `A##_iter_end` return end of iterator

### Additional Settings
There are various settings one can adjust to fit the vector to their needs. To use those, I strongly
recommend the following:
- In header files (using the `INCLUDE` macro) -> `#define` the respective settings before
  including `vec.h`. Before the end of your header `#undef` them, so that other possible vectors
  that may create a vector of such a "modified" vector with custom settings reverts back to the
  defaults
- In source files (using the `IMPLEMENT` macro) -> `#define` the respective settings after you
  included your custom vector header and have them be equally set.

#### Settings list
- `VEC_SETTINGS_KEEP_ZERO_END` number; specify how much zero memory should be kept at the end when
  reserving memory (e.g. string implementation)
- `VEC_SETTINGS_STRUCT_ITEMS` literal; specify the name of the `items` placeholder to something
  else, if you so desire (e.g. string implementation, where it makes more sense to use another
  literal besides the previously mentioned for the string placeholder)
- `VEC_SETTINGS_DEFAULT_SIZE` number; specify how many item spaces shall be reserved minimally

# todo so I don't forget
- add `pop_at` pop item at index
- add `emplace` insert item before index
- add stuff that allows to pop/push/insert/emplace an entire array, or a subsection...
- if we pushed from the front, then N.first can actually be reduced, instead of moving everything
  one back!
- bundle the snippets I copied around used for freeing into it's own function
- consider removing `N.first` and anything relating to it... in order to reduce compile time...?

