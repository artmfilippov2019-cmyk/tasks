#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "Parser.h"
#include "tokenizer.h"

int main() {
	std::string line;
	if (!std::getline(std::cin, line)) {
		return 1;
	}

	std::set<char> variablesSet;
	bool tokenSuccess = true;
	std::vector<Token> tokens = tokenize(line, variablesSet, tokenSuccess);

	if (!tokenSuccess || tokens.empty()) {
		return 1;
	}

	std::map<char, bool> dummyValues;
	for (char v : variablesSet) dummyValues[v] = false;

	Parser syntaxChecker(tokens, dummyValues);
	syntaxChecker.run();

	if (syntaxChecker.error) {
		return 1;
	}

	std::vector<char> vars(variablesSet.begin(), variablesSet.end());

	for (char v : vars) {
		std::cout << v << " ";
	}
	std::cout << "Result" << std::endl;

	size_t n = vars.size();
	size_t rows = 1 << n;

	for (size_t i = 0; i < rows; ++i) {
		std::map<char, bool> currentValues;

		for (size_t j = 0; j < n; ++j) {
			bool val = (i >> (n - 1 - j)) & 1;
			currentValues[vars[j]] = val;
			std::cout << val << " ";
		}

		Parser evaluator(tokens, currentValues);
		bool result = evaluator.run();

		if (evaluator.error) {
			return 1;
		}

		std::cout << result << std::endl;
	}

	return 0;
}
