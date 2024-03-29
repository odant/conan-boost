# Generated by `boostdep --cmake proto`
# Copyright 2020 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5...3.16)

project(boost_proto VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(boost_proto INTERFACE)
add_library(Boost::proto ALIAS boost_proto)

target_include_directories(boost_proto INTERFACE include)

target_link_libraries(boost_proto
  INTERFACE
    Boost::config
    Boost::core
    Boost::fusion
    Boost::mpl
    Boost::preprocessor
    Boost::range
    Boost::static_assert
    Boost::type_traits
    Boost::typeof
    Boost::utility
)

if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")

  add_subdirectory(test)

endif()

