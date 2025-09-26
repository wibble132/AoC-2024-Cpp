cmake_minimum_required(VERSION 3.16)

project(AoC-2024)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)


add_executable(
    AoC-2024
    main.cpp
        days/day01.cpp
        days/day01.h
        days/base.cpp
        days/base.h
        days/day02.cpp
        days/day02.h
        days/day03.cpp
        days/day03.h
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${BIN_DIR}/${CMAKE_PROJECT_NAME}
        COMMENT "Copying binary files to ${BIN_DIR}"
)
