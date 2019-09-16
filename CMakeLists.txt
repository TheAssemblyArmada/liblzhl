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

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)

check_cxx_compiler_flag("-fvisibility=hidden" HAS_VISIBILITY)

file(GLOB_RECURSE LZHL_SOURCES "src/*.cpp")

if(CMAKE_CONFIGURATION_TYPES)
    # Glob all the header files together to add to the project for msvc/xcode.
    # Not ideal as CMake won't notice if you add any until something else prompts a CMake run
    # but this is largely a convenience feature for MSVC/XCode users.
    file(GLOB_RECURSE LZHL_HEADERS "src/*.h")

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

if(LZHL_ENABLE_STATIC)
    add_library(lzhl_static_lib STATIC ${LZHL_SOURCES} ${LZHL_HEADERS} include/lzhl/lzhl.h)
    set_target_properties(lzhl_static_lib PROPERTIES PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/lzhl/lzhl.h OUTPUT_NAME lzhl)
    target_include_directories(lzhl_static_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src 
    )

    list(APPEND INSTALL_TARGETS lzhl_static_lib)
endif()

if(LZHL_ENABLE_SHARED)
    add_library(lzhl_shared_lib SHARED ${LZHL_SOURCES} ${LZHL_HEADERS} include/lzhl/lzhl.h)
    set_target_properties(lzhl_shared_lib PROPERTIES 
        OUTPUT_NAME lzhl
        POSITION_INDEPENDENT_CODE ON
        PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/lzhl/lzhl.h
        SOVERSION 1.0
        VERSION 1.0.0
        CXX_VISIBILITY_PRESET hidden)

    if(WIN32)
        target_sources(lzhl_shared_lib PRIVATE src/lzhl.def)
    endif()

    if(HAS_VISIBILITY)
        target_compile_definitions(lzhl_shared_lib PRIVATE -DLZHL_SHARED)
    endif()

    set_target_properties(lzhl_shared_lib PROPERTIES ARCHIVE_OUTPUT_NAME lzhl_import)

    target_include_directories(lzhl_shared_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src 
    )
    
    list(APPEND INSTALL_TARGETS lzhl_shared_lib)
endif()

install(TARGETS ${INSTALL_TARGETS}
	EXPORT LzhlTargets
	RUNTIME	DESTINATION			${CMAKE_INSTALL_BINDIR}
	ARCHIVE DESTINATION			${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION			${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION	${CMAKE_INSTALL_INCLUDEDIR}/lzhl
)
