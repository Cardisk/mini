#include "gtest/gtest.h"

#include "../src/mini.h"

TEST(PropertyInsertion, GlobalInsertion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_prop("foo", "bar"));
    // testing the longest way
    ASSERT_EQ("bar", obj.get_global().get_prop("foo"));
    // testing the shortest way
    ASSERT_EQ("bar", obj.get_prop_from_path("foo"));
}

TEST(PropertyInsertion, NestedInsertion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection"));
    // testing the insertion
    ASSERT_EQ("subsection", obj.get_global().get_section("subsection").get_name());
    EXPECT_TRUE(obj.get_global().get_section("subsection").add_prop("foo", "bar"));
    // testing the longest way
    ASSERT_EQ("bar", obj.get_global().get_section("subsection").get_prop("foo"));
    // testing the shortest way
    ASSERT_EQ("bar", obj.get_prop_from_path("subsection/foo"));
}

TEST(PropertyCollision, GlobalCollision) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_prop("foo", "bar"));
    // testing the longest way
    ASSERT_EQ("bar", obj.get_global().get_prop("foo"));
    // testing the shortest way
    ASSERT_EQ("bar", obj.get_prop_from_path("foo"));
    // ----------
    EXPECT_FALSE(obj.get_global().add_prop("foo", "COLLISION"));
    // testing the longest way
    ASSERT_EQ("bar", obj.get_global().get_prop("foo"));
    // testing the shortest way
    ASSERT_EQ("bar", obj.get_prop_from_path("foo"));
}

TEST(PropertyCollision, NestedCollision) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection"));
    // testing the insertion
    ASSERT_EQ("subsection", obj.get_global().get_section("subsection").get_name());
    EXPECT_TRUE(obj.get_global().get_section("subsection").add_prop("foo", "bar"));
    // testing the longest way
    ASSERT_EQ("bar", obj.get_global().get_section("subsection").get_prop("foo"));
    // testing the shortest way
    ASSERT_EQ("bar", obj.get_prop_from_path("subsection/foo"));
    // ----------
    EXPECT_FALSE(obj.get_global().get_section("subsection").add_prop("foo", "COLLISION"));
    // testing the longest way
    ASSERT_EQ("bar", obj.get_global().get_section("subsection").get_prop("foo"));
    // testing the shortest way
    ASSERT_EQ("bar", obj.get_prop_from_path("subsection/foo"));
}

TEST(PropertyDeletion, LongWayDeletion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_prop("foo", "bar"));
    // testing the longest way
    ASSERT_EQ("bar", obj.get_global().get_prop("foo"));
    // testing the shortest way
    ASSERT_EQ("bar", obj.get_prop_from_path("foo"));
    // ----------
    EXPECT_TRUE(obj.get_global().del_prop("foo"));
    // testing the longest way
    EXPECT_THROW(obj.get_global().get_prop("foo"), std::out_of_range);
    // testing the shortest way
    EXPECT_THROW(obj.get_prop_from_path("foo"), std::out_of_range);
}

TEST(PropertyDeletion, ShortWayDeletion) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_prop("foo", "bar"));
    // testing the longest way
    ASSERT_EQ("bar", obj.get_global().get_prop("foo"));
    // testing the shortest way
    ASSERT_EQ("bar", obj.get_prop_from_path("foo"));
    // ----------
    EXPECT_TRUE(obj.del_prop_from_path("foo"));
    // testing the longest way
    EXPECT_THROW(obj.get_global().get_prop("foo"), std::out_of_range);
    // testing the shortest way
    EXPECT_THROW(obj.get_prop_from_path("foo"), std::out_of_range);
}

TEST(PropertyNotFound, GlobalNotFound) {
    mini::Object obj;
    // testing the longest way
    EXPECT_THROW(obj.get_global().get_prop("foo"), std::out_of_range);
    // testing the shortest way
    EXPECT_THROW(obj.get_prop_from_path("foo"), std::out_of_range);
}

TEST(PropertyNotFound, NestedNotFound) {
    mini::Object obj;
    EXPECT_TRUE(obj.get_global().add_section("subsection"));
    // testing the insertion
    ASSERT_EQ("subsection", obj.get_global().get_section("subsection").get_name());
    // testing the longest way
    EXPECT_THROW(obj.get_global().get_section("subsection").get_prop("foo"), std::out_of_range);
    // testing the shortest way
    EXPECT_THROW(obj.get_prop_from_path("subsection/foo"), std::out_of_range);
}
