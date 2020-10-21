cmake_minimum_required (VERSION 3.8)

add_executable (game_loop "game_loop.c")
target_link_libraries (game_loop PRIVATE razor)

