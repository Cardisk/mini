#include <iostream>

#include "mini.h"

using namespace mini;

int main(void) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    Object config = read("resources/config.ini");

    std::cout << "There are 2 ways of accessing the fields:" << std::endl << std::endl;

    // -------------
    std::string test1 = "test1";
    std::cout << "VALUE OF FIELD: " << test1 << std::endl;
    std::cout << "\tshortest way: ";
    std::cout << config.get_prop_from_path(test1) << std::endl;
    std::cout << "\tlongest way: ";
    std::cout << config.get_global().get_prop(test1) << std::endl;
    std::cout << std::endl;
    // -------------
    std::string test2 = "subsection/test2";
    std::cout << "VALUE OF FIELD: " << test2 << std::endl;
    std::cout << "\tshortest way: ";
    std::cout << config.get_prop_from_path(test2) << std::endl;
    std::cout << "\tlongest way: ";
    std::cout << config.get_global().get_section("subsection").get_prop("test2") << std::endl;
    std::cout << std::endl;
    // -------------
    std::string subsubsection = "subsection/subsubsection";
    std::cout << "SECTION NAME OF: " << subsubsection << std::endl;
    std::cout << "\tshortest way: ";
    std::cout << config.get_section_from_path(subsubsection).get_name() << std::endl;
    std::cout << "\tlongest way: ";
    std::cout << config.get_global().get_section("subsection").get_section("subsubsection").get_name() << std::endl;
    std::cout << std::endl;
    // -------------
    std::string test3 = "subsection/foo/test3";
    std::cout << "VALUE OF FIELD: " << test3 << std::endl;
    std::cout << "\t" << config.get_prop_from_path(test3) << std::endl;
    std::cout << std::endl;
    // -------------
    std::string test4 = "subsection/foo/test4";
    std::cout << "VALUE OF FIELD: " << test4 << std::endl;
    std::cout << "\t" << config.get_prop_from_path(test4) << std::endl;
    std::cout << std::endl;
    // -------------

    // rewriting the same file to see the differences.
    config.set_file_path("resources/runtime.ini");
    write(config);

    return 0;
}
