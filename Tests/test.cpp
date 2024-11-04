#include <gtest/gtest.h>
#include "../lexical_analyzer.h"

TEST(LexicalAnalyzerTest, RegexTest) {
    std::string code = R"(mov ax, 10
add bx, cx;
#include <iostream>
"This is a string constant"
'c'
0x1F
3.14
// single-line comment
/* multi-line comment */)";

    std::vector<Token> lexemes = analyzeWithRegex(code);

    ASSERT_EQ(lexemes.size(), 10);  // Перевірте правильну кількість лексем

    ASSERT_EQ(lexemes[0].type, LexemeType::RESERVED_WORD);   // "mov"
    ASSERT_EQ(lexemes[1].type, LexemeType::IDENTIFIER);      // "ax"
    ASSERT_EQ(lexemes[2].type, LexemeType::PUNCTUATION);     // ","
    ASSERT_EQ(lexemes[3].type, LexemeType::NUMBER_DECIMAL);  // "10"
    ASSERT_EQ(lexemes[4].type, LexemeType::RESERVED_WORD);   // "add"
    ASSERT_EQ(lexemes[5].type, LexemeType::IDENTIFIER);      // "bx"
    ASSERT_EQ(lexemes[6].type, LexemeType::PUNCTUATION);     // ","
    ASSERT_EQ(lexemes[7].type, LexemeType::IDENTIFIER);      // "cx"
    ASSERT_EQ(lexemes[8].type, LexemeType::COMMENT);         // "// single-line comment"
    ASSERT_EQ(lexemes[9].type, LexemeType::COMMENT);         // "/* multi-line comment */"
}

TEST(LexicalAnalyzerTest, AutomatonTest) {
    std::string code = R"(mov ax, 10
add bx, cx;
#include <iostream>
"This is a string constant"
'c'
0x1F
3.14
// single-line comment
/* multi-line comment */)";

    std::vector<Token> lexemes = analyzeWithAutomaton(code);

    ASSERT_EQ(lexemes.size(), 10);  // Перевірте правильну кількість лексем

    ASSERT_EQ(lexemes[0].type, LexemeType::RESERVED_WORD);   // "mov"
    ASSERT_EQ(lexemes[1].type, LexemeType::IDENTIFIER);      // "ax"
    ASSERT_EQ(lexemes[2].type, LexemeType::PUNCTUATION);     // ","
    ASSERT_EQ(lexemes[3].type, LexemeType::NUMBER_DECIMAL);  // "10"
    ASSERT_EQ(lexemes[4].type, LexemeType::RESERVED_WORD);   // "add"
    ASSERT_EQ(lexemes[5].type, LexemeType::IDENTIFIER);      // "bx"
    ASSERT_EQ(lexemes[6].type, LexemeType::PUNCTUATION);     // ","
    ASSERT_EQ(lexemes[7].type, LexemeType::IDENTIFIER);      // "cx"
    ASSERT_EQ(lexemes[8].type, LexemeType::COMMENT);         // "// single-line comment"
    ASSERT_EQ(lexemes[9].type, LexemeType::COMMENT);         // "/* multi-line comment */"
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
