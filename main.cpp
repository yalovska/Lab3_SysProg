#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <regex>
#include <algorithm>
#include <cctype>

enum TokenType {
    DECIMAL_NUMBER, HEX_NUMBER, FLOAT_NUMBER, STRING_CONSTANT, CHAR_CONSTANT,
    PREPROCESSOR_DIRECTIVE, COMMENT, RESERVED_WORD, OPERATOR, DELIMITER,
    IDENTIFIER, LABEL, ERROR
};

struct Token {
    std::string value;
    TokenType type;
};

TokenType determineTokenTypeFA(const std::string& token);
TokenType determineTokenTypeRegex(const std::string& token);

bool isDecimalNumberFA(const std::string& token);
bool isHexNumberFA(const std::string& token);
bool isFloatNumberFA(const std::string& token);
bool isStringConstantFA(const std::string& token);
bool isCharConstantFA(const std::string& token);
bool isIdentifierFA(const std::string& token);
bool isLabelFA(const std::string& token);

bool isDecimalNumberRegex(const std::string& token);
bool isHexNumberRegex(const std::string& token);
bool isFloatNumberRegex(const std::string& token);
bool isStringConstantRegex(const std::string& token);
bool isCharConstantRegex(const std::string& token);
bool isLabelRegex(const std::string& token);

bool isReservedWord(const std::string& token);
bool isOperator(const std::string& token);
bool isDelimiter(const std::string& token);

std::vector<Token> tokenizeFA(const std::string& code);
std::vector<Token> tokenizeRegex(const std::string& code);

void displayTokens(const std::vector<Token>& tokens);

template <typename Func>
long long measureExecutionTime(Func func, const std::string& code) {
    auto start = std::chrono::high_resolution_clock::now();
    func(code);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    std::ifstream file("code.asm");
    if (!file.is_open()) {
        std::cerr << "Error opening file: code.asm. Please check if the file exists and is accessible." << std::endl;
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (code.empty()) {
        std::cerr << "File is empty or could not be read properly." << std::endl;
        return 1;
    }

    std::cout << "=== Finite Automata Tokenizer ===" << std::endl;
    auto faTime = measureExecutionTime(tokenizeFA, code);
    std::vector<Token> faTokens = tokenizeFA(code);

    std::cout << "\n=== Tokens (Finite Automata) ===" << std::endl;
    displayTokens(faTokens);

    std::cout << "\n=== Regex Tokenizer ===" << std::endl;
    auto regexTime = measureExecutionTime(tokenizeRegex, code);
    std::vector<Token> regexTokens = tokenizeRegex(code);

    std::cout << "\n=== Tokens (Regex) ===" << std::endl;
    displayTokens(regexTokens);

    std::cout << "\n=== Execution Times ===" << std::endl;
    std::cout << "Finite Automata Tokenizer Time: " << faTime << " microseconds\n";
    std::cout << "Regex Tokenizer Time: " << regexTime << " microseconds\n";

    return 0;
}

TokenType determineTokenTypeFA(const std::string& token) {
    if (token.substr(0, 2) == ";;") return COMMENT;
    if (isLabelFA(token)) return LABEL;
    if (isDecimalNumberFA(token)) return DECIMAL_NUMBER;
    if (isHexNumberFA(token)) return HEX_NUMBER;
    if (isFloatNumberFA(token)) return FLOAT_NUMBER;
    if (isStringConstantFA(token)) return STRING_CONSTANT;
    if (isCharConstantFA(token)) return CHAR_CONSTANT;
    if (!token.empty() && token[0] == '#') return PREPROCESSOR_DIRECTIVE;
    if (isReservedWord(token)) return RESERVED_WORD;
    if (isOperator(token)) return OPERATOR;
    if (isDelimiter(token)) return DELIMITER;
    if (isIdentifierFA(token)) return IDENTIFIER;
    return ERROR;
}

TokenType determineTokenTypeRegex(const std::string& token) {
    if (token.substr(0, 2) == ";;") return COMMENT;
    if (isLabelRegex(token)) return LABEL;
    if (isDecimalNumberRegex(token)) return DECIMAL_NUMBER;
    if (isHexNumberRegex(token)) return HEX_NUMBER;
    if (isFloatNumberRegex(token)) return FLOAT_NUMBER;
    if (isStringConstantRegex(token)) return STRING_CONSTANT;
    if (isCharConstantRegex(token)) return CHAR_CONSTANT;
    if (!token.empty() && token[0] == '#') return PREPROCESSOR_DIRECTIVE;
    if (isReservedWord(token)) return RESERVED_WORD;
    if (isOperator(token)) return OPERATOR;
    if (isDelimiter(token)) return DELIMITER;
    if (std::regex_match(token, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*$"))) return IDENTIFIER;
    return ERROR;
}


bool isDecimalNumberFA(const std::string& token) {
    return !token.empty() && std::all_of(token.begin(), token.end(), ::isdigit);
}

bool isHexNumberFA(const std::string& token) {
    if (token.size() < 3 || token[0] != '0' || token[1] != 'x') return false;
    return std::all_of(token.begin() + 2, token.end(), ::isxdigit);
}

bool isFloatNumberFA(const std::string& token) {
    size_t dotCount = std::count(token.begin(), token.end(), '.');
    return dotCount == 1 && token.find('.') != 0 && token.find('.') != token.size() - 1 &&
           std::all_of(token.begin(), token.end(), [](char c) { return std::isdigit(c) || c == '.' || c == 'e' || c == 'E'; });
}

bool isStringConstantFA(const std::string& token) {
    return token.size() >= 2 && token.front() == '"' && token.back() == '"';
}

bool isCharConstantFA(const std::string& token) {
    return (token.size() == 3 && token.front() == '\'' && token.back() == '\'') ||
           (token.size() == 4 && token.front() == '\'' && token.back() == '\'' && token[1] == '\\');
}

bool isIdentifierFA(const std::string& token) {
    if (token.empty() || (!std::isalpha(token[0]) && token[0] != '_')) return false;
    return std::all_of(token.begin() + 1, token.end(), [](char c) { return std::isalnum(c) || c == '_'; });
}

bool isLabelFA(const std::string& token) {
    return token.back() == ':' && isIdentifierFA(token.substr(0, token.size() - 1));
}

bool isDecimalNumberRegex(const std::string& token) {
    return std::regex_match(token, std::regex("^[0-9]+$"));
}

bool isHexNumberRegex(const std::string& token) {
    return std::regex_match(token, std::regex("^0x[a-fA-F0-9]+$"));
}

bool isFloatNumberRegex(const std::string& token) {
    return std::regex_match(token, std::regex("^\\d+\\.\\d+([eE][+-]?\\d+)?$"));
}

bool isStringConstantRegex(const std::string& token) {
    return std::regex_match(token, std::regex("^\".*\"$"));
}

bool isCharConstantRegex(const std::string& token) {
    return std::regex_match(token, std::regex("^'.{1}'$"));
}

bool isLabelRegex(const std::string& token) {
    return std::regex_match(token, std::regex("^[a-zA-Z_][a-zA-Z0-9_]*:$"));
}

bool isReservedWord(const std::string& token) {
    static const std::vector<std::string> reservedWords = {
            "MOV", "ADD", "SUB", "PUSH", "POP", "CMP", "JMP",
            "JNE", "HLT", "RET", "NOP", "OFFSET"
    };
    return std::find(reservedWords.begin(), reservedWords.end(), token) != reservedWords.end();
}

bool isOperator(const std::string& token) {
    static const std::vector<std::string> operators = {"+", "-", "*", "/"};
    return std::find(operators.begin(), operators.end(), token) != operators.end();
}

bool isDelimiter(const std::string& token) {
    static const std::vector<std::string> delimiters = {",", ";", "(", ")", ":", "{"};
    return std::find(delimiters.begin(), delimiters.end(), token) != delimiters.end();
}

std::vector<Token> tokenizeFA(const std::string& code) {
    std::vector<Token> tokens;
    std::string buffer;

    for (size_t i = 0; i < code.size(); ++i) {
        char ch = code[i];

        // Якщо пробіл, обробляємо буфер
        if (isspace(ch)) {
            if (!buffer.empty()) {
                tokens.push_back({buffer, determineTokenTypeFA(buffer)});
                buffer.clear();
            }
            continue;
        }

        // Коментар ;; до кінця рядка
        if ((buffer == ";;" || (buffer.empty() && ch == ';' && i + 1 < code.size() && code[i + 1] == ';')) &&
            (i + 2 >= code.size() || code[i + 2] != ';')) {
            if (buffer.empty()) buffer = ";;";
            while (i + 1 < code.size() && code[i + 1] != '\n') {
                buffer += code[++i];
            }
            tokens.push_back({buffer, COMMENT});
            buffer.clear();
            continue;
        }

        // Роздільники або оператори
        if (isDelimiter(std::string(1, ch)) || isOperator(std::string(1, ch))) {
            if (!buffer.empty()) {
                tokens.push_back({buffer, determineTokenTypeFA(buffer)});
                buffer.clear();
            }
            tokens.push_back({std::string(1, ch), isDelimiter(std::string(1, ch)) ? DELIMITER : OPERATOR});
            continue;
        }

        buffer += ch;
    }

    if (!buffer.empty()) tokens.push_back({buffer, determineTokenTypeFA(buffer)});
    return tokens;
}


std::vector<Token> tokenizeRegex(const std::string& code) {
    std::vector<Token> tokens;

    // Регулярний вираз для токенів
    std::regex tokenRegex(R"((;;.*|0x[a-fA-F0-9]+|\d+(\.\d+([eE][+-]?\d+)?)?|[a-zA-Z_][a-zA-Z0-9_]*|".*?"|'.'|[+\-*/(),:{}]))");

    std::sregex_iterator iter(code.begin(), code.end(), tokenRegex);
    std::sregex_iterator end;

    while (iter != end) {
        tokens.push_back({iter->str(), determineTokenTypeRegex(iter->str())});
        ++iter;
    }
    return tokens;
}


void displayTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "<" << token.value << ", ";
        switch (token.type) {
            case DECIMAL_NUMBER: std::cout << "DECIMAL_NUMBER"; break;
            case HEX_NUMBER: std::cout << "HEX_NUMBER"; break;
            case FLOAT_NUMBER: std::cout << "FLOAT_NUMBER"; break;
            case STRING_CONSTANT: std::cout << "STRING_CONSTANT"; break;
            case CHAR_CONSTANT: std::cout << "CHAR_CONSTANT"; break;
            case PREPROCESSOR_DIRECTIVE: std::cout << "PREPROCESSOR_DIRECTIVE"; break;
            case COMMENT: std::cout << "COMMENT"; break;
            case RESERVED_WORD: std::cout << "RESERVED_WORD"; break;
            case OPERATOR: std::cout << "OPERATOR"; break;
            case DELIMITER: std::cout << "DELIMITER"; break;
            case IDENTIFIER: std::cout << "IDENTIFIER"; break;
            case LABEL: std::cout << "LABEL"; break;
            case ERROR: std::cout << "ERROR"; break;
        }
        std::cout << ">" << std::endl;
    }
}
