cmake_minimum_required(VERSION 3.18)
project(cg VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.h")
list(REMOVE_ITEM SRC_FILES "src/main.cpp")

if (MSVC)
    set(PROJECT_WARNINGS /utf-8 /W4 /WX)
else ()
    set(PROJECT_WARNINGS -Wall -Wextra -Wpedantic -Werror)
endif ()

set(BUILD_SHARED_LIBS OFF)
include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC "libs/glad/src/glad.c")
target_include_directories(glad PUBLIC "libs/glad/include")
if (MSVC)
    target_compile_options(glad PRIVATE /W0)
else()
    target_compile_options(glad PRIVATE -w)
endif()

add_library(core_lib STATIC ${SRC_FILES} ${HEADER_FILES})
target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(core_lib PUBLIC libs/stb)
target_include_directories(core_lib SYSTEM PUBLIC libs)
target_compile_options(core_lib PRIVATE ${PROJECT_WARNINGS})
target_link_libraries(core_lib PUBLIC glfw glad)

if (WIN32)
    target_link_libraries(core_lib PUBLIC dwmapi)
endif ()

add_executable(start src/main.cpp)
target_link_libraries(start PRIVATE core_lib)
target_compile_options(start PRIVATE ${PROJECT_WARNINGS})
set_target_properties(start PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/res")