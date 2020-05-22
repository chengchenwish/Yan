#include"gtest/gtest.h"
#include<fstream>
#include "../parser/lexer.h"
class lexerTest :public testing::Test {



};
TEST(lexerTest, test)
{
 std::string inputfile = "test.c";
  std::ofstream f(inputfile);
  f<<"int main";//() {"<<"\n" \
//  <<"int a; int b; char c; const d;\n"\
 // <<"static const long testcc;\n}";
  f.flush();
  f.close();
  using namespace Yan;
  lexer l(inputfile);
  Token t;
  l.scan(&t);
  std::vector<Token> tlist;
  std::vector<TokenType>expect {TokenType::T_INT,TokenType::T_IDENT};
  while(t.type !=TokenType::T_EOF)
  {
    tlist.push_back(t);
    
    l.scan(&t);
  }
  ASSERT_TRUE(tlist.size()== expect.size());

  for (int i= 0;i<tlist.size();i++)
  {
    ASSERT_TRUE(static_cast<uint32_t>(tlist[i].type) == static_cast<uint32_t>(expect[i]));
  }



}
