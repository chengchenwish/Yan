#include"gtest/gtest.h"
#include<fstream>
#include "../src/lexer.h"

 static std::string inputfile = "test.c";
class lexerTest :public testing::Test {

};
TEST(lexerTest, test_basic)
{
  std::ofstream f(inputfile);
  f<<"int main; long a; a += 1; a /=2; -=,*=";
  f.flush();
  f.close();
  using namespace Yan;
  lexer l(inputfile);
  Token t;
  l.scan(&t);
  std::vector<Token> tlist;
  std::vector<TokenType>expect {TokenType::T_INT,TokenType::T_IDENT,TokenType::T_SEMI,
  TokenType::T_LONG,TokenType::T_IDENT,TokenType::T_SEMI,TokenType::T_IDENT,
  TokenType::T_ASPLUS,TokenType::T_INTLIT,TokenType::T_SEMI,TokenType::T_IDENT,TokenType::T_ASSLASH,
  TokenType::T_INTLIT,TokenType::T_SEMI, TokenType::T_ASMINUS, TokenType::T_COMMA,TokenType::T_ASSTAR};
  while(t.type !=TokenType::T_EOF)
  {
    tlist.push_back(t);
    Info(t.tostring().c_str());
    
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
    
    ASSERT_TRUE(static_cast<uint32_t>(tlist[i].type) == static_cast<uint32_t>(expect[i]));
  }
  ASSERT_TRUE(tlist[7].getValue() == '\b');
  ASSERT_TRUE(tlist[11].getValue() == 'y');  
  ASSERT_TRUE(tlist[15].getValue() == 65); 
  ASSERT_TRUE(tlist[21].getText() == "hello world");


}
TEST(lexerTest, test_skipcomments)
{
  std::ofstream f(inputfile);
  f<<"//int main \n int a;";
  f.flush();
  f.close();
  using namespace Yan;
  lexer l(inputfile);
  Token t;
  l.scan(&t);
  std::vector<Token> tlist;
  std::vector<TokenType>expect {TokenType::T_COMMENTS,TokenType::T_INT,TokenType::T_IDENT,TokenType::T_SEMI};
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
