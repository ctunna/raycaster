cmake_minimum_required(VERSION 3.16)
project(raycaster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-deprecated-declarations)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS *.cc)

add_executable(raycaster ${SOURCES})
target_link_libraries(raycaster PRIVATE sfml-graphics)
target_compile_features(raycaster PRIVATE cxx_std_20)
target_include_directories(raycaster PRIVATE ${CMAKE_SOURCE_DIR}/raycaster)

if(WIN32)
    add_custom_command(
        TARGET raycaster
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:raycaster>
        VERBATIM)
endif()

install(TARGETS raycaster)
