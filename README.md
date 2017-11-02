John Shutt's "Kernel" language implemented on ABE (C) 64-bit runtime. A [previous implementation](https://github.com/dalnefre/kernel_abe) targeted 32-bit platforms.

## Kernel

[Kernel](https://web.cs.wpi.edu/~jshutt/kernel.html) is a language in the LISP/Scheme family.

## Actor Based Runtime (ABE)

ABE is a platform for writing [Actors](https://en.wikipedia.org/wiki/Actor_model) in C.

### Encoding

Our target architecture is [LP64](https://en.wikipedia.org/wiki/64-bit_computing#64-bit_data_models), where `int` is 32 bits; `void *`, `ptrdiff_t`, and `size_t` are all 64 bits (8 bytes). The ABE runtime uses a single-sized universal signed integer `WORD` value type (64 bits, in this case), which can be safely cast to/from `CONS *`. Memory is managed in `CONS`-sized units, each of which holds a pair of values (16 bytes). The garbage collector's book-keeping has 50% overhead for each allocation (32 bytes). Assuming 8-byte alignment for allocations, the least-significant 3 bits of each `WORD` can be used for type encoding.

```
    2#000...000000  -- Boolean FALSE (also NULL in C)
    2#000...000001  -- Boolean TRUE
    2#xxx...xxx000  -- Pair (CONS) Reference
    2#xxx...xxx001  -- Object Data Pointer
    2#xxx...xxx100  -- Atom/Symbol Reference
    2#xxx...xxx101  -- Actor Address/Capability
    2#xxx...xxxx10  -- Executable Code Address
    2#xxx...xxxx11  -- Number (int) Literal
```

For compatibility with C-language definitions, the Boolean values `TRUE` and `FALSE` **must** be encoded as `1` and `0` respectively. Although C requires `NULL` to also be `0`, we do not use `NULL` directly. Instead, there is a statically-allocated `CONS` which is hard-coded to represent `NIL` for list-building purposes. Dynamically-allocated memory will never be placed at address `0`, so `TRUE` and `FALSE` do not conflict with type-tagged allocated values.

Literal values are encoded with a 2-LSB type tag and use logical right-shift to recover the value they represent. Reference values are encoded with a 3-LSB type tag and simply mask off the type tag to recover the address they represent.
