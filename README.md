John Shutt's ["Kernel"](https://web.cs.wpi.edu/~jshutt/kernel.html) language implemented on ABE (C) 64-bit runtime. A [previous implementation](https://github.com/dalnefre/kernel_abe) targeted 32-bit platforms.

This implementation is built in layers. The lowest layer is [ABE](ABE.md), an Actor-Based Environment written in C. The actor design for the Kernel interpreter is based on a proof-of-concept written in Humus, and described in ["Semantic Extensibility with *Vau*"](http://www.dalnefre.com/wp/2011/12/semantic-extensibility-with-vau/). In fact, most of the C code for the ABE actor behaviors is prefaced by corresponding Humus source (in comments). The result is an interpreter for a [Kernel-inspired](Kernel.md) language, which reads from source files and the interactive console.

## Using Docker

The [Docker](https://www.docker.com/) container environment provides a stable portable target platform for our project. Create a Docker image named `abe64` in which to compile and run Kernel.

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
