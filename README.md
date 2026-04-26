# Project README

## Overview
Cmd_Excel is a multi-language programming environment designed to support various functionalities such as mathematical operations, input/output, system calls, and basic threading. The project includes a set of libraries and tools that allow users to write scripts in different languages.

## Features
- **Mathematical Operations**: Supports basic arithmetic operations.
- **Input/Output**: Functions for reading from and writing to the console.
- **System Calls**: Support for basic system calls such as sleeping.
- **Basic Threading**: Provides functions for basic threading operations.
- **Custom Languages**: Allows users to write scripts in custom languages like .rex, .ll, and .omml.

## Project Structure
```
Cmd_Excel/
├── build/              # Compiled output (.exe files)
├── bin/                # Shared libraries (.so or .dll)
├── libs/               # Source files for the shared libraries
├── lib/                # Custom library support
├── code/               # Scripts in custom languages
├── data/               # Data files (e.g., .txt, dumped files)
├── assets/             # Images and sound files
├── src/                # Source code including Main.c
│   ├── Main.c          # Entry point
│   └── *.h             # Standalone header-based C-files
├── Makefile.linux      # Linux build configuration
├── Makefile.windows    # Windows build configuration
├── Makefile.wine       # Wine build configuration for Windows cross-compilation
├── Makefile.web        # Emscripten build configuration for WebAssembly
└── README.md           # This file
```

### Prerequisites
- **C/C++ Compiler and Debugger**: GCC, Clang
- **Make Utility**
- **Standard Development Tools**
- **Libraries Needed**: Depending on the target platform (Windows, Linux), specific libraries might be required.

## Build & Run
The project uses Makefiles to manage builds across different platforms. Below are the commands for building and running the project:

### Build Process

#### Linux
```sh
cd Cmd_Excel
make -f Makefile.linux all
```

For a clean build:
```sh
make -f Makefile.linux clean
make -f Makefile.linux all
```

To build libraries specifically:
```sh
make -f Makefile.linux cleanlib
make -f Makefile.linux lib
```

#### Windows
```sh
cd Cmd_Excel
make -f Makefile.windows all
```

For a clean build:
```sh
make -f Makefile.windows clean
make -f Makefile.windows all
```

#### Wine (Cross-compiling for Windows)
```sh
cd Cmd_Excel
make -f Makefile.wine all
```

For a clean build:
```sh
make -f Makefile.wine clean
make -f Makefile.wine all
```

#### WebAssembly (Emscripten)
```sh
cd Cmd_Excel
make -f Makefile.web all
```

For a clean build:
```sh
make -f Makefile.web clean
make -f Makefile.web all
```

### Execution
To run the compiled executable, use `make` with the appropriate target:

#### Linux
```sh
make -f Makefile.linux exe
```

#### Windows (assuming the output is a .dll or .exe)
```sh
make -f Makefile.windows exe
```

#### Wine
```sh
make -f Makefile.wine exe
```

#### WebAssembly
```sh
make -f Makefile.web exe
```

The `README.md` file and `LICENCE` are also included in the project structure.