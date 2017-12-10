# SLB
#
# Copyright Michael Park, 2017
# Copyright Agustin Berge, 2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

add_library(Catch2 OBJECT catch.cpp)

set(_tests
  functional/invoke
  functional/mem_fn
  functional/not_fn
  type_traits
  utility)
foreach(_test ${_tests})
  set(_test_file ${_test}.cpp)
  get_filename_component(_test_folder ${_test_file} DIRECTORY)
  string(REPLACE "/" "." _test ${_test})

  add_executable(test.${_test} ${_test_file} $<TARGET_OBJECTS:Catch2>)
  target_link_libraries(test.${_test} SLB)
  set_property(TARGET test.${_test} PROPERTY FOLDER "test/${_test_folder}")

  file(STRINGS ${_test_file} _lines)
  foreach(_line ${_lines})
    if(_line MATCHES "^TEST_CASE\\(\"([A-Za-z_0-9()-]+)\"")
      set(_case ${CMAKE_MATCH_1})
      string(MAKE_C_IDENTIFIER ${_case} _case_id)

      add_test(
        NAME test.${_test}.${_case_id}
        COMMAND test.${_test} "${_case}")
    endif()
  endforeach()
endforeach()
