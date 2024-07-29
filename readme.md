# Dynastatic

This library represents another solution to the problem of dynamic memory allocation in a program.

Usually, using standard dynamic memory allocation functions such as malloc and free is quite expensive in terms of computational resources. On the other hand, memory for static variables is usually allocated by the compiler at the stage of program creation, it is allocated along with the program code. The "cost" of static variables is much lower than the "cost" of dynamic variables allocated from the heap at runtime.

This library combines the low overhead of creating and using static variables with the ability to dynamically allocate variables during program execution. The main idea is static memory allocation, which is then used as a basis for dynamic memory allocation for data.

# Basic Usage

```
#include "dynastatic.h"
...

/* 1 */
typedef struct {
   int a;
   char *b;
} foo_t;

...
/* 2 */
DEFINE_DYNASTATIC(foo_t, 1000);

...
/* 3 */
foo_t *foo;
foo = DYNASTATIC_CALLOC(foo_t);

...
/* 4 */
DYNASTATIC_FREE(foo_t, foo);
...
```

**Under the hood**

1. Define your data type. It can be any C data structure/union, or you can use some simple type.
1. `DEFINE_DYNASTATIC` macro is used to define and allocate all required static variables and arrays. In the example, static array of 1000 foo_t structures will be reserved by the compiler in the data segment of the executable code. 
1. Use `DYNASTATIC_ALLOC` macro to allocate new pointer to your structure. This call will try to find *available* (not yet allocated) foo_t structure in our static array, and will return pointer to this structure. In case all 1000 structures are already allocated, `DYNASTATIC_ALLOC` will call simple `calloc` to allocate new structure from the dynamic heap.
1. Call to `DYNASTATIC_FREE` will check the way of how foo structure was allocated. In case the structure was allocated from the static array, it will be marked as unallocated and available for subsequent allocations. In other case (structure was calloc-ed from the dynamic heap), `DYNASTATIC_FREE` will call `free` to dispose memory resources.

# Dynastatic Macros And Functions
* Macro `DEFINE_DYNASTATIC(<type_name>, <static_array_capacity>)`. Defines and initialises `<type_name>` static array of `<static_array_capacity>` elements, along with internal static variables and array
* Macro `DECLARE_DYNASTATIC(<type_name>)`. Can be used to declare external `<type_name>_calloc` and `<type_name>_free` functions in source files that not use `DEFINE_DYNASTATIC` macro
* Macro `DYNASTATIC_CALLOC(<type_name>)`. Can be used to dynamically allocate `<type_name>` instance from static array or from dynamic heap. This macro available only in source file where appropriate `DEFINE_DYNASTATIC` was used. In other sources use `<type_name>_calloc` function 
* Macro `DYNASTATIC_FREE(<type_name>, <instance>)`. Frees `<type_name>` `<instance>` - returns it to static array or physically frees allocated memory
* `<type_name>_calloc()` function - does the same as `DYNASTATIC_CALLOC` macro
* `<type_name>_free(<instance>)` function - does the same as `DYNASTATIC_FREE` macro

# Benchmark
See dynastatic_test.c for details.

```
100000 calls to dynastatic calloc/free takes 1651200 nanoseconds
100000 calls to stdlib's calloc/free takes 41972100 nanoseconds
Dynastatic is 25 times faster
```