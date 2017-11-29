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
           .---.---. MINE
           | o | o-------> state
           .-|-.---.
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

Our memory management strategy fulfills three simultaneous goals:

 * Avoid fragmentation of allocatable memory
 * Maintain stable addresses for allocated objects
 * Perform safe garbage-collection concurrent with ongoing allocations

Our approach is based on Henry Baker's ["Treadmill"](https://dl.acm.org/citation.cfm?id=130862) algorithm, published as *The treadmill: real-time garbage collection without motion sickness* in **ACM SIGPLAN Notices**, Volume 27 Issue 3, March 1992.

Fragmentation is avoided by allocating all memory in the same `CELL`-sized units. These cells contain 2 user-controlled pointers (`first` and `rest`), and 2 GC-private fields `_prev` and `_next`. A statically-allocated `CELL` represents the `NIL` value. It is not linked into the GC lists.

```
       NIL
        |   +---------------+
first:  +-->|      NIL      |
            +---------------+
rest:       |      NIL      |
            +-----------+---+
_prev:      |       0   | Z |
            +-----------+---+
_next:      |       0       |
            +---------------+
````

There are 5 double-linked lists and 2 phase variables used by the garbage collector:

 * `AGED` &#8212; Candidates for garbage collection
 * `SCAN` &#8212; In-use cells needing to be scanned
 * `FRESH` &#8212; Collectable cells marked in-use since the last GC began
 * `FREE` &#8212; Unused collectable cells available for allocation
 * `PERM` &#8212; Non-collectable (permanent) cells available for allocation

There are 4 garbage-collection phase-marker values (stored in low bits of the `_prev` field):

 * **Z** &#8212; Not visible to GC
 * **X** &#8212; Permanently allocated
 * **0** &#8212; Even-phase allocation
 * **1** &#8212; Odd-phase allocation

Each GC list has a header `CELL`. The `first` field of the header is used to cache the length of the list.

After allocation of 2 collectable and 2 permanent cells (reachable from `root`), the GC lists could look like this:
````
PREV_PHASE = 0
MARK_PHASE = 1

      AGED
        |   +---------------+
first:  +-->|       0       |
            +---------------+
rest:       |      NIL      |
            +-----------+---+
_prev:  +-----------o   | Z |<--+
        |   +-----------+---+   |
_next:  +-->|       o-----------+
            +---------------+

      SCAN
        |   +---------------+
first:  +-->|       0       |
            +---------------+
rest:       |      NIL      |
            +-----------+---+
_prev:  +-----------o   | Z |<--+
        |   +-----------+---+   |
_next:  +-->|       o-----------+
            +---------------+

      FRESH                   root
        |   +---------------+  |   +---------------+      +---------------+
first:  +-->|       2       |  +-->|      NIL      |      |       o---------------+
            +---------------+      +---------------+      +---------------+       |
rest:       |      NIL      |      |       o------------->|      NIL      |       |
            +-----------+---+      +-----------+---+      +-----------+---+       |
_prev:  +-----------o   | Z |<-------------o   | 1 |<-------------o   | 1 |<--+   |
        |   +-----------+---+      +-----------+---+      +-----------+---+   |   |
_next:  +-->|       o------------->|       o------------->|       o-----------+   |
            +---------------+      +---------------+      +---------------+       |
                                                                                  |
                                                      +---------------------------+
                                                      |
                                                      |   +---------------+      +---------------+
                                         ATOM("n") ---+-->|       o------------->|      'n'      |
                                                          +---------------+      +---------------+
                                                          |      NIL      |      |      NIL      |
                                                          +-----------+---+      +-----------+---+
                                                          |       -   | X |      |       -   | X |
                                                          +-----------+---+      +-----------+---+
                                                          |       -       |      |       -       |
                                                          +---------------+      +---------------+

      FREE
        |   +---------------+      +---------------+      +---------------+      +---------------+
first:  +-->|       3       |      |       -       |      |       -       |      |       -       |
            +---------------+      +---------------+      +---------------+      +---------------+
rest:       |      NIL      |      |       -       |      |       -       |      |       -       |
            +-----------+---+      +-----------+---+      +-----------+---+      +-----------+---+
_prev:  +-----------o   | Z |<-------------o   | Z |<-------------o   | Z |<-------------o   | Z |<--+
        |   +-----------+---+      +-----------+---+      +-----------+---+      +-----------+---+   |
_next:  +-->|       o------------->|       o------------->|       o------------->|       o-----------+
            +---------------+      +---------------+      +---------------+      +---------------+

      PERM
        |   +---------------+      +---------------+      +---------------+      +---------------+
first:  +-->|       3       |      |       -       |      |       -       |      |       -       |
            +---------------+      +---------------+      +---------------+      +---------------+
rest:       |      NIL      |      |       -       |      |       -       |      |       -       |
            +-----------+---+      +-----------+---+      +-----------+---+      +-----------+---+
_prev:  +-----------o   | Z |<-------------o   | Z |<-------------o   | Z |<-------------o   | Z |<--+
        |   +-----------+---+      +-----------+---+      +-----------+---+      +-----------+---+   |
_next:  +-->|       o------------->|       o------------->|       o------------->|       o-----------+
            +---------------+      +---------------+      +---------------+      +---------------+
````
Normally, cells are allocated from the `FREE` list. These cells are subject to garbage collection. They use phase-marker **0** or **1**, depending on the value of `MARK_PHASE`. The `FREE` list contains collectable cells available for allocation. When the `FREE` list is exhausted, a new batch of cells is allocated from system memory and linked into this list. When a `FREE` cell is allocated (and phase-marked), it is moved to the `FRESH` list.

Some cells, such as those used to represent symbolic constants (ATOMs), are *permanently* allocated. They are immune from garbage collection. They use phase-marker **X**. The `PERM` list contains permanent cells available for allocation. When the `PERM` list is exhausted, a new batch of cells is allocated from system memory and linked into this list.

### Garbage Collection Algorithm

The garbage collection algorithm is essentially as follows:

 1. Move all cells from `FRESH` to `AGED`, and swap phase-markers (even/odd)
 2. Move `root` cell from `AGED` to `SCAN` and mark it
 3. While there are cells in `SCAN`
     1. Move a cell from `SCAN` to `FRESH`
     2. Scan the cell, moving reachable cells from `AGED` to `SCAN` and mark them
 4. Move remaining `AGED` cells to `FREE` (they're unreachable)

Note that allocation from the `FREE` list may continue concurrent with garbage collection. All `FREE` cells allocated after step 1 of the GC algorithm will be marked with the new/current phase-marker, as will all cells added to the `SCAN` list during GC. All cells moved from `FRESH` to `AGED` in step 1 will (initially) be marked with the old/previous phase-marker.

#### Start New GC Pass

When normal (collectable) cells are allocated, they are marked with the current phase-marker and moved onto the `FRESH` list. The GC process begins with moving all the `FRESH` cells to `AGED`, and swapping the phase-markers (starting a new phase). At this point, cells marked with the old/previous phase-marker are candidates for garbage collection, if they are not reachable from the current root cell. Subsequent allocations will be marked with the new/current phase-marker and moved to `FRESH`.

After the GC phase-swap, the GC lists could look like this:
````
PREV_PHASE = 1
MARK_PHASE = 0

      AGED                    root
        |   +---------------+  |   +---------------+      +---------------+
first:  +-->|       2       |  +-->|      NIL      |      |       o---------------+
            +---------------+      +---------------+      +---------------+       |
rest:       |      NIL      |      |       o------------->|      NIL      |       |
            +-----------+---+      +-----------+---+      +-----------+---+       |
_prev:  +-----------o   | Z |<-------------o   | 1 |<-------------o   | 1 |<--+   |
        |   +-----------+---+      +-----------+---+      +-----------+---+   |   |
_next:  +-->|       o------------->|       o------------->|       o-----------+   |
            +---------------+      +---------------+      +---------------+       |
                                                                                  |
                                                      +---------------------------+
                                                      |
                                                      |   +---------------+      +---------------+
                                         ATOM("n") ---+-->|       o------------->|      'n'      |
                                                          +---------------+      +---------------+
                                                          |      NIL      |      |      NIL      |
                                                          +-----------+---+      +-----------+---+
                                                          |       -   | X |      |       -   | X |
                                                          +-----------+---+      +-----------+---+
                                                          |       -       |      |       -       |
                                                          +---------------+      +---------------+

      SCAN
        |   +---------------+
first:  +-->|       0       |
            +---------------+
rest:       |      NIL      |
            +-----------+---+
_prev:  +-----------o   | Z |<--+
        |   +-----------+---+   |
_next:  +-->|       o-----------+
            +---------------+

      FRESH
        |   +---------------+
first:  +-->|       0       |
            +---------------+
rest:       |      NIL      |
            +-----------+---+
_prev:  +-----------o   | Z |<--+
        |   +-----------+---+   |
_next:  +-->|       o-----------+
            +---------------+
````

#### Scan Reachable Cells

The first `AGED` cell to be scanned is the `root` cell, from which all other in-use cells can be reached. The `root` cell is moved from the `AGED` list to the `SCAN` list, and marked with the current phase-marker. So the `SCAN` list starts with exactly one cell to examine.

The main loop of the GC process moves cells one-at-a-time from the `SCAN` list to the `FRESH` list. The cell's `first` and `rest` pointers are then examined. If they refer to a collectable cell, the phase-marker of the referenced cell is examined. If the phase-marker is the current phase-marker, then the cell has already been determined to be in-use, otherwise that cell is moved from `AGED` to `SCAN` for subsequent consideration, and marked with the current phase-marker.

After the `root` cell is scanned, the GC lists could look like this:
````
PREV_PHASE = 1
MARK_PHASE = 0

      AGED
        |   +---------------+
first:  +-->|       0       |
            +---------------+
rest:       |      NIL      |
            +-----------+---+
_prev:  +-----------o   | Z |<--+
        |   +-----------+---+   |
_next:  +-->|       o-----------+
            +---------------+
                                                       
                                                          +---------------+      +---------------+
                                         ATOM("n") ---+-->|       o------------->|      'n'      |
                                                      |   +---------------+      +---------------+
                                                      |   |      NIL      |      |      NIL      |
                                                      |   +-----------+---+      +-----------+---+
                                                      |   |       -   | X |      |       -   | X |
                                                      |   +-----------+---+      +-----------+---+
                                                      |   |       -       |      |       -       |
                                                      |   +---------------+      +---------------+
                                                      |
                               +----------------------|----+
      SCAN                     |                      |    |
        |   +---------------+  |   +---------------+  |    |
first:  +-->|       1       |  |   |       o----------+    |
            +---------------+  |   +---------------+       |
rest:       |      NIL      |  +-->|      NIL      |       |
            +-----------+---+      +-----------+---+       |
_prev:  +-----------o   | Z |<-------------o   | 0 |<--+   |
        |   +-----------+---+      +-----------+---+   |   |
_next:  +-->|       o------------->|       o-----------+   |
            +---------------+      +---------------+       |
                                                           |
      FRESH                   root                         |
        |   +---------------+  |   +---------------+       |
first:  +-->|       1       |  +-->|      NIL      |       |
            +---------------+      +---------------+       |
rest:       |      NIL      |      |       o---------------+
            +-----------+---+      +-----------+---+
_prev:  +-----------o   | Z |<-------------o   | 0 |<--+
        |   +-----------+---+      +-----------+---+   |
_next:  +-->|       o------------->|       o-----------+
            +---------------+      +---------------+
````

Scanning the `root` cell put one cell on the `SCAN` list. The `first` of that cell is an Atom, and the `rest` is `NIL`. Only mutable Pairs and Actors (to reach their "state") are scanned for GC, so this GC pass will be done after scanning this cell (moving it to `FRESH`).

During the GC pass, additional cells may be allocated (from the `FREE` list). They will be marked with the current phase-marker and placed on the `FRESH` list, indicating they are in-use. In addition, whenever a cell is tranversed (e.g.: by `car` or `cdr`) its phase-marker is checked. If the cell is marked with the previous phase-marker it must be on the `AGED` list. If so, that cell is moved to `SCAN` and marked with the current phase-marker, because we know it is in-use.

#### Free Unreachable Cells

When the `SCAN` list is empty, we've marked and moved all reachable cells to `FRESH`, any remaining cells on the `AGED` list are unreachable. All remaining cells on the `AGED` list are moved to `FREE`, making them available for subsequent re-allocation.

In our example, there were no unreachable cells to collect. At the GC pass is completed, the the GC lists could look like this:
````
PREV_PHASE = 1
MARK_PHASE = 0

      AGED
        |   +---------------+
first:  +-->|       0       |
            +---------------+
rest:       |      NIL      |
            +-----------+---+
_prev:  +-----------o   | Z |<--+
        |   +-----------+---+   |
_next:  +-->|       o-----------+
            +---------------+

      SCAN
        |   +---------------+
first:  +-->|       0       |
            +---------------+
rest:       |      NIL      |
            +-----------+---+
_prev:  +-----------o   | Z |<--+
        |   +-----------+---+   |
_next:  +-->|       o-----------+
            +---------------+

                                                          +---------------+      +---------------+
                                         ATOM("n") ---+-->|       o------------->|      'n'      |
                                                      |   +---------------+      +---------------+
                                                      |   |      NIL      |      |      NIL      |
                                                      |   +-----------+---+      +-----------+---+
                                                      |   |       -   | X |      |       -   | X |
                                                      |   +-----------+---+      +-----------+---+
                                                      |   |       -       |      |       -       |
                                                      |   +---------------+      +---------------+
                                                      |
                                                      +----------------------+
      FRESH                   root                                           |
        |   +---------------+  |   +---------------+      +---------------+  |
first:  +-->|       2       |  +-->|      NIL      |      |       o----------+
            +---------------+      +---------------+      +---------------+
rest:       |      NIL      |      |       o------------->|      NIL      |
            +-----------+---+      +-----------+---+      +-----------+---+
_prev:  +-----------o   | Z |<-------------o   | 0 |<-------------o   | 0 |<--+
        |   +-----------+---+      +-----------+---+      +-----------+---+   |
_next:  +-->|       o------------->|       o------------->|       o-----------+
            +---------------+      +---------------+      +---------------+
````

#### Garbage Collection Summary

Garbage-collected memory cells are maintained in separate pools. The `FRESH` pool contains cells known to be in-use at some time after the beginning of the last GC pass. These are all marked with the current phase-marker. When a GC pass begins, all the `FRESH` cells are moved to `AGED` and the phase-markers flip, starting a new allocation phase. Note that the cells we moved are now all marked with the *previous* phase-marker.

The GC pass traverses each cell reachable (via `first` or `rest`) from the `root`. All reachable cells are updated with the *current* phase-marker, and moved to `FRESH`. At the end of the GC pass, any cells remaining in `AGED` are unreachable, so they are recycled by moving them to `FREE`.

The GC pools are maintained as double-linked lists through the `_prev` and `_next` pointers. Note that there is no "movement" or copying of cells. All cells remain at their original address, as they are linked into and out of GC pools. Also, new cells may be safely allocated (from the `FREE` list) while the GC algorithm is scanning `AGED` cells.

No fragmentation is possible, since all allocations are the same size (one cell). Garbage-collection may safely proceed concurrent with allocation and message-delivery. And all allocated addresses are stable.
