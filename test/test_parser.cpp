#include"gtest/gtest.h"
class parserTest :public testing::Test {

};
TEST(parserTest, test)
{
  int a =8;
  ASSERT_TRUE(a==8);
  ASSERT_TRUE(a==89);
}
