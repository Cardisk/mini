#include <iostream>

#include "mini.h"

using namespace mini;

int main(void) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    Object config = read("resources/config.ini");
    std::cout << config.get_prop_from_path("test1") << std::endl;
    std::cout << config.get_global().get_prop("test1") << std::endl;
    std::cout << config.get_prop_from_path("subsection/test2") << std::endl;
    std::cout << config.get_global().get_section("subsection").get_prop("test2") << std::endl;
    std::cout << config.get_section_from_path("subsection/subsubsection").get_name() << " under subsection" << std::endl;
    std::cout << config.get_global().get_section("subsection").get_section("subsubsection").get_name() << " under subsection" << std::endl;
    std::cout << config.get_prop_from_path("subsection/foo/test3") << std::endl;
    std::cout << config.get_prop_from_path("subsection/foo/test4") << std::endl;

    config.set_file_path("resources/runtime.ini");
    write(config);
    return 0;
}
