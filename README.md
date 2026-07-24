# SwiftGrep

**Author:** Luke Young  
**Language Standard:** C++20

SwiftGrep is a cross-platform command-line text searching utility written in modern C++. It is inspired by the Unix `grep` utility and is being developed as a learning project focused on writing clean, portable, and efficient C++.

## Command Syntax

`swiftGrep [OPTIONS] SEARCH_TEXT PATH...`

## Current Features

- Search text files for matching strings
- Command-line interface
- Basic command-line argument validation
- Multiple file support
- Recursive directory traversal (-r)

Supported flags:
- `-i` Case-insensitive search
- `-v` Invert matches
- `-c` Count matching lines
- `-l` Display line numbers
- `-f` Display file names
- `-r` Recursively search directories

## Development Roadmap

### Completed
- [x] Basic file reader
- [x] Basic string search
- [x] Command-line argument support
- [x] Display file name and line number
- [x] Argument validation
- [x] Case-insensitive searching 
- [x] Multiple file support
- [x] Support multiple command-line flags simultaneously
- [x] Recursive directory searching

### Planned
- [ ] Unit tests
- [ ] Performance optimization
- [ ] Cross-platform build system (CMake)
- [ ] Continuous Integration (GitHub Actions)
- [ ] Version 1.0 release

## Usage

### Basic search

```bash
swiftGrep hello file.txt
```

### Search multiple files

```bash
swiftGrep hello file1.txt file2.txt file3.txt
```

### Case-insensitive search

```bash
swiftGrep -i hello file.txt
```
### Recursive directory search

```bash
swiftGrep -r hello test/
```
> **Note:** `-r` searches all regular files within the specified directory and its subdirectories. Directories that cannot be accessed due to permissions are skipped automatically.

### Combine flags

```bash
swiftGrep -i -l -f hello file1.txt file2.txt
```

### Search for text beginning with `-`

```bash
swiftGrep -- -x file.txt
```

> **Note:** `--` marks the end of command-line options. Any argument following `--` is treated as search text, even if it begins with a hyphen (`-`).

