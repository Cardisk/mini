#include "gtest/gtest.h"

#include "../src/mini.h"

TEST(Reading, Successful) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    mini::Object obj = mini::read("resources/config.ini");
    ASSERT_EQ("value_for_test_1", obj.get_prop_from_path("test1"));
    ASSERT_EQ("subsection", obj.get_section_from_path("subsection").get_name());
    ASSERT_EQ("value_for_test_2", obj.get_prop_from_path("subsection/test2"));
    ASSERT_EQ("subsubsection", obj.get_section_from_path("subsection/subsubsection").get_name());
    ASSERT_EQ("foo", obj.get_section_from_path("subsection/foo").get_name());
    ASSERT_EQ("value_for_test_3", obj.get_prop_from_path("subsection/foo/test3"));
    ASSERT_EQ("value_for_test_4", obj.get_prop_from_path("subsection/foo/test4"));
}

TEST(Reading, Malformed) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    EXPECT_THROW(mini::read("resources/throwing1.ini"), std::runtime_error);
    EXPECT_THROW(mini::read("resources/throwing2.ini"), std::runtime_error);
}

TEST(Writing, Writing) {
    // the path is viewed from the root of the project.
    // change here if something is not going to work.
    mini::Object out("resources/testRuntime.ini");
    EXPECT_TRUE(out.get_global().add_prop("testRuntime1", "value_for_testRuntime_1"));
    EXPECT_TRUE(out.get_global().add_section("subsection"));
    EXPECT_TRUE(out.get_section_from_path("subsection").add_prop("testRuntime2", "value_for_testRuntime_2"));
    EXPECT_TRUE(out.get_section_from_path("subsection").add_section("subsubsection"));
    EXPECT_TRUE(out.get_section_from_path("subsection/subsubsection").add_prop("testRuntime3", "value_for_testRuntime_3"));
    // this call can throw an error.
    EXPECT_NO_THROW(mini::write(out));
    // ----------
    mini::Object in = mini::read("resources/testRuntime.ini");
    ASSERT_EQ("value_for_testRuntime_1", in.get_prop_from_path("testRuntime1"));
    ASSERT_EQ("subsection", in.get_section_from_path("subsection").get_name());
    ASSERT_EQ("value_for_testRuntime_2", in.get_prop_from_path("subsection/testRuntime2"));
    ASSERT_EQ("subsubsection", in.get_section_from_path("subsection/subsubsection").get_name());
    ASSERT_EQ("value_for_testRuntime_3", in.get_prop_from_path("subsection/subsubsection/testRuntime3"));
}
