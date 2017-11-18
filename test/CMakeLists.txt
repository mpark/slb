# SLB
#
# Copyright Michael Park, 2017
# Copyright Agustin Berge, 2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://www.boost.org/LICENSE_1_0.txt)

set(_tests
  type_traits)
foreach(_test ${_tests})
  set(_test_file ${_test}.cpp)
  string(REPLACE "/" "." _test ${_test})
  add_executable(test.${_test} ${_test_file})
  target_link_libraries(test.${_test} SLB)

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
