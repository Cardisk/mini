#include "gtest/gtest.h"

#include "../src/mini.h"

TEST(MiniTest, ObjectConstruction) {
    mini::Object obj("my_file.ini");
    ASSERT_EQ("my_file.ini", obj.get_file_path());
    ASSERT_EQ("global", obj.get_global().get_name());
}
