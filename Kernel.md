# Kernel

[Kernel](https://web.cs.wpi.edu/~jshutt/kernel.html) is a language in the LISP/Scheme family. This implementation, while faithful to the novel semantics of Kernel and consistent with John's specification, is **not** a conforming Kernel because it does not provide all of the required features defined in the Kernel standard (R<sup>-1</sup>RK). However, we believe it is a useful subset which illustrates our own novel actor-based approach to the implementation. If you are looking for efficiency, look elsewhere &#8212; this implementation is a nested meta-interpreter.

## Kernel Standard Library

The Kernel standard (R<sup>-1</sup>RK) defines the facilities provided by the "kernel standard environment", and the symbol names under which they are bound. The tables below list these facilities by section, in the order they are defined. Facilities marked with `o` are *optional* in the standard. The `Humus` column marks facilities defined by the Humus source in ["Semantic Extensibility with *Vau*"](http://www.dalnefre.com/wp/2011/12/semantic-extensibility-with-vau/). The `abe64` column marks facilities defined by the C source in "kernel.c". The `library` column marks facilities defined by the Kernel source in "library.knl".

### 4. Core types and primitive features

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

### 5. Core library features (I)

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
$sequence                            | x      | x       | x       | 
list                                 | x      | x       | x       | 
list*                                | x      |         |         | x
$lambda                              | x      | x       | x       | 
car                                  | x      |         | x       | 
cdr                                  | x      |         | x       | 
caar, cadr, ... cddddr               | x      |         |         | x
apply                                | x      |         | x       | 
$cond                                | x      |         |         | x
get-list-metrics                     | x      |         |         | 
list-tail                            | x      |         |         | 
encycle!                             | o      |         |         | 
map                                  | x      |         | x       | 
$let                                 | x      |         |         | x

### 6. Core library features (II)

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

### 8. Encapsulations

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
make-encapsulation-type              | x      | x       | x       | 

### 9. Promises

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
promise?                             | x      |         |         | 
force                                | x      |         |         | 
$lazy                                | x      |         |         | 
memoize                              | x      |         |         | 

### 12. Numbers

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
number?                              | x      |         | x       | 
integer?                             | x      |         |         | 
=?                                   | x      |         | x       | 
&lt;?                                   | x      |         | x       | 
&lt;=?                                  | x      |         | x       | 
&gt;=?                                  | x      |         | x       | 
&gt;?                                   | x      |         | x       | 
\+                                    | x      |         | x       | 
\*                                    | x      |         | x       | 
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

### 15. Ports

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
write                                | x      |         | x       | 

### Non-standard extensions

symbol                    | R<sup>-1</sup>RK  | Humus   | abe64   | library
-------------------------------------|--------|---------|---------|------------
newline                              |        |         | x       | 
$concurrent                          |        |         | x       | 
$parallel                            |        |         |         | x
time-now                             |        |         | x       | 
Y                                    |        |         |         | x

## Internal Structure

All values in the Kernel language are represented by actors in ABE. This includes numbers, Boolean `TRUE`/`FALSE`, pairs, and `NIL`. The C code implementation of Kernel features has visibility to the ABE-representation of Kernel objects. Several well-known Kernel objects are represented by statically-allocated actors. For example, the Kernel object with external representation `()` is represented at the ABE level by the actor `a_nil`, with behavior `null_type` and state `NIL`.

```
a_nil
 |     .---.---.
 +---> | o | / |
       .-|-.---.
         v
     null_type
```

The Kernel Boolean constants with external representations `#t` and `#f` are represented at the ABE level by the actors `a_true` and `a_false`, with behavior `bool_type` and state `BOOLEAN(TRUE)`/`BOOLEAN(FALSE)`.

```
a_true             a_false
 |     .---.---.    |     .---.---.
 +---> | o |-T-|    +---> | o |-F-|
       .-|-.---.          .-|-.---.
         v                  v
     bool_type          bool_type
```

Kernel pair values are represented at the ABE level by actors with `cons_type` behavior (or `pair_type` if immutable). The `car` and `cdr` of the pair are represented at the ABE level by a CONS cell, held in the state of the pair actor.

```
pair
 |     .---.---.
 +---> | o | o------+
       .-|-.---.    |
         v          v
     cons_type    +---+---+ cdr
                  | o | o------>
                  +-|-+---+
                car |
                    v
```

To illustrate a more complex example, the Kernel list with external representations `(1 2)` or `(1 . (2 . ()))`, is represented at the ABE level by a chain of actors.

```
list                                                        a_nil
 |     .---.---.                     .---.---.               |     .---.---.
 +---> | o | o------+          +---> | o | o------+          +---> | o | / |
       .-|-.---.    |          |     .-|-.---.    |          |     .-|-.---.
         v          v          |       v          v          |       v
     cons_type    +---+---+    |   cons_type    +---+---+    |   null_type
                  | o | o------+                | o | o------+
                  +-|-+---+                     +-|-+---+
                    |                             |
                    v                             v
                  .---.---.                     .---.---.
                  | o | 1 |                     | o | 2 |
                  .-|-.---.                     .-|-.---.
                    v                             v
              number_type                   number_type
```

Kernel environments are an encapsulted type which maps symbol names to values. The bindings in an environment are mutable, so different values may be returned at different times. Internally, the environment type stores bindings as an ABE-level map structure. Environments are chained together through their `parent` pointers (terminating with `NIL`). This example binds `x` to the Kernel value `()`, and `y` to the Kernel value `3`.

```
                          parent
                            ^
env                         |
 |     .---.---.          +-|-+---+          +---+---+               +---+---+
 +---> | o | o----------> | o | o----------> | o | o---------------> | o | / |
       .-|-.---.          +---+---+          +-|-+---+               +-|-+---+
         v                                     |                       |
      env_type                                 v                       v
                                             +---+---+               +---+---+     .---.---.
                                             | o | o-----> a_nil     | o | o-----> | o | 3 |
                                             +-|-+---+               +-|-+---+     .-|-.---.
                                               |                       |             v
                                               v                       v       number_type
                                         ATOM("x")               ATOM("y")
```
