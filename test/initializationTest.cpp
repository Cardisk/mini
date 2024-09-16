#include "gtest/gtest.h"

#include "../src/mini.h"

TEST(GeneralPurpose, ObjectConstruction) {
    mini::Object obj("my_file.ini");
    ASSERT_EQ("my_file.ini", obj.get_file_path());
    ASSERT_EQ("global", obj.get_global().get_name());
}

TEST(GeneralPurpose, SectionConstruction) {
    mini::Section obj;
    ASSERT_EQ("global", obj.get_name());
    ASSERT_EQ("", obj.get_path());
}
