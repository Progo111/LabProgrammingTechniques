cmake_minimum_required(VERSION 3.10)
project(MatplotlibCppExample)

# Найти Python
find_package(Python3 REQUIRED COMPONENTS Development)

set(CXX_STANDARD 20)

# Добавить Matplotlib-cpp
include_directories(${PROJECT_SOURCE_DIR}/matplotlib-cpp)
include_directories(${Python3_INCLUDE_DIRS})

# Создать исполняемый файл
add_executable(MatplotlibCppExample main.cpp)

# Подключить библиотеки Python
target_link_libraries(MatplotlibCppExample ${Python3_LIBRARIES})
