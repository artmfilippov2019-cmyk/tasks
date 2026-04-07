#ifndef TASKS_TOKENIZER_H
#define TASKS_TOKENIZER_H

#include <set>
#include <string>
#include <vector>
#include "Token.h"

std::vector<Token> tokenize(const std::string& input, std::set<char>& variables, bool& success);

#endif
