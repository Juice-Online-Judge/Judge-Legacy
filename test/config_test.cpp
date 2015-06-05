#include <gtest/gtest.h>

#include "../src/config.cpp"

class ConfigTest : public ::testing::Test {
public:
  ConfigTest();
  ~ConfigTest();

protected:
  Config config;
};

TEST_F(ConfigTest, it_should_set_and_get_file_name) {
  config.load_config("fixtures/config.json");
  EXPECT_EQ(config.get_filename(), "fixtures/config.json");
}
