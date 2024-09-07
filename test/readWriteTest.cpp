#include "gtest/gtest.h"

#include "../src/mini.h"

TEST(ReadWrite, WritingTest) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    mini::Object obj("resources/testRuntime.ini");
    bool result = obj.get_global().add_prop("testRuntime1", "value_for_testRuntime_1");
    EXPECT_TRUE(result);
    result = obj.get_global().add_section("subsection");
    EXPECT_TRUE(result);
    result = obj.get_section_from_path("subsection").add_prop("testRuntime2", "value_for_testRuntime_2");
    EXPECT_TRUE(result);
    result = obj.get_section_from_path("subsection").add_section("subsubsection");
    EXPECT_TRUE(result);
    result = obj.get_section_from_path("subsection/subsubsection").add_prop("testRuntime3", "value_for_testRuntime_3");
    EXPECT_TRUE(result);
    result = mini::write(obj);
    EXPECT_TRUE(result);
}

TEST(ReadWrite, ReadingTest) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    mini::Object obj = mini::read("resources/config.ini");
    auto result = obj.get_prop_from_path("test1");
    ASSERT_EQ("value_for_test_1", result);
}
