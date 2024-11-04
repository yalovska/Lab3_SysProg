#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <string>
#include <vector>
#include <set>

// Типи лексем
enum class LexemeType {
    NUMBER_DECIMAL,
    NUMBER_HEX,
    STRING_CONSTANT,
    CHAR_CONSTANT,
    COMMENT,
    RESERVED_WORD,
    OPERATOR,
    PUNCTUATION,
    IDENTIFIER,
    LEXEME_ERROR,
    PREPROCESSOR
};

enum class State {
    START,
    IN_NUMBER,
    IN_FLOAT,
    IN_HEX_NUMBER,
    IN_STRING_CONSTANT,
    IN_CHAR_CONSTANT,
    IN_IDENTIFIER,
    IN_COMMENT,
    IN_MULTILINE_COMMENT,
    IN_PREPROCESSOR,
    STATE_ERROR
};

struct Token {
    std::string value;
    LexemeType type;
};

// Оголошення змінної reservedWords як extern
extern std::set<std::string> reservedWords;

// Оголошення функцій для аналізу тексту
std::vector<Token> analyzeWithRegex(const std::string &text);
std::vector<Token> analyzeWithAutomaton(const std::string &text);

#endif // LEXICAL_ANALYZER_H
