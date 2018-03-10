/*
  SLB

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_TEST_UTILS_HPP
#define SLB_TEST_UTILS_HPP

#include "catch.hpp"

#include <type_traits>

#define CHECK_DECLTYPE(T, ...)                                                 \
  CHECK(std::is_same<T, decltype(__VA_ARGS__)>::value)

#define CHECK_NOEXCEPT(...) CHECK(noexcept(__VA_ARGS__))
#define CHECK_NOEXCEPT_IF(C, ...) CHECK(noexcept(__VA_ARGS__) == C)

#define CHECK_CXX11_CONSTEXPR(Tag, ...)                                        \
  constexpr decltype((__VA_ARGS__)) check_cxx11_constexpr_##Tag =              \
      (__VA_ARGS__);                                                           \
  (void)check_cxx11_constexpr_##Tag

#define CHECK_CXX14_CONSTEXPR(Tag, ...)                                        \
  struct check_cxx14_constexpr_helper_##Tag {                                  \
    static constexpr bool check(){__VA_ARGS__};                                \
  };                                                                           \
  constexpr bool check_cxx14_constexpr_##Tag =                                 \
      check_cxx14_constexpr_helper_##Tag::check();                             \
  CHECK(check_cxx14_constexpr_##Tag)

#define CHECK_NESTED(...)                                                      \
  do {                                                                         \
    INFO(__FILE__ "(" << __LINE__ << "): " #__VA_ARGS__);                      \
    check_##__VA_ARGS__;                                                       \
  } while (false)

#endif // SLB_TEST_UTILS_HPP
