cmake_minimum_required(VERSION 3.1.0)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(lzhl VERSION 1.0.0 LANGUAGES CXX)

# configuration options
option(LZHL_ENABLE_STATIC "Build static lzhl library." ON)
option(LZHL_ENABLE_SHARED "Build shared lzhl library." OFF)
if(NOT LZHL_ENABLE_STATIC AND NOT LZHL_ENABLE_SHARED)
    set(LZHL_ENABLE_STATIC ON)
endif()

if(CMAKE_CONFIGURATION_TYPES)
    # Glob all the header files together to add to the project for msvc/xcode.
    # Not ideal as CMake won't notice if you add any until something else prompts a CMake run
    # but this is largely a convenience feature for MSVC/XCode users.
    file(GLOB_RECURSE LZHL_HEADERS "src/*.h")
    file(GLOB_RECURSE LZHL_SOURCES "src/*.cpp")

    foreach(_LZHL_SOURCE IN ITEMS ${LZHL_SOURCES})
        get_filename_component(_LZHL_SOURCE_PATH "${_LZHL_SOURCE}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src" "" _LZHL_GROUP_PATH "${_LZHL_SOURCE_PATH}")
        string(REPLACE "/" "\\" _LZHL_GROUP_PATH "${_LZHL_GROUP_PATH}")
        source_group("Source Files\\${_LZHL_GROUP_PATH}" FILES "${_LZHL_SOURCE}")
    endforeach()

    foreach(_LZHL_HEADER IN ITEMS ${LZHL_HEADERS})
        get_filename_component(_LZHL_HEADER_PATH "${_LZHL_HEADER}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src" "" _LZHL_HGROUP_PATH "${_LZHL_HEADER_PATH}")
        string(REPLACE "/" "\\" _LZHL_HGROUP_PATH "${_LZHL_HGROUP_PATH}")
        source_group("Private Header Files\\${_LZHL_HGROUP_PATH}" FILES "${_LZHL_HEADER}")
    endforeach()
    
    source_group("Public Header Files\\" FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/lzhl/lzhl.h")
endif()

add_library(lzhl_object_lib OBJECT ${LZHL_SOURCES} ${LZHL_HEADERS} include/lzhl/lzhl.h)
set_target_properties(lzhl_object_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(lzhl_object_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src 
)

if(LZHL_ENABLE_STATIC)
    add_library(lzhl_static_lib STATIC $<TARGET_OBJECTS:lzhl_object_lib>)
    
    set_target_properties(lzhl_static_lib PROPERTIES OUTPUT_NAME lzhl)
    target_include_directories(lzhl_static_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src 
    )
endif()

if(LZHL_ENABLE_SHARED)
    add_library(lzhl_shared_lib SHARED $<TARGET_OBJECTS:lzhl_object_lib>)
    set_target_properties(lzhl_shared_lib PROPERTIES OUTPUT_NAME lzhl)
    if(LZHL_ENABLE_STATIC)
        # avoid name clash between static DLL import-lib
        set_target_properties(lzhl_shared_lib PROPERTIES ARCHIVE_OUTPUT_NAME lzhl_dllimport)
    endif()
    set_target_properties(lzhl_shared_lib PROPERTIES SOVERSION 1.0 VERSION 1.0.0) # ABI version is 1.0.0 !
    set_target_properties(lzhl_shared_lib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_include_directories(lzhl_static_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE common 
    )
endif()
