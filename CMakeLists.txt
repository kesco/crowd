cmake_minimum_required(VERSION 3.5)
project(crowd)

enable_testing()

# if(CMAKE_GENERATOR STREQUAL Xcode)
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/Debug)
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/Release)
#     set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/Debug)
#     set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/Release)
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/Debug)
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/Release)
# endif()

if(MSVC)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
  set(BOOST_ALL_DYN_LINK OFF)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()


find_package(Boost 1.60.0 REQUIRED COMPONENTS
  system filesystem thread regex date_time unit_test_framework)
if(Boost_FOUND)
  message(STATUS "Found Boost Libary: ${Boost_LIBRARIES}.")
  include_directories(SYSTEM ${Boost_INCLUDE_DIR})
else()
  message(FATAL_ERROR "Boost Libary Not Found.")
endif()

set(EXT_ROOT "${CMAKE_SOURCE_DIR}/ext")

set(HOEDOWN_DIR "${EXT_ROOT}/hoedown")
include_directories("${HOEDOWN_DIR}/src")

set(YAML_DIR "${EXT_ROOT}/yaml-cpp")
include_directories("${YAML_DIR}/include")

set(MSTCH_DIR "${EXT_ROOT}/mstch")
include_directories(${mstch_INCLUDE_DIR})

add_subdirectory(ext)
add_subdirectory(${MSTCH_DIR})

set(HEADER_FILES
  main.hpp
  error.hpp
  parser.hpp)
set(SOURCE_FILES
  main.cpp
  error.cpp
  parser.cpp)

add_subdirectory(test)

add_executable(crowd ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(crowd ${Boost_LIBRARIES} hoedown yaml mstch)