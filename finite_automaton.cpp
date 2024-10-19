#include "lexical_analyzer.h"
#include <iostream>
#include <vector>
#include <cctype>
#include <set>

// Множина зарезервованих слів
std::set<std::string> reservedWords = {"mov", "add", "sub", "jmp"};

std::vector<Token> analyzeWithFiniteAutomaton(const std::string &text) {
    std::vector<Token> tokens;
    std::string currentToken;
    State currentState = START;

    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];

        switch (currentState) {
            case START:
                if (isdigit(c)) {
                    currentState = IN_NUMBER;
                    currentToken += c;
                } else if (c == '0' && (i + 1 < text.length()) && (text[i + 1] == 'x' || text[i + 1] == 'X')) {
                    currentState = IN_HEX_NUMBER;  // Шістнадцяткове число
                    currentToken += c;
                    i++;  // Пропускаємо 'x' або 'X'
                } else if (isalpha(c)) {
                    currentState = IN_IDENTIFIER;
                    currentToken += c;
                } else if (c == '#') {  // Препроцесор
                    currentState = IN_PREPROCESSOR;
                    currentToken += c;
                } else if (c == '/' && i + 1 < text.length() && text[i + 1] == '*') {  // Багаторядковий коментар
                    currentState = IN_MULTILINE_COMMENT;
                    currentToken += "/*";
                    i++;  // Пропускаємо '*'
                } else if (c == '\"') {
                    currentState = IN_STRING_CONSTANT;
                    currentToken += c;
                } else if (c == '\'') {  // Символьна константа
                    currentState = IN_CHAR_CONSTANT;
                    currentToken += c;
                } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                    tokens.push_back({std::string(1, c), OPERATOR});  // Оператор
                } else if (c == ';') {  // Коментар або розділовий знак
                    currentToken += c;
                    currentState = IN_COMMENT;
                } else if (isspace(c)) {
                    // Пропускаємо пробіли
                } else if (c == ',' || c == '.') {
                    tokens.push_back({std::string(1, c), PUNCTUATION});  // Розділові знаки
                } else {
                    currentState = STATE_ERROR;
                }
                break;

            case IN_NUMBER:
                if (isdigit(c)) {
                    currentToken += c;
                } else if (c == '.' && isdigit(text[i + 1])) {  // Десяткове дробове число
                    currentToken += c;
                    currentState = IN_FLOAT;  // Новий стан для чисел з плаваючою крапкою
                } else {
                    tokens.push_back({currentToken, NUMBER_DECIMAL});
                    currentToken.clear();
                    currentState = START;
                    i--;  // Повертаємо символ назад для повторної обробки
                }
                break;

            case IN_FLOAT:
                if (isdigit(c)) {
                    currentToken += c;
                } else {
                    tokens.push_back({currentToken, NUMBER_DECIMAL});
                    currentToken.clear();
                    currentState = START;
                    i--;  // Повертаємо символ назад для повторної обробки
                }
                break;

            case IN_HEX_NUMBER:
                if (isxdigit(c)) {
                    currentToken += c;
                } else {
                    tokens.push_back({currentToken, NUMBER_HEX});
                    currentToken.clear();
                    currentState = START;
                    i--;  // Повертаємо символ назад
                }
                break;

            case IN_STRING_CONSTANT:
                currentToken += c;
                if (c == '\"') {
                    tokens.push_back({currentToken, STRING_CONSTANT});
                    currentToken.clear();
                    currentState = START;
                }
                break;

            case IN_CHAR_CONSTANT:
                currentToken += c;
                if (c == '\'') {
                    tokens.push_back({currentToken, CHAR_CONSTANT});
                    currentToken.clear();
                    currentState = START;
                }
                break;

            case IN_IDENTIFIER:
                if (isalnum(c)) {
                    currentToken += c;
                } else {
                    if (reservedWords.find(currentToken) != reservedWords.end()) {
                        tokens.push_back({currentToken, RESERVED_WORD});
                    } else {
                        tokens.push_back({currentToken, IDENTIFIER});
                    }
                    currentToken.clear();
                    currentState = START;
                    i--;  // Повертаємо символ назад для повторної обробки
                }
                break;

            case IN_COMMENT:
                currentToken += c;
                if (c == '\n') {  // Важливо правильно завершити коментар
                    tokens.push_back({currentToken, COMMENT});
                    currentToken.clear();
                    currentState = START;
                }
                break;

            case IN_MULTILINE_COMMENT:
                currentToken += c;
                if (c == '*' && i + 1 < text.length() && text[i + 1] == '/') {  // Закінчення багаторядкового коментаря
                    currentToken += '/';
                    tokens.push_back({currentToken, COMMENT});
                    currentToken.clear();
                    currentState = START;
                    i++;  // Пропускаємо '/'
                } else if (i == text.length() - 1) {  // Якщо коментар не закінчився
                    std::cerr << "Error: Unclosed multiline comment\n";
                    currentState = STATE_ERROR;
                }
                break;

            case IN_PREPROCESSOR:
                if (isalnum(c) || c == '_' || c == ' ' || c == '#' || c == '<' || c == '>' || c == '/' || c == '.') {
                    currentToken += c;
                } else {
                    tokens.push_back({currentToken, PREPROCESSOR});
                    currentToken.clear();
                    currentState = START;
                    i--;  // Повертаємо символ назад для повторної обробки
                }
                break;

            case STATE_ERROR:
                std::cerr << "Lexical error at character: " << c << "\n";
                tokens.push_back({std::string(1, c), LEXEME_ERROR});
                currentState = START;
                break;

            default:
                throw std::runtime_error("Unknown state detected!");

        }
    }

    // Додаємо останній токен, якщо він існує
    if (!currentToken.empty()) {
        if (reservedWords.find(currentToken) != reservedWords.end()) {
            tokens.push_back({currentToken, RESERVED_WORD});
        } else {
            tokens.push_back({currentToken, IDENTIFIER});
        }
    }

    return tokens;
}
