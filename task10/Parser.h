#ifndef TASKS_PARSER_H
#define TASKS_PARSER_H

#include <cstddef>
#include <map>
#include <vector>
#include "Token.h"

class Parser {
public:
	const std::vector<Token>& tokens;
	size_t pos;
	std::map<char, bool>& values;
	bool error;

	Parser(const std::vector<Token>& t, std::map<char, bool>& v);

	void match(TokenType expected);
	bool parseExpression();
	bool parseTerm();
	bool parseFactor();
	bool run();
};

#endif
