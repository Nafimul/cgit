cmake_minimum_required(VERSION 3.15)

project(CGit C)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
add_link_options(-fsanitize=address)

add_executable(
    main
    src/main.c
    src/linked_list.c
    src/utils.c
    src/txt_file.c
)

target_include_directories(
    main
    PRIVATE include
)