#include "gtest/gtest.h"

#include "../src/mini.h"

TEST(MiniTest, ObjectConstruction) {
    mini::Object ini;
    ini.set_file_path("my_file.ini");
    ASSERT_EQ("my_file.ini", ini.get_file_path());
    ASSERT_EQ("global", ini.get_global().get_name());
}
