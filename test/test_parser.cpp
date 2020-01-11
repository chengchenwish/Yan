#include"gtest/gtest.h"
class parserTest :public testing::Test {

};
TEST(parserTest, test)
{
  int a =8;
  ASSERT_TRUE(a==8);
  ASSERT_TRUE(a==89);
}
int main(int argc, char** argv) {
      testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
        
}
