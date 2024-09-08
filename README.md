# mINI

An easy to use parser for ini files.

## Cloning

With `googletest` being a submodule, to correctly fetch everything run:

```bash
git clone --recurse-submodules <url>
```

## Quick start

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

`get_prop_from_path()` accepts a string with the path to the property separated by default (but customizable) by '/' assuming the last word as the property name.

`get_section_from_path()` does the same thing but returns a reference to the desired section.

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

