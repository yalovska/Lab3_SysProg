cmake_minimum_required(VERSION 3.23)
project(Lab3_sp)

set(CMAKE_CXX_STANDARD 17)

# Основна програма
add_executable(Lab3_sp main.cpp finite_automaton.cpp regex_analyzer.cpp)

# Підключаємо GoogleTest для тестів
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
link_directories(/usr/local/lib)  # Додаємо шлях до бібліотек Homebrew (переконайтеся, що шлях правильний)

# Створюємо окремий виконуваний файл для тестів
add_executable(Tests Tests/test.cpp finite_automaton.cpp regex_analyzer.cpp)

# Перевіряємо, чи знайшли GoogleTest, перед тим як лінкувати
if(GTest_FOUND)
    target_link_libraries(Tests GTest::GTest GTest::Main pthread)
else()
    message(FATAL_ERROR "GoogleTest not found. Make sure it is installed and available.")
endif()
