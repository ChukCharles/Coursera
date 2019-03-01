#pragma once

#include <sstream>
#include <vector>
using namespace std;

enum class TokenType {
  DATE, 		// 2018-01-19
  EVENT, 		// "smb birthday"
  COLUMN,  		// strings: 'date' or 'event'
  LOGICAL_OP,   // AND, OR
  COMPARE_OP,   // >, >=, <, <=, !=, ==
  PAREN_LEFT,   // (
  PAREN_RIGHT,  // )
};

struct Token {
  const string value;
  const TokenType type;
};

vector<Token> Tokenize(istream& cl);
