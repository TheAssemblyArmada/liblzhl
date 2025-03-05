cmake_minimum_required(VERSION 3.19)

# Disable default MSVC warning level so we can set it ourselves.
if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW)
endif()

# Default to release for single target generators if nothing specified.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
endif()

# We don't support in tree builds, so help people make the right choice.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(FeatureSummary)

project(LZHL VERSION 1.0.0 LANGUAGES CXX)

# Make sure symbols are not exported by default.
if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
    NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

# configuration options
if(DEFINED LZHL_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${LZHL_SHARED_LIBS}")
endif()

add_feature_info(SharedLibs LZHL_SHARED_LIBS "Generate a shared library for LZHL")

include(GNUInstallDirs)
include(TargetExports)

add_library(lzhl
    include/lzhl/lzhl.h
    src/huff.cpp
    src/lz.cpp
    src/lzhl.cpp
    src/_huff.h
    src/_lz.h
    src/_lzhl.h
)

add_library(LZHL::LZHL ALIAS lzhl)

if(BUILD_SHARED_LIBS)
    set_target_properties(lzhl PROPERTIES POSITION_INDEPENDENT_CODE ON) 
    set_target_properties(lzhl PROPERTIES ARCHIVE_OUTPUT_NAME lzhl.dll)
endif()

# Set the compiler features expected/required
target_compile_features(lzhl PUBLIC cxx_std_11)

set_target_properties(lzhl PROPERTIES SOVERSION ${LZHL_VERSION_MAJOR} VERSION ${LZHL_VERSION})
target_include_directories(lzhl PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_exports(lzhl SYMBOLS 
    LZHLCreateCompressor
    LZHLCompressorCalcMaxBuf
    LZHLCompress
    LZHLDestroyCompressor
    LZHLCreateDecompressor
    LZHLDecompress
    LZHLDecompressReset
    LZHLDestroyDecompressor
)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
option(LZHL_INCLUDE_PACKAGING "Include packaging rules for LZHL" "${is_top_level}")
add_feature_info(Packaging LZHL_INCLUDE_PACKAGING "Generate packaging rules for LZHL")

if(LZHL_INCLUDE_PACKAGING)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    if (NOT DEFINED LZHL_INSTALL_CMAKEDIR)
       set(LZHL_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/LZHL" CACHE STRING "Path to LZHL CMake files")
    endif ()
    
    install(TARGETS lzhl 
        EXPORT LZHL_Targets
        RUNTIME COMPONENT LZHL_Runtime
        LIBRARY COMPONENT LZHL_Runtime
        NAMELINK_COMPONENT LZHL_Development
        ARCHIVE COMPONENT LZHL_Development
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    install(DIRECTORY "${LZHL_SOURCE_DIR}/include/" "${LZHL_BINARY_DIR}/include/"
        TYPE INCLUDE
        COMPONENT LZHL_Development
    )
    
    if(BUILD_SHARED_LIBS)
        set(type shared)
    else()
        set(type static)
    endif()

    install(EXPORT LZHL_Targets
        DESTINATION "${LZHL_INSTALL_CMAKEDIR}"
        NAMESPACE LZHL::
        FILE LZHL-${type}-targets.cmake
        COMPONENT LZHL_Development
    )
    
    write_basic_package_version_file(
        LZHLConfigVersion.cmake
        COMPATIBILITY SameMajorVersion
    )
    
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/LZHLConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/LZHLConfigVersion.cmake"
        DESTINATION "${LZHL_INSTALL_CMAKEDIR}"
        COMPONENT LZHL_Development
    )
endif()

feature_summary(WHAT ENABLED_FEATURES DESCRIPTION "Enabled features:")
feature_summary(WHAT DISABLED_FEATURES DESCRIPTION "Disabled features:")
