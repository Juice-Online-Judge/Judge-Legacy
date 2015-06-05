#include <gtest/gtest.h>

#include "../src/child_error_status.cpp"

TEST(ChildErrorStatusTEST, it_should_determied_status_code) {
  EXPECT_FALSE(is_error_status(1));
  EXPECT_FALSE(is_error_status(0));
  EXPECT_TRUE(is_error_status(80));
  EXPECT_TRUE(is_error_status(81));
}
