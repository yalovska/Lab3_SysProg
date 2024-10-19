#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <string>
#include <vector>

// Оголошення типів лексем
enum LexemeType {
    NUMBER_DECIMAL,   // 0
    NUMBER_HEX,       // 1
    STRING_CONSTANT,  // 2
    CHAR_CONSTANT,    // 3
    COMMENT,          // 4
    RESERVED_WORD,    // 5
    OPERATOR,         // 6
    PUNCTUATION,      // 7
    IDENTIFIER,       // 8
    LEXEME_ERROR,     // 9
    PREPROCESSOR      // 10

};

// Стани для скінченого автомата
enum State {
    START,              // Початковий стан
    IN_NUMBER,          // Стан для десяткових чисел
    IN_FLOAT,
    IN_HEX_NUMBER,      // Стан для шістнадцяткових чисел
    IN_STRING_CONSTANT, // Стан для рядкових констант
    IN_CHAR_CONSTANT,   // Стан для символьних констант
    IN_IDENTIFIER,      // Стан для ідентифікаторів
    IN_COMMENT,         // Стан для однорядкових коментарів
    IN_MULTILINE_COMMENT, // Стан для багаторядкових коментарів
    IN_PREPROCESSOR,    // Стан для директив препроцесора
    STATE_ERROR         // Стан для помилок
};

// Структура Token для зберігання лексем і їх типів
struct Token {
    std::string value;
    LexemeType type;
};

// Функція для аналізу тексту
std::vector<Token> analyzeWithFiniteAutomaton(const std::string &text);

#endif // LEXICAL_ANALYZER_H
