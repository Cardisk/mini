#include <iostream>

#include "mini.h"

using namespace mini;

int main(void) {
    Object config = read("resources/config.ini");
    std::cout << config.global.get_prop("test1") << std::endl;
    std::cout << config.global.get_section("subsection").get_prop("test2") << std::endl;
    return 0;
}
