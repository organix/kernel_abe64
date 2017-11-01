John Shutt's "Kernel" language implemented on ABE (C) 64-bit runtime. A [previous implementation](https://github.com/dalnefre/kernel_abe) targeted 32-bit platforms.

## Kernel

[Kernel](https://web.cs.wpi.edu/~jshutt/kernel.html) is a language in the LISP/Scheme family.

## Actor Based Runtime (ABE)

ABE is a platform for writing [Actors](https://en.wikipedia.org/wiki/Actor_model) in C.

### Encoding

Our target architecture is [LP64](https://en.wikipedia.org/wiki/64-bit_computing#64-bit_data_models), where `int` is 32 bits; `void *`, `ptrdiff_t`, and `size_t` are all 64 bits (8 bytes). The ABE runtime uses a single-sized universal signed integer `WORD` value type (64 bits, in this case), which can be safely cast to/from `CONS *`. Memory is managed in `CONS`-sized units, each of which holds a pair of values (16 bytes). The garbage collector manages blocks with 50% overhead for it's own bookkeeping (32 bytes). Assuming 8-byte alignment for allocations, the least-significant 3 bits of each `WORD` can be used for type encoding.

```
    2#..xxxxx1  -- Integer/Pointer Literal
    2#..xxx000  -- reserved
    2#..xxx010  -- Atom/Symbol Reference
    2#..xxx100  -- Pair (CONS) Reference
    2#..xxx110  -- Actor Reference
```
