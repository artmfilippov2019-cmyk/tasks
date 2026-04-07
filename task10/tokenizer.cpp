#include "tokenizer.h"
#include <cctype>

std::vector<Token> tokenize(const std::string& input, std::set<char>& variables, bool& success) {
	std::vector<Token> tokens;
	size_t i = 0;
	success = true;

	while (i < input.length()) {
		if (std::isspace(static_cast<unsigned char>(input[i]))) {
			i++;
			continue;
		}

		if (input[i] == '(') {
			tokens.push_back({TOK_LPAR, 0});
			i++;
		}
		else if (input[i] == ')') {
			tokens.push_back({TOK_RPAR, 0});
			i++;
		} 
		else if (std::isalpha(static_cast<unsigned char>(input[i]))) {
			std::string word;
			while (i < input.length() && std::isalnum(static_cast<unsigned char>(input[i]))) {
				word += input[i];
				i++;
			}

			for (char c : word) {
				if (!std::isupper(static_cast<unsigned char>(c))) {
					success = false;
					return tokens;
				}
			}

			if (word == "AND") {
				tokens.push_back({TOK_AND, 0});
			} 
			else if (word == "OR") {
				tokens.push_back({TOK_OR, 0});
			} 
			else if (word == "NOT") {
				tokens.push_back({TOK_NOT, 0});
			} 
			else if (word.length() == 1) {
				tokens.push_back({TOK_VAR, word[0]});
				variables.insert(word[0]);
			} 
			else {
				success = false;
				return tokens;
			}
		} 
		else {
			success = false;
			return tokens;
		}
	}
	tokens.push_back({TOK_END, 0});
	return tokens;
}
