cmake_minimum_required(VERSION 3.25)
project(Vtuber_Renderer)

# NOTE: CMake can only generate module build files for Ninja and VS 2022 

# Is there a better way of doing this?
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.27") 
    message("CMake Version is greater than 3.27")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
elseif(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.26") 
    message("CMake Version is greater than 3.26")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
elseif(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.25")
    message("CMake Version is greater than 3.25")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
endif()

# Enable MSVC Module support
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

# Must have at least c++20
set(CMAKE_CXX_STANDARD 20)

# Platform
if (WIN32)
    add_compile_definitions(_WIN64_IMPL_)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
elseif (UNIX AND NOT APPLE)
    add_compile_definitions(_LINUX_IMPL_)
endif()

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

# Default to C++ extensions being off. Clang's modules support have trouble
# with extensions right now and it is not required for any other compiler
# How true is this still?
# set(CMAKE_CXX_EXTENSIONS OFF)

set(MODULE_DIR             "${CMAKE_CURRENT_SOURCE_DIR}/code/modules")
set(MODULE_CSTD_DIR        "${MODULE_DIR}/cstd")
set(MODULE_CORE_DIR        "${MODULE_DIR}/core")
set(MODULE_APP_DIR         "${MODULE_DIR}/app")
set(MODULE_PLATFORM_DIR    "${MODULE_DIR}/platform")

#
# C++ Standard Library Module
# NOTE: on MSVC, you can alternatively import the standard library as a module.
#

add_library(cstd STATIC)

target_sources(cstd
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${MODULE_CSTD_DIR}
        FILES
            ${MODULE_CSTD_DIR}/cstd.ixx
)

#
# Core Module
#

add_library(core STATIC)

target_sources(core
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${MODULE_CORE_DIR}
        FILES
            ${MODULE_CORE_DIR}/core.ixx
)

# link dependent modules
target_link_libraries(core PRIVATE cstd)

#
# Application Module
#

add_library(app STATIC)

target_sources(app
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${MODULE_APP_DIR}
        FILES
            ${MODULE_APP_DIR}/app.ixx
)

# link dependent modules
target_link_libraries(app PRIVATE cstd)
target_link_libraries(app PRIVATE core)
target_link_libraries(app PRIVATE platform)

#
# Platform Module
#

add_library(platform STATIC)

target_sources(platform
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${MODULE_PLATFORM_DIR}
        FILES
            ${MODULE_PLATFORM_DIR}/platform.ixx
            ${MODULE_PLATFORM_DIR}/platform-window.ixx
            #${MODULE_PLATFORM_DIR}/win32/platform-window.cpp
)

if (WIN32)
    target_sources(platform PRIVATE ${MODULE_PLATFORM_DIR}/win32/platform-window.cpp)
endif(WIN32)

# link dependent modules
target_link_libraries(platform PRIVATE cstd)
target_link_libraries(platform PRIVATE core)

#
# Executable
#

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/code/main.cpp)
else()
    add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/code/main.cpp)
endif(WIN32)

target_link_libraries(${PROJECT_NAME} PRIVATE cstd)
# target_link_libraries(${PROJECT_NAME} PRIVATE core)
target_link_libraries(${PROJECT_NAME} PRIVATE app)