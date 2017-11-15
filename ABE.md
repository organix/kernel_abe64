# Actor Based Runtime (ABE)

ABE is a platform for writing [Actors](https://en.wikipedia.org/wiki/Actor_model) in C.

## Encoding

Our target architecture is [LP64](https://en.wikipedia.org/wiki/64-bit_computing#64-bit_data_models), where `int` is 32 bits (4 bytes); `void *`, `ptrdiff_t`, and `size_t` are all 64 bits (8 bytes). The ABE runtime uses a single-sized universal signed integer `WORD` value type (64 bits, in this case), which can be safely cast to/from `CONS *`. Memory is managed in `CONS`-sized units, each of which holds a pair of values (16 bytes). The garbage collector's book-keeping doubles the size of each allocation (32 bytes). Assuming 8-byte alignment for allocations, the least-significant 3 bits of each `WORD` can be used for type encoding. However, sufficient distinctions can be made using only the lowest 2 bits.

```
    2#000...000000  -- Boolean FALSE (also NULL in C)
    2#000...000001  -- Boolean TRUE
    2#xxx...xxxx00  -- Pair (CONS) Reference
    2#xxx...xxxx01  -- Actor Address/Capability (or Object Pointer)
    2#xxx...xxxx10  -- Atom/Symbol Reference
    2#xxx...xxxx11  -- Number (int) Literal (or Code Address)
```

The Boolean values `TRUE` and `FALSE` **must** be encoded as `1` and `0` respectively, for compatibility with C-language definitions. Also, C requires `NULL` to also be `0`, but we do not use `NULL` directly. Instead, there is a statically-allocated `CONS` which is hard-coded to represent `NIL` for list-building purposes. Dynamically-allocated memory will never be placed at address `0`, so `TRUE` and `FALSE` do not conflict with type-tagged allocated values. Literal values (Number/Address) use logical right-shift to recover the value they represent. Reference values (Pair/Actor/Atom) simply mask off the type tag to recover the address they represent.

## Internal Structure

### Actor Message Events

All computation in an actor system occurs in response to message events. Each actor message events delivers an immutable *message* to a target *actor*. Several macros are defined to allow easy access to components of the event from within a (C-coded) *behavior*:

 * `WHAT` &#8212; the *message* contents
 * `SELF` &#8212; the target *actor*
 * `THIS` &#8212; the C procedure defining the actor's *behavior*
 * `MINE` &#8212; the private *state* of this actor instance

The structure of the private *state*, if any, is determined solely by the implementation of the *behavior* procedure.

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

### Atomic Symbols

Atomic symbols are a memoized set of short character strings. Whenever a particular atomic symbol is encountered, a reference to a single immutable shared object is returned. This means that pointer equality implies symbol identity. Symbol matching is a simple pointer comparison.

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
           ATOM("n") ----> | o | / |
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

The `car` of each ATOM can be read as a `NIL`-terminated linked-list of characters, starting from the end of the name. The `cdr` of each ATOM is a `NIL`-terminated linked-list of suffixes, adding one character to the ATOM. Thus the `root` is a list of single-character atoms, each containing a list of all suffixes.

## Garbage Collection

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

There are 5 double-linked lists and 2 phase variables used by the garbage collector:

 * `AGED` &#8212; ...
 * `SCAN` &#8212; ...
 * `FRESH` &#8212; Collectable cells allocated since the last GC
 * `FREE` &#8212; Unused collectable cells available for allocation
 * `PERM` &#8212; Non-collectable (permanent) cells available for allocation

There are 4 garbage-collection phase-marker values (stored in low bits of the `_prev` field):

 * **Z** &#8212; Not visible to GC
 * **X** &#8212; Permanently allocated
 * **0** &#8212; Even-phase allocation
 * **1** &#8212; Odd-phase allocation

After allocation of 2 collectable and 2 permanent cells (reachable from `root`), the GC lists could look like this:
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

      FRESH                     root
        |   +------------------+  |   +------------------+      +------------------+
first:  +-->|        0         |  +-->|       NIL        |      |        o-----------------+
            +------------------+      +------------------+      +------------------+       |
rest:       |       NIL        |      |        o--------------->|       NIL        |       |
            +--------------+---+      +--------------+---+      +--------------+---+       |
_prev:  +------------o     | Z |<--------------o     | 1 |<--------------o     | 1 |<--+   |
        |   +--------------+---+      +--------------+---+      +--------------+---+   |   |
_next:  +-->|        o--------------->|        o--------------->|        o-------------+   |
            +------------------+      +------------------+      +------------------+       |
                                                                                           |
                                                            +------------------------------+
                                                            |
                                                            |   +------------------+      +------------------+
                                                            +-->|        o--------------->|       'n'        |
                                                                +------------------+      +------------------+
                                                                |       NIL        |      |       NIL        |
                                                                +--------------+---+      +--------------+---+
                                                                |        -     | X |      |        -     | X |
                                                                +--------------+---+      +--------------+---+
                                                                |        -         |      |        -         |
                                                                +------------------+      +------------------+

      FREE
        |   +------------------+      +------------------+      +------------------+      +------------------+
first:  +-->|        3         |      |        -         |      |        -         |      |        -         |
            +------------------+      +------------------+      +------------------+      +------------------+
rest:       |       NIL        |      |        -         |      |        -         |      |        -         |
            +--------------+---+      +--------------+---+      +--------------+---+      +--------------+---+
_prev:  +------------o     | Z |<--------------o     | Z |<--------------o     | Z |<--------------o     | Z |<--+
        |   +--------------+---+      +--------------+---+      +--------------+---+      +--------------+---+   |
_next:  +-->|        o--------------->|        o--------------->|        o--------------->|        o-------------+
            +------------------+      +------------------+      +------------------+      +------------------+

      PERM
        |   +------------------+      +------------------+      +------------------+      +------------------+
first:  +-->|        3         |      |        -         |      |        -         |      |        -         |
            +------------------+      +------------------+      +------------------+      +------------------+
rest:       |       NIL        |      |        -         |      |        -         |      |        -         |
            +--------------+---+      +--------------+---+      +--------------+---+      +--------------+---+
_prev:  +------------o     | Z |<--------------o     | Z |<--------------o     | Z |<--------------o     | Z |<--+
        |   +--------------+---+      +--------------+---+      +--------------+---+      +--------------+---+   |
_next:  +-->|        o--------------->|        o--------------->|        o--------------->|        o-------------+
            +------------------+      +------------------+      +------------------+      +------------------+
````
Normally, cells are allocated from the `FREE` list. These cells are subject to garbage collection. They use phase-marker **0** or **1**, depending on the value of `MARK_PHASE`. The `FREE` list contains collectable cells available for allocation. When the `FREE` list is exhausted, a new batch of cells is allocated from system memory and linked into this list. When a `FREE` cell is allocated (and phase-marked), it is moved to the `FRESH` list.

Some cells, such as those used to represent symbolic constants (ATOMs), are *permanently* allocated. They are immune from garbage collection. They use phase-marker **X**. The `PERM` list contains permanent cells available for allocation. When the `PERM` list is exhausted, a new batch of cells is allocated from system memory and linked into this list.

The garbage collection algorithm is essentially as follows:
````
gc(root) {
	gc_age_cells();  /* move "fresh" cells to the "aged" list for possible collection */
	if (!nilp(root)) {  /* scan "root" cell */
		gc_scan_cell(root);  /* consider a value for addition to the "scan" list, return TRUE if queued */
	}
	while (gc_refresh_cell() == TRUE)  /* process a cell from the "scan" list, return FALSE if none remain */
		;
	gc_free_cells();  /* move unmarked "aged" cells to "free" list after scanning */
}
````
