![mini logo](https://github.com/cardisk/mini/blob/main/images/mini.svg)

## 

An easy to use parser for ini files.

> [!IMPORTANT]
> With `googletest` being a submodule, to correctly fetch everything run:
>```bash
>git clone --recurse-submodules <url>
>```

## Building

This project uses `cmake` build system. You can write manually all the commands
to bootstrap the repository, or  lazily you can just type `make` inside the
parent directory and it will prepare everything by itself.

Makefile targets:
- all: compiles both the example and test
- example: compiles only the example part
- test: compiles only the test part
- run_example: runs the example
- run_test: runs the tests
- clean: removes the build directories

## Quick start

> [!TIP]
>`get_prop_from_path` retrieves the property by searching inside the path
> provided. By default the separator is '/' but it's customizable.
> 
> `get_section_from_path` retrieves the sections (as a reference) by searching
> inside the path provided. By default the separator is '/' but it's customizable.

Reading an existing file:

```cpp
#include <iostream>
#include "mini.h"

// Optional.
using namespace mini;

int main(void) {
    Object file = read("path/to/file.ini");

    std::cout << file.get_prop_from_path("path/to/property"); 
    return 0;
}
```

Writing to a file:

```cpp
#include <iostream>
#include "mini.h"

// Optional.
using namespace mini;

int main(void) {
    Object file = Object("path/to/the/new/file.ini");

    file.get_global().add_prop("name", "value");
    file.get_global().add_section("name");
    file.get_section_from_path("name").add_prop("name", "value");

    write(file);
    return 0;
}
```
