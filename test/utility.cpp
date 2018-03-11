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

#include "catch.hpp"

#define CHECK_CXX14_CONSTEXPR(Tag, ...)                                        \
  struct check_cxx14_constexpr_helper_##Tag {                                  \
    static SLB_CXX14_CONSTEXPR bool check(){__VA_ARGS__};                      \
  };                                                                           \
  SLB_CXX14_CONSTEXPR bool check_cxx14_constexpr_##Tag =                       \
      check_cxx14_constexpr_helper_##Tag::check();                             \
  CHECK(check_cxx14_constexpr_##Tag)

// [utility.exchange], exchange

// template<class T, class U = T>
//   constexpr T exchange(T& obj, U&& new_val);
TEST_CASE("exchange", "[utility.exchange]") {
  struct T {
    int val;
    T(int val) : val(val) {}
    T(T&&) = default;
    void operator=(int x) { val = x; }
    T& operator=(T&&) = default;
  } obj(42);
  CHECK(std::is_same<decltype(slb::exchange(obj, 43)), T>::value);
  CHECK(slb::exchange(obj, 43).val == 42);
  CHECK(slb::exchange(obj, {44}).val == 43);
  CHECK(obj.val == 44);
  CHECK_CXX14_CONSTEXPR(exchange, {
    int obj = 42;
    int old_obj = slb::exchange(obj, 43);
    return old_obj == 42 && obj == 43;
  });

  /* T old_value = std::move(obj); */ {
    struct T {
      int val;
      T(int val) : val(val) {}
      T(T const&) : val(-1) {}
      explicit T(T&&) = default;
      void operator=(int x) { val = x; }
    } obj1(42);
    CHECK(slb::exchange(obj1, 43).val == -1);
    CHECK(obj1.val == 43);

    /* throws */ {
      struct T {
        T() {}
        T(T const&) { throw 0; }
        explicit T(T&&) noexcept = default;
        T& operator=(T&&) noexcept = default;
      } tobj1;
      CHECK_THROWS(slb::exchange(tobj1, {}));
    }
  }

  /* obj = std::forward<U>(new_val); */ {
    struct T {
      int val;
      T(int val) : val(val) {}
      T(T&&) = default;
      void operator=(int const& x) { val = x; }
      void operator=(int&&) { val = -1; }
    } obj2(42);
    CHECK(slb::exchange(obj2, 43).val == 42);
    CHECK(obj2.val == -1);

    /* throws */ {
      struct T {
        T() {}
        T(T&&) noexcept = default;
        T& operator=(T const&) noexcept = default;
        T& operator=(T&&) { throw 0; }
      } tobj2;
      CHECK_THROWS(slb::exchange(tobj2, {}));
    }
  }
}

// [utility.as_const], as_const

// template<class T>
//   constexpr add_const_t<T>& as_const(T& t) noexcept;
// template<class T>
//   void as_const(const T&&) = delete;
namespace as_const_deleted {
void as_const(...);
using slb::as_const;

template <typename T, typename Enable = void>
struct no_result : std::true_type {};

template <typename T>
struct no_result<T,
                 decltype(
                     (void)(as_const_deleted::as_const(std::declval<T>())))>
    : std::false_type {};
} // namespace as_const_deleted

TEST_CASE("as_const", "[utility.as_const]") {
  static int t = 42;
  CHECK(std::is_same<decltype(slb::as_const(t)), int const&>::value);
  CHECK(noexcept(slb::as_const(t)));
  CHECK(slb::as_const(t) == 42);
  constexpr int const& tr = slb::as_const(t);
  (void)tr;
  CHECK_CXX14_CONSTEXPR(tv, {
    int t = 42;
    return slb::as_const(t) == t;
  });

  static constexpr int const ct = 42;
  CHECK(std::is_same<decltype(slb::as_const(ct)), int const&>::value);
  CHECK(noexcept(slb::as_const(ct)));
  CHECK(slb::as_const(ct) == 42);
  constexpr int const& ctr = slb::as_const(ct);
  constexpr bool ctv = slb::as_const(ct) == ct;
  (void)ctr;
  (void)ctv;

  using as_const_deleted::no_result;
  CHECK(no_result<int&&>::value);
  CHECK(no_result<int const&&>::value);
  CHECK_FALSE(no_result<int&>::value);
}

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
