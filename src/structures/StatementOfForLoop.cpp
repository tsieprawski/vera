//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "StatementOfForLoop.h"
#include "IsTokenWithName.h"

#include <vector>
#include <map>
#include <algorithm>

#define IS_NOT_TOKEN "the first element of the collection is not a token of 'for' type."
#define WITHOUT_CONDITIONAL_ARGUMENTS "The statement not contain conditional arguments."
#define WITHOUT_STATEMENT_FOR_SCOPE "The statement not contain a scope associated."
#define TOKEN_NAME  "for"
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

namespace Vera
{
namespace Structures
{

bool
hasTokenNameLastSentence(const Statement& current, std::string tokenName)
{
  const Statement& partialStatement = current.statementSequence_.back();
  const Token& token = partialStatement.tokenSequence_.back();

  return IsTokenWithName(tokenName)(token) == true;
}

StatementOfForLoop::StatementOfForLoop(Statement& statement,
  Tokens::TokenSequence::const_iterator& it,
  Tokens::TokenSequence::const_iterator& end)
: StatementsBuilder(statement)
{
  initialize(it, end);
}

void
StatementOfForLoop::initialize(Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  Statement& current = getCurrentStatement();

  push(*it);
  ++it;

  addEachInvalidToken(it, end);

  Tokens::TokenSequence::const_iterator endArguments = std::find_if(it+1,
      end,
      EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

  IS_EQUAL_RETURN(it, end);

  if (endArguments < end)
  {
    ++endArguments;
  }

  //builder(current,it, endArguments);
  parseScope(it, endArguments);

  IS_EQUAL_RETURN(it, end);

  addEachInvalidToken(it, end);

  IS_EQUAL_RETURN(it, end);

  //builder(current,it, end);
  parseScope(it, end);

 // IS_EQUAL_RETURN(it, end);
 // parseScope(it, end);
}

const Statement&
StatementOfForLoop::getArgumentStatementFromConditionalSentence()
{
  if (getCurrentStatement().statementSequence_.size() == 0)
  {
    throw StatementsError(WITHOUT_CONDITIONAL_ARGUMENTS);
  }

  return getCurrentStatement().statementSequence_[0];
}

const Statement&
StatementOfForLoop::getStatementScope()
{
  if (getCurrentStatement().statementSequence_.size() < 2)
  {
    throw StatementsError(WITHOUT_STATEMENT_FOR_SCOPE);
  }

  return getCurrentStatement().statementSequence_[1];
}

bool
StatementOfForLoop::parseArguments(Tokens::TokenSequence::const_iterator& it,
     Tokens::TokenSequence::const_iterator& end)
{
  IS_EQUAL_RETURN_FALSE(it, end);
  addEachInvalidToken(it, end);
  IS_EQUAL_RETURN_FALSE(it, end);

  if (IsTokenWithName(LEFTPAREN_TOKEN_NAME)(*it) == false)
  {
    return false;
  }

  Statement& current = add();
  current.push(*it);

  Tokens::TokenSequence::const_iterator endArguments = std::find_if(it+1,
      end,
      EndsWithCorrectPattern(LEFTPAREN_TOKEN_NAME, RIGHTPAREN_TOKEN_NAME));

  for (int i = 0; i < 3; ++i)
  {
    IS_EQUAL_RETURN_FALSE(it, end);
    ++it;
    IS_EQUAL_RETURN_FALSE(it, end);
    StatementsBuilder partial(current);

    partial.builder(current, it, endArguments);
  }

  current.push(*it);
  ++it;
  return true;
}

bool
StatementOfForLoop::isValid(Tokens::TokenSequence::const_iterator it,
  Tokens::TokenSequence::const_iterator end)
{
  IS_EQUAL_RETURN_FALSE(it, end);

  if (it->name_.compare(TOKEN_NAME) != 0)
  {
    return false;
  }

  Tokens::TokenSequence::const_iterator itMatched = std::find_if(it+1,
    end,
    IsValidTokenForStatement());

  IS_EQUAL_RETURN_FALSE(itMatched, end);

  return itMatched->name_.compare(LEFTPAREN_TOKEN_NAME) == 0;
}

bool
StatementOfForLoop::create(Statement& statement,
    Tokens::TokenSequence::const_iterator& it,
    Tokens::TokenSequence::const_iterator& end)
{
  bool successful = false;

  if (isValid(it, end) == true)
  {
    StatementOfForLoop builder(StatementsBuilder(statement).add(), it, end);
    successful = true;
  }

  return successful;
}

} // Structures namespace
} // Vera namespace
