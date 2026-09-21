# vecmat

A small C library for vectors and matrices, built from scratch.

Vectors are implemented. Matrix support is planned.

## Build

```sh
make
```

On Windows with MinGW:

```powershell
mingw32-make.exe
```

Or directly:

```sh
gcc -Iinclude -Wall -Wextra -std=c99 -o demo_vector examples/demo_vector.c src/vector.c -lm
```

## Run

```sh
./demo_vector
```

On Windows:

```powershell
.\demo_vector.exe
```

## Current tree

```text
vecmat/
|-- include/
|   `-- vecmat/
|       `-- vector.h       Public vector API
|-- src/
|   `-- vector.c           Vector implementation
|-- examples/
|   `-- demo_vector.c      Example program
|-- Makefile
|-- README.md
`-- .gitignore
```
