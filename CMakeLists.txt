#
# Copyright (c) 2025-present Henri Michelon
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.29)

#######################################################
project(lysa_dev)
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.env.cmake")
    message(FATAL_ERROR "Please create a .env.cmake file with the LYSA_PROJECT_DIR variable")
endif ()
include(.env.cmake)
if (NOT DEFINED LYSA_PROJECT_DIR)
    message(FATAL_ERROR "Please set LYSA_PROJECT_DIR in the .env.cmake file")
endif()
set(DIRECTX_BACKEND ON)

#######################################################
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
    set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
endif()
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
if(WIN32)
    add_compile_definitions(WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
endif ()

#######################################################
include(FetchContent)
include(cmake/shaders.cmake)
include(cmake/libraries.cmake)

#######################################################
set(LYSA_TARGET "lysa")
add_subdirectory(${LYSA_PROJECT_DIR} external_lib_build/${LYSA_TARGET})
set(LYSA_SRC_DIR ${LYSA_PROJECT_DIR}/src)
set(LYSA_TARGET_SHADERS "lysa_shaders")
set(LYSA_SHADERS_BUILD_DIR ${LYSA_PROJECT_DIR}/shaders)
set(LYSA_SHADERS_INCLUDE_DIR ${LYSA_PROJECT_DIR}/src/lysa/shaders)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SHADERS_SRC_DIR ${SRC_DIR}/shaders)
set(SHADERS_INCLUDE_DIR ${SRC_DIR}/shaders)
set(SHADERS_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

#######################################################
# Slang shaders
file(MAKE_DIRECTORY ${SHADERS_BUILD_DIR})
set(SHADERS_SOURCE_FILES
        "${SHADERS_SRC_DIR}/highlight.frag.slang"
        "${SHADERS_SRC_DIR}/scale.vert.slang"
        "${SHADERS_SRC_DIR}/uv_gradient.frag.slang"
)
compile_shaders(shaders ${SHADERS_BUILD_DIR} ${SHADERS_INCLUDE_DIR} ${LYSA_SHADERS_INCLUDE_DIR} ${SHADERS_SOURCE_FILES})
add_custom_target(shaders_copy
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${LYSA_SHADERS_BUILD_DIR}
        ${SHADERS_BUILD_DIR}
        COMMENT "Copying Lysa shaders"
        VERBATIM
)
add_dependencies(shaders_copy ${LYSA_TARGET} lysa_shaders)

#######################################################
# Common files
set(COMMON_SOURCES
)
set(COMMON_MODULES
)

#######################################################
function(build_target TARGET_NAME SRCS MODULES)
    message("Building target ${TARGET_NAME}")
    add_executable(${TARGET_NAME} ${COMMON_SOURCES} ${SRCS})
    target_sources(${TARGET_NAME}
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
            ${COMMON_MODULES}
            ${MODULES}
    )
    compile_options(${TARGET_NAME})
    target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_DIR} ${LYSA_INCLUDE_DIR} ${xxhash_SOURCE_DIR})
    target_link_libraries(${TARGET_NAME} ${LYSA_TARGET} std-cxx-modules xxhash)
    add_dependencies(${TARGET_NAME} shaders_copy)
    if(WIN32)
        if (DIRECTX_BACKEND)
            target_link_libraries(${TARGET_NAME} Xinput dinput8 dxguid dxgi d3d12 d3dcompiler)
        endif ()
        if(MINGW)
            target_link_options(${TARGET_NAME} PRIVATE "-mwindows")
        endif()
        set_target_properties(${TARGET_NAME} PROPERTIES
                WIN32_EXECUTABLE TRUE)
    endif()
endfunction()

#######################################################
set(MY_TARGET_SRC
        ${SRC_DIR}/MyApp.cpp
        ${SRC_DIR}/nodes/Crate.cpp
        ${SRC_DIR}/nodes/Player.cpp
        ${SRC_DIR}/scenes/AddRemove.cpp
        ${SRC_DIR}/scenes/BaseScene.cpp
        ${SRC_DIR}/scenes/Physics.cpp
        ${SRC_DIR}/scenes/TrianglesScene.cpp
        ${SRC_DIR}/scenes/Vector.cpp
        ${SRC_DIR}/scenes/ViewScene.cpp
        ${SRC_DIR}/scenes/ViewAssets.cpp)
set(MY_TARGET_MODULES
        ${SRC_DIR}/Layers.ixx
        ${SRC_DIR}/nodes/Crate.ixx
        ${SRC_DIR}/nodes/Player.ixx
        ${SRC_DIR}/scenes/AddRemove.ixx
        ${SRC_DIR}/scenes/BaseScene.ixx
        ${SRC_DIR}/scenes/Physics.ixx
        ${SRC_DIR}/scenes/TrianglesScene.ixx
        ${SRC_DIR}/scenes/Vector.ixx
        ${SRC_DIR}/scenes/ViewScene.ixx
        ${SRC_DIR}/scenes/ViewAssets.ixx)
build_target(myapp "${MY_TARGET_SRC}" "${MY_TARGET_MODULES}")
