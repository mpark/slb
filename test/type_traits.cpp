/*
  SLB.TypeTraits

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <slb/type_traits.hpp>

#include <functional>
#include <type_traits>

#include "catch.hpp"

#define CHECK_NESTED(...)                                                      \
  do {                                                                         \
    INFO(__FILE__ "(" << __LINE__ << "): " #__VA_ARGS__);                      \
    check_##__VA_ARGS__;                                                       \
  } while (false)

// [meta.help], helper class

// template<class T, T v> struct integral_constant;

template <typename T, T V>
T deduce_std_integral_constant(std::integral_constant<T, V>) {
  return V;
}

TEST_CASE("integral_constant", "[meta.help]") {
  /* using value_type = T; */ {
    CHECK(std::is_same<slb::integral_constant<int, 0>::value_type, int>::value);
  }

  /* using type = integral_constant; */ {
    CHECK(std::is_same<slb::integral_constant<int, 0>::type,
                       slb::integral_constant<int, 0>>::value);
  }

  /* constexpr operator value_type() const noexcept; */ {
    slb::integral_constant<int, 0> ic;
    CHECK(std::is_convertible<decltype(ic), int>::value);
    CHECK(noexcept(static_cast<int>(ic)));
    CHECK(static_cast<int>(ic) == 0);
    constexpr int ic_value = ic;
    CHECK(ic_value == 0);
  }

  /* constexpr value_type operator()() const noexcept; */ {
    slb::integral_constant<int, 0> ic;
    CHECK(std::is_same<decltype(ic()), int>::value);
    CHECK(noexcept(ic()));
    CHECK(ic() == 0);
    constexpr int ic_value = ic();
    CHECK(ic_value == 0);
  }

  /* std-compatible */ {
    slb::integral_constant<int, 0> slb_ic;
    std::integral_constant<int, 0> std_ic = slb_ic;
    (void)std_ic;

    CHECK(deduce_std_integral_constant(slb_ic) == 0);
  }
}

// template<bool B>
//   using bool_constant = integral_constant<bool, B>;

#if __cpp_lib_bool_constant
template <bool B>
bool deduce_std_bool_constant(std::bool_constant<B>) {
  return B;
}
#endif

template <bool B>
bool deduce_std_bool_integral_constant(std::integral_constant<bool, B>) {
  return B;
}

TEST_CASE("bool_constant", "[meta.help]") {
  CHECK(std::is_same<slb::bool_constant<true>::type,
                     slb::integral_constant<bool, true>>::value);

  /* using true_type = bool_constant<true>; */ {
    CHECK(std::is_same<slb::true_type, slb::bool_constant<true>>::value);
  }

  /* using false_type = bool_constant<false>; */ {
    CHECK(std::is_same<slb::false_type, slb::bool_constant<false>>::value);
  }

  /* std-compatible */ {
    slb::integral_constant<bool, true> ic_true;
    std::integral_constant<bool, true> std_true = ic_true;
    (void)std_true;

#if __cpp_lib_bool_constant
    CHECK(deduce_std_bool_constant(ic_true) == true);
#endif
    CHECK(deduce_std_bool_integral_constant(ic_true) == true);

    slb::integral_constant<bool, false> ic_false;
    std::integral_constant<bool, false> std_false = ic_false;
    (void)std_false;

#if __cpp_lib_bool_constant
    CHECK(deduce_std_bool_constant(ic_false) == false);
#endif
    CHECK(deduce_std_bool_integral_constant(ic_false) == false);
  }
}

// [meta.unary.cat], primary type categories

// template<class T> struct is_void;
TEST_CASE("is_void", "[meta.unary.cat]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_void<void>>::value);
}

// template<class T> struct is_integral;
TEST_CASE("is_integral", "[meta.unary.cat]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_integral<int>>::value);
}

// template<class T> struct is_floating_point;
TEST_CASE("is_floating_point", "[meta.unary.cat]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_floating_point<double>>::value);
}

// template<class T> struct is_array;
TEST_CASE("is_array", "[meta.unary.cat]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_array<int[3]>>::value);
}

// template<class T> struct is_pointer;
TEST_CASE("is_pointer", "[meta.unary.cat]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_pointer<int*>>::value);
}

// template<class T> struct is_lvalue_reference;
TEST_CASE("is_lvalue_reference", "[meta.unary.cat]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_lvalue_reference<int&>>::value);
}

// template<class T> struct is_rvalue_reference;
TEST_CASE("is_rvalue_reference", "[meta.unary.cat]") {
  CHECK(
      std::is_base_of<slb::true_type, slb::is_rvalue_reference<int&&>>::value);
}

// template<class T> struct is_member_object_pointer;
TEST_CASE("is_member_object_pointer", "[meta.unary.cat]") {
  class C {};
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_member_object_pointer<int C::*>>::value);

  CHECK(std::is_base_of<slb::false_type,
                        slb::is_member_object_pointer<int (C::*)()>>::value);
  CHECK(std::is_base_of<
        slb::false_type,
        slb::is_member_object_pointer<int (C::*)() noexcept>>::value);
}

// template<class T> struct is_member_function_pointer;
TEST_CASE("is_member_function_pointer", "[meta.unary.cat]") {
  class C {};
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_member_function_pointer<int (C::*)()>>::value);
  CHECK(std::is_base_of<
        slb::true_type,
        slb::is_member_function_pointer<int (C::*)() noexcept>>::value);

  CHECK(std::is_base_of<slb::false_type,
                        slb::is_member_function_pointer<int C::*>>::value);
}

// template<class T> struct is_enum;
TEST_CASE("is_enum", "[meta.unary.cat]") {
  enum E {};
  CHECK(std::is_base_of<slb::true_type, slb::is_enum<E>>::value);
}

// template<class T> struct is_union;
TEST_CASE("is_union", "[meta.unary.cat]") {
  union U {};
  CHECK(std::is_base_of<slb::true_type, slb::is_union<U>>::value);
}

// template<class T> struct is_class;
TEST_CASE("is_class", "[meta.unary.cat]") {
  class C {};
  CHECK(std::is_base_of<slb::true_type, slb::is_class<C>>::value);
}

// template<class T> struct is_function;
TEST_CASE("is_function", "[meta.unary.cat]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_function<void()>>::value);
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_function<void() noexcept>>::value);

  CHECK(std::is_base_of<slb::true_type, slb::is_function<void(...)>>::value);
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_function<void(...) noexcept>>::value);

  CHECK(std::is_base_of<slb::false_type, slb::is_function<int const>>::value);
  CHECK(std::is_base_of<slb::false_type, slb::is_function<int&>>::value);
}

// template<class T> struct is_null_pointer;
TEST_CASE("is_null_pointer", "[meta.unary.cat]") {
  using nullptr_t = decltype(nullptr);
  CHECK(
      std::is_base_of<slb::true_type, slb::is_null_pointer<nullptr_t>>::value);

  {
    CHECK(slb::is_null_pointer<nullptr_t>::value);
    CHECK(slb::is_null_pointer<nullptr_t const>::value);
    CHECK(slb::is_null_pointer<nullptr_t volatile>::value);
    CHECK(slb::is_null_pointer<nullptr_t const volatile>::value);
    CHECK_FALSE(slb::is_null_pointer<int>::value);
  }
}

// [meta.unary.comp], composite type categories

// template<class T> struct is_reference;
TEST_CASE("is_reference", "[meta.unary.comp]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_reference<int&>>::value);
}

// template<class T> struct is_arithmetic;
TEST_CASE("is_arithmetic", "[meta.unary.comp]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_arithmetic<int>>::value);
}

// template<class T> struct is_fundamental;
TEST_CASE("is_fundamental", "[meta.unary.comp]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_fundamental<int>>::value);
}

// template<class T> struct is_object;
TEST_CASE("is_object", "[meta.unary.comp]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_object<int>>::value);

  CHECK(std::is_base_of<slb::false_type, slb::is_object<void()>>::value);
  CHECK(
      std::is_base_of<slb::false_type, slb::is_object<void() noexcept>>::value);
}

// template<class T> struct is_scalar;
TEST_CASE("is_scalar", "[meta.unary.comp]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_scalar<int>>::value);
}

// template<class T> struct is_compound;
TEST_CASE("is_compound", "[meta.unary.comp]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_compound<int[3]>>::value);
}

// template<class T> struct is_member_pointer;
TEST_CASE("is_member_pointer", "[meta.unary.comp]") {
  class C {};
  CHECK(
      std::is_base_of<slb::true_type, slb::is_member_pointer<int C::*>>::value);
}

// [meta.unary.prop], type properties

// template<class T> struct is_const;
TEST_CASE("is_const", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_const<int const>>::value);
}

// template<class T> struct is_volatile;
TEST_CASE("is_volatile", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_volatile<int volatile>>::value);
}

// template<class T> struct is_trivial;
TEST_CASE("is_trivial", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_trivial<int>>::value);
}

#if SLB_TRIVIALITY_TRAITS
// template<class T> struct is_trivially_copyable;
TEST_CASE("is_trivially_copyable", "[meta.unary.prop]") {
  CHECK(
      std::is_base_of<slb::true_type, slb::is_trivially_copyable<int>>::value);
}
#endif

// template<class T> struct is_standard_layout;
TEST_CASE("is_standard_layout", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_standard_layout<int>>::value);
}

// template<class T> struct is_empty;
TEST_CASE("is_empty", "[meta.unary.prop]") {
  struct Empty {};
  CHECK(std::is_base_of<slb::true_type, slb::is_empty<Empty>>::value);
}

// template<class T> struct is_polymorphic;
TEST_CASE("is_polymorphic", "[meta.unary.prop]") {
  class Polymorphic {
    virtual void fun() {}
  };
  CHECK(
      std::is_base_of<slb::true_type, slb::is_polymorphic<Polymorphic>>::value);
}

// template<class T> struct is_abstract;
TEST_CASE("is_abstract", "[meta.unary.prop]") {
  class Abstract {
    virtual void fun() = 0;
  };
  CHECK(std::is_base_of<slb::true_type, slb::is_abstract<Abstract>>::value);
}

#if __cpp_lib_is_final || __has_feature(is_final) || (__GNUC__ > 4) ||         \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
// template<class T> struct is_final;
TEST_CASE("is_final", "[meta.unary.prop]") {
  class Final final {};
  CHECK(std::is_base_of<slb::true_type, slb::is_final<Final>>::value);
}
#endif

#if __cpp_lib_is_aggregate || __has_feature(is_aggregate) || (__GNUC__ >= 7)
// template<class T> struct is_aggregate;
TEST_CASE("is_aggregate", "[meta.unary.prop]") {
  struct Aggregate {
    int obj;
  };
  CHECK(std::is_base_of<slb::true_type, slb::is_aggregate<Aggregate>>::value);
}
#endif

// template<class T> struct is_signed;
TEST_CASE("is_signed", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_signed<signed int>>::value);
}

// template<class T> struct is_unsigned;
TEST_CASE("is_unsigned", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_unsigned<unsigned int>>::value);
}

// template<class T, class... Args> struct is_constructible;
TEST_CASE("is_constructible", "[meta.unary.prop]") {
  CHECK(
      std::is_base_of<slb::true_type, slb::is_constructible<int, int>>::value);

  /* destructible */ {
    struct ND {
      ND(int) {}
      ~ND() = delete;
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_constructible<ND, int>>::value);
  }
}

// template<class T> struct is_default_constructible;
TEST_CASE("is_default_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_default_constructible<int>>::value);

  /* destructible */ {
    struct ND {
      ND() {}
      ~ND() = delete;
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_default_constructible<ND>>::value);
  }
}

// template<class T> struct is_copy_constructible;
TEST_CASE("is_copy_constructible", "[meta.unary.prop]") {
  CHECK(
      std::is_base_of<slb::true_type, slb::is_copy_constructible<int>>::value);

  /* destructible */ {
    struct ND {
      ND(ND const&) {}
      ~ND() = delete;
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_copy_constructible<ND>>::value);
  }
}

// template<class T> struct is_move_constructible;
TEST_CASE("is_move_constructible", "[meta.unary.prop]") {
  CHECK(
      std::is_base_of<slb::true_type, slb::is_move_constructible<int>>::value);

  /* destructible */ {
    struct ND {
      ND(ND&&) {}
      ~ND() = delete;
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_move_constructible<ND>>::value);
  }
}

#if SLB_TRIVIALITY_TRAITS
// template<class T, class ...Args> struct is_trivially_constructible;
TEST_CASE("is_trivially_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_trivially_constructible<int>>::value);

  /* destructible */ {
    struct ND {
      ND() = default;
      ~ND() = delete;
    };
    struct NTD {
      NTD() = default;
      ~NTD() {}
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_constructible<ND>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_constructible<NTD>>::value);
  }
}
#endif

// template<class T> struct is_trivially_default_constructible;
TEST_CASE("is_trivially_default_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_trivially_default_constructible<int>>::value);

  {
    struct E {};
    struct S {
      int m;
    };
    struct D {
      int m;
      D() = default;
    };
    struct U {
      int m;
      U() : m(0) {}
    };
    struct ND {
      int m;
      ND() = default;
      ~ND() = delete;
    };
    struct NTD {
      int m;
      NTD() = default;
      ~NTD() {}
    };

    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_default_constructible<int>>::value);
    CHECK(
        std::is_base_of<slb::false_type,
                        slb::is_trivially_default_constructible<int&>>::value);
    CHECK(
        std::is_base_of<slb::false_type,
                        slb::is_trivially_default_constructible<int&&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_default_constructible<E>>::value);
    CHECK(std::is_base_of<
          slb::true_type,
          slb::is_trivially_default_constructible<E const>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_default_constructible<S>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_default_constructible<D>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_default_constructible<U>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_default_constructible<U&>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_default_constructible<U&&>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_default_constructible<ND>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_default_constructible<NTD>>::value);
    CHECK(
        std::is_base_of<slb::false_type,
                        slb::is_trivially_default_constructible<NTD&>>::value);
    CHECK(
        std::is_base_of<slb::false_type,
                        slb::is_trivially_default_constructible<NTD&&>>::value);
  }
}

// template<class T> struct is_trivially_copy_constructible;
TEST_CASE("is_trivially_copy_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_trivially_copy_constructible<int>>::value);

  {
    struct S {
      int m;
    };
    struct D {
      int m;
      D(D const&) = default;
    };
    struct U {
      int m;
      U(U const& u) : m(u.m) {}
    };
    struct ND {
      int m;
      ND(ND const&) = default;
      ~ND() = delete;
    };
    struct NTD {
      int m;
      NTD(NTD const&) = default;
      ~NTD() {}
    };

    CHECK(std::is_base_of<
          slb::true_type,
          slb::is_trivially_copy_constructible<int const>>::value);
    CHECK(std::is_base_of<
          slb::true_type,
          slb::is_trivially_copy_constructible<int const&>>::value);
    CHECK(std::is_base_of<
          slb::false_type,
          slb::is_trivially_copy_constructible<int const&&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_constructible<S>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_constructible<D>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_copy_constructible<U>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_constructible<U&>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_copy_constructible<U&&>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_copy_constructible<ND>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_copy_constructible<NTD>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_constructible<NTD&>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_copy_constructible<NTD&&>>::value);
  }
}

// template<class T> struct is_trivially_move_constructible;
TEST_CASE("is_trivially_move_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_trivially_move_constructible<int>>::value);

  {
    struct S {
      int m;
    };
    struct D {
      int m;
      D(D&&) = default;
    };
    struct U {
      int m;
      U(U&& u) : m(u.m) {}
    };
    struct ND {
      int m;
      ND(ND&&) = default;
      ~ND() = delete;
    };
    struct NTD {
      int m;
      NTD(NTD&&) = default;
      ~NTD() {}
    };

    CHECK(std::is_base_of<
          slb::true_type,
          slb::is_trivially_move_constructible<int const>>::value);
    CHECK(std::is_base_of<
          slb::true_type,
          slb::is_trivially_move_constructible<int const&>>::value);
    CHECK(std::is_base_of<
          slb::true_type,
          slb::is_trivially_move_constructible<int const&&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_constructible<S>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_constructible<D>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_move_constructible<U>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_constructible<U&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_constructible<U&&>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_move_constructible<ND>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_move_constructible<NTD>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_constructible<NTD&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_constructible<NTD&&>>::value);
  }
}

// template<class T, class ...Args> struct is_nothrow_constructible;
TEST_CASE("is_nothrow_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_nothrow_constructible<int, int>>::value);

  /* destructible */ {
    struct ND {
      ND(int) noexcept {}
      ~ND() = delete;
    };
    struct NND {
      NND(int) noexcept {}
      ~NND() noexcept(false) {}
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_nothrow_constructible<ND, int>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_nothrow_constructible<NND, int>>::value);
  }
}

// template<class T> struct is_nothrow_default_constructible;
TEST_CASE("is_nothrow_default_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_nothrow_default_constructible<int>>::value);

  /* destructible */ {
    struct ND {
      ND() noexcept {}
      ~ND() = delete;
    };
    struct NND {
      NND() noexcept {}
      ~NND() noexcept(false) {}
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_nothrow_default_constructible<ND>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_nothrow_default_constructible<NND>>::value);
  }
}

// template<class T> struct is_nothrow_copy_constructible;
TEST_CASE("is_nothrow_copy_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_nothrow_copy_constructible<int>>::value);

  /* destructible */ {
    struct ND {
      ND(ND const&) noexcept {}
      ~ND() = delete;
    };
    struct NND {
      NND(NND const&) noexcept {}
      ~NND() noexcept(false) {}
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_nothrow_copy_constructible<ND>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_nothrow_copy_constructible<NND>>::value);
  }
}

// template<class T> struct is_nothrow_move_constructible;
TEST_CASE("is_nothrow_move_constructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_nothrow_move_constructible<int>>::value);

  /* destructible */ {
    struct ND {
      ND(ND&&) noexcept {}
      ~ND() = delete;
    };
    struct NND {
      NND(NND&&) noexcept {}
      ~NND() noexcept(false) {}
    };

    CHECK(std::is_base_of<slb::false_type,
                          slb::is_nothrow_move_constructible<ND>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_nothrow_move_constructible<NND>>::value);
  }
}

// template<class T> struct is_destructible;
TEST_CASE("is_destructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_destructible<int>>::value);
}

// template<class T> struct is_trivially_destructible;
TEST_CASE("is_trivially_destructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_trivially_destructible<int>>::value);
}

// template<class T> struct is_nothrow_destructible;
TEST_CASE("is_nothrow_destructible", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_nothrow_destructible<int>>::value);
}

// template<class T, class U> struct is_assignable;
TEST_CASE("is_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_assignable<int&, int>>::value);
}

// template<class T> struct is_copy_assignable;
TEST_CASE("is_copy_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_copy_assignable<int>>::value);
}

// template<class T> struct is_move_assignable;
TEST_CASE("is_move_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_move_assignable<int>>::value);
}

#if SLB_TRIVIALITY_TRAITS
// template<class T, class U> struct is_trivially_assignable;
TEST_CASE("is_trivially_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_trivially_assignable<int&, int>>::value);
}
#endif

// template<class T> struct is_trivially_copy_assignable;
TEST_CASE("is_trivially_copy_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_trivially_copy_assignable<int>>::value);

  {
    struct S {
      int m;
    };
    struct D {
      int m;
      D& operator=(D const&) = default;
    };
    struct U {
      int m;
      U& operator=(U const& u) { return m = u.m, *this; }
    };
    struct NTD {
      int m;
      NTD& operator=(NTD const&) = default;
      ~NTD() {}
    };

    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_assignable<int&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_assignable<int&&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_assignable<S>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_assignable<D>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_copy_assignable<U>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_copy_assignable<U&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_copy_assignable<NTD>>::value);
  }
}

// template<class T> struct is_trivially_move_assignable;
TEST_CASE("is_trivially_move_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_trivially_move_assignable<int>>::value);

  {
    struct S {
      int m;
    };
    struct D {
      int m;
      D& operator=(D&&) = default;
    };
    struct U {
      int m;
      U& operator=(U&& u) { return m = u.m, *this; }
    };
    struct NTD {
      int m;
      NTD& operator=(NTD&&) = default;
      ~NTD() {}
    };

    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_assignable<int&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_assignable<int&&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_assignable<S>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_assignable<D>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_move_assignable<U>>::value);
    CHECK(std::is_base_of<slb::false_type,
                          slb::is_trivially_move_assignable<U&>>::value);
    CHECK(std::is_base_of<slb::true_type,
                          slb::is_trivially_move_assignable<NTD>>::value);
  }
}

// template<class T, class U> struct is_nothrow_assignable;
TEST_CASE("is_nothrow_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_nothrow_assignable<int&, int>>::value);
}

// template<class T> struct is_nothrow_copy_assignable;
TEST_CASE("is_nothrow_copy_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_nothrow_copy_assignable<int>>::value);
}

// template<class T> struct is_nothrow_move_assignable;
TEST_CASE("is_nothrow_move_assignable", "[meta.unary.prop]") {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_nothrow_move_assignable<int>>::value);
}

// template<class T> struct has_virtual_destructor;
TEST_CASE("has_virtual_destructor", "[meta.unary.prop]") {
  class WithVirtualDestructor {
    virtual ~WithVirtualDestructor() {}
  };
  CHECK(std::is_base_of<
        slb::true_type,
        slb::has_virtual_destructor<WithVirtualDestructor>>::value);
}

#if __cpp_lib_has_unique_object_representations ||                             \
    __has_feature(has_unique_object_representations) || (__GNUC__ >= 7)
// template<class T> struct has_unique_object_representations;
TEST_CASE("has_unique_object_representations", "[meta.unary.prop]") {
  class WithUniqueObjectRepresentations {
    int obj;
  };
  CHECK(std::is_base_of<slb::true_type,
                        slb::has_unique_object_representations<
                            WithUniqueObjectRepresentations>>::value);
}
#endif

// [meta.unary.prop.query] type property queries

// template<class T> struct alignment_of;
TEST_CASE("alignment_of", "[meta.unary.prop.query]") {
  CHECK(std::is_base_of<slb::integral_constant<std::size_t, alignof(int)>,
                        slb::alignment_of<int>>::value);
}

// template<class T> struct rank;
TEST_CASE("rank", "[meta.unary.prop.query]") {
  CHECK(std::is_base_of<slb::integral_constant<std::size_t, 3>,
                        slb::rank<int[2][2][2]>>::value);
}

// template<class T, unsigned I = 0> struct extent;
TEST_CASE("extent", "[meta.unary.prop.query]") {
  CHECK(std::is_base_of<slb::integral_constant<std::size_t, 2>,
                        slb::extent<int[2][3]>>::value);

  CHECK(std::is_base_of<slb::integral_constant<std::size_t, 3>,
                        slb::extent<int[2][3], 1>>::value);
}

// [meta.rel], type relations

// template<class T, class U> struct is_same;
TEST_CASE("is_same", "[meta.rel]") {
  CHECK(std::is_base_of<slb::true_type, slb::is_same<int, int>>::value);
}

// template<class Base, class Derived> struct is_base_of;
TEST_CASE("is_base_of", "[meta.rel]") {
  class Base {};
  class Derived : public Base {};
  CHECK(std::is_base_of<slb::true_type, slb::is_base_of<Base, Derived>>::value);
}

// template<class From, class To> struct is_convertible;
TEST_CASE("is_convertible", "[meta.rel]") {
  CHECK(
      std::is_base_of<slb::true_type, slb::is_convertible<float, int>>::value);
}

// template <class Fn, class... ArgTypes>
// struct is_invocable;
// template <class R, class Fn, class... ArgTypes>
// struct is_invocable_r;
// template <class Fn, class... ArgTypes>
// struct is_nothrow_invocable;
// template <class R, class Fn, class... ArgTypes>
// struct is_nothrow_invocable_r;
template <typename T, bool IsNothrow = true>
struct smart_ptr {
  T* ptr;
  T& operator*() const noexcept(IsNothrow) { return *ptr; }
};
template <typename T>
using smart_ptr_throws = smart_ptr<T, false>;

template <typename T, bool IsNothrow = true>
struct conv_to {
  operator T() const noexcept(IsNothrow) { return T(); }
};
template <typename T>
using conv_to_throws = conv_to<T, false>;

template <typename T, bool IsNothrow = true>
struct conv_from {
  conv_from(T) noexcept(IsNothrow) {}
};
template <typename T>
using conv_from_throws = conv_from<T, false>;

// Account for P0012: "Make exception specifications be part of the type
// system".
static constexpr bool p0012 = !std::is_same<void(), void() noexcept>::value;

std::true_type const nothrows{};
std::false_type const throws{};
std::integral_constant<bool, p0012> const p0012_nothrows{};

template <typename R,
          typename F,
          typename... Args,
          bool IsNothrow,
          bool IsNothrowR = IsNothrow>
void check_invocable(std::integral_constant<bool, IsNothrow>,
                     std::integral_constant<bool, IsNothrowR> = {}) {
  CHECK(std::is_base_of<slb::true_type, slb::is_invocable<F, Args...>>::value);
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_invocable_r<R, F, Args...>>::value);
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_invocable_r<void, F, Args...>>::value);
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_invocable_r<void const, F, Args...>>::value);

  CHECK(std::is_base_of<slb::bool_constant<IsNothrow>,
                        slb::is_nothrow_invocable<F, Args...>>::value);
  CHECK(std::is_base_of<slb::bool_constant<IsNothrowR>,
                        slb::is_nothrow_invocable_r<R, F, Args...>>::value);
  CHECK(std::is_base_of<slb::bool_constant<IsNothrow>,
                        slb::is_nothrow_invocable_r<void, F, Args...>>::value);
  CHECK(std::is_base_of<
        slb::bool_constant<IsNothrow>,
        slb::is_nothrow_invocable_r<void const, F, Args...>>::value);
}

template <typename F, typename... Args>
void check_not_invocable() {
  CHECK(std::is_base_of<slb::false_type, slb::is_invocable<F, Args...>>::value);
  CHECK(std::is_base_of<slb::false_type,
                        slb::is_invocable_r<void, F, Args...>>::value);
  CHECK(std::is_base_of<slb::false_type,
                        slb::is_invocable_r<void const, F, Args...>>::value);

  CHECK(std::is_base_of<slb::false_type,
                        slb::is_nothrow_invocable<F, Args...>>::value);
  CHECK(std::is_base_of<slb::false_type,
                        slb::is_nothrow_invocable_r<void, F, Args...>>::value);
  CHECK(std::is_base_of<
        slb::false_type,
        slb::is_nothrow_invocable_r<void const, F, Args...>>::value);
}

TEST_CASE("is_invocable", "[meta.rel]") {
  struct C {
    int fun(int) noexcept(p0012) { return 0; }
    int cfun(int) const noexcept(p0012) { return 1; }
    int lfun(int) & noexcept(p0012) { return 2; }
    int rfun(int) && noexcept(p0012) { return 3; }
    int clfun(int) const& noexcept(p0012) { return 4; }
    int crfun(int) const&& noexcept(p0012) { return 5; }
  };
  struct D : C {};

  /* mem-fun-ptr */ {
    using Fn = decltype(&C::fun);

    CHECK_NESTED(invocable<int, Fn, C&, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fn, C const&, int>());
    CHECK_NESTED(invocable<int, Fn, C&&, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fn, C const&&, int>());

    CHECK_NESTED(invocable<int, Fn, D&, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fn, D const&, int>());
    CHECK_NESTED(invocable<int, Fn, D&&, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fn, D const&&, int>());

    CHECK_NESTED(
        invocable<int, Fn, std::reference_wrapper<C>, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fn, std::reference_wrapper<C const>, int>());

    CHECK_NESTED(invocable<int, Fn, C*, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fn, C const*, int>());

    CHECK_NESTED(invocable<int, Fn, smart_ptr<C>, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fn, smart_ptr<C const>, int>());

    CHECK_NESTED(invocable<int, Fn, smart_ptr_throws<C>, int>(throws));
    CHECK_NESTED(not_invocable<Fn, smart_ptr_throws<C const>, int>());

    CHECK_NESTED(not_invocable<Fn>());
    CHECK_NESTED(not_invocable<Fn, int&>());
    CHECK_NESTED(not_invocable<Fn, C&, C>());

    using Fnc = decltype(&C::cfun);

    CHECK_NESTED(invocable<int, Fnc, C&, int>(p0012_nothrows));
    CHECK_NESTED(invocable<int, Fnc, C const&, int>(p0012_nothrows));
    CHECK_NESTED(invocable<int, Fnc, C&&, int>(p0012_nothrows));
    CHECK_NESTED(invocable<int, Fnc, C const&&, int>(p0012_nothrows));

    using Fnl = decltype(&C::lfun);

    CHECK_NESTED(invocable<int, Fnl, C&, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fnl, C const&, int>());
    CHECK_NESTED(not_invocable<Fnl, C&&, int>());
    CHECK_NESTED(not_invocable<Fnl, C const&&, int>());

    using Fnr = decltype(&C::rfun);

    CHECK_NESTED(not_invocable<Fnr, C&, int>());
    CHECK_NESTED(not_invocable<Fnr, C const&, int>());
    CHECK_NESTED(invocable<int, Fnr, C&&, int>(p0012_nothrows));
    CHECK_NESTED(not_invocable<Fnr, C const&&, int>());

    using Fncl = decltype(&C::clfun);

    CHECK_NESTED(invocable<int, Fncl, C&, int>(p0012_nothrows));
    CHECK_NESTED(invocable<int, Fncl, C const&, int>(p0012_nothrows));
    CHECK_NESTED(invocable<int, Fncl, C&&, int>(p0012_nothrows));
    CHECK_NESTED(invocable<int, Fncl, C const&&, int>(p0012_nothrows));

    using Fncr = decltype(&C::crfun);

    CHECK_NESTED(not_invocable<Fncr, C&, int>());
    CHECK_NESTED(not_invocable<Fncr, C const&, int>());
    CHECK_NESTED(invocable<int, Fncr, C&&, int>(p0012_nothrows));
    CHECK_NESTED(invocable<int, Fncr, C const&&, int>(p0012_nothrows));
  }

  /* mem-obj-ptr */ {
    using Fn = int C::*;

    CHECK_NESTED(invocable<int&, Fn, C&>(nothrows));
    CHECK_NESTED(invocable<int const&, Fn, C const&>(nothrows));
    CHECK_NESTED(invocable<int&&, Fn, C&&>(nothrows));
    CHECK_NESTED(invocable<int const&&, Fn, C const&&>(nothrows));

    CHECK_NESTED(invocable<int&, Fn, D&>(nothrows));
    CHECK_NESTED(invocable<int const&, Fn, D const&>(nothrows));
    CHECK_NESTED(invocable<int&&, Fn, D&&>(nothrows));
    CHECK_NESTED(invocable<int const&&, Fn, D const&&>(nothrows));

    CHECK_NESTED(invocable<int&, Fn, std::reference_wrapper<C>>(nothrows));
    CHECK_NESTED(
        invocable<int const&, Fn, std::reference_wrapper<C const>>(nothrows));

    CHECK_NESTED(invocable<int&, Fn, C*>(nothrows));
    CHECK_NESTED(invocable<int const&, Fn, C const*>(nothrows));

    CHECK_NESTED(invocable<int&, Fn, smart_ptr<C>>(nothrows));
    CHECK_NESTED(invocable<int const&, Fn, smart_ptr<C const>>(nothrows));

    CHECK_NESTED(invocable<int&, Fn, smart_ptr_throws<C>>(throws));
    CHECK_NESTED(invocable<int const&, Fn, smart_ptr_throws<C const>>(throws));

    CHECK_NESTED(not_invocable<Fn>());
    CHECK_NESTED(not_invocable<Fn, int&>());
    CHECK_NESTED(not_invocable<Fn, C&, int>());
  }

  /* fun-obj */ {
    struct Fn {
      int operator()(int) noexcept { return 42; }
    };

    CHECK_NESTED(invocable<int, Fn, int>(nothrows));
    CHECK_NESTED(invocable<int, Fn, conv_to<int>>(nothrows));
    CHECK_NESTED(invocable<int, Fn, conv_to_throws<int>>(throws));
    CHECK_NESTED(invocable<conv_from<int>, Fn, int>(nothrows));
    CHECK_NESTED(invocable<conv_from_throws<int>, Fn, int>(nothrows, throws));
    CHECK_NESTED(invocable<conv_from_throws<int>, Fn, conv_to_throws<int>>(
        throws, throws));

    CHECK_NESTED(not_invocable<Fn>());
    CHECK_NESTED(not_invocable<Fn, void*>());
    CHECK_NESTED(not_invocable<Fn, int, int>());

    struct S {
      static int f(int) noexcept(p0012) { return 0; }
    };
    using Fn_ptr = decltype(&S::f);

    CHECK_NESTED(invocable<int, Fn_ptr, int>(p0012_nothrows));
  }
}

// [meta.trans.cv], const-volatile modifications

// template<class T>
//   using remove_const_t    = typename remove_const<T>::type;
TEST_CASE("remove_const_t", "[meta.trans.cv]") {
  CHECK(std::is_same<slb::remove_const_t<int const>,
                     std::remove_const<int const>::type>::value);
}

// template<class T>
//   using remove_volatile_t = typename remove_volatile<T>::type;
TEST_CASE("remove_volatile_t", "[meta.trans.cv]") {
  CHECK(std::is_same<slb::remove_volatile_t<int volatile>,
                     std::remove_volatile<int volatile>::type>::value);
}

// template<class T>
//   using remove_cv_t       = typename remove_cv<T>::type;
TEST_CASE("remove_cv_t", "[meta.trans.cv]") {
  CHECK(std::is_same<slb::remove_cv_t<int const volatile>,
                     std::remove_cv<int const volatile>::type>::value);
}

// template<class T>
//   using add_const_t       = typename add_const<T>::type;
TEST_CASE("add_const_t", "[meta.trans.cv]") {
  CHECK(std::is_same<slb::add_const_t<int>, std::add_const<int>::type>::value);
}

// template<class T>
//   using add_volatile_t    = typename add_volatile<T>::type;
TEST_CASE("add_volatile_t", "[meta.trans.cv]") {
  CHECK(std::is_same<slb::add_volatile_t<int>,
                     std::add_volatile<int>::type>::value);
}

// template<class T>
//   using add_cv_t          = typename add_cv<T>::type;
TEST_CASE("add_cv_t", "[meta.trans.cv]") {
  CHECK(std::is_same<slb::add_cv_t<int>, std::add_cv<int>::type>::value);
}

// [meta.trans.ref], reference modifications

// template<class T>
//   using remove_reference_t     = typename remove_reference<T>::type;
TEST_CASE("remove_reference_t", "[meta.trans.ref]") {
  CHECK(std::is_same<slb::remove_reference_t<int&>,
                     std::remove_reference<int&>::type>::value);
}

// template<class T>
//   using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
TEST_CASE("add_lvalue_reference_t", "[meta.trans.ref]") {
  CHECK(std::is_same<slb::add_lvalue_reference_t<int>,
                     std::add_lvalue_reference<int>::type>::value);
}

// template<class T>
//   using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
TEST_CASE("add_rvalue_reference_t", "[meta.trans.ref]") {
  CHECK(std::is_same<slb::add_rvalue_reference_t<int>,
                     std::add_rvalue_reference<int>::type>::value);
}

// [meta.trans.sign], sign modifications

// template<class T>
//   using make_signed_t   = typename make_signed<T>::type;
TEST_CASE("make_signed_t", "[meta.trans.sign]") {
  CHECK(std::is_same<slb::make_signed_t<int>,
                     std::make_signed<int>::type>::value);
}

// template<class T>
//   using make_unsigned_t = typename make_unsigned<T>::type;
TEST_CASE("make_unsigned_t", "[meta.trans.sign]") {
  CHECK(std::is_same<slb::make_unsigned_t<int>,
                     std::make_unsigned<int>::type>::value);
}

// [meta.trans.arr], array modifications

// template<class T>
//   using remove_extent_t      = typename remove_extent<T>::type;
TEST_CASE("remove_extent_t", "[meta.trans.arr]") {
  CHECK(std::is_same<slb::remove_extent_t<int[3]>,
                     std::remove_extent<int[3]>::type>::value);
}

// template<class T>
//   using remove_all_extents_t = typename remove_all_extents<T>::type;
TEST_CASE("remove_all_extents_t", "[meta.trans.arr]") {
  CHECK(std::is_same<slb::remove_all_extents_t<int[3][2][1]>,
                     std::remove_all_extents<int[3][2][1]>::type>::value);
}

// [meta.trans.ptr], pointer modifications

// template<class T>
//   using remove_pointer_t = typename remove_pointer<T>::type;
TEST_CASE("remove_pointer_t", "[meta.trans.ptr]") {
  CHECK(std::is_same<slb::remove_pointer_t<int*>,
                     std::remove_pointer<int*>::type>::value);
}

// template<class T>
//   using add_pointer_t    = typename add_pointer<T>::type;
TEST_CASE("add_pointer_t", "[meta.trans.ptr]") {
  CHECK(std::is_same<slb::add_pointer_t<int>,
                     std::add_pointer<int>::type>::value);
}

// [meta.trans.other], other transformations

// template<size_t Len, class... Types> struct aligned_union;
TEST_CASE("aligned_union", "[meta.trans.other]") {
  union U {
    int x;
    double y;
  };

  // The member typedef `type` shall be a trivial standard-layout type suitable
  // for use as uninitialized storage for any object whose type is listed in
  // `Types`; its size shall be at least `Len`.
  {
    using type = slb::aligned_union<0, int, double>::type;
    CHECK(std::is_trivial<type>::value);
    CHECK(std::is_standard_layout<type>::value);
    CHECK(alignof(type) % alignof(U) == 0);

    CHECK(sizeof(slb::aligned_union<sizeof(int) * 2, int>::type) >=
          sizeof(int) * 2);
  }

  // The static member `alignment_value` shall be an integral constant of type
  // `size_t` whose value is the strictest alignment of all types listed in
  // `Types`.
  {
    using au0 = slb::aligned_union<0, int, double>;
    CHECK(
        std::is_same<decltype(au0::alignment_value), std::size_t const>::value);
    CHECK(au0::alignment_value == alignof(U));
    constexpr std::size_t au0_alignment_value = au0::alignment_value;
    (void)au0_alignment_value;
  }
}

// template<class T> struct remove_cvref;
TEST_CASE("remove_cvref", "[meta.trans.other]") {
  CHECK(std::is_same<slb::remove_cvref<int>::type, int>::value);
  CHECK(std::is_same<slb::remove_cvref<int const>::type, int>::value);
  CHECK(std::is_same<slb::remove_cvref<int volatile>::type, int>::value);
  CHECK(std::is_same<slb::remove_cvref<int const volatile>::type, int>::value);

  CHECK(std::is_same<slb::remove_cvref<int&>::type, int>::value);
  CHECK(std::is_same<slb::remove_cvref<int const&>::type, int>::value);
  CHECK(std::is_same<slb::remove_cvref<int volatile&>::type, int>::value);
  CHECK(std::is_same<slb::remove_cvref<int const volatile&>::type, int>::value);

  CHECK(std::is_same<slb::remove_cvref<int&&>::type, int>::value);
  CHECK(std::is_same<slb::remove_cvref<int const&&>::type, int>::value);
  CHECK(std::is_same<slb::remove_cvref<int volatile&&>::type, int>::value);
  CHECK(
      std::is_same<slb::remove_cvref<int const volatile&&>::type, int>::value);
}

// template<size_t Len,
//          size_t Align = default-alignment> // see [meta.trans.other]
//   using aligned_storage_t = typename aligned_storage<Len, Align>::type;
TEST_CASE("aligned_storage_t", "[meta.trans.other]") {
  CHECK(std::is_same<slb::aligned_storage_t<sizeof(int)>,
                     std::aligned_storage<sizeof(int)>::type>::value);

  CHECK(std::is_same<
        slb::aligned_storage_t<sizeof(int), alignof(int)>,
        std::aligned_storage<sizeof(int), alignof(int)>::type>::value);
}

// template<size_t Len, class... Types>
//   using aligned_union_t   = typename aligned_union<Len, Types...>::type;
TEST_CASE("aligned_union_t", "[meta.trans.other]") {
  CHECK(std::is_same<slb::aligned_union_t<1, int, float, double>,
                     slb::aligned_union<1, int, float, double>::type>::value);
}

// template<class T>
//   using remove_cvref_t    = typename remove_cvref<T>::type;
TEST_CASE("remove_cvref_t", "[meta.trans.other]") {
  CHECK(std::is_same<slb::remove_cvref_t<int const volatile&>,
                     slb::remove_cvref<int const volatile&>::type>::value);
}

// template<class T>
//   using decay_t           = typename decay<T>::type;
TEST_CASE("decay_t", "[meta.trans.other]") {
  CHECK(std::is_same<slb::decay_t<int const&>,
                     std::decay<int const&>::type>::value);
}

// template<bool b, class T = void>
//   using enable_if_t       = typename enable_if<b, T>::type;
TEST_CASE("enable_if_t", "[meta.trans.other]") {
  CHECK(std::is_same<slb::enable_if_t<true, int>,
                     std::enable_if<true, int>::type>::value);
}

// template<bool b, class T, class F>
//   using conditional_t     = typename conditional<b, T, F>::type;
TEST_CASE("conditional_t", "[meta.trans.other]") {
  CHECK(std::is_same<slb::conditional_t<true, int, float>,
                     std::conditional<true, int, float>::type>::value);
}

// template<class... T>
//   using common_type_t     = typename common_type<T...>::type;
TEST_CASE("common_type_t", "[meta.trans.other]") {
  CHECK(std::is_same<slb::common_type_t<int, float, double>,
                     std::common_type<int, float, double>::type>::value);
}

// template<class T>
//   using underlying_type_t = typename underlying_type<T>::type;
TEST_CASE("underlying_type_t", "[meta.trans.other]") {
  enum E : int {};
  CHECK(std::is_same<slb::underlying_type_t<E>,
                     std::underlying_type<E>::type>::value);
}

// template <class Fn, class... ArgTypes> struct invoke_result;
template <typename T>
struct no_result_void {
  using type = void;
};

template <typename T, typename Enable = void>
struct no_result : std::true_type {};

template <typename T>
struct no_result<T, typename no_result_void<typename T::type>::type>
    : std::false_type {};

TEST_CASE("invoke_result", "[meta.trans.other]") {
  struct C {};
  struct D : C {};

  /* mem-fun-ptr */ {
    using Fn = int (C::*)(int);

    CHECK(std::is_same<slb::invoke_result<Fn, C&, int>::type, int>::value);
    CHECK(no_result<slb::invoke_result<Fn, C const&, int>>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, C&&, int>::type, int>::value);
    CHECK(no_result<slb::invoke_result<Fn, C const&&, int>>::value);

    CHECK(std::is_same<slb::invoke_result<Fn, D&, int>::type, int>::value);
    CHECK(no_result<slb::invoke_result<Fn, D const&, int>>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, D&&, int>::type, int>::value);
    CHECK(no_result<slb::invoke_result<Fn, D const&&, int>>::value);

    CHECK(std::is_same<
          slb::invoke_result<Fn, std::reference_wrapper<C>, int>::type,
          int>::value);
    CHECK(no_result<
          slb::invoke_result<Fn, std::reference_wrapper<C const>, int>>::value);

    CHECK(std::is_same<slb::invoke_result<Fn, C*, int>::type, int>::value);
    CHECK(no_result<slb::invoke_result<Fn, C const*, int>>::value);

    CHECK(std::is_same<slb::invoke_result<Fn, smart_ptr<C>, int>::type,
                       int>::value);
    CHECK(no_result<slb::invoke_result<Fn, smart_ptr<C const>, int>>::value);

    CHECK(no_result<slb::invoke_result<Fn>>::value);
    CHECK(no_result<slb::invoke_result<Fn, int&>>::value);
    CHECK(no_result<slb::invoke_result<Fn, C&, C>>::value);

    using Fnc = int (C::*)(int) const;

    CHECK(std::is_same<slb::invoke_result<Fnc, C&, int>::type, int>::value);
    CHECK(
        std::is_same<slb::invoke_result<Fnc, C const&, int>::type, int>::value);
    CHECK(std::is_same<slb::invoke_result<Fnc, C&&, int>::type, int>::value);
    CHECK(std::is_same<slb::invoke_result<Fnc, C const&&, int>::type,
                       int>::value);

    using Fnl = int (C::*)(int)&;

    CHECK(std::is_same<slb::invoke_result<Fnl, C&, int>::type, int>::value);
    CHECK(no_result<slb::invoke_result<Fnl, C const&, int>>::value);
    CHECK(no_result<slb::invoke_result<Fnl, C&&, int>>::value);
    CHECK(no_result<slb::invoke_result<Fnl, C const&&, int>>::value);

    using Fnr = int (C::*)(int)&&;

    CHECK(no_result<slb::invoke_result<Fnr, C&, int>>::value);
    CHECK(no_result<slb::invoke_result<Fnr, C const&, int>>::value);
    CHECK(std::is_same<slb::invoke_result<Fnr, C&&, int>::type, int>::value);
    CHECK(no_result<slb::invoke_result<Fnr, C const&&, int>>::value);

    using Fncl = int (C::*)(int) const&;

    CHECK(std::is_same<slb::invoke_result<Fncl, C&, int>::type, int>::value);
    CHECK(std::is_same<slb::invoke_result<Fncl, C const&, int>::type,
                       int>::value);
    CHECK(std::is_same<slb::invoke_result<Fncl, C&&, int>::type, int>::value);
    CHECK(std::is_same<slb::invoke_result<Fncl, C const&&, int>::type,
                       int>::value);

    using Fncr = int (C::*)(int) const&&;

    CHECK(no_result<slb::invoke_result<Fncr, C&, int>>::value);
    CHECK(no_result<slb::invoke_result<Fncr, C const&, int>>::value);
    CHECK(std::is_same<slb::invoke_result<Fncr, C&&, int>::type, int>::value);
    CHECK(std::is_same<slb::invoke_result<Fncr, C const&&, int>::type,
                       int>::value);
  }

  /* mem-obj-ptr */ {
    using Fn = int C::*;

    CHECK(std::is_same<slb::invoke_result<Fn, C&>::type, int&>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, C const&>::type,
                       int const&>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, C&&>::type, int&&>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, C const&&>::type,
                       int const&&>::value);

    CHECK(std::is_same<slb::invoke_result<Fn, D&>::type, int&>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, D const&>::type,
                       int const&>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, D&&>::type, int&&>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, D const&&>::type,
                       int const&&>::value);

    CHECK(std::is_same<slb::invoke_result<Fn, std::reference_wrapper<C>>::type,
                       int&>::value);
    CHECK(std::is_same<
          slb::invoke_result<Fn, std::reference_wrapper<C const>>::type,
          int const&>::value);

    CHECK(std::is_same<slb::invoke_result<Fn, C*>::type, int&>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, C const*>::type,
                       int const&>::value);

    CHECK(
        std::is_same<slb::invoke_result<Fn, smart_ptr<C>>::type, int&>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, smart_ptr<C const>>::type,
                       int const&>::value);

    CHECK(no_result<slb::invoke_result<Fn>>::value);
    CHECK(no_result<slb::invoke_result<Fn, int&>>::value);
    CHECK(no_result<slb::invoke_result<Fn, C&, int>>::value);
  }

  /* fun-obj */ {
    struct Fn {
      int operator()(int) noexcept { return 42; }
    };

    CHECK(std::is_same<slb::invoke_result<Fn, int>::type, int>::value);
    CHECK(std::is_same<slb::invoke_result<Fn, conv_to<int>>::type, int>::value);

    CHECK(no_result<slb::invoke_result<Fn>>::value);
    CHECK(no_result<slb::invoke_result<Fn, void*>>::value);
    CHECK(no_result<slb::invoke_result<Fn, int, int>>::value);

    using Fn_ptr = int (*)(int);

    CHECK(std::is_same<slb::invoke_result<Fn_ptr, int>::type, int>::value);
  }
}

// template <class Fn, class... ArgTypes>
// using invoke_result_t = typename invoke_result<Fn, ArgTypes...>::type;
TEST_CASE("invoke_result_t", "[meta.trans.other]") {
  CHECK(std::is_same<
        slb::invoke_result_t<int (*)(int), int>,
        typename slb::invoke_result<int (*)(int), int>::type>::value);
}

// template<class...>
//   using void_t            = void;

template <typename T, typename Enable = void>
struct has_nested_type : std::false_type {};

template <typename T>
struct has_nested_type<T, slb::void_t<typename T::type>> : std::true_type {};

TEST_CASE("void_t", "[meta.trans.other]") {
  CHECK(std::is_same<slb::void_t<int, float, double>, void>::value);

  /* sfinae */ {
    struct S {
      using type = int;
    };
    CHECK(has_nested_type<S>::value);
    CHECK_FALSE(has_nested_type<int>::value);
  }
}

// [meta.logical], logical operator traits

template <bool B>
struct Weird {
  struct Value {
    operator int() const noexcept { return !B; }
    constexpr explicit operator bool() const noexcept { return B; }
  };

  static constexpr Value value{};
};

template <bool B>
constexpr typename Weird<B>::Value Weird<B>::value;

// template<class... B> struct conjunction;
TEST_CASE("conjunction", "[meta.logical]") {
  /* length: 0 */ {
    CHECK(std::is_base_of<slb::true_type, slb::conjunction<>>::value);
  }

  /* length: 1 */ {
    CHECK(std::is_base_of<slb::true_type,
                          slb::conjunction<slb::true_type>>::value);

    CHECK(std::is_base_of<slb::false_type,
                          slb::conjunction<slb::false_type>>::value);

    CHECK(std::is_base_of<Weird<true>, slb::conjunction<Weird<true>>>::value);
    CHECK(std::is_base_of<Weird<false>, slb::conjunction<Weird<false>>>::value);
  }

  /* length: 2 */ {
    CHECK(std::is_base_of<
          slb::true_type,
          slb::conjunction<slb::true_type, slb::true_type>>::value);

    CHECK(std::is_base_of<
          slb::false_type,
          slb::conjunction<slb::true_type, slb::false_type>>::value);

    CHECK(std::is_base_of<
          slb::false_type,
          slb::conjunction<slb::false_type, slb::true_type>>::value);

    CHECK(std::is_base_of<
          slb::false_type,
          slb::conjunction<slb::false_type, slb::false_type>>::value);

    CHECK(std::is_base_of<Weird<true>,
                          slb::conjunction<Weird<true>, Weird<true>>>::value);

    CHECK(std::is_base_of<Weird<false>,
                          slb::conjunction<Weird<true>, Weird<false>>>::value);

    CHECK(std::is_base_of<Weird<false>,
                          slb::conjunction<Weird<false>, Weird<true>>>::value);

    CHECK(std::is_base_of<Weird<false>,
                          slb::conjunction<Weird<false>, Weird<false>>>::value);
  }

  /* short-circuiting */ {
    CHECK(std::is_base_of<slb::false_type,
                          slb::conjunction<slb::false_type, void>>::value);

    CHECK(std::is_base_of<
          Weird<false>,
          slb::conjunction<Weird<true>, Weird<false>, void>>::value);
  }
}

// template<class... B> struct disjunction;
TEST_CASE("disjunction", "[meta.logical]") {
  /* length: 0 */ {
    CHECK(std::is_base_of<slb::false_type, slb::disjunction<>>::value);
  }

  /* length: 1 */ {
    CHECK(std::is_base_of<std::true_type,
                          slb::disjunction<slb::true_type>>::value);

    CHECK(std::is_base_of<std::false_type,
                          slb::disjunction<slb::false_type>>::value);

    CHECK(std::is_base_of<Weird<true>, slb::disjunction<Weird<true>>>::value);
    CHECK(std::is_base_of<Weird<false>, slb::disjunction<Weird<false>>>::value);
  }

  /* length: 2 */ {
    CHECK(std::is_base_of<
          slb::true_type,
          slb::disjunction<slb::true_type, slb::true_type>>::value);

    CHECK(std::is_base_of<
          slb::true_type,
          slb::disjunction<slb::true_type, slb::false_type>>::value);

    CHECK(std::is_base_of<
          slb::true_type,
          slb::disjunction<slb::false_type, slb::true_type>>::value);

    CHECK(std::is_base_of<
          slb::false_type,
          slb::disjunction<slb::false_type, slb::false_type>>::value);

    CHECK(std::is_base_of<Weird<true>,
                          slb::disjunction<Weird<true>, Weird<true>>>::value);

    CHECK(std::is_base_of<Weird<true>,
                          slb::disjunction<Weird<true>, Weird<false>>>::value);

    CHECK(std::is_base_of<Weird<true>,
                          slb::disjunction<Weird<false>, Weird<true>>>::value);

    CHECK(std::is_base_of<Weird<false>,
                          slb::disjunction<Weird<false>, Weird<false>>>::value);
  }

  /* short-circuiting */ {
    CHECK(std::is_base_of<slb::true_type,
                          slb::disjunction<slb::true_type, void>>::value);

    CHECK(std::is_base_of<
          Weird<true>,
          slb::disjunction<Weird<false>, Weird<true>, void>>::value);
  }
}

// template<class B> struct negation;
TEST_CASE("negation", "[meta.logical]") {
  CHECK(std::is_base_of<slb::false_type, slb::negation<slb::true_type>>::value);
  CHECK(std::is_base_of<slb::true_type, slb::negation<slb::false_type>>::value);

  CHECK(std::is_base_of<slb::true_type,
                        slb::negation<slb::negation<Weird<true>>>>::value);
  CHECK(std::is_base_of<slb::false_type,
                        slb::negation<slb::negation<Weird<false>>>>::value);
}

// [meta.endian], endian

// enum class endian {
//   little = see below,
//   big    = see below,
//   native = see below
// };
TEST_CASE("endian", "[meta.endian]") {
  CHECK(slb::endian::little != slb::endian::big);

#if defined(_WIN32) || (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  CHECK(slb::endian::native == slb::endian::little);
#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
  CHECK(slb::endian::native == slb::endian::big);
#else
  CHECK(slb::endian::native != slb::endian::little);
  CHECK(slb::endian::native != slb::endian::big);
#endif
}

#if SLB_HAS_CXX14_VARIABLE_TEMPLATES

// [meta.unary.cat], primary type categories

// template<class T>
//   inline constexpr bool is_void_v = is_void<T>::value;
TEST_CASE("is_void_v", "[meta.unary.cat]") {
  CHECK(std::is_same<decltype(slb::is_void_v<void>), bool const>::value);
  CHECK(slb::is_void_v<void> == slb::is_void<void>::value);
  constexpr bool v = slb::is_void_v<void>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
TEST_CASE("is_null_pointer_v", "[meta.unary.cat]") {
  using nullptr_t = decltype(nullptr);
  CHECK(std::is_same<decltype(slb::is_null_pointer_v<nullptr_t>),
                     bool const>::value);
  CHECK(slb::is_null_pointer_v<nullptr_t> ==
        slb::is_null_pointer<nullptr_t>::value);
  constexpr bool v = slb::is_null_pointer_v<nullptr_t>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_integral_v = is_integral<T>::value;
TEST_CASE("is_integral_v", "[meta.unary.cat]") {
  CHECK(std::is_same<decltype(slb::is_integral_v<int>), bool const>::value);
  CHECK(slb::is_integral_v<int> == slb::is_integral<int>::value);
  constexpr bool v = slb::is_integral_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
TEST_CASE("is_floating_point_v", "[meta.unary.cat]") {
  CHECK(std::is_same<decltype(slb::is_floating_point_v<double>),
                     bool const>::value);
  CHECK(slb::is_floating_point_v<double> ==
        slb::is_floating_point<double>::value);
  constexpr bool v = slb::is_floating_point_v<double>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_array_v = is_array<T>::value;
TEST_CASE("is_array_v", "[meta.unary.cat]") {
  CHECK(std::is_same<decltype(slb::is_array_v<int[3]>), bool const>::value);
  CHECK(slb::is_array_v<int[3]> == slb::is_array<int[3]>::value);
  constexpr bool v = slb::is_array_v<int[3]>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_pointer_v = is_pointer<T>::value;
TEST_CASE("is_pointer_v", "[meta.unary.cat]") {
  CHECK(std::is_same<decltype(slb::is_pointer_v<int*>), bool const>::value);
  CHECK(slb::is_pointer_v<int*> == slb::is_pointer<int*>::value);
  constexpr bool v = slb::is_pointer_v<int*>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_lvalue_reference_v
//     = is_lvalue_reference<T>::value;
TEST_CASE("is_lvalue_reference_v", "[meta.unary.cat]") {
  CHECK(std::is_same<decltype(slb::is_lvalue_reference_v<int&>),
                     bool const>::value);
  CHECK(slb::is_lvalue_reference_v<int&> ==
        slb::is_lvalue_reference<int&>::value);
  constexpr bool v = slb::is_lvalue_reference_v<int&>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_rvalue_reference_v =
//   is_rvalue_reference<T>::value;
TEST_CASE("is_rvalue_reference_v", "[meta.unary.cat]") {
  CHECK(std::is_same<decltype(slb::is_rvalue_reference_v<int&&>),
                     bool const>::value);
  CHECK(slb::is_rvalue_reference_v<int&&> ==
        slb::is_rvalue_reference<int&&>::value);
  constexpr bool v = slb::is_rvalue_reference_v<int&&>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_member_object_pointer_v
//     = is_member_object_pointer<T>::value;
TEST_CASE("is_member_object_pointer_v", "[meta.unary.cat]") {
  class C {};
  CHECK(std::is_same<decltype(slb::is_member_object_pointer_v<int C::*>),
                     bool const>::value);
  CHECK(slb::is_member_object_pointer_v<int C::*> ==
        slb::is_member_object_pointer<int C::*>::value);
  constexpr bool v = slb::is_member_object_pointer_v<int C::*>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_member_function_pointer_v
//     = is_member_function_pointer<T>::value;
TEST_CASE("is_member_function_pointer_v", "[meta.unary.cat]") {
  class C {};
  CHECK(std::is_same<decltype(slb::is_member_function_pointer_v<int (C::*)()>),
                     bool const>::value);
  CHECK(slb::is_member_function_pointer_v<int (C::*)()> ==
        slb::is_member_function_pointer<int (C::*)()>::value);
  constexpr bool v = slb::is_member_function_pointer_v<int (C::*)()>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_enum_v = is_enum<T>::value;
TEST_CASE("is_enum_v", "[meta.unary.cat]") {
  enum E {};
  CHECK(std::is_same<decltype(slb::is_enum_v<E>), bool const>::value);
  CHECK(slb::is_enum_v<E> == slb::is_enum<E>::value);
  constexpr bool v = slb::is_enum_v<E>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_union_v = is_union<T>::value;
TEST_CASE("is_union_v", "[meta.unary.cat]") {
  union U {};
  CHECK(std::is_same<decltype(slb::is_union_v<U>), bool const>::value);
  CHECK(slb::is_union_v<U> == slb::is_union<U>::value);
  constexpr bool v = slb::is_union_v<U>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_class_v = is_class<T>::value;
TEST_CASE("is_class_v", "[meta.unary.cat]") {
  class C {};
  CHECK(std::is_same<decltype(slb::is_class_v<C>), bool const>::value);
  CHECK(slb::is_class_v<C> == slb::is_class<C>::value);
  constexpr bool v = slb::is_class_v<C>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_function_v = is_function<T>::value;
TEST_CASE("is_function_v", "[meta.unary.cat]") {
  {
    CHECK(
        std::is_same<decltype(slb::is_function_v<void()>), bool const>::value);
    CHECK(slb::is_function_v<void()> == slb::is_function<void()>::value);
    constexpr bool v = slb::is_function_v<void()>;
    (void)v;
  }

  {
    CHECK(std::is_same<decltype(slb::is_function_v<void() noexcept>),
                       bool const>::value);
    CHECK(slb::is_function_v<void() noexcept> ==
          slb::is_function<void() noexcept>::value);
    constexpr bool v = slb::is_function_v<void() noexcept>;
    (void)v;
  }
}

// [meta.unary.comp], composite type categories

// template<class T>
//   inline constexpr bool is_reference_v = is_reference<T>::value;
TEST_CASE("is_reference_v", "[meta.unary.comp]") {
  CHECK(std::is_same<decltype(slb::is_reference_v<int&>), bool const>::value);
  CHECK(slb::is_reference_v<int&> == slb::is_reference<int&>::value);
  constexpr bool v = slb::is_reference_v<int&>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
TEST_CASE("is_arithmetic_v", "[meta.unary.comp]") {
  CHECK(std::is_same<decltype(slb::is_arithmetic_v<int>), bool const>::value);
  CHECK(slb::is_arithmetic_v<int> == slb::is_arithmetic<int>::value);
  constexpr bool v = slb::is_arithmetic_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_fundamental_v = is_fundamental<T>::value;
TEST_CASE("is_fundamental_v", "[meta.unary.comp]") {
  CHECK(std::is_same<decltype(slb::is_fundamental_v<int>), bool const>::value);
  CHECK(slb::is_fundamental_v<int> == slb::is_fundamental<int>::value);
  constexpr bool v = slb::is_fundamental_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_object_v = is_object<T>::value;
TEST_CASE("is_object_v", "[meta.unary.comp]") {
  CHECK(std::is_same<decltype(slb::is_object_v<int>), bool const>::value);
  CHECK(slb::is_object_v<int> == slb::is_object<int>::value);
  constexpr bool v = slb::is_object_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_scalar_v = is_scalar<T>::value;
TEST_CASE("is_scalar_v", "[meta.unary.comp]") {
  CHECK(std::is_same<decltype(slb::is_scalar_v<int>), bool const>::value);
  CHECK(slb::is_scalar_v<int> == slb::is_scalar<int>::value);
  constexpr bool v = slb::is_scalar_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_compound_v = is_compound<T>::value;
TEST_CASE("is_compound_v", "[meta.unary.comp]") {
  CHECK(std::is_same<decltype(slb::is_compound_v<int[3]>), bool const>::value);
  CHECK(slb::is_compound_v<int[3]> == slb::is_compound<int[3]>::value);
  constexpr bool v = slb::is_compound_v<int[3]>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;
TEST_CASE("is_member_pointer_v", "[meta.unary.comp]") {
  class C {};
  CHECK(std::is_same<decltype(slb::is_member_pointer_v<int C::*>),
                     bool const>::value);
  CHECK(slb::is_member_pointer_v<int C::*> ==
        slb::is_member_pointer<int C::*>::value);
  constexpr bool v = slb::is_member_pointer_v<int C::*>;
  (void)v;
}

// [meta.unary.prop], type properties

// template<class T>
//   inline constexpr bool is_const_v = is_const<T>::value;
TEST_CASE("is_const_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_const_v<int const>), bool const>::value);
  CHECK(slb::is_const_v<int const> == slb::is_const<int const>::value);
  constexpr bool v = slb::is_const_v<int const>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_volatile_v = is_volatile<T>::value;
TEST_CASE("is_volatile_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_volatile_v<int volatile>),
                     bool const>::value);
  CHECK(slb::is_volatile_v<int volatile> ==
        slb::is_volatile<int volatile>::value);
  constexpr bool v = slb::is_volatile_v<int volatile>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_trivial_v = is_trivial<T>::value;
TEST_CASE("is_trivial_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivial_v<int>), bool const>::value);
  CHECK(slb::is_trivial_v<int> == slb::is_trivial<int>::value);
  constexpr bool v = slb::is_trivial_v<int>;
  (void)v;
}

#if SLB_TRIVIALITY_TRAITS
// template<class T>
//   inline constexpr bool is_trivially_copyable_v
//     = is_trivially_copyable<T>::value;
TEST_CASE("is_trivially_copyable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_copyable_v<int>),
                     bool const>::value);
  CHECK(slb::is_trivially_copyable_v<int> ==
        slb::is_trivially_copyable<int>::value);
  constexpr bool v = slb::is_trivially_copyable_v<int>;
  (void)v;
}
#endif

// template<class T>
//   inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;
TEST_CASE("is_standard_layout_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_standard_layout_v<int>),
                     bool const>::value);
  CHECK(slb::is_standard_layout_v<int> == slb::is_standard_layout<int>::value);
  constexpr bool v = slb::is_standard_layout_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_empty_v = is_empty<T>::value;
TEST_CASE("is_empty_v", "[meta.unary.prop]") {
  struct Empty {};
  CHECK(std::is_same<decltype(slb::is_empty_v<Empty>), bool const>::value);
  CHECK(slb::is_empty_v<Empty> == slb::is_empty<Empty>::value);
  constexpr bool v = slb::is_empty_v<Empty>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;
TEST_CASE("is_polymorphic_v", "[meta.unary.prop]") {
  class Polymorphic {
    virtual void fun() {}
  };
  CHECK(std::is_same<decltype(slb::is_polymorphic_v<Polymorphic>),
                     bool const>::value);
  CHECK(slb::is_polymorphic_v<Polymorphic> ==
        slb::is_polymorphic<Polymorphic>::value);
  constexpr bool v = slb::is_polymorphic_v<Polymorphic>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_abstract_v = is_abstract<T>::value;
TEST_CASE("is_abstract_v", "[meta.unary.prop]") {
  class Abstract {
    virtual void fun() = 0;
  };
  CHECK(
      std::is_same<decltype(slb::is_abstract_v<Abstract>), bool const>::value);
  CHECK(slb::is_abstract_v<Abstract> == slb::is_abstract<Abstract>::value);
  constexpr bool v = slb::is_abstract_v<Abstract>;
  (void)v;
}

#if __cpp_lib_is_final || __has_feature(is_final) || (__GNUC__ > 4) ||         \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
// template<class T>
//   inline constexpr bool is_final_v = is_final<T>::value;
TEST_CASE("is_final_v", "[meta.unary.prop]") {
  class Final final {};
  CHECK(std::is_same<decltype(slb::is_final_v<Final>), bool const>::value);
  CHECK(slb::is_final_v<Final> == slb::is_final<Final>::value);
  constexpr bool v = slb::is_final_v<Final>;
  (void)v;
}
#endif

#if __cpp_lib_is_aggregate || __has_feature(is_aggregate) || (__GNUC__ >= 7)
// template<class T>
//   inline constexpr bool is_aggregate_v = is_aggregate<T>::value;
TEST_CASE("is_aggregate_v", "[meta.unary.prop]") {
  struct Aggregate {
    int obj;
  };
  CHECK(std::is_same<decltype(slb::is_aggregate_v<Aggregate>),
                     bool const>::value);
  CHECK(slb::is_aggregate_v<Aggregate> == slb::is_aggregate<Aggregate>::value);
  constexpr bool v = slb::is_aggregate_v<Aggregate>;
  (void)v;
}
#endif

// template<class T>
//   inline constexpr bool is_signed_v = is_signed<T>::value;
TEST_CASE("is_signed_v", "[meta.unary.prop]") {
  CHECK(
      std::is_same<decltype(slb::is_signed_v<signed int>), bool const>::value);
  CHECK(slb::is_signed_v<signed int> == slb::is_signed<signed int>::value);
  constexpr bool v = slb::is_signed_v<signed int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_unsigned_v = is_unsigned<T>::value;
TEST_CASE("is_unsigned_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_unsigned_v<unsigned int>),
                     bool const>::value);
  CHECK(slb::is_unsigned_v<unsigned int> ==
        slb::is_unsigned<unsigned int>::value);
  constexpr bool v = slb::is_unsigned_v<unsigned int>;
  (void)v;
}

// template<class T, class... Args>
//   inline constexpr bool is_constructible_v
//     = is_constructible<T, Args...>::value;
TEST_CASE("is_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_constructible_v<int, int>),
                     bool const>::value);
  CHECK(slb::is_constructible_v<int, int> ==
        slb::is_constructible<int, int>::value);
  constexpr bool v = slb::is_constructible_v<int, int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_default_constructible_v
//     = is_default_constructible<T>::value;
TEST_CASE("is_default_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_default_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_default_constructible_v<int> ==
        slb::is_default_constructible<int>::value);
  constexpr bool v = slb::is_default_constructible_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_copy_constructible_v
//     = is_copy_constructible<T>::value;
TEST_CASE("is_copy_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_copy_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_copy_constructible_v<int> ==
        slb::is_copy_constructible<int>::value);
  constexpr bool v = slb::is_copy_constructible_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_move_constructible_v
//     = is_move_constructible<T>::value;
TEST_CASE("is_move_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_move_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_move_constructible_v<int> ==
        slb::is_move_constructible<int>::value);
  constexpr bool v = slb::is_move_constructible_v<int>;
  (void)v;
}

// template<class T, class U>
//   inline constexpr bool is_assignable_v = is_assignable<T, U>::value;
TEST_CASE("is_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_assignable_v<int&, int>),
                     bool const>::value);
  CHECK(slb::is_assignable_v<int&, int> ==
        slb::is_assignable<int&, int>::value);
  constexpr bool v = slb::is_assignable_v<int&, int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;
TEST_CASE("is_copy_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_copy_assignable_v<int>),
                     bool const>::value);
  CHECK(slb::is_copy_assignable_v<int> == slb::is_copy_assignable<int>::value);
  constexpr bool v = slb::is_copy_assignable_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;
TEST_CASE("is_move_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_move_assignable_v<int>),
                     bool const>::value);
  CHECK(slb::is_move_assignable_v<int> == slb::is_move_assignable<int>::value);
  constexpr bool v = slb::is_move_assignable_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_destructible_v = is_destructible<T>::value;
TEST_CASE("is_destructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_destructible_v<int>), bool const>::value);
  CHECK(slb::is_destructible_v<int> == slb::is_destructible<int>::value);
  constexpr bool v = slb::is_destructible_v<int>;
  (void)v;
}

#if SLB_TRIVIALITY_TRAITS

// template<class T, class... Args>
//   inline constexpr bool is_trivially_constructible_v
//     = is_trivially_constructible<T, Args...>::value;
TEST_CASE("is_trivially_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_trivially_constructible_v<int> ==
        slb::is_trivially_constructible<int>::value);
  constexpr bool v = slb::is_trivially_constructible_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_trivially_default_constructible_v
//     = is_trivially_default_constructible<T>::value;
TEST_CASE("is_trivially_default_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_default_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_trivially_default_constructible_v<int> ==
        slb::is_trivially_default_constructible<int>::value);
  constexpr bool v = slb::is_trivially_default_constructible_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_trivially_copy_constructible_v
//     = is_trivially_copy_constructible<T>::value;
TEST_CASE("is_trivially_copy_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_copy_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_trivially_copy_constructible_v<int> ==
        slb::is_trivially_copy_constructible<int>::value);
  constexpr bool v = slb::is_trivially_copy_constructible_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_trivially_move_constructible_v
//     = is_trivially_move_constructible<T>::value;
TEST_CASE("is_trivially_move_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_move_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_trivially_move_constructible_v<int> ==
        slb::is_trivially_move_constructible<int>::value);
  constexpr bool v = slb::is_trivially_move_constructible_v<int>;
  (void)v;
}

// template<class T, class U>
//   inline constexpr bool is_trivially_assignable_v
//     = is_trivially_assignable<T, U>::value;
TEST_CASE("is_trivially_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_assignable_v<int&, int>),
                     bool const>::value);
  CHECK(slb::is_trivially_assignable_v<int&, int> ==
        slb::is_trivially_assignable<int&, int>::value);
  constexpr bool v = slb::is_trivially_assignable_v<int&, int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_trivially_copy_assignable_v
//     = is_trivially_copy_assignable<T>::value;
TEST_CASE("is_trivially_copy_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_copy_assignable_v<int>),
                     bool const>::value);
  CHECK(slb::is_trivially_copy_assignable_v<int> ==
        slb::is_trivially_copy_assignable<int>::value);
  constexpr bool v = slb::is_trivially_copy_assignable_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_trivially_move_assignable_v
//     = is_trivially_move_assignable<T>::value;
TEST_CASE("is_trivially_move_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_move_assignable_v<int>),
                     bool const>::value);
  CHECK(slb::is_trivially_move_assignable_v<int> ==
        slb::is_trivially_move_assignable<int>::value);
  constexpr bool v = slb::is_trivially_move_assignable_v<int>;
  (void)v;
}

#endif

// template<class T>
//   inline constexpr bool is_trivially_destructible_v
//     = is_trivially_destructible<T>::value;
TEST_CASE("is_trivially_destructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_trivially_destructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_trivially_destructible_v<int> ==
        slb::is_trivially_destructible<int>::value);
  constexpr bool v = slb::is_trivially_destructible_v<int>;
  (void)v;
}

// template<class T, class... Args>
//   inline constexpr bool is_nothrow_constructible_v
//     = is_nothrow_constructible<T, Args...>::value;
TEST_CASE("is_nothrow_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_nothrow_constructible_v<int, int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_constructible_v<int, int> ==
        slb::is_nothrow_constructible<int, int>::value);
  constexpr bool v = slb::is_nothrow_constructible_v<int, int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_nothrow_default_constructible_v
//     = is_nothrow_default_constructible<T>::value;
TEST_CASE("is_nothrow_default_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_nothrow_default_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_default_constructible_v<int> ==
        slb::is_nothrow_default_constructible<int>::value);
  constexpr bool v = slb::is_nothrow_default_constructible_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_nothrow_copy_constructible_v
//     = is_nothrow_copy_constructible<T>::value;
TEST_CASE("is_nothrow_copy_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_nothrow_copy_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_copy_constructible_v<int> ==
        slb::is_nothrow_copy_constructible<int>::value);
  constexpr bool v = slb::is_nothrow_copy_constructible_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_nothrow_move_constructible_v
//     = is_nothrow_move_constructible<T>::value;
TEST_CASE("is_nothrow_move_constructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_nothrow_move_constructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_move_constructible_v<int> ==
        slb::is_nothrow_move_constructible<int>::value);
  constexpr bool v = slb::is_nothrow_move_constructible_v<int>;
  (void)v;
}

// template<class T, class U>
//   inline constexpr bool is_nothrow_assignable_v
//     = is_nothrow_assignable<T, U>::value;
TEST_CASE("is_nothrow_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_nothrow_assignable_v<int&, int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_assignable_v<int&, int> ==
        slb::is_nothrow_assignable<int&, int>::value);
  constexpr bool v = slb::is_nothrow_assignable_v<int&, int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_nothrow_copy_assignable_v
//     = is_nothrow_copy_assignable<T>::value;
TEST_CASE("is_nothrow_copy_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_nothrow_copy_assignable_v<int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_copy_assignable_v<int> ==
        slb::is_nothrow_copy_assignable<int>::value);
  constexpr bool v = slb::is_nothrow_copy_assignable_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_nothrow_move_assignable_v
//     = is_nothrow_move_assignable<T>::value;
TEST_CASE("is_nothrow_move_assignable_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_nothrow_move_assignable_v<int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_move_assignable_v<int> ==
        slb::is_nothrow_move_assignable<int>::value);
  constexpr bool v = slb::is_nothrow_move_assignable_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool is_nothrow_destructible_v
//     = is_nothrow_destructible<T>::value;
TEST_CASE("is_nothrow_destructible_v", "[meta.unary.prop]") {
  CHECK(std::is_same<decltype(slb::is_nothrow_destructible_v<int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_destructible_v<int> ==
        slb::is_nothrow_destructible<int>::value);
  constexpr bool v = slb::is_nothrow_destructible_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr bool has_virtual_destructor_v
//     = has_virtual_destructor<T>::value;
TEST_CASE("has_virtual_destructor_v", "[meta.unary.prop]") {
  class WithVirtualDestructor {
    virtual ~WithVirtualDestructor() {}
  };
  CHECK(std::is_same<decltype(
                         slb::has_virtual_destructor_v<WithVirtualDestructor>),
                     bool const>::value);
  CHECK(slb::has_virtual_destructor_v<WithVirtualDestructor> ==
        slb::has_virtual_destructor<WithVirtualDestructor>::value);
  constexpr bool v = slb::has_virtual_destructor_v<WithVirtualDestructor>;
  (void)v;
}

// template<class T>
//   inline constexpr bool has_unique_object_representations_v
//     = has_unique_object_representations<T>::value;
#if __cpp_lib_has_unique_object_representations ||                             \
    __has_feature(has_unique_object_representations) || (__GNUC__ >= 7)
// template<class T> struct has_unique_object_representations;
TEST_CASE("has_unique_object_representations_v", "[meta.unary.prop]") {
  class WithUniqueObjectRepresentations {
    int obj;
  };
  CHECK(std::is_same<decltype(slb::has_unique_object_representations_v<
                              WithUniqueObjectRepresentations>),
                     bool const>::value);
  CHECK(slb::has_unique_object_representations_v<
            WithUniqueObjectRepresentations> ==
        slb::has_unique_object_representations<
            WithUniqueObjectRepresentations>::value);
  constexpr bool v =
      slb::has_unique_object_representations_v<WithUniqueObjectRepresentations>;
  (void)v;
}
#endif

// [meta.unary.prop.query], type property queries

// template<class T>
//   inline constexpr size_t alignment_of_v = alignment_of<T>::value;
TEST_CASE("alignment_of_v", "[meta.unary.prop.query]") {
  CHECK(std::is_same<decltype(slb::alignment_of_v<int>),
                     std::size_t const>::value);
  CHECK(slb::alignment_of_v<int> == slb::alignment_of<int>::value);
  constexpr std::size_t v = slb::alignment_of_v<int>;
  (void)v;
}

// template<class T>
//   inline constexpr size_t rank_v = rank<T>::value;
TEST_CASE("rank_v", "[meta.unary.prop.query]") {
  CHECK(std::is_same<decltype(slb::rank_v<int[2][2][2]>),
                     std::size_t const>::value);
  CHECK(slb::rank_v<int[2][2][2]> == slb::rank<int[2][2][2]>::value);
  constexpr std::size_t v = slb::rank_v<int[2][2][2]>;
  (void)v;
}

// template<class T, unsigned I = 0>
//   inline constexpr size_t extent_v = extent<T, I>::value;
TEST_CASE("extent_v", "[meta.unary.prop.query]") {
  CHECK(std::is_same<decltype(slb::extent_v<int[2][3]>),
                     std::size_t const>::value);
  CHECK(slb::extent_v<int[2][3]> == slb::extent<int[2][3]>::value);
  constexpr std::size_t v = slb::extent_v<int[2][3]>;
  (void)v;
}

// [meta.rel], type relations

// template<class T, class U>
//   inline constexpr bool is_same_v = is_same<T, U>::value;
TEST_CASE("is_same_v", "[meta.rel]") {
  CHECK(std::is_same<decltype(slb::is_same_v<int, int>), bool const>::value);
  CHECK(slb::is_same_v<int, int> == slb::is_same<int, int>::value);
  constexpr bool v = slb::is_same_v<int, int>;
  (void)v;
}

// template<class Base, class Derived>
//   inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
TEST_CASE("is_base_of_v", "[meta.rel]") {
  class Base {};
  class Derived : public Base {};
  CHECK(std::is_same<decltype(slb::is_base_of_v<Base, Derived>),
                     bool const>::value);
  CHECK(slb::is_base_of_v<Base, Derived> ==
        slb::is_base_of<Base, Derived>::value);
  constexpr bool v = slb::is_base_of_v<Base, Derived>;
  (void)v;
}

// template<class From, class To>
//   inline constexpr bool is_convertible_v = is_convertible<From, To>::value;
TEST_CASE("is_convertible_v", "[meta.rel]") {
  CHECK(std::is_same<decltype(slb::is_convertible_v<float, int>),
                     bool const>::value);
  CHECK(slb::is_convertible_v<float, int> ==
        slb::is_convertible<float, int>::value);
  constexpr bool v = slb::is_convertible_v<float, int>;
  (void)v;
}

// template<class Fn, class... ArgTypes>
//   inline constexpr bool is_invocable_v
//     = is_invocable<Fn, ArgTypes...>::value;
TEST_CASE("is_invocable_v", "[meta.rel]") {
  struct F {
    int operator()(int) const noexcept { return 0; }
  };
  CHECK(std::is_same<decltype(slb::is_invocable_v<F, int>), bool const>::value);
  CHECK(slb::is_invocable_v<F, int> == slb::is_invocable<F, int>::value);
  constexpr bool v = slb::is_invocable_v<F, int>;
  (void)v;
}

// template<class R, class Fn, class... ArgTypes>
//   inline constexpr bool is_invocable_r_v
//     = is_invocable_r<R, Fn, ArgTypes...>::value;
TEST_CASE("is_invocable_r_v", "[meta.rel]") {
  struct F {
    int operator()(int) const noexcept { return 0; }
  };

  {
    CHECK(std::is_same<decltype(slb::is_invocable_r_v<int, F, int>),
                       bool const>::value);
    CHECK(slb::is_invocable_r_v<int, F, int> ==
          slb::is_invocable_r<int, F, int>::value);
    constexpr bool v = slb::is_invocable_r_v<int, F, int>;
    (void)v;
  }

  {
    CHECK(std::is_same<decltype(slb::is_invocable_r_v<void, F, int>),
                       bool const>::value);
    CHECK(slb::is_invocable_r_v<void, F, int> ==
          slb::is_invocable_r<void, F, int>::value);
    constexpr bool v = slb::is_invocable_r_v<void, F, int>;
    (void)v;
  }
}

// template<class Fn, class... ArgTypes>
//   inline constexpr bool is_nothrow_invocable_v
//     = is_nothrow_invocable<Fn, ArgTypes...>::value;
TEST_CASE("is_nothrow_invocable_v", "[meta.rel]") {
  struct F {
    int operator()(int) const noexcept { return 0; }
  };
  CHECK(std::is_same<decltype(slb::is_nothrow_invocable_v<F, int>),
                     bool const>::value);
  CHECK(slb::is_nothrow_invocable_v<F, int> ==
        slb::is_nothrow_invocable<F, int>::value);
  constexpr bool v = slb::is_nothrow_invocable_v<F, int>;
  (void)v;
}

// template<class R, class Fn, class... ArgTypes>
//   inline constexpr bool is_nothrow_invocable_r_v
//     = is_nothrow_invocable_r<R, Fn, ArgTypes...>::value;
TEST_CASE("is_nothrow_invocable_r_v", "[meta.rel]") {
  struct F {
    int operator()(int) const noexcept { return 0; }
  };

  {
    CHECK(std::is_same<decltype(slb::is_nothrow_invocable_r_v<int, F, int>),
                       bool const>::value);
    CHECK(slb::is_nothrow_invocable_r_v<int, F, int> ==
          slb::is_nothrow_invocable_r<int, F, int>::value);
    constexpr bool v = slb::is_nothrow_invocable_r_v<int, F, int>;
    (void)v;
  }

  {
    CHECK(std::is_same<decltype(slb::is_nothrow_invocable_r_v<void, F, int>),
                       bool const>::value);
    CHECK(slb::is_nothrow_invocable_r_v<void, F, int> ==
          slb::is_nothrow_invocable_r<void, F, int>::value);
    constexpr bool v = slb::is_nothrow_invocable_r_v<void, F, int>;
    (void)v;
  }
}

// [meta.logical], logical operator traits

// template<class... B>
//   inline constexpr bool conjunction_v = conjunction<B...>::value;
TEST_CASE("conjunction_v", "[meta.logical]") {
  {
    CHECK(std::is_same<decltype(slb::conjunction_v<>), bool const>::value);
    CHECK(slb::conjunction_v<> == slb::conjunction<>::value);
    constexpr bool v = slb::conjunction_v<>;
    (void)v;
  }

  {
    CHECK(std::is_same<decltype(slb::conjunction_v<slb::true_type>),
                       bool const>::value);
    CHECK(slb::conjunction_v<slb::true_type> ==
          slb::conjunction<slb::true_type>::value);
    constexpr bool v = slb::conjunction_v<slb::true_type>;
    (void)v;
  }

  {
    CHECK(std::is_same<decltype(slb::conjunction_v<slb::false_type>),
                       bool const>::value);
    CHECK(slb::conjunction_v<slb::false_type> ==
          slb::conjunction<slb::false_type>::value);
    constexpr bool v = slb::conjunction_v<slb::false_type>;
    (void)v;
  }
}

// template<class... B>
//   inline constexpr bool disjunction_v = disjunction<B...>::value;
TEST_CASE("disjunction_v", "[meta.logical]") {
  {
    CHECK(std::is_same<decltype(slb::disjunction_v<>), bool const>::value);
    CHECK(slb::disjunction_v<> == slb::disjunction<>::value);
    constexpr bool v = slb::disjunction_v<>;
    (void)v;
  }

  {
    CHECK(std::is_same<decltype(slb::disjunction_v<slb::true_type>),
                       bool const>::value);
    CHECK(slb::disjunction_v<slb::true_type> ==
          slb::disjunction<slb::true_type>::value);
    constexpr bool v = slb::disjunction_v<slb::true_type>;
    (void)v;
  }

  {
    CHECK(std::is_same<decltype(slb::disjunction_v<slb::false_type>),
                       bool const>::value);
    CHECK(slb::disjunction_v<slb::false_type> ==
          slb::disjunction<slb::false_type>::value);
    constexpr bool v = slb::disjunction_v<slb::false_type>;
    (void)v;
  }
}

// template<class B>
//   inline constexpr bool negation_v = negation<B>::value;
TEST_CASE("negation_v", "[meta.logical]") {
  {
    CHECK(std::is_same<decltype(slb::negation_v<slb::true_type>),
                       bool const>::value);
    CHECK(slb::negation_v<slb::true_type> ==
          slb::negation<slb::true_type>::value);
    constexpr bool v = slb::negation_v<slb::true_type>;
    (void)v;
  }

  {
    CHECK(std::is_same<decltype(slb::negation_v<slb::false_type>),
                       bool const>::value);
    CHECK(slb::negation_v<slb::false_type> ==
          slb::negation<slb::false_type>::value);
    constexpr bool v = slb::negation_v<slb::false_type>;
    (void)v;
  }
}

#endif
