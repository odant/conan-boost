# Generated by `boostdep --cmake lambda2`
# Copyright 2020, 2021 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.8...3.20)

project(boost_lambda2 VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

add_library(boost_lambda2 INTERFACE)
add_library(Boost::lambda2 ALIAS boost_lambda2)

target_include_directories(boost_lambda2 INTERFACE include)

target_compile_features(boost_lambda2 INTERFACE cxx_std_14)

if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")

  add_subdirectory(test)

endif()
