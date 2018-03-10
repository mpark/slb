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
#include "test_utils.hpp"

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
  CHECK_DECLTYPE(T, slb::exchange(obj, 43));
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

// [forward], forward/move

// template<class T>
//   constexpr T&& forward(remove_reference_t<T>& t) noexcept;
// template<class T>
//   constexpr T&& forward(remove_reference_t<T>&& t) noexcept;
TEST_CASE("forward", "[forward]") {
  struct S {};

  static S s;
  static S const cs{};

  // lvalue -> lvalue
  /* type */ {
    CHECK(std::is_same<decltype(slb::forward<S&>(s)), S&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const&>(s)), S const&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const&>(cs)), S const&>::value);
  }
  /* `noexcept` */ {
    CHECK(noexcept(slb::forward<S&>(s)));
    CHECK(noexcept(slb::forward<S const&>(s)));
    CHECK(noexcept(slb::forward<S const&>(cs)));
  }
  /* C++11 `constexpr` */ {
    constexpr S& a = slb::forward<S&>(s);
    constexpr S const& b = slb::forward<S const&>(s);
    constexpr S const& c = slb::forward<S const&>(cs);
    (void)a;
    (void)b;
    (void)c;
  }

  // lvalue -> rvalue
  /* type */ {
    CHECK(std::is_same<decltype(slb::forward<S>(s)), S&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S&&>(s)), S&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const>(s)), S const&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const&&>(s)), S const&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const>(cs)), S const&&>::value);
    CHECK(
        std::is_same<decltype(slb::forward<S const&&>(cs)), S const&&>::value);
  }
  /* `noexcept` */ {
    CHECK(noexcept(slb::forward<S>(s)));
    CHECK(noexcept(slb::forward<S&&>(s)));
    CHECK(noexcept(slb::forward<S const>(s)));
    CHECK(noexcept(slb::forward<S const&&>(s)));
    CHECK(noexcept(slb::forward<S const>(cs)));
    CHECK(noexcept(slb::forward<S const&&>(cs)));
  }
#ifndef _MSC_VER // MSVC doesn't like constexpr rvalue-refs to static variables.
  /* C++11 `constexpr` */ {
    constexpr S&& a = slb::forward<S>(s);
    constexpr S&& b = slb::forward<S&&>(s);
    constexpr S const&& c = slb::forward<S const>(s);
    constexpr S const&& d = slb::forward<S const&&>(s);
    constexpr S const&& e = slb::forward<S const>(cs);
    constexpr S const&& f = slb::forward<S const&&>(cs);
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
  }
#endif

  // rvalue -> rvalue
  struct {
    constexpr S operator()() const noexcept { return S{}; }
  } make_s;

  struct {
    constexpr S const operator()() const noexcept { return S{}; }
  } make_cs;

  /* type */ {
    CHECK(std::is_same<decltype(slb::forward<S>(make_s())), S&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S&&>(make_s())), S&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const>(make_s())),
                       S const&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const&&>(make_s())),
                       S const&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const>(make_cs())),
                       S const&&>::value);
    CHECK(std::is_same<decltype(slb::forward<S const&&>(make_cs())),
                       S const&&>::value);
  }
  /* `noexcept` */ {
    CHECK(noexcept(slb::forward<S>(make_s())));
    CHECK(noexcept(slb::forward<S&&>(make_s())));
    CHECK(noexcept(slb::forward<S const>(make_s())));
    CHECK(noexcept(slb::forward<S const&&>(make_s())));
    CHECK(noexcept(slb::forward<S const>(make_cs())));
    CHECK(noexcept(slb::forward<S const&&>(make_cs())));
  }
  /* C++11 `constexpr` */ {
    constexpr S a = slb::forward<S>(make_s());
    constexpr S b = slb::forward<S&&>(make_s());
    constexpr S c = slb::forward<S const>(make_s());
    constexpr S d = slb::forward<S const&&>(make_s());
    constexpr S e = slb::forward<S const>(make_cs());
    constexpr S f = slb::forward<S const&&>(make_cs());
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
  }

  CHECK_CXX14_CONSTEXPR(forward, {
    int x = 0;
    int const cx = 1;
    struct {
      constexpr int operator()() const noexcept { return 2; }
    } make_x{};
    return slb::forward<int&>(x) == 0 && slb::forward<int const&>(x) == 0 &&
           slb::forward<int const&>(cx) == 1 && slb::forward<int>(x) == 0 &&
           slb::forward<int&&>(x) == 0 && slb::forward<int const>(x) == 0 &&
           slb::forward<int const&&>(x) == 0 &&
           slb::forward<int const>(cx) == 1 &&
           slb::forward<int const&&>(cx) == 1 &&
           slb::forward<int>(make_x()) == 2 &&
           slb::forward<int&&>(make_x()) == 2 &&
           slb::forward<int const>(make_x()) == 2 &&
           slb::forward<int const&&>(make_x()) == 2;
  });
}

// template<class T>
//   constexpr remove_reference_t<T>&& move(T&&) noexcept;
TEST_CASE("move", "[forward]") {
  struct S {};

  // lvalue -> rvalue
  static S s;
  static S const cs{};

  /* type */ {
    CHECK(std::is_same<decltype(slb::move(s)), S&&>::value);
    CHECK(std::is_same<decltype(slb::move(cs)), S const&&>::value);
  }
  /* `noexcept` */ {
    CHECK(noexcept(slb::move(s)));
    CHECK(noexcept(slb::move(cs)));
  }
#ifndef _MSC_VER // MSVC doesn't like constexpr rvalue-refs to static variables.
  /* C++11 `constexpr` */ {
    constexpr S&& a = slb::move(s);
    constexpr S const&& b = slb::move(cs);
    (void)a;
    (void)b;
  }
#endif

  // rvalue -> rvalue
  struct {
    constexpr S operator()() const noexcept { return S{}; }
  } make_s;

  struct {
    constexpr S const operator()() const noexcept { return S{}; }
  } make_cs;

  /* type */ {
    CHECK(std::is_same<decltype(slb::move(make_s())), S&&>::value);
    CHECK(std::is_same<decltype(slb::move(make_cs())), S const&&>::value);
  }
  /* `noexcept` */ {
    CHECK(noexcept(slb::move(make_s())));
    CHECK(noexcept(slb::move(make_cs())));
  }
  /* C++11 `constexpr` */ {
#if __has_warning("-Wpessimizing-move")
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpessimizing-move"
#endif
    constexpr S a = slb::move(make_s());
    constexpr S b = slb::move(make_cs());
#if __has_warning("-Wpessimizing-move")
#pragma clang diagnostic pop
#endif
    (void)a;
    (void)b;
  }

  CHECK_CXX14_CONSTEXPR(move, {
    int x = 0;
    int const cx = 1;
    struct {
      constexpr int operator()() const noexcept { return 2; }
    } make_x{};
    return slb::move(x) == 0 && slb::move(cx) == 1 && slb::move(make_x()) == 2;
  });
}

// template<class T>
//   constexpr conditional_t<
//       !is_nothrow_move_constructible_v<T> && is_copy_constructible_v<T>,
//       const T&,
//       T&&>
//     move_if_noexcept(T& x) noexcept;

template <bool IsNothrow = true>
struct MoveNothrow {
  MoveNothrow() = default;
  MoveNothrow(MoveNothrow const&) {}
  MoveNothrow(MoveNothrow&&) noexcept(IsNothrow) {}
};

TEST_CASE("move_if_noexcept", "[forward]") {
  struct MoveOnly {
    MoveOnly() = default;
    MoveOnly(MoveOnly const&) = delete;
    MoveOnly(MoveOnly&&) {}
  };

  struct Legacy {
    Legacy() = default;
    Legacy(Legacy const&) {}
  };

  struct Weird {
    Weird() = default;
    Weird(Weird const&) {}
    Weird(Weird const&&) noexcept {}
  };

  // lvalue -> rvalue
  static MoveNothrow<true> mnt;
  static MoveOnly mo;
  static MoveOnly const cmo{};
  static Weird w{};
  static Weird const cw{};

  /* type */ {
    CHECK(std::is_same<decltype(slb::move_if_noexcept(mnt)),
                       MoveNothrow<true>&&>::value);
    CHECK(std::is_same<decltype(slb::move_if_noexcept(mo)), MoveOnly&&>::value);
    CHECK(std::is_same<decltype(slb::move_if_noexcept(cmo)),
                       MoveOnly const&&>::value);
    CHECK(std::is_same<decltype(slb::move_if_noexcept(w)), Weird&&>::value);
    CHECK(std::is_same<decltype(slb::move_if_noexcept(cw)),
                       Weird const&&>::value);
  }
  /* `noexcept` */ {
    CHECK(noexcept(slb::move_if_noexcept(mnt)));
    CHECK(noexcept(slb::move_if_noexcept(mo)));
    CHECK(noexcept(slb::move_if_noexcept(cmo)));
    CHECK(noexcept(slb::move_if_noexcept(w)));
    CHECK(noexcept(slb::move_if_noexcept(cw)));
  }
#ifndef _MSC_VER // MSVC doesn't like constexpr rvalue-refs to static variables.
  /* C++11 `constexpr` */ {
    constexpr MoveNothrow<true>&& a = slb::move_if_noexcept(mnt);
    constexpr MoveOnly&& b = slb::move_if_noexcept(mo);
    constexpr MoveOnly const&& c = slb::move_if_noexcept(cmo);
    constexpr Weird&& d = slb::move_if_noexcept(w);
    constexpr Weird const&& e = slb::move_if_noexcept(cw);
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
  }
#endif

  // lvalue -> lvalue const
  static MoveNothrow<true> const cmnt{};
  static MoveNothrow<false> mnf;
  static MoveNothrow<false> const cmnf{};
  static Legacy l;
  static Legacy const cl{};

  /* type */ {
    CHECK(std::is_same<decltype(slb::move_if_noexcept(cmnt)),
                       MoveNothrow<true> const&>::value);
    CHECK(std::is_same<decltype(slb::move_if_noexcept(mnf)),
                       MoveNothrow<false> const&>::value);
    CHECK(std::is_same<decltype(slb::move_if_noexcept(cmnf)),
                       MoveNothrow<false> const&>::value);
    CHECK(
        std::is_same<decltype(slb::move_if_noexcept(l)), Legacy const&>::value);
    CHECK(std::is_same<decltype(slb::move_if_noexcept(cl)),
                       Legacy const&>::value);
  }
  /* `noexcept` */ {
    CHECK(noexcept(slb::move_if_noexcept(cmnt)));
    CHECK(noexcept(slb::move_if_noexcept(mnf)));
    CHECK(noexcept(slb::move_if_noexcept(cmnf)));
    CHECK(noexcept(slb::move_if_noexcept(l)));
    CHECK(noexcept(slb::move_if_noexcept(cl)));
  }
  /* C++11 `constexpr` */ {
    constexpr MoveNothrow<true> const& a = slb::move_if_noexcept(cmnt);
    constexpr MoveNothrow<false> const& b = slb::move_if_noexcept(mnf);
    constexpr MoveNothrow<false> const& c = slb::move_if_noexcept(cmnf);
    constexpr Legacy const& d = slb::move_if_noexcept(l);
    constexpr Legacy const& e = slb::move_if_noexcept(cl);
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
  }

  CHECK_CXX14_CONSTEXPR(move_if_noexcept, {
    int x = 0;
    int const cx = 1;
    return slb::move_if_noexcept(x) == 0 && slb::move_if_noexcept(cx) == 1;
  });
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
    CHECK_DECLTYPE(std::size_t, is::size());
    CHECK_NOEXCEPT(is::size());
    CHECK(is::size() == 4);
    CHECK_CXX11_CONSTEXPR(size, is::size());
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
void check_make_integer_sequence() {
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
  CHECK_NESTED(make_integer_sequence<char>());
  CHECK_NESTED(make_integer_sequence<signed char>());
  CHECK_NESTED(make_integer_sequence<unsigned char>());
  CHECK_NESTED(make_integer_sequence<char16_t>());
  CHECK_NESTED(make_integer_sequence<char32_t>());
  CHECK_NESTED(make_integer_sequence<wchar_t>());
  CHECK_NESTED(make_integer_sequence<short>());
  CHECK_NESTED(make_integer_sequence<int>());
  CHECK_NESTED(make_integer_sequence<long>());
  CHECK_NESTED(make_integer_sequence<long long>());
  CHECK_NESTED(make_integer_sequence<unsigned short>());
  CHECK_NESTED(make_integer_sequence<unsigned int>());
  CHECK_NESTED(make_integer_sequence<unsigned long>());
  CHECK_NESTED(make_integer_sequence<unsigned long long>());
  CHECK_NESTED(make_integer_sequence<std::size_t>());
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
