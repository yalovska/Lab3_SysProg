cmake_minimum_required(VERSION 3.23)
project(Lab3_sp)

set(CMAKE_CXX_STANDARD 17)

# Основна програма
add_executable(Lab3_sp main.cpp finite_automaton.cpp)

# Підключаємо GoogleTest для тестів
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Створюємо окремий виконуваний файл для тестів
add_executable(Tests Tests/test.cpp finite_automaton.cpp)

# Лінкуємо GoogleTest до тестів
target_link_libraries(Tests ${GTEST_LIBRARIES} pthread)
