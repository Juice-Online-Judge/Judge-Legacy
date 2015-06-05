#include <gtest/gtest.h>

#include "../src/child_error_status.cpp"

TEST(ChildErrorStatusTest, it_should_determied_status_code) {
  EXPECT_FALSE(is_error_status(1));
  EXPECT_FALSE(is_error_status(0));
  EXPECT_TRUE(is_error_status(80));
  EXPECT_TRUE(is_error_status(81));
}

TEST(ChildErrorStatusTest, it_should_return_error_msg) {
  EXPECT_EQ(get_error_message(INPUT_FILE_NOT_FOUND_ERROR), "Could not found input file");
  EXPECT_EQ(get_error_message(FILE_OPEN_ERROR), "Could not open file");
  EXPECT_EQ(get_error_message(DUP2_ERROR), "dup2 error");
  EXPECT_EQ(get_error_message(EXEC_ERROR), "exec error");
}
