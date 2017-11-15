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
Y                                    |        |         |         | x
