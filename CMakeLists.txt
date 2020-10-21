cmake_minimum_required (VERSION 3.8)

project (razor VERSION 0.0.0 LANGUAGES C)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/lib)

find_package (glad 4.5.2 REQUIRED)
find_package (SDL2 REQUIRED)
find_package (rigid 0.1.1 REQUIRED)
find_package (OpenGL REQUIRED)

add_library (${PROJECT_NAME} SHARED
        include/razor.h
        "src/razor_game_loop.c"
 "src/razor_shader.c")
target_include_directories (${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_definitions (${PROJECT_NAME} PRIVATE RZ_BUILD)
target_link_libraries (${PROJECT_NAME}
        PUBLIC
            rigid::rigid
        PRIVATE
            SDL2::SDL2
            OpenGL::GL
            glad
)

set_target_properties (${PROJECT_NAME} PROPERTIES
        LINKER_LANGUAGE C
)

add_subdirectory (third_party/glad)
add_subdirectory (demos)

enable_testing ()
add_subdirectory (tests)

