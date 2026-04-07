#include "Parser.h"

Parser::Parser(const std::vector<Token>& t, std::map<char, bool>& v)
	: tokens(t), pos(0), values(v), error(false) {}

void Parser::match(TokenType expected) {
	if (pos >= tokens.size() || tokens[pos].type != expected) {
		error = true;
	} 
	else {
		pos++;
	}
}

bool Parser::parseExpression() {
	bool left = parseTerm();
	if (error) return false;

	while (pos < tokens.size() && tokens[pos].type == TOK_OR) {
		pos++;
		bool right = parseTerm();
		if (error) return false;
		left = left || right;
	}
	return left;
}

bool Parser::parseTerm() {
	bool left = parseFactor();
	if (error) return false;

	while (pos < tokens.size() && tokens[pos].type == TOK_AND) {
		pos++;
		bool right = parseFactor();
		if (error) return false;
		left = left && right;
	}
	return left;
}

bool Parser::parseFactor() {
	if (pos >= tokens.size()) {
		error = true;
		return false;
	}

	if (tokens[pos].type == TOK_NOT) {
		pos++;
		bool val = parseFactor();
		if (error) return false;
		return !val;
	}
	if (tokens[pos].type == TOK_LPAR) {
		pos++;
		bool val = parseExpression();
		if (error) return false;
		match(TOK_RPAR);
		return val;
	}
	if (tokens[pos].type == TOK_VAR) {
		char v = tokens[pos].varName;
		pos++;
		return values[v];
	}
	error = true;
	return false;
}

bool Parser::run() {
	bool res = parseExpression();
	if (!error && pos != tokens.size() - 1) {
		error = true;
	}
	return res;
}
