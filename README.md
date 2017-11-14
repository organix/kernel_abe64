John Shutt's "Kernel" language implemented on ABE (C) 64-bit runtime. A [previous implementation](https://github.com/dalnefre/kernel_abe) targeted 32-bit platforms.

## Kernel

[Kernel](https://web.cs.wpi.edu/~jshutt/kernel.html) is a language in the LISP/Scheme family. This implementation, while faithful to the novel semantics of Kernel and consistent with John's specification, is **not** a conforming Kernel because it does not provide all of the required features defined in the Kernel standard (R<sup>-1</sup>RK). However, we believe it is a useful subset which illustrates our own novel actor-based approach to the implementation. If you are looking for efficiency, look elsewhere. This implementation is a deeply-nested meta-interpreter.

### Using Docker

Create a Docker image named `abe64` in which to compile and run Kernel.

```
docker build -t abe64 .
```

Compile and test the `abe` and `kernel` programs within the `abe64` Docker image.

```
docker run -v $(pwd):/src --rm -it abe64
```

Run the Kernel binary in iteractive mode.

```
docker run -v $(pwd):/src --rm -it abe64 ./kernel -i 
```

There are a few Kernel source files (with `.knl` extensions) which may be pre-loaded into the environment by naming them on the command line. For example, `library.knl` contains standard library definitions.

```
docker run -v $(pwd):/src --rm -it abe64 ./kernel -i library.knl
```

### Kernel Standard Library

The Kernel standard (R<sup>-1</sup>RK) defines the facilities provided by the "kernel standard environment", and the symbol names under which they are bound. The tables below list these facilities by section, in the order they are defined. Facilities marked with `o` are *optional* in the standard. The `Humus` column marks facilities defined by the Humus source in ["Semantic Extensibility with *Vau*"](http://www.dalnefre.com/wp/2011/12/semantic-extensibility-with-vau/). The `abe64` column marks facilities defined by the C source in "kernel.c". The `library` column marks facilities defined by the Kernel source in "library.knl".

#### 4. Core types and primitive features

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
boolean?                             | x      | x       | x       | 
eq?                                  | o      | x       | x       | 
equal?                               | x      |         |         | x
symbol?                              | x      | x       | x       | 
inert?                               | x      | x       | x       | 
$if                                  | x      | x       | x       | 
pair?                                | x      | x       | x       | 
null?                                | x      | x       | x       | 
cons                                 | x      | x       | x       | 
set-car!                             | o      |         | x       | 
set-cdr!                             | o      |         | x       | 
copy-es-immutable                    | o      |         | x       | 
environment?                         | x      | x       | x       | 
ignore?                              | x      | x       | x       | 
eval                                 | x      | x       | x       | 
make-environment                     | x      | x       | x       | 
$define!                             | x      | x       | x       | 
operative?                           | x      | x       | x       | 
applicative?                         | x      | x       | x       | 
$vau                                 | x      | x       | x       | 
wrap                                 | x      | x       | x       | 
unwrap                               | x      | x       | x       | 

#### 5. Core library features (I)

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
$sequence                            | x      | x       | x       | 
list                                 | x      | x       | x       | 
list*                                | x      |         |         | x
$lambda                              | x      | x       | x       | 
car                                  | x      |         |         | x
cdr                                  | x      |         |         | x
caar, cadr, ... cddddr               | x      |         |         | x
apply                                | x      |         |         | x
$cond                                | x      |         |         | x
get-list-metrics                     | x      |         |         | 
list-tail                            | x      |         |         | 
encycle!                             | o      |         |         | 
map                                  | x      |         | x       | 
$let                                 | x      |         |         | x

#### 6. Core library features (II)

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
not?                                 | x      |         |         | x
and?                                 | x      |         |         | 
or?                                  | x      |         |         | 
$and?                                | x      |         |         | x
$or?                                 | x      |         |         | x
length                               | x      |         |         | 
append                               | x      |         |         | 
filter                               | x      |         |         | 
reduce                               | x      |         |         | x
get-current-environment              | x      | x       |         | x
make-kernel-standard-environment     | x      | x       |         | 
$let*                                | x      |         |         | 
$letrec                              | x      |         |         | 
$letrec*                             | x      |         |         | 
$let-redirect                        | x      |         |         | 
$let-safe                            | x      |         |         | 
$remote-eval                         | x      |         |         | x
$set!                                | o      |         |         | x
$provide!                            | x      |         |         | x
$import!                             | x      |         |         | 
for-each                             | x      |         |         | 

#### 8. Encapsulations

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
make-encapsulation-type              | x      | x       | x       | 

#### 9. Promises

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
promise?                             | x      |         |         | 
force                                | x      |         |         | 
$lazy                                | x      |         |         | 
memoize                              | x      |         |         | 

#### 12. Numbers

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
number?                              | x      |         | x       | 
integer?                             | x      |         |         | 
=?                                   | x      |         |         | 
&lt;?                                   | x      |         |         | 
&lt;=?                                  | x      |         |         | 
&gt;=?                                  | x      |         |         | 
&gt;?                                   | x      |         |         | 
\+                                    | x      |         |         | 
\*                                    | x      |         |         | 
\-                                    | x      |         |         | 
zero?                                | x      |         |         | 
div                                  | x      |         |         | 
mod                                  | x      |         |         | 
div-and-mod                          | x      |         |         | 
positive?                            | x      |         |         | 
negative?                            | x      |         |         | 
odd?                                 | x      |         |         | 
even?                                | x      |         |         | 
abs                                  | x      |         |         | 
max                                  | x      |         |         | 
min                                  | x      |         |         | 
lcm                                  | x      |         |         | 
gcd                                  | x      |         |         | 

#### 15. Ports

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
write                                | x      |         | x       | 

#### Non-standard extensions

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
newline                              |        |         | x       | 
$concurrent                          |        |         | x       | 
$parallel                            |        |         |         | x
Y                                    |        |         |         | x

## Actor Based Runtime (ABE)

ABE is a platform for writing [Actors](https://en.wikipedia.org/wiki/Actor_model) in C.

### Encoding

Our target architecture is [LP64](https://en.wikipedia.org/wiki/64-bit_computing#64-bit_data_models), where `int` is 32 bits (4 bytes); `void *`, `ptrdiff_t`, and `size_t` are all 64 bits (8 bytes). The ABE runtime uses a single-sized universal signed integer `WORD` value type (64 bits, in this case), which can be safely cast to/from `CONS *`. Memory is managed in `CONS`-sized units, each of which holds a pair of values (16 bytes). The garbage collector's book-keeping has 50% overhead for each allocation (32 bytes). Assuming 8-byte alignment for allocations, the least-significant 3 bits of each `WORD` can be used for type encoding. However, sufficient distinctions can be made using only the lowest 2 bits.

```
    2#000...000000  -- Boolean FALSE (also NULL in C)
    2#000...000001  -- Boolean TRUE
    2#xxx...xxxx00  -- Pair (CONS) Reference
    2#xxx...xxxx01  -- Actor Address/Capability (or Object Pointer)
    2#xxx...xxxx10  -- Atom/Symbol Reference
    2#xxx...xxxx11  -- Number (int) Literal (or Code Address)
```

The Boolean values `TRUE` and `FALSE` **must** be encoded as `1` and `0` respectively, for compatibility with C-language definitions. Also, C requires `NULL` to also be `0`, but we do not use `NULL` directly. Instead, there is a statically-allocated `CONS` which is hard-coded to represent `NIL` for list-building purposes. Dynamically-allocated memory will never be placed at address `0`, so `TRUE` and `FALSE` do not conflict with type-tagged allocated values. Literal values (Number/Address) use logical right-shift to recover the value they represent. Reference values (Pair/Actor/Atom) simply mask off the type tag to recover the address they represent.

### Internal Structure

#### Actor Message Events

```
           +---+---+ WHAT
EVENT ---> | o | o-------> message
           +-|-+---+
        SELF |
             v
           +---+---+ MINE
           | o | o-------> state
           +-|-+---+
        THIS |
             v
          behavior
```

#### Atomic Symbols

Empty symbol table:
```
           +---+---+
root ----> | / | / |
           +---+---+
```

After `CONS* n = ATOM("n")`:
```
           +---+---+       +---+---+
root ----> | / | o-------> | o | / |
           +---+---+       +-|-+---+
                             |
                             v
                           +---+---+
           ATOM('n') ----> | o | / |
                           +-|-+---+
                             |
                             v
                           +---+---+
                           |'n'| / |
                           +---+---+
```

After `CONS* x = ATOM("x")`:
```
           +---+---+       +---+---+       +---+---+
root ----> | / | o-------> | o | o-------> | o | / |
           +---+---+       +-|-+---+       +-|-+---+
                             |               |
                 ATOM("x")   v   ATOM("n")   v
                       |   +---+---+   |   +---+---+
                       +-> | o | / |   +-> | o | / |
                           +-|-+---+       +-|-+---+
                             |               |
                             v               v
                           +---+---+       +---+---+
                           |'x'| / |       |'n'| / |
                           +---+---+       +---+---+
```

After `CONS* nil = ATOM("nil")`:
```
           +---+---+       +---+---+       +---+---+
root ----> | / | o-------> | o | o-------> | o | / |
           +---+---+       +-|-+---+       +-|-+---+
                             |               |
                 ATOM("x")   v   ATOM("n")   v
                       |   +---+---+   |   +---+---+       +---+---+
                       +-> | o | / |   +-> | o | o-------> | o | / |
                           +-|-+---+       +-|-+---+       +-|-+---+
                             |               |               |
                             v               v   ATOM("ni")  v
                           +---+---+       +---+---+   |   +---+---+       +---+---+
                           |'x'| / |   +-> |'n'| / |   +-> | o | o-------> | o | / |
                           +---+---+   |   +---+---+       +-|-+---+       +-|-+---+
                                       |                     |               |
                                       |                     v   ATOM("nil") v
                                       |                   +---+---+   |   +---+---+
                                       |               +-> |'i'| o |   +-> | o | / |
                                       |               |   +---+-|-+       +-|-+---+
                                       +---------------|---------+           v
                                                       |                   +---+---+
                                                       |                   |'l'| o |
                                                       |                   +---+-|-+
                                                       +-------------------------+
```

After `CONS* no = ATOM("no")`:
```
           +---+---+       +---+---+       +---+---+
root ----> | / | o-------> | o | o-------> | o | / |
           +---+---+       +-|-+---+       +-|-+---+
                             |               |
                 ATOM("x")   v   ATOM("n")   v
                       |   +---+---+   |   +---+---+       +---+---+       +---+---+
                       +-> | o | / |   +-> | o | o-------> | o | o-------> | o | / |
                           +-|-+---+       +-|-+---+       +-|-+---+       +-|-+---+
                             |               |               |               |
                             v               v   ATOM("no")  v   ATOM("ni")  v
                           +---+---+       +---+---+   |   +---+---+   |   +---+---+       +---+---+
                           |'x'| / |   +-> |'n'| / |   +-> | o | / |   +-> | o | o-------> | o | / |
                           +---+---+   |   +---+---+       +-|-+---+       +-|-+---+       +-|-+---+
                                       |                     |               |               |
                                       |                     v               v   ATOM("nil") v
                                       |                   +---+---+       +---+---+   |   +---+---+
                                       |                   |'o'| o |   +-> |'i'| o |   +-> | o | / |
                                       |                   +---+-|-+   |   +---+-|-+       +-|-+---+
                                       +-------------------------+-----|---------+           v
                                                                       |                   +---+---+
                                                                       |                   |'l'| o |
                                                                       |                   +---+-|-+
                                                                       +-------------------------+
```

The `car` of each ATOM can be read as a `NIL`-terminated linked-list of characters, starting from the end of the name. The 'cdr' of each ATOM is a `NIL`-terminated linked-list of suffixes, adding one character to the ATOM. Thus the `root` is a list of single-character atoms, each containing a list of all suffixes.

### Garbage Collection

A statically-allocated `CELL` represents the `NIL` value. It is not linked into the GC lists.

```
       NIL
        |   +------------------+
first:  +-->|       NIL        |
            +------------------+
rest:       |       NIL        |
            +--------------+---+
_prev:      |        0     | Z |
            +--------------+---+
_next:      |        0         |
            +------------------+
````

There are 5 double-linked lists and 2 phase variables used by the garbage collector. Each of the lists are initially empty. There are 4 garbage-collection phase-marker values (stored in `_prev`):

 * **Z** -- Permanently allocated
 * **X** -- Marked in-use during heap-walk
 * **0** -- Even-phase allocation
 * **1** -- Odd-phase allocation

````
PREV_PHASE = 0
MARK_PHASE = 1

      AGED
        |   +------------------+
first:  +-->|        0         |
            +------------------+
rest:       |       NIL        |
            +--------------+---+
_prev:  +------------o     | Z |<--+
        |   +--------------+---+   |
_next:  +-->|        o-------------+
            +------------------+

      SCAN
        |   +------------------+
first:  +-->|        0         |
            +------------------+
rest:       |       NIL        |
            +--------------+---+
_prev:  +------------o     | Z |<--+
        |   +--------------+---+   |
_next:  +-->|        o-------------+
            +------------------+

      FRESH
        |   +------------------+
first:  +-->|        0         |
            +------------------+
rest:       |       NIL        |
            +--------------+---+
_prev:  +------------o     | Z |<--+
        |   +--------------+---+   |
_next:  +-->|        o-------------+
            +------------------+

      FREE
        |   +------------------+
first:  +-->|        0         |
            +------------------+
rest:       |       NIL        |
            +--------------+---+
_prev:  +------------o     | Z |<--+
        |   +--------------+---+   |
_next:  +-->|        o-------------+
            +------------------+

      PERM
        |   +------------------+
first:  +-->|        0         |
            +------------------+
rest:       |       NIL        |
            +--------------+---+
_prev:  +------------o     | Z |<--+
        |   +--------------+---+   |
_next:  +-->|        o-------------+
            +------------------+
````
