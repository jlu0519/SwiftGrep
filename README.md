# SwiftGrep

SwiftGrep is a cross-platform, grep-inspired command-line text search utility written in C++20.

SwiftGrep is being developed as a learning project for modern C++, testing, filesystem operations, and performance optimization. It is not intended to be a drop-in replacement for GNU grep.

## Features

* Regular expression searching
* Case-insensitive and inverted matching
* Match counting and line-number output
* Multiple-file and recursive directory searching
* Combined command-line flags
* Linux and Windows support

## Build

### Requirements

* C++20-compatible compiler
* CMake 3.16 or newer
* Git

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

The executable will be created inside the build directory.

For an optimized release build:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

The executable will be created inside the build-release directory.

## Usage

```text
swiftgrep [OPTIONS] PATTERN PATH...
```

### Supported Flags

* `-i` Case-insensitive search
* `-v` Invert matches
* `-c` Count matching lines
* `-l` Display line numbers
* `-f` Display file names
* `-r` Recursively search directories

### Basic Search

```bash
swiftgrep hello file.txt
```

### Regex Search

```bash
swiftgrep '[0-9]+' file.txt
```

### Search Multiple Files

```bash
swiftgrep hello file1.txt file2.txt file3.txt
```

### Case-Insensitive Search

```bash
swiftgrep -i hello file.txt
```

### Recursive Directory Search

```bash
swiftgrep -r hello test/
```

> **Note:** `-r` searches regular files within the specified directory and its subdirectories. Directories that cannot be accessed are skipped.

### Combine Flags

```bash
swiftgrep -i -l -f hello file1.txt file2.txt
```

or

```bash
swiftgrep -ilf hello file1.txt file2.txt
```

### Search for Text Beginning with `-`

```bash
swiftgrep -- -x file.txt
```

> **Note:** `--` marks the end of command-line options. Any argument following `--` is treated as search text, even if it begins with a hyphen (`-`).

## Testing

Tests are written with GoogleTest and can be run with CTest:

```bash
ctest --test-dir build --output-on-failure
```

## Development Roadmap

### Completed

* [x] Basic file reader
* [x] Basic string search
* [x] Command-line argument support
* [x] Display file name and line number
* [x] Argument validation
* [x] Case-insensitive searching
* [x] Multiple file support
* [x] Support multiple command-line flags simultaneously
* [x] Recursive directory searching
* [x] Short combination flags, e.g. `-ilf`
* [x] Regex support
* [x] CMake setup
* [x] GoogleTest integration
* [x] Argument parsing unit tests
* [x] Search unit tests
* [x] v0.1.0 release
* [x] Performance profiling
* [x] Continuous Integration with GitHub Actions

### Planned

* [ ] Search performance optimization
* [ ] Multithreaded file searching


## License

SwiftGrep is licensed under the MIT License. See `LICENSE` for details.
