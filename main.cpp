#include <iostream>
#include <chrono>
#include "lexical_analyzer.h"

int main() {
    std::string programCode = "mov ax, 10\nadd bx, cx;";
    std::cout << "Analyzing text using finite automaton:\n";

    // Вимірюємо час для аналізу зі скінченим автоматом
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Token> tokens = analyzeWithFiniteAutomaton(programCode);
    auto end = std::chrono::high_resolution_clock::now();

    // Виводимо результати
    for (const auto &token : tokens) {
        std::cout << "<" << token.value << " , " << token.type << ">\n";
    }

    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken using finite automaton: " << duration.count() << " seconds\n";

    return 0;
}
