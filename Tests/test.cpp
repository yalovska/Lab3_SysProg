#include <gtest/gtest.h>
#include "../lexical_analyzer.h"

TEST(LexicalAnalyzerTest, RegexTest) {
std::string code = "mov ax, 10\nadd bx, cx; // коментар";
std::vector<Token> lexemes = analyzeWithFiniteAutomaton(code);

ASSERT_EQ(lexemes[0].type, RESERVED_WORD);  // "mov" є зарезервованим словом
ASSERT_EQ(lexemes[1].type, IDENTIFIER);     // "ax" є ідентифікатором
ASSERT_EQ(lexemes[2].type, PUNCTUATION);    // "," є розділовим знаком
ASSERT_EQ(lexemes[3].type, NUMBER_DECIMAL); // "10" є десятковим числом
ASSERT_EQ(lexemes[4].type, RESERVED_WORD);  // "add" є зарезервованим словом
ASSERT_EQ(lexemes[5].type, IDENTIFIER);     // "bx" є ідентифікатором
ASSERT_EQ(lexemes[6].type, PUNCTUATION);    // "," є розділовим знаком
ASSERT_EQ(lexemes[7].type, IDENTIFIER);     // "cx" є ідентифікатором
ASSERT_EQ(lexemes[8].type, PUNCTUATION);    // ";" є розділовим знаком
ASSERT_EQ(lexemes[9].type, COMMENT);        // "// коментар" є коментарем
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
