/*
  SLB.Utility

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <slb/utility.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// [intseq], Compile-time integer sequences

// template<class T, T...>
//   struct integer_sequence;
#if SLB_INTEGER_SEQUENCE
template <typename T, T... Is>
std::size_t deduce_std_integer_sequence(std::integer_sequence<T, Is...>) {
  return sizeof...(Is);
}
#endif

TEST_CASE("integer_sequence", "[intseq.intseq]") {
  /* using value_type = T; */ {
    CHECK(std::is_same<slb::integer_sequence<int, -1, 1>::value_type,
                       int>::value);
  }

  /* static constexpr size_t size() noexcept; */ {
    using is = slb::integer_sequence<int, 0, 1, 3, 2>;
    CHECK(std::is_same<decltype(is::size()), std::size_t>::value);
    CHECK(noexcept(is::size()));
    CHECK(is::size() == 4);
    constexpr std::size_t size = is::size();
    (void)size;
  }

  /* integer_sequence<bool, Bs...> */ {
    slb::integer_sequence<bool, false, true, false, true> bs;
    (void)bs;
  }

#if SLB_INTEGER_SEQUENCE
  /* std-compatible */ {
    slb::integer_sequence<int, 2, 1, -1, -2> slb_is;
    std::integer_sequence<int, 2, 1, -1, -2> std_is = slb_is;
    (void)std_is;

    CHECK(deduce_std_integer_sequence(slb_is) == 4);
  }
#endif
}

// template<size_t... I>
//   using index_sequence = integer_sequence<size_t, I...>;
TEST_CASE("index_sequence", "[utility.syn]") {
  CHECK(std::is_same<slb::index_sequence<>,
                     slb::integer_sequence<std::size_t>>::value);

  CHECK(std::is_same<slb::index_sequence<0>,
                     slb::integer_sequence<std::size_t, 0>>::value);

  CHECK(std::is_same<slb::index_sequence<1, 0>,
                     slb::integer_sequence<std::size_t, 1, 0>>::value);

  CHECK(std::is_same<slb::index_sequence<2, 1, 0>,
                     slb::integer_sequence<std::size_t, 2, 1, 0>>::value);

#if SLB_INTEGER_SEQUENCE
  /* std-compatible */ {
    slb::index_sequence<101, 202, 303, 404> slb_is;
    std::index_sequence<101, 202, 303, 404> std_is = slb_is;
    (void)std_is;

    CHECK(deduce_std_integer_sequence(slb_is) == 4);
  }
#endif
}

// template<class T, T N>
//   using make_integer_sequence = integer_sequence<T, see below>;
template <typename T>
void test_make_integer_sequence() {
  CHECK(std::is_same<slb::make_integer_sequence<T, 0>,
                     slb::integer_sequence<T>>::value);

  CHECK(std::is_same<slb::make_integer_sequence<T, 1>,
                     slb::integer_sequence<T, 0>>::value);

  CHECK(std::is_same<slb::make_integer_sequence<T, 2>,
                     slb::integer_sequence<T, 0, 1>>::value);

  CHECK(std::is_same<slb::make_integer_sequence<T, 3>,
                     slb::integer_sequence<T, 0, 1, 2>>::value);

  CHECK(std::is_same<slb::make_integer_sequence<T, 4>,
                     slb::integer_sequence<T, 0, 1, 2, 3>>::value);

  CHECK(std::is_same<slb::make_integer_sequence<T, 5>,
                     slb::integer_sequence<T, 0, 1, 2, 3, 4>>::value);

  CHECK(std::is_same<slb::make_integer_sequence<T, 6>,
                     slb::integer_sequence<T, 0, 1, 2, 3, 4, 5>>::value);

  CHECK(std::is_same<slb::make_integer_sequence<T, 7>,
                     slb::integer_sequence<T, 0, 1, 2, 3, 4, 5, 6>>::value);
}

TEST_CASE("make_integer_sequence", "[intseq.make]") {
  test_make_integer_sequence<char>();
  test_make_integer_sequence<signed char>();
  test_make_integer_sequence<unsigned char>();
  test_make_integer_sequence<char16_t>();
  test_make_integer_sequence<char32_t>();
  test_make_integer_sequence<wchar_t>();
  test_make_integer_sequence<short>();
  test_make_integer_sequence<int>();
  test_make_integer_sequence<long>();
  test_make_integer_sequence<long long>();
  test_make_integer_sequence<unsigned short>();
  test_make_integer_sequence<unsigned int>();
  test_make_integer_sequence<unsigned long>();
  test_make_integer_sequence<unsigned long long>();
  test_make_integer_sequence<std::size_t>();
}

// template<size_t N>
//   using make_index_sequence = make_integer_sequence<size_t, N>;
TEST_CASE("make_index_sequence", "[utility.syn]") {
  CHECK(
      std::is_same<slb::make_index_sequence<0>, slb::index_sequence<>>::value);

  CHECK(
      std::is_same<slb::make_index_sequence<1>, slb::index_sequence<0>>::value);

  CHECK(std::is_same<slb::make_index_sequence<2>,
                     slb::index_sequence<0, 1>>::value);

  CHECK(std::is_same<slb::make_index_sequence<3>,
                     slb::index_sequence<0, 1, 2>>::value);
}

// template<class... T>
//   using index_sequence_for = make_index_sequence<sizeof...(T)>;
TEST_CASE("index_sequence_for", "[utility.syn]") {
  CHECK(std::is_same<slb::index_sequence_for<>, slb::index_sequence<>>::value);

  CHECK(std::is_same<slb::index_sequence_for<void>,
                     slb::index_sequence<0>>::value);

  CHECK(std::is_same<slb::index_sequence_for<void, void>,
                     slb::index_sequence<0, 1>>::value);

  CHECK(std::is_same<slb::index_sequence_for<void, void, void>,
                     slb::index_sequence<0, 1, 2>>::value);
}
