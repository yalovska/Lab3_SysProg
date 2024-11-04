#include "lexical_analyzer.h"
#include <iostream>
#include <vector>
#include <set>
#include <cctype>

std::vector<Token> analyzeWithAutomaton(const std::string &text) {
    std::vector<Token> tokens;
    std::string currentToken;
    State currentState = State::START;

    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];

        switch (currentState) {
            case State::START:
                if (isdigit(c)) {
                    currentToken += c;
                    if (c == '0' && (i + 1 < text.length()) && (text[i + 1] == 'x' || text[i + 1] == 'X')) {
                        currentState = State::IN_HEX_NUMBER;
                        currentToken += text[++i];  // Додаємо 'x' і переходимо до шестнадцяткового числа
                    } else {
                        currentState = State::IN_NUMBER;
                    }
                } else if (isalpha(c)) {
                    currentToken += c;
                    currentState = State::IN_IDENTIFIER;
                } else if (c == '#') {
                    currentToken += c;
                    currentState = State::IN_PREPROCESSOR;
                } else if (c == '/' && i + 1 < text.length()) {
                    if (text[i + 1] == '/') {
                        currentToken += "//";
                        currentState = State::IN_COMMENT;
                        i++;
                    } else if (text[i + 1] == '*') {
                        currentToken += "/*";
                        currentState = State::IN_MULTILINE_COMMENT;
                        i++;
                    }
                } else if (c == '\"') {
                    currentToken += c;
                    currentState = State::IN_STRING_CONSTANT;
                } else if (c == '\'') {
                    currentToken += c;
                    currentState = State::IN_CHAR_CONSTANT;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    tokens.push_back({std::string(1, c), LexemeType::OPERATOR});
                } else if (c == ',' || c == ';' || c == '.') {
                    tokens.push_back({std::string(1, c), LexemeType::PUNCTUATION});
                } else if (isspace(c)) {
                    // Пропускаємо пробіли
                } else {
                    currentState = State::STATE_ERROR;
                }
                break;

            case State::IN_NUMBER:
                if (isdigit(c)) {
                    currentToken += c;
                } else if (c == '.' && isdigit(text[i + 1])) {
                    currentToken += c;
                    currentState = State::IN_FLOAT;
                } else {
                    tokens.push_back({currentToken, LexemeType::NUMBER_DECIMAL});
                    currentToken.clear();
                    currentState = State::START;
                    i--;  // Повертаємо один символ назад
                }
                break;

            case State::IN_FLOAT:
                if (isdigit(c)) {
                    currentToken += c;
                } else {
                    tokens.push_back({currentToken, LexemeType::NUMBER_DECIMAL});
                    currentToken.clear();
                    currentState = State::START;
                    i--;
                }
                break;

            case State::IN_HEX_NUMBER:
                if (isxdigit(c)) {
                    currentToken += c;
                } else {
                    tokens.push_back({currentToken, LexemeType::NUMBER_HEX});
                    currentToken.clear();
                    currentState = State::START;
                    i--;
                }
                break;

            case State::IN_STRING_CONSTANT:
                currentToken += c;
                if (c == '\"') {
                    tokens.push_back({currentToken, LexemeType::STRING_CONSTANT});
                    currentToken.clear();
                    currentState = State::START;
                }
                break;

            case State::IN_CHAR_CONSTANT:
                currentToken += c;
                if (c == '\'') {
                    tokens.push_back({currentToken, LexemeType::CHAR_CONSTANT});
                    currentToken.clear();
                    currentState = State::START;
                }
                break;

            case State::IN_IDENTIFIER:
                if (isalnum(c)) {
                    currentToken += c;
                } else {
                    if (reservedWords.find(currentToken) != reservedWords.end()) {
                        tokens.push_back({currentToken, LexemeType::RESERVED_WORD});
                    } else {
                        tokens.push_back({currentToken, LexemeType::IDENTIFIER});
                    }
                    currentToken.clear();
                    currentState = State::START;
                    i--;
                }
                break;

            case State::IN_COMMENT:
                currentToken += c;
                if (c == '\n') {
                    tokens.push_back({currentToken, LexemeType::COMMENT});
                    currentToken.clear();
                    currentState = State::START;
                }
                break;

            case State::IN_MULTILINE_COMMENT:
                currentToken += c;
                if (c == '*' && i + 1 < text.length() && text[i + 1] == '/') {
                    currentToken += '/';
                    tokens.push_back({currentToken, LexemeType::COMMENT});
                    currentToken.clear();
                    currentState = State::START;
                    i++;
                }
                break;

            case State::IN_PREPROCESSOR:
                if (isalnum(c) || c == '<' || c == '>' || c == ' ') {
                    currentToken += c;
                } else {
                    tokens.push_back({currentToken, LexemeType::PREPROCESSOR});
                    currentToken.clear();
                    currentState = State::START;
                    i--;
                }
                break;

            case State::STATE_ERROR:
                std::cerr << "Lexical error at character: " << c << "\n";
                tokens.push_back({std::string(1, c), LexemeType::LEXEME_ERROR});
                currentState = State::START;
                break;
        }
    }

    if (!currentToken.empty()) {
        if (reservedWords.find(currentToken) != reservedWords.end()) {
            tokens.push_back({currentToken, LexemeType::RESERVED_WORD});
        } else {
            tokens.push_back({currentToken, LexemeType::IDENTIFIER});
        }
    }

    return tokens;
}
