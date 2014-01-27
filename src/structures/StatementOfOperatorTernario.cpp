//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfOperatorTernario.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define WITHOUT_STATEMENT_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "namespace"
#define COLON_TOKEN_NAME  "colon"
#define QUESTION_MARK_TOKEN_NAME  "question_mark"
#define LEFTPAREN_TOKEN_NAME  "leftparen"
#define RIGHTPAREN_TOKEN_NAME  "rightparen"
#define LEFTBRACE_TOKEN_NAME  "leftbrace"
#define RIGHTBRACE_TOKEN_NAME  "rightbrace"
#define SEMICOLON_TOKEN_NAME  "semicolon"
#define IS_EQUAL_RETURN(left, right) \
  {\
    if (left == right) \
    { \
      return;\
    }\
  }
#define IS_EQUAL_RETURN_FALSE(left, right) \
  {\
    if (left == right) \
    { \
      return false;\
    }\
  }
#define IS_EQUAL_BREAK(left, right) \
  {\
    if (left == right) \
    { \
      break;\
    }\
  }

namespace Vera
{
namespace Structures
{

StatementOfOperatorTernario::StatementOfOperatorTernario(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  initialize(it, end);
}

void
StatementOfOperatorTernario::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Tokens::TokenSequence::const_iterator endMatched = std::find_if(it,
      end, IsTokenWithName(SEMICOLON_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator colonMatched = std::find_if(it,
      endMatched, IsTokenWithName(COLON_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator questionMarkMatched = std::find_if(it,
      endMatched, IsTokenWithName(QUESTION_MARK_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator first;
  Tokens::TokenSequence::const_iterator second;


  first = questionMarkMatched;
  second = colonMatched;


  Statement& current = getCurrentStatement();

  {
    push(*it);
    ++it;

    StatementsBuilder partial(current);

    partial.builder(partial.getCurrentStatement(), it, second);
  }

  {
    StatementsBuilder partial(current);

    push(*second);
    ++it;
    partial.builder(partial.getCurrentStatement(), it, endMatched);
  }

  IS_EQUAL_RETURN(it, end);
  //--it;
  //IS_EQUAL_RETURN(it, end);
 // push(*it);
}

const Statement&
StatementOfOperatorTernario::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_STATEMENT_SCOPE);
  }

  return getCurrentStatement().statementSequence_[0];
}

bool
StatementOfOperatorTernario::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  if (it->name_.compare(COLON_TOKEN_NAME) != 0 && it->name_.compare(QUESTION_MARK_TOKEN_NAME) != 0)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator endMatched = std::find_if(it,
      end, IsTokenWithName(SEMICOLON_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator colonMatched = std::find_if(it,
      endMatched, IsTokenWithName(COLON_TOKEN_NAME));

  Tokens::TokenSequence::const_iterator questionMarkMatched = std::find_if(it,
      endMatched, IsTokenWithName(QUESTION_MARK_TOKEN_NAME));

  return (questionMarkMatched < colonMatched && colonMatched < endMatched && questionMarkMatched < endMatched);
}


bool
StatementOfOperatorTernario::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfOperatorTernario builder(statement, it, end);
  }

  return successful;
}

} // Vera namespace
} // Structures namespace
