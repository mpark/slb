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

# Config
include(CMakePackageConfigHelpers)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/slb-config.cmake.in [[
# SLB
#
# Copyright Michael Park, 2017
# Copyright Agustin Berge, 2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://www.boost.org/LICENSE_1_0.txt)

# - Config file for SLB
#
#   SLB_INCLUDE_DIRS - include directories
#   SLB_LIBRARIES    - libraries to link against
#
# The following `IMPORTED` target is also defined:
#
#   SLB::SLB
@PACKAGE_INIT@
include(${CMAKE_CURRENT_LIST_DIR}/slb-targets.cmake)

get_target_property(SLB_INCLUDE_DIRS SLB::SLB INTERFACE_INCLUDE_DIRECTORIES)
set_and_check(SLB_INCLUDE_DIRS "${SLB_INCLUDE_DIRS}")
set(SLB_LIBRARIES SLB::SLB)
]])

configure_package_config_file(
  ${CMAKE_CURRENT_BINARY_DIR}/slb-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/slb-config.cmake
  INSTALL_DESTINATION lib/cmake/slb
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

# Install
install(DIRECTORY include/slb DESTINATION include)
install(TARGETS SLB EXPORT _targets)
install(EXPORT _targets
  DESTINATION lib/cmake/slb
  NAMESPACE SLB::
  FILE slb-targets.cmake)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/slb-config.cmake
  DESTINATION lib/cmake/slb)
