cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_C_COMPILER "clang")

project(NBots)

# Build Library
file(GLOB_RECURSE NB_SOURCES "sources/*.c")

set(NB_EXCLUDE_IMPORTED_LIBS false)
if (NB_EXCLUDE_IMPORTED_LIBS)
  message("== NB: Exclude imported libs")
  add_definitions(-DNB_EXCLUDE_IMPORTED_LIBS)
  set(NB_IMPORTED_DIRS "/imported_libs/")
  foreach(NB_SOURCE ${NB_SOURCES})
    string(FIND ${NB_SOURCE} ${NB_IMPORTED_DIRS} NB_EXCLUDE_DIR_FOUND)
    if (NOT ${NB_EXCLUDE_DIR_FOUND} EQUAL -1)
      list(REMOVE_ITEM NB_SOURCES ${NB_SOURCE})
    endif ()
  endforeach(NB_SOURCE)
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories("headers")

add_library(objlib OBJECT ${NB_SOURCES})
target_compile_options(objlib PUBLIC -std=c99 -Wall)
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1) # -fPIC
add_library(nbots SHARED $<TARGET_OBJECTS:objlib>)
add_library(nbots_static STATIC $<TARGET_OBJECTS:objlib>)
set_target_properties(nbots_static PROPERTIES OUTPUT_NAME nbots)

# Build CUnit
file(GLOB_RECURSE CUNIT_SOURCES "utest/sources/cunit/*.c")

include_directories("utest/headers/cunit")
add_library(cunit STATIC ${CUNIT_SOURCES})

# Build Unit Tests
include_directories("utest/headers")
file(GLOB_RECURSE sources_utests "utest/sources/nb/*.c")
add_executable(utests ${sources_utests} utest/cunit_register.c)
target_compile_options(utests PUBLIC -std=c99 -Wall)
target_link_libraries(utests cunit nbots m)

# include(CTest)
enable_testing()

add_test(unit_tests utests)
set_property(TEST unit_tests PROPERTY ENVIRONMENT
  "NB_FONTS_DIR=${CMAKE_SOURCE_DIR}/resources/fonts")