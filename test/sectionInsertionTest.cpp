#include "gtest/gtest.h"

#include "../src/mini.h"

TEST(SectionInsertion, GlobalSectionInsertion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("Foo"));
    ASSERT_EQ("Foo", obj.get_section_from_path("Foo").get_name()); 
}

TEST(SectionInsertion, SectionNestingInsertion) {
    mini::Object obj;
    obj.get_global().add_section("Foo");
    EXPECT_TRUE(obj.get_section_from_path("Foo").add_section("Bar"));
    ASSERT_EQ("Bar", obj.get_section_from_path("Foo/Bar").get_name()); 
}
