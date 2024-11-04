#include "lexical_analyzer.h"
#include <regex>
#include <vector>
#include <set>

std::set<std::string> reservedWords = {"mov", "add", "sub", "jmp"};

std::vector<Token> analyzeWithRegex(const std::string &text) {
    std::vector<Token> tokens;

    // Регулярні вирази для різних лексем
    std::regex comment_regex(R"(//[^\n]*|/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)");
    std::regex preprocessor_regex(R"(#include\s*<[^>]+>)");
    std::regex float_number_regex(R"(\b\d+\.\d+\b)");
    std::regex hex_number_regex(R"(0x[0-9a-fA-F]+)");
    std::regex decimal_number_regex(R"(\b\d+\b)");  // Додано оголошення decimal_number_regex
    std::regex string_constant_regex(R"("([^"\\]|\\.)*")");
    std::regex char_constant_regex(R"('([^'\\]|\\.)')");
    std::regex identifier_regex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");
    std::regex punctuation_regex(R"([,;.\(\)])");
    std::regex operator_regex(R"([+\-*/])");

    // Комбінований регулярний вираз для пошуку лексем
    std::regex combined_regex(
            R"((//[^\n]*|/\*[^*]*\*+(?:[^/*][^*]*\*+)*/)|(#include\s*<[^>]+>)|(\b\d+\.\d+\b)|(0x[0-9a-fA-F]+)|"
            R"(\b\d+\b)|("([^"\\]|\\.)*")|('([^'\\]|\\.)')|(\b[a-zA-Z_][a-zA-Z0-9_]*\b)|([,;.\(\)])|([+\-*/]))");

    auto words_begin = std::sregex_iterator(text.begin(), text.end(), combined_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        const std::smatch &match = *i; // Змінено на const std::smatch& для Clang-Tidy
        std::string lexeme = match.str();

        if (std::regex_match(lexeme, comment_regex)) {
            tokens.push_back({lexeme, LexemeType::COMMENT});
        } else if (std::regex_match(lexeme, preprocessor_regex)) {
            tokens.push_back({lexeme, LexemeType::PREPROCESSOR});
        } else if (std::regex_match(lexeme, hex_number_regex)) {
            tokens.push_back({lexeme, LexemeType::NUMBER_HEX});
        } else if (std::regex_match(lexeme, float_number_regex)) {
            tokens.push_back({lexeme, LexemeType::NUMBER_DECIMAL});
        } else if (std::regex_match(lexeme, decimal_number_regex)) {
            tokens.push_back({lexeme, LexemeType::NUMBER_DECIMAL});
        } else if (std::regex_match(lexeme, string_constant_regex)) {
            tokens.push_back({lexeme, LexemeType::STRING_CONSTANT});
        } else if (std::regex_match(lexeme, char_constant_regex)) {
            tokens.push_back({lexeme, LexemeType::CHAR_CONSTANT});
        } else if (std::regex_match(lexeme, identifier_regex)) {
            if (reservedWords.find(lexeme) != reservedWords.end()) {
                tokens.push_back({lexeme, LexemeType::RESERVED_WORD});
            } else {
                tokens.push_back({lexeme, LexemeType::IDENTIFIER});
            }
        } else if (std::regex_match(lexeme, punctuation_regex)) {
            tokens.push_back({lexeme, LexemeType::PUNCTUATION});
        } else if (std::regex_match(lexeme, operator_regex)) {
            tokens.push_back({lexeme, LexemeType::OPERATOR});
        } else {
            tokens.push_back({lexeme, LexemeType::LEXEME_ERROR});
        }
    }

    return tokens;
}
