#include "gtest/gtest.h"
#include <stdexcept>

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
    // this call can throw an error.
    EXPECT_NO_THROW(mini::write(obj));
}

TEST(ReadWrite, ReadingTest) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    mini::Object obj = mini::read("resources/config.ini");
    auto result = obj.get_prop_from_path("test1");
    ASSERT_EQ("value_for_test_1", result);
}

TEST(ReadWrite, ThrowingReading) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    EXPECT_THROW(mini::read("resources/throwing1.ini"), std::runtime_error);
    EXPECT_THROW(mini::read("resources/throwing2.ini"), std::runtime_error);
}
