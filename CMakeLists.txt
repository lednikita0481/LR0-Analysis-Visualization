cmake_minimum_required(VERSION 3.10)
project(LR0)

set(CMAKE_CXX_STANDARD 11)

# Добавляем директории с заголовочными файлами
include_directories(include)

# Список исходных файлов
set(SOURCES
    src/main.cpp
    src/Node.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/flex.cpp
)

# Генерация исполняемого файла
add_executable(main ${SOURCES})

# Генерация flex.cpp из flex.l перед сборкой
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/flex.cpp
    COMMAND flex -o ${CMAKE_CURRENT_SOURCE_DIR}/src/flex.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/flex.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/flex.l
)

# Добавляем сгенерированный flex.cpp в сборку
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/flex.cpp PROPERTIES GENERATED TRUE)
target_sources(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/flex.cpp)

# Добавляем цель для выполнения программы
add_custom_target(run
    COMMAND main test.txt
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

# Очистка сборочных файлов
add_custom_target(clear
    COMMAND ${CMAKE_COMMAND} -E remove main ${CMAKE_CURRENT_SOURCE_DIR}/src/build/*.o
    DEPENDS main
)
