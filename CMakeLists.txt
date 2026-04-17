cmake_minimum_required(VERSION 3.24)

set(VCPKG_TARGET_TRIPLET "x64-windows-static-md" CACHE STRING "")

project(NakedFollowerFix VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED)

# CommonLibSSE-NG Setup
find_package(CommonLibSSE CONFIG REQUIRED)

target_sources(${PROJECT_NAME} PRIVATE
    src/main.cpp
    src/NakedFixManager.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    CommonLibSSE::CommonLibSSE
)

# SKSE Plugin information
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    SUFFIX ".dll"
)

# Version definitions for Next Gen
target_compile_definitions(${PROJECT_NAME} PRIVATE
    SKSE_SUPPORT_SSE
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive-)
endif()
