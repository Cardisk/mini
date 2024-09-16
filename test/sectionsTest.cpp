#include "gtest/gtest.h"

#include "../src/mini.h"

TEST(SectionInsertion, GlobalInsertion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection"));
    // testing the longest way
    ASSERT_EQ("subsection", obj.get_global().get_section("subsection").get_name());
    // testing the shortest way
    ASSERT_EQ("subsection", obj.get_section_from_path("subsection").get_name());
}

TEST(SectionInsertion, NestedInsertion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection1"));
    // testing the insertion
    ASSERT_EQ("subsection1", obj.get_global().get_section("subsection1").get_name());
    EXPECT_TRUE(obj.get_global().get_section("subsection1").add_section("subsection2"));
    // testing the longest way
    ASSERT_EQ("subsection2", obj.get_global().get_section("subsection1").get_section("subsection2").get_name());
    // testing the shortest way
    ASSERT_EQ("subsection2", obj.get_section_from_path("subsection1/subsection2").get_name());
}

TEST(SectionCollision, GlobalCollision) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection"));
    // testing the longest way
    ASSERT_EQ("subsection", obj.get_global().get_section("subsection").get_name());
    // testing the shortest way
    ASSERT_EQ("subsection", obj.get_section_from_path("subsection").get_name());
    // ----------
    EXPECT_FALSE(obj.get_global().add_section("subsection"));
}

TEST(SectionCollision, NestedCollision) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection1"));
    // testing the longest way
    ASSERT_EQ("subsection1", obj.get_global().get_section("subsection1").get_name());
    // testing the shortest way
    ASSERT_EQ("subsection1", obj.get_section_from_path("subsection1").get_name());

    EXPECT_TRUE(obj.get_section_from_path("subsection1").add_section("subsection2"));
    // testing the longest way
    ASSERT_EQ("subsection2", obj.get_global().get_section("subsection1").get_section("subsection2").get_name());
    // testing the shortest way
    ASSERT_EQ("subsection2", obj.get_section_from_path("subsection1/subsection2").get_name());
    // ----------
    EXPECT_FALSE(obj.get_section_from_path("subsection1").add_section("subsection2"));
}

TEST(SectionDeletion, LongWayDeletion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection"));
    // testing the longest way
    ASSERT_EQ("subsection", obj.get_global().get_section("subsection").get_name());
    // testing the shortest way
    ASSERT_EQ("subsection", obj.get_section_from_path("subsection").get_name());
    // ----------
    EXPECT_TRUE(obj.get_global().del_section("subsection"));
    // testing the longest way
    EXPECT_THROW(obj.get_global().get_section("subsection"), std::out_of_range);
    // testing the shortest way
    EXPECT_THROW(obj.get_section_from_path("subsection"), std::out_of_range);
}

TEST(SectionDeletion, ShortWayDeletion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection"));
    // testing the longest way
    ASSERT_EQ("subsection", obj.get_global().get_section("subsection").get_name());
    // testing the shortest way
    ASSERT_EQ("subsection", obj.get_section_from_path("subsection").get_name());
    // ----------
    EXPECT_TRUE(obj.del_section_from_path("subsection"));
    // testing the longest way
    EXPECT_THROW(obj.get_global().get_section("subsection"), std::out_of_range);
    // testing the shortest way
    EXPECT_THROW(obj.get_section_from_path("subsection"), std::out_of_range);
}

TEST(SectionNotFound, GlobalNotFound) {
    mini::Object obj;
    // testing the longest way
    EXPECT_THROW(obj.get_global().get_section("subsection"), std::out_of_range);
    // testing the shortest way
    EXPECT_THROW(obj.get_section_from_path("subsection"), std::out_of_range);
}

TEST(SectionNotFound, NestedNotFound) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection1"));
    // testing the insertion
    ASSERT_EQ("subsection1", obj.get_global().get_section("subsection1").get_name());
    // testing the longest way
    EXPECT_THROW(obj.get_global().get_section("subsection1").get_section("subsection2"), std::out_of_range);
    // testing the shortest way
    EXPECT_THROW(obj.get_section_from_path("subsection1/subsection2"), std::out_of_range);
}
