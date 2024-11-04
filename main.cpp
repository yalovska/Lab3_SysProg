#include <iostream>
#include <chrono>
#include <fstream>
#include "lexical_analyzer.h"

// Функція для виводу типу лексеми як текстового рядка
std::string lexemeTypeToString(LexemeType type) {
    switch (type) {
        case LexemeType::NUMBER_DECIMAL: return "NUMBER_DECIMAL";
        case LexemeType::NUMBER_HEX: return "NUMBER_HEX";
        case LexemeType::STRING_CONSTANT: return "STRING_CONSTANT";
        case LexemeType::CHAR_CONSTANT: return "CHAR_CONSTANT";
        case LexemeType::COMMENT: return "COMMENT";
        case LexemeType::RESERVED_WORD: return "RESERVED_WORD";
        case LexemeType::OPERATOR: return "OPERATOR";
        case LexemeType::PUNCTUATION: return "PUNCTUATION";
        case LexemeType::IDENTIFIER: return "IDENTIFIER";
        case LexemeType::LEXEME_ERROR: return "LEXEME_ERROR";
        case LexemeType::PREPROCESSOR: return "PREPROCESSOR";
        default: return "UNKNOWN";
    }
}

// Функція для читання тексту з файлу
std::string readFileContent(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не вдалося відкрити файл: " + filename + ". Переконайтесь, що файл існує у вказаному розташуванні.");
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}


int main(int argc, char *argv[]) {
    std::string filename;
    if (argc != 2) {
        std::cerr << "Ім'я файлу не надано. Використовую файл за замовчуванням 'input.txt'.\n";
        filename = "input.txt";  // Шлях до файлу за замовчуванням
    } else {
        filename = argv[1];
    }

    try {
        std::string programCode = readFileContent(filename);

        // Аналіз тексту за допомогою регулярних виразів
        std::cout << "Аналіз тексту за допомогою регулярних виразів:\n";
        auto start = std::chrono::high_resolution_clock::now();
        auto tokensRegex = analyzeWithRegex(programCode);
        auto end = std::chrono::high_resolution_clock::now();
        for (const auto &token : tokensRegex) {
            std::cout << "<" << token.value << " , " << lexemeTypeToString(token.type) << ">\n";
        }
        std::chrono::duration<double> duration = end - start;
        std::cout << "Час, витрачений на регулярні вирази: " << duration.count() << " секунд\n\n";

        // Аналіз тексту за допомогою скінченного автомату
        std::cout << "Аналіз тексту за допомогою скінченного автомату:\n";
        start = std::chrono::high_resolution_clock::now();
        auto tokensAutomaton = analyzeWithAutomaton(programCode);
        end = std::chrono::high_resolution_clock::now();
        for (const auto &token : tokensAutomaton) {
            std::cout << "<" << token.value << " , " << lexemeTypeToString(token.type) << ">\n";
        }
        duration = end - start;
        std::cout << "Час, витрачений на скінченний автомат: " << duration.count() << " секунд\n";

    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
