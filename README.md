# KComp Frontend Compiler

## Overview

KComp is a frontend compiler for a specific Kaleidoscope-like language. This project utilizes LLVM for generating Intermediate Representation (IR), along with Bison and Flex for parsing and lexical analysis, respectively.

## Prerequisites

Before compiling or using `KComp`, ensure you have the following dependencies installed on your machine:

- **LLVM 17**: For backend IR generation and optimization.

- **Clang++**: Required for compiling the project. Ensure it is compatible with LLVM 17.

- **Bison version 3.8.2**: For parsing the language syntax.

- **Flex version 2.6.4**: For lexical analysis.

Please refer to the respective official documentation for installation instructions for these dependencies.

## Compilation with Makefile

The project includes Makefiles for both macOS and Linux, designed to accommodate different operating systems. During the compilation process, two additional directories are created: `obj` for object files and `bin` for the executable file.

### Makefile Commands

Use the `-f` option with the `make` command to specify the appropriate Makefile for your system. For Linux, the command would be:

```bash

make -f Makefile.Linux [command]

```

#### Available Commands

- `make -f [Makefile] all`: Compiles necessary components, resulting in the `kcomp` executable being placed in the `bin` directory.

- `make -f [Makefile] clean`: Removes all generated files, including the contents of the `obj` and `bin` directories.

## Usage

After compilation, you can find the `kcomp` executable in the `bin` directory. To use it, execute the following command:

```bash

bin/kcomp [options] <file.k>

```

Where `<file.k>` is your Kaleidoscope-like source file. Available options include:

- `-p`: Enables debug traces in the parser.

- `-s`: Enables debug traces in the scanner.

### Example:

```bash

bin/kcomp -p -s example.k

```
## Additional Notes

KComp outputs the IR to `stderr`. To capture the compiler's output, redirect `stderr` to a `.ll` file:

```bash

bin/kcomp example.k 2> example.ll

```