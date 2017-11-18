# SLB
#
# Copyright Michael Park, 2017
# Copyright Agustin Berge, 2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.0)

project(SLB CXX)

include(CTest)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(SLB_BUILD_TESTING "Compile with unit tests." ON)
else()
  option(SLB_BUILD_TESTING "Compile with unit tests." OFF)
endif()

# Build
add_library(SLB INTERFACE)
target_include_directories(SLB INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_library(SLB::SLB ALIAS SLB)

# Test
if(BUILD_TESTING AND SLB_BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

# Install
install(DIRECTORY include/slb DESTINATION include)
