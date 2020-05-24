#include"gtest/gtest.h"
#include<fstream>
#include "../src/lexer.h"

 static std::string inputfile = "test.c";
class lexerTest :public testing::Test {

};
TEST(lexerTest, test)
{
  std::ofstream f(inputfile);
  f<<"int main";
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
TEST(lexerTest, test_scanchar)
{
  std::ofstream f(inputfile);
  f<<"char c,ff; c = '\b';  ff = 'y'; a ='\101'; char*s = \"hello world\";";
  f.flush();
  f.close();
  using namespace Yan;
  lexer l(inputfile);
  Token t;
  l.scan(&t);
  std::vector<Token> tlist;
  std::vector<TokenType>expect {TokenType::T_CHAR,TokenType::T_IDENT,TokenType::T_COMMA,
  TokenType::T_IDENT,TokenType::T_SEMI,TokenType::T_IDENT,TokenType::T_ASSIGN,TokenType::T_INTLIT,TokenType::T_SEMI,
  TokenType::T_IDENT,TokenType::T_ASSIGN,TokenType::T_INTLIT,TokenType::T_SEMI,TokenType::T_IDENT,
  TokenType::T_ASSIGN,TokenType::T_INTLIT,TokenType::T_SEMI,TokenType::T_CHAR,TokenType::T_STAR,
  TokenType::T_IDENT,  TokenType::T_ASSIGN,TokenType::T_STRLIT,TokenType::T_SEMI};
  while(t.type !=TokenType::T_EOF)
  {
    tlist.push_back(t);
    
    l.scan(&t);
  }
  ASSERT_TRUE(tlist.size()== expect.size());

  for (int i= 0;i<tlist.size();i++)
  {
    Info(tlist[i].tostring().c_str());
    ASSERT_TRUE(static_cast<uint32_t>(tlist[i].type) == static_cast<uint32_t>(expect[i]));
  }
  ASSERT_TRUE(tlist[7].getValue() == '\b');
  ASSERT_TRUE(tlist[11].getValue() == 'y');  
  ASSERT_TRUE(tlist[15].getValue() == 65); 
  ASSERT_TRUE(tlist[21].getText() == "hello world");


}
