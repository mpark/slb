/*
  SLB.TypeTraits

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_TYPE_TRAITS_HPP
#define SLB_TYPE_TRAITS_HPP

/*

Header <type_traits> synopsis [meta.type.synop]

namespace std {
  // [meta.help], helper class
  template<class T, T v> struct integral_constant;

  template<bool B>
    using bool_constant = integral_constant<bool, B>;
  using true_type  = bool_constant<true>;
  using false_type = bool_constant<false>;

  // [meta.unary.cat], primary type categories
  template<class T> struct is_void;
  template<class T> struct is_null_pointer;
  template<class T> struct is_integral;
  template<class T> struct is_floating_point;
  template<class T> struct is_array;
  template<class T> struct is_pointer;
  template<class T> struct is_lvalue_reference;
  template<class T> struct is_rvalue_reference;
  template<class T> struct is_member_object_pointer;
  template<class T> struct is_member_function_pointer;
  template<class T> struct is_enum;
  template<class T> struct is_union;
  template<class T> struct is_class;
  template<class T> struct is_function;

  // [meta.unary.comp], composite type categories
  template<class T> struct is_reference;
  template<class T> struct is_arithmetic;
  template<class T> struct is_fundamental;
  template<class T> struct is_object;
  template<class T> struct is_scalar;
  template<class T> struct is_compound;
  template<class T> struct is_member_pointer;

  // [meta.unary.prop], type properties
  template<class T> struct is_const;
  template<class T> struct is_volatile;
  template<class T> struct is_trivial;
  template<class T> struct is_trivially_copyable;
  template<class T> struct is_standard_layout;
  template<class T> struct is_empty;
  template<class T> struct is_polymorphic;
  template<class T> struct is_abstract;
  template<class T> struct is_final;
  template<class T> struct is_aggregate;

  template<class T> struct is_signed;
  template<class T> struct is_unsigned;

  template<class T, class... Args> struct is_constructible;
  template<class T> struct is_default_constructible;
  template<class T> struct is_copy_constructible;
  template<class T> struct is_move_constructible;

  template<class T, class U> struct is_assignable;
  template<class T> struct is_copy_assignable;
  template<class T> struct is_move_assignable;

  template<class T, class U> struct is_swappable_with;
  template<class T> struct is_swappable;

  template<class T> struct is_destructible;

  template<class T, class... Args> struct is_trivially_constructible;
  template<class T> struct is_trivially_default_constructible;
  template<class T> struct is_trivially_copy_constructible;
  template<class T> struct is_trivially_move_constructible;

  template<class T, class U> struct is_trivially_assignable;
  template<class T> struct is_trivially_copy_assignable;
  template<class T> struct is_trivially_move_assignable;
  template<class T> struct is_trivially_destructible;

  template<class T, class... Args> struct is_nothrow_constructible;
  template<class T> struct is_nothrow_default_constructible;
  template<class T> struct is_nothrow_copy_constructible;
  template<class T> struct is_nothrow_move_constructible;

  template<class T, class U> struct is_nothrow_assignable;
  template<class T> struct is_nothrow_copy_assignable;
  template<class T> struct is_nothrow_move_assignable;

  template<class T, class U> struct is_nothrow_swappable_with;
  template<class T> struct is_nothrow_swappable;

  template<class T> struct is_nothrow_destructible;

  template<class T> struct has_virtual_destructor;

  template<class T> struct has_unique_object_representations;

  // [meta.unary.prop.query], type property queries
  template<class T> struct alignment_of;
  template<class T> struct rank;
  template<class T, unsigned I = 0> struct extent;

  // [meta.rel], type relations
  template<class T, class U> struct is_same;
  template<class Base, class Derived> struct is_base_of;
  template<class From, class To> struct is_convertible;

  template<class Fn, class... ArgTypes> struct is_invocable;
  template<class R, class Fn, class... ArgTypes> struct is_invocable_r;

  template<class Fn, class... ArgTypes> struct is_nothrow_invocable;
  template<class R, class Fn, class... ArgTypes> struct is_nothrow_invocable_r;

  // [meta.trans.cv], const-volatile modifications
  template<class T> struct remove_const;
  template<class T> struct remove_volatile;
  template<class T> struct remove_cv;
  template<class T> struct add_const;
  template<class T> struct add_volatile;
  template<class T> struct add_cv;

  template<class T>
    using remove_const_t    = typename remove_const<T>::type;
  template<class T>
    using remove_volatile_t = typename remove_volatile<T>::type;
  template<class T>
    using remove_cv_t       = typename remove_cv<T>::type;
  template<class T>
    using add_const_t       = typename add_const<T>::type;
  template<class T>
    using add_volatile_t    = typename add_volatile<T>::type;
  template<class T>
    using add_cv_t          = typename add_cv<T>::type;

  // [meta.trans.ref], reference modifications
  template<class T> struct remove_reference;
  template<class T> struct add_lvalue_reference;
  template<class T> struct add_rvalue_reference;

  template<class T>
    using remove_reference_t     = typename remove_reference<T>::type;
  template<class T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
  template<class T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

  // [meta.trans.sign], sign modifications
  template<class T> struct make_signed;
  template<class T> struct make_unsigned;

  template<class T>
    using make_signed_t   = typename make_signed<T>::type;
  template<class T>
    using make_unsigned_t = typename make_unsigned<T>::type;

  // [meta.trans.arr], array modifications
  template<class T> struct remove_extent;
  template<class T> struct remove_all_extents;

  template<class T>
    using remove_extent_t      = typename remove_extent<T>::type;
  template<class T>
    using remove_all_extents_t = typename remove_all_extents<T>::type;

  // [meta.trans.ptr], pointer modifications
  template<class T> struct remove_pointer;
  template<class T> struct add_pointer;

  template<class T>
    using remove_pointer_t = typename remove_pointer<T>::type;
  template<class T>
    using add_pointer_t    = typename add_pointer<T>::type;

  // [meta.trans.other], other transformations
  template<size_t Len,
           size_t Align = default-alignment> // see [meta.trans.other]
    struct aligned_storage;
  template<size_t Len, class... Types> struct aligned_union;
  template<class T> struct remove_cvref;
  template<class T> struct decay;
  template<bool, class T = void> struct enable_if;
  template<bool, class T, class F> struct conditional;
  template<class... T> struct common_type;
  template<class T> struct underlying_type;
  template<class Fn, class... ArgTypes> struct invoke_result;

  template<size_t Len,
           size_t Align = default-alignment> // see [meta.trans.other]
    using aligned_storage_t = typename aligned_storage<Len, Align>::type;
  template<size_t Len, class... Types>
    using aligned_union_t   = typename aligned_union<Len, Types...>::type;
  template<class T>
    using remove_cvref_t    = typename remove_cvref<T>::type;
  template<class T>
    using decay_t           = typename decay<T>::type;
  template<bool b, class T = void>
    using enable_if_t       = typename enable_if<b, T>::type;
  template<bool b, class T, class F>
    using conditional_t     = typename conditional<b, T, F>::type;
  template<class... T>
    using common_type_t     = typename common_type<T...>::type;
  template<class T>
    using underlying_type_t = typename underlying_type<T>::type;
  template<class Fn, class... ArgTypes>
    using invoke_result_t   = typename invoke_result<Fn, ArgTypes...>::type;
  template<class...>
    using void_t            = void;

  // [meta.logical], logical operator traits
  template<class... B> struct conjunction;
  template<class... B> struct disjunction;
  template<class B> struct negation;

  // [meta.endian], endian
  enum class endian {
    little = see below,
    big    = see below,
    native = see below
  };

  // [meta.unary.cat], primary type categories
  template<class T>
    inline constexpr bool is_void_v = is_void<T>::value;
  template<class T>
    inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
  template<class T>
    inline constexpr bool is_integral_v = is_integral<T>::value;
  template<class T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
  template<class T>
    inline constexpr bool is_array_v = is_array<T>::value;
  template<class T>
    inline constexpr bool is_pointer_v = is_pointer<T>::value;
  template<class T>
    inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
  template<class T>
    inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;
  template<class T>
    inline constexpr bool is_member_object_pointer_v
      = is_member_object_pointer<T>::value;
  template<class T>
    inline constexpr bool is_member_function_pointer_v
      = is_member_function_pointer<T>::value;
  template<class T>
    inline constexpr bool is_enum_v = is_enum<T>::value;
  template<class T>
    inline constexpr bool is_union_v = is_union<T>::value;
  template<class T>
    inline constexpr bool is_class_v = is_class<T>::value;
  template<class T>
    inline constexpr bool is_function_v = is_function<T>::value;

  // [meta.unary.comp], composite type categories
  template<class T>
    inline constexpr bool is_reference_v = is_reference<T>::value;
  template<class T>
    inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
  template<class T>
    inline constexpr bool is_fundamental_v = is_fundamental<T>::value;
  template<class T>
    inline constexpr bool is_object_v = is_object<T>::value;
  template<class T>
    inline constexpr bool is_scalar_v = is_scalar<T>::value;
  template<class T>
    inline constexpr bool is_compound_v = is_compound<T>::value;
  template<class T>
    inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

  // [meta.unary.prop], type properties
  template<class T>
    inline constexpr bool is_const_v = is_const<T>::value;
  template<class T>
    inline constexpr bool is_volatile_v = is_volatile<T>::value;
  template<class T>
    inline constexpr bool is_trivial_v = is_trivial<T>::value;
  template<class T>
    inline constexpr bool is_trivially_copyable_v
      = is_trivially_copyable<T>::value;
  template<class T>
    inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;
  template<class T>
    inline constexpr bool is_empty_v = is_empty<T>::value;
  template<class T>
    inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;
  template<class T>
    inline constexpr bool is_abstract_v = is_abstract<T>::value;
  template<class T>
    inline constexpr bool is_final_v = is_final<T>::value;
  template<class T>
    inline constexpr bool is_aggregate_v = is_aggregate<T>::value;
  template<class T>
    inline constexpr bool is_signed_v = is_signed<T>::value;
  template<class T>
    inline constexpr bool is_unsigned_v = is_unsigned<T>::value;
  template<class T, class... Args>
    inline constexpr bool is_constructible_v
      = is_constructible<T, Args...>::value;
  template<class T>
    inline constexpr bool is_default_constructible_v
      = is_default_constructible<T>::value;
  template<class T>
    inline constexpr bool is_copy_constructible_v
      = is_copy_constructible<T>::value;
  template<class T>
    inline constexpr bool is_move_constructible_v
      = is_move_constructible<T>::value;
  template<class T, class U>
    inline constexpr bool is_assignable_v = is_assignable<T, U>::value;
  template<class T>
    inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;
  template<class T>
    inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;
  template<class T, class U>
    inline constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;
  template<class T>
    inline constexpr bool is_swappable_v = is_swappable<T>::value;
  template<class T>
    inline constexpr bool is_destructible_v = is_destructible<T>::value;
  template<class T, class... Args>
    inline constexpr bool is_trivially_constructible_v
      = is_trivially_constructible<T, Args...>::value;
  template<class T>
    inline constexpr bool is_trivially_default_constructible_v
      = is_trivially_default_constructible<T>::value;
  template<class T>
    inline constexpr bool is_trivially_copy_constructible_v
      = is_trivially_copy_constructible<T>::value;
  template<class T>
    inline constexpr bool is_trivially_move_constructible_v
      = is_trivially_move_constructible<T>::value;
  template<class T, class U>
    inline constexpr bool is_trivially_assignable_v
      = is_trivially_assignable<T, U>::value;
  template<class T>
    inline constexpr bool is_trivially_copy_assignable_v
      = is_trivially_copy_assignable<T>::value;
  template<class T>
    inline constexpr bool is_trivially_move_assignable_v
      = is_trivially_move_assignable<T>::value;
  template<class T>
    inline constexpr bool is_trivially_destructible_v
      = is_trivially_destructible<T>::value;
  template<class T, class... Args>
    inline constexpr bool is_nothrow_constructible_v
      = is_nothrow_constructible<T, Args...>::value;
  template<class T>
    inline constexpr bool is_nothrow_default_constructible_v
      = is_nothrow_default_constructible<T>::value;
  template<class T>
    inline constexpr bool is_nothrow_copy_constructible_v
    = is_nothrow_copy_constructible<T>::value;
  template<class T>
    inline constexpr bool is_nothrow_move_constructible_v
      = is_nothrow_move_constructible<T>::value;
  template<class T, class U>
    inline constexpr bool is_nothrow_assignable_v
      = is_nothrow_assignable<T, U>::value;
  template<class T>
    inline constexpr bool is_nothrow_copy_assignable_v
      = is_nothrow_copy_assignable<T>::value;
  template<class T>
    inline constexpr bool is_nothrow_move_assignable_v
      = is_nothrow_move_assignable<T>::value;
  template<class T, class U>
    inline constexpr bool is_nothrow_swappable_with_v
      = is_nothrow_swappable_with<T, U>::value;
  template<class T>
    inline constexpr bool is_nothrow_swappable_v
      = is_nothrow_swappable<T>::value;
  template<class T>
    inline constexpr bool is_nothrow_destructible_v
      = is_nothrow_destructible<T>::value;
  template<class T>
    inline constexpr bool has_virtual_destructor_v
      = has_virtual_destructor<T>::value;
  template<class T>
    inline constexpr bool has_unique_object_representations_v
      = has_unique_object_representations<T>::value;

  // [meta.unary.prop.query], type property queries
  template<class T>
    inline constexpr size_t alignment_of_v = alignment_of<T>::value;
  template<class T>
    inline constexpr size_t rank_v = rank<T>::value;
  template<class T, unsigned I = 0>
    inline constexpr size_t extent_v = extent<T, I>::value;

  // [meta.rel], type relations
  template<class T, class U>
    inline constexpr bool is_same_v = is_same<T, U>::value;
  template<class Base, class Derived>
    inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
  template<class From, class To>
    inline constexpr bool is_convertible_v = is_convertible<From, To>::value;
  template<class Fn, class... ArgTypes>
    inline constexpr bool is_invocable_v = is_invocable<Fn, ArgTypes...>::value;
  template<class R, class Fn, class... ArgTypes>
    inline constexpr bool is_invocable_r_v
      = is_invocable_r<R, Fn, ArgTypes...>::value;
  template<class Fn, class... ArgTypes>
    inline constexpr bool is_nothrow_invocable_v
      = is_nothrow_invocable<Fn, ArgTypes...>::value;
  template<class R, class Fn, class... ArgTypes>
    inline constexpr bool is_nothrow_invocable_r_v
      = is_nothrow_invocable_r<R, Fn, ArgTypes...>::value;

  // [meta.logical], logical operator traits
  template<class... B>
    inline constexpr bool conjunction_v = conjunction<B...>::value;
  template<class... B>
    inline constexpr bool disjunction_v = disjunction<B...>::value;
  template<class B>
    inline constexpr bool negation_v = negation<B>::value;
}

*/

#include <type_traits>

#include <cstddef>

#include "detail/config.hpp"
#include "detail/invoke.hpp"
#include "detail/lib.hpp"

namespace slb {

// libstdc++ did not mark `operator value_type` / `operator()` as `noexcept`
// until version 8.
#if __cpp_lib_integral_constant_callable &&                                    \
    (!defined(__GLIBCXX__) || __has_include(<filesystem>))
#define SLB_INTEGRAL_CONSTANT 2 // available / conforming
#else
#define SLB_INTEGRAL_CONSTANT 1 // available / non-conforming
#endif

// [meta.help], helper class

#if SLB_INTEGRAL_CONSTANT == 2 // C++14
using std::integral_constant;
#else
template <typename T, T v>
struct integral_constant : std::integral_constant<T, v> {
  using value_type = T;
  using type = integral_constant;

  constexpr operator value_type() const noexcept { return this->value; }
  constexpr value_type operator()() const noexcept { return this->value; }
};
#endif

template <bool B>
using bool_constant = slb::integral_constant<bool, B>;

using true_type = slb::bool_constant<true>;
using false_type = slb::bool_constant<false>;

// [meta.unary.cat], primary type categories

#if SLB_INTEGRAL_CONSTANT == 2 // C++14
using std::is_void;
using std::is_integral;
using std::is_floating_point;
using std::is_array;
using std::is_pointer;
using std::is_lvalue_reference;
using std::is_rvalue_reference;
using std::is_enum;
using std::is_union;
using std::is_class;
#else
template <typename T>
struct is_void : slb::bool_constant<std::is_void<T>::value> {};

template <typename T>
struct is_integral : slb::bool_constant<std::is_integral<T>::value> {};

template <typename T>
struct is_floating_point
    : slb::bool_constant<std::is_floating_point<T>::value> {};

template <typename T>
struct is_array : slb::bool_constant<std::is_array<T>::value> {};

template <typename T>
struct is_pointer : slb::bool_constant<std::is_pointer<T>::value> {};

template <typename T>
struct is_lvalue_reference
    : slb::bool_constant<std::is_lvalue_reference<T>::value> {};

template <typename T>
struct is_rvalue_reference
    : slb::bool_constant<std::is_rvalue_reference<T>::value> {};

template <typename T>
struct is_enum : slb::bool_constant<std::is_enum<T>::value> {};

template <typename T>
struct is_union : slb::bool_constant<std::is_union<T>::value> {};

template <typename T>
struct is_class : slb::bool_constant<std::is_class<T>::value> {};
#endif

#if SLB_INTEGRAL_CONSTANT == 2 && __cpp_lib_is_null_pointer // C++14
using std::is_null_pointer;
#else
template <typename T>
struct is_null_pointer
    : slb::bool_constant<
          std::is_same<std::nullptr_t,
                       typename std::remove_cv<T>::type>::value> {};
#endif

// libstdc++ `is_function` does not recognize `noexcept` until version 7.
#if !defined(__GLIBCXX__) || __has_include(<variant>) // >= libstdc++-7
#define SLB_IS_FUNCTION SLB_INTEGRAL_CONSTANT
#else
#define SLB_IS_FUNCTION 0 // not available
#endif

#if SLB_IS_FUNCTION == 2
using std::is_member_object_pointer;
using std::is_member_function_pointer;
using std::is_function;
#elif SLB_IS_FUNCTION == 1
template <typename T>
struct is_member_object_pointer
    : slb::bool_constant<std::is_member_object_pointer<T>::value> {};

template <typename T>
struct is_member_function_pointer
    : slb::bool_constant<std::is_member_function_pointer<T>::value> {};

template <typename T>
struct is_function : slb::bool_constant<std::is_function<T>::value> {};
#else
template <typename T>
struct is_member_object_pointer : slb::false_type {};

template <typename T, typename C>
struct is_member_object_pointer<T C::*>
    : slb::bool_constant<!detail::lib::is_function<T>::value> {};

template <typename T>
struct is_member_function_pointer : slb::false_type {};

template <typename T, typename C>
struct is_member_function_pointer<T C::*>
    : slb::bool_constant<detail::lib::is_function<T>::value> {};

template <typename T>
struct is_function : slb::bool_constant<detail::lib::is_function<T>::value> {};
#endif

// [meta.unary.comp], composite type categories

#if SLB_INTEGRAL_CONSTANT == 2 // C++14
using std::is_reference;
using std::is_arithmetic;
using std::is_fundamental;
using std::is_scalar;
using std::is_compound;
using std::is_member_pointer;
#else
template <typename T>
struct is_reference : slb::bool_constant<std::is_reference<T>::value> {};

template <typename T>
struct is_arithmetic : slb::bool_constant<std::is_arithmetic<T>::value> {};

template <typename T>
struct is_fundamental : slb::bool_constant<std::is_fundamental<T>::value> {};

template <typename T>
struct is_scalar : slb::bool_constant<std::is_scalar<T>::value> {};

template <typename T>
struct is_compound : slb::bool_constant<std::is_compound<T>::value> {};

template <typename T>
struct is_member_pointer
    : slb::bool_constant<std::is_member_pointer<T>::value> {};
#endif

#if SLB_IS_FUNCTION == 2
using std::is_object;
#elif SLB_IS_FUNCTION == 1
template <typename T>
struct is_object : slb::bool_constant<std::is_object<T>::value> {};
#else
template <typename T>
struct is_object : slb::bool_constant<!detail::lib::is_function<T>::value &&
                                      !std::is_reference<T>::value &&
                                      !std::is_void<T>::value> {};
#endif

// [meta.unary.prop], type properties

#if SLB_INTEGRAL_CONSTANT == 2 // C++14
using std::is_const;
using std::is_volatile;
using std::is_trivial;
using std::is_standard_layout;
using std::is_empty;
using std::is_polymorphic;
using std::is_abstract;
#else
template <typename T>
struct is_const : slb::bool_constant<std::is_const<T>::value> {};

template <typename T>
struct is_volatile : slb::bool_constant<std::is_volatile<T>::value> {};

template <typename T>
struct is_trivial : slb::bool_constant<std::is_trivial<T>::value> {};

template <typename T>
struct is_standard_layout
    : slb::bool_constant<std::is_standard_layout<T>::value> {};

template <typename T>
struct is_empty : slb::bool_constant<std::is_empty<T>::value> {};

template <typename T>
struct is_polymorphic : slb::bool_constant<std::is_polymorphic<T>::value> {};

template <typename T>
struct is_abstract : slb::bool_constant<std::is_abstract<T>::value> {};
#endif

#if !defined(__GLIBCXX__) || __has_include(<codecvt>) // >= libstdc++-5
#define SLB_TRIVIALITY_TRAITS SLB_INTEGRAL_CONSTANT
#else
#define SLB_TRIVIALITY_TRAITS 0 // not available
#endif

#if SLB_TRIVIALITY_TRAITS == 2
using std::is_trivially_copyable;
#elif SLB_TRIVIALITY_TRAITS == 1
template <typename T>
struct is_trivially_copyable
    : slb::bool_constant<std::is_trivially_copyable<T>::value> {};
#else
template <typename T>
struct is_trivially_copyable {
  static_assert(detail::lib::always_false<T>::value,
                "`is_trivially_copyable` is not available.");
};
#endif

#if SLB_INTEGRAL_CONSTANT == 2 && __cpp_lib_is_final // C++14
using std::is_final;
#elif __has_feature(is_final) || (__GNUC__ > 4) ||                             \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
template <typename T>
struct is_final : slb::bool_constant<__is_final(T)> {};
#else
template <typename T>
struct is_final {
  static_assert(detail::lib::always_false<T>::value,
                "`is_final` is not available.");
};
#endif

#if SLB_INTEGRAL_CONSTANT == 2 && __cpp_lib_is_aggregate // C++17
using std::is_aggregate;
#elif __has_feature(is_aggregate) || (__GNUC__ >= 7)
template <typename T>
struct is_aggregate : slb::bool_constant<__is_aggregate(T)> {};
#else
template <typename T>
struct is_aggregate {
  static_assert(detail::lib::always_false<T>::value,
                "`is_aggregate` is not available.");
};
#endif

#if SLB_INTEGRAL_CONSTANT == 2 // C++14
using std::is_signed;
using std::is_unsigned;

using std::is_constructible;
using std::is_default_constructible;
using std::is_copy_constructible;
using std::is_move_constructible;

using std::is_nothrow_constructible;
using std::is_nothrow_default_constructible;
using std::is_nothrow_copy_constructible;
using std::is_nothrow_move_constructible;

using std::is_destructible;
using std::is_trivially_destructible;
using std::is_nothrow_destructible;

using std::is_assignable;
using std::is_copy_assignable;
using std::is_move_assignable;

using std::is_nothrow_assignable;
using std::is_nothrow_copy_assignable;
using std::is_nothrow_move_assignable;
#else
template <typename T>
struct is_signed : slb::bool_constant<std::is_signed<T>::value> {};

template <typename T>
struct is_unsigned : slb::bool_constant<std::is_unsigned<T>::value> {};

template <typename T, typename... Args>
struct is_constructible
    : slb::bool_constant<std::is_constructible<T, Args...>::value> {};

template <typename T>
struct is_default_constructible
    : slb::bool_constant<std::is_default_constructible<T>::value> {};

template <typename T>
struct is_copy_constructible
    : slb::bool_constant<std::is_copy_constructible<T>::value> {};

template <typename T>
struct is_move_constructible
    : slb::bool_constant<std::is_move_constructible<T>::value> {};

template <typename T, typename... Args>
struct is_nothrow_constructible
    : slb::bool_constant<std::is_nothrow_constructible<T, Args...>::value> {};

template <typename T>
struct is_nothrow_default_constructible
    : slb::bool_constant<std::is_nothrow_default_constructible<T>::value> {};

template <typename T>
struct is_nothrow_copy_constructible
    : slb::bool_constant<std::is_nothrow_copy_constructible<T>::value> {};

template <typename T>
struct is_nothrow_move_constructible
    : slb::bool_constant<std::is_nothrow_move_constructible<T>::value> {};

template <typename T>
struct is_destructible : slb::bool_constant<std::is_destructible<T>::value> {};

template <typename T>
struct is_trivially_destructible
    : slb::bool_constant<std::is_trivially_destructible<T>::value> {};

template <typename T>
struct is_nothrow_destructible
    : slb::bool_constant<std::is_nothrow_destructible<T>::value> {};

template <typename T, typename U>
struct is_assignable : slb::bool_constant<std::is_assignable<T, U>::value> {};

template <typename T>
struct is_copy_assignable
    : slb::bool_constant<std::is_copy_assignable<T>::value> {};

template <typename T>
struct is_move_assignable
    : slb::bool_constant<std::is_move_assignable<T>::value> {};

template <typename T, typename U>
struct is_nothrow_assignable
    : slb::bool_constant<std::is_nothrow_assignable<T, U>::value> {};

template <typename T>
struct is_nothrow_copy_assignable
    : slb::bool_constant<std::is_nothrow_copy_assignable<T>::value> {};

template <typename T>
struct is_nothrow_move_assignable
    : slb::bool_constant<std::is_nothrow_move_assignable<T>::value> {};
#endif

#if SLB_TRIVIALITY_TRAITS == 2
using std::is_trivially_constructible;
using std::is_trivially_default_constructible;
using std::is_trivially_copy_constructible;
using std::is_trivially_move_constructible;
using std::is_trivially_assignable;
using std::is_trivially_copy_assignable;
using std::is_trivially_move_assignable;
#elif SLB_TRIVIALITY_TRAITS == 1
template <typename T, typename... Args>
struct is_trivially_constructible
    : slb::bool_constant<std::is_trivially_constructible<T, Args...>::value> {};

template <typename T>
struct is_trivially_default_constructible
    : slb::bool_constant<std::is_trivially_default_constructible<T>::value> {};

template <typename T>
struct is_trivially_copy_constructible
    : slb::bool_constant<std::is_trivially_copy_constructible<T>::value> {};

template <typename T>
struct is_trivially_move_constructible
    : slb::bool_constant<std::is_trivially_move_constructible<T>::value> {};

template <typename T, typename U>
struct is_trivially_assignable
    : slb::bool_constant<std::is_trivially_assignable<T, U>::value> {};

template <typename T>
struct is_trivially_copy_assignable
    : slb::bool_constant<std::is_trivially_copy_assignable<T>::value> {};

template <typename T>
struct is_trivially_move_assignable
    : slb::bool_constant<std::is_trivially_move_assignable<T>::value> {};
#else
template <typename T, typename... Args>
struct is_trivially_constructible {
  static_assert(detail::lib::always_false<T>::value,
                "`is_trivially_constructible` is not available.");
};

template <typename T>
struct is_trivially_default_constructible {
  static_assert(detail::lib::always_false<T>::value,
                "`is_trivially_default_constructible` is not available.");
};

template <typename T>
struct is_trivially_copy_constructible {
  static_assert(detail::lib::always_false<T>::value,
                "`is_trivially_copy_constructible` is not available.");
};

template <typename T>
struct is_trivially_move_constructible {
  static_assert(detail::lib::always_false<T>::value,
                "`is_trivially_move_constructible` is not available.");
};

template <typename T, typename U>
struct is_trivially_assignable {
  static_assert(detail::lib::always_false<T>::value,
                "`is_trivially_assignable` is not available.");
};

template <typename T>
struct is_trivially_copy_assignable {
  static_assert(detail::lib::always_false<T>::value,
                "`is_trivially_copy_assignable` is not available.");
};

template <typename T>
struct is_trivially_move_assignable {
  static_assert(detail::lib::always_false<T>::value,
                "`is_trivially_move_assignable` is not available.");
};
#endif

#if SLB_INTEGRAL_CONSTANT == 2 && __cpp_lib_is_swappable // C++17
using std::is_swappable_with;
using std::is_swappable;
using std::is_nothrow_swappable_with;
using std::is_nothrow_swappable;
#else
// TODO
// template <typename T, typename U> struct is_swappable_with;
// template <typename T> struct is_swappable;
// template <typename T, typename U> struct is_nothrow_swappable_with;
// template <typename T> struct is_nothrow_swappable;
#endif

#if SLB_INTEGRAL_CONSTANT == 2 // C++14
using std::has_virtual_destructor;
#else
template <typename T>
struct has_virtual_destructor
    : slb::bool_constant<std::has_virtual_destructor<T>::value> {};
#endif

#if SLB_INTEGRAL_CONSTANT == 2 &&                                              \
    __cpp_lib_has_unique_object_representations // C++17
using std::has_unique_object_representations;
#elif __has_feature(has_unique_object_representations) || (__GNUC__ >= 7)
template <typename T>
struct has_unique_object_representations
    : slb::bool_constant<__has_unique_object_representations(T)> {};
#else
template <typename T>
struct has_unique_object_representations {
  static_assert(detail::lib::always_false<T>::value,
                "`has_unique_object_representations` is not available.");
};
#endif

// [meta.unary.prop.query] type property queries

#if SLB_INTEGRAL_CONSTANT == 2 // C++14
using std::alignment_of;
using std::rank;
using std::extent;
#else
template <typename T>
struct alignment_of
    : slb::integral_constant<std::size_t, std::alignment_of<T>::value> {};

template <typename T>
struct rank : slb::integral_constant<std::size_t, std::rank<T>::value> {};

template <typename T, unsigned I = 0>
struct extent : slb::integral_constant<std::size_t, std::extent<T, I>::value> {
};
#endif

// [meta.rel], type relations

#if SLB_INTEGRAL_CONSTANT == 2 // C++14
using std::is_same;
using std::is_base_of;
using std::is_convertible;
#else
template <typename T, typename U>
struct is_same : slb::bool_constant<std::is_same<T, U>::value> {};

template <typename Base, typename Derived>
struct is_base_of : slb::bool_constant<std::is_base_of<Base, Derived>::value> {
};

template <typename From, typename To>
struct is_convertible
    : slb::bool_constant<std::is_convertible<From, To>::value> {};
#endif

// We only enable the C++17 implementation under C++2a here to account for
// P0704: "Fixing const-qualified pointers to members".

#if SLB_INTEGRAL_CONSTANT == 2 && __cpp_lib_is_invocable /* C++17 */ &&        \
    __cplusplus > 201703L /* C++2a */
using std::is_invocable;
using std::is_invocable_r;
using std::is_nothrow_invocable;
using std::is_nothrow_invocable_r;
#else
namespace detail {

template <typename F, typename Enable = void>
struct is_invocable_impl {
  static constexpr bool value = false;
  static constexpr bool nothrow = false;
};

template <typename F, typename... Args>
struct is_invocable_impl<F(Args...),
                         decltype((void)(detail::invoke(
                             std::declval<F>(), std::declval<Args>()...)))> {
  static constexpr bool value = true;
  static constexpr bool nothrow =
      noexcept(detail::invoke(std::declval<F>(), std::declval<Args>()...));
};

template <typename R, typename F, typename Enable = void>
struct is_invocable_r_impl {
  static constexpr bool value = false;
  static constexpr bool nothrow = false;
};

template <typename R, typename F, typename... Args>
struct is_invocable_r_impl<R,
                           F(Args...),
                           decltype((void)(detail::invoke_r<R>(
                               std::declval<F>(), std::declval<Args>()...)))> {
  static constexpr bool value = true;
  static constexpr bool nothrow =
      noexcept(detail::invoke_r<R>(std::declval<F>(), std::declval<Args>()...));
};

} // namespace detail

template <typename F, typename... Args>
struct is_invocable
    : slb::bool_constant<detail::is_invocable_impl<F && (Args && ...)>::value> {
};

template <typename R, typename F, typename... Args>
struct is_invocable_r
    : slb::bool_constant<
          detail::is_invocable_r_impl<R, F && (Args && ...)>::value> {};

template <typename F, typename... Args>
struct is_nothrow_invocable
    : slb::bool_constant<
          detail::is_invocable_impl<F && (Args && ...)>::nothrow> {};

template <typename R, typename F, typename... Args>
struct is_nothrow_invocable_r
    : slb::bool_constant<
          detail::is_invocable_r_impl<R, F && (Args && ...)>::nothrow> {};
#endif

// [meta.trans.cv], const-volatile modifications

using std::remove_const;
using std::remove_volatile;
using std::remove_cv;
using std::add_const;
using std::add_volatile;
using std::add_cv;

template <typename T>
using remove_const_t = typename slb::remove_const<T>::type;

template <typename T>
using remove_volatile_t = typename slb::remove_volatile<T>::type;

template <typename T>
using remove_cv_t = typename slb::remove_cv<T>::type;

template <typename T>
using add_const_t = typename slb::add_const<T>::type;

template <typename T>
using add_volatile_t = typename slb::add_volatile<T>::type;

template <typename T>
using add_cv_t = typename slb::add_cv<T>::type;

// [meta.trans.ref], reference modifications

using std::remove_reference;
using std::add_lvalue_reference;
using std::add_rvalue_reference;

template <typename T>
using remove_reference_t = typename slb::remove_reference<T>::type;

template <typename T>
using add_lvalue_reference_t = typename slb::add_lvalue_reference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename slb::add_rvalue_reference<T>::type;

// [meta.trans.sign], sign modifications

using std::make_signed;
using std::make_unsigned;

template <typename T>
using make_signed_t = typename slb::make_signed<T>::type;

template <typename T>
using make_unsigned_t = typename slb::make_unsigned<T>::type;

// [meta.trans.arr], array modifications

using std::remove_extent;
using std::remove_all_extents;

template <typename T>
using remove_extent_t = typename slb::remove_extent<T>::type;

template <typename T>
using remove_all_extents_t = typename slb::remove_all_extents<T>::type;

// [meta.trans.ptr], pointer modifications

using std::remove_pointer;
using std::add_pointer;

template <typename T>
using remove_pointer_t = typename slb::remove_pointer<T>::type;

template <typename T>
using add_pointer_t = typename slb::add_pointer<T>::type;

// [meta.trans.other], other transformations

using std::aligned_storage;

// libstdc++ did not provide `aligned_union` until version 5.
// libc++ does not provide an out-of-class definition for `alignment_value`.
#if !defined(__GLIBCXX__) || __has_include(<codecvt>) // >= libstdc++-5
#if !defined(_LIBCPP_VERSION)
#define SLB_ALIGNED_UNION 2 // available / conforming
#else
#define SLB_ALIGNED_UNION 1 // available / non-conforming
#endif
#else
#define SLB_ALIGNED_UNION 0 // not available
#endif

#if SLB_ALIGNED_UNION == 2
using std::aligned_union;
#else
#if SLB_ALIGNED_UNION == 1
template <std::size_t Len, typename... Ts>
struct aligned_union : std::aligned_union<Len, Ts...> {
  static constexpr std::size_t alignment_value =
      std::aligned_union<Len, Ts...>::alignment_value;
};
#else
namespace detail {
template <std::size_t... Vs>
struct static_max;

template <std::size_t V>
struct static_max<V> {
  static constexpr std::size_t value = V;
};

template <std::size_t V0, std::size_t V1, std::size_t... Vs>
struct static_max<V0, V1, Vs...> : static_max<(V0 < V1 ? V1 : V0), Vs...> {};
} // namespace detail

template <std::size_t Len, typename... Ts>
struct aligned_union {
  static_assert(sizeof...(Ts) > 0, "At least one type shall be provided.");

  static constexpr std::size_t alignment_value =
      detail::static_max<alignof(Ts)...>::value;

  using type = typename std::aligned_storage<
      detail::static_max<Len, sizeof(Ts)...>::value,
      alignment_value>::type;
};
#endif

template <std::size_t Len, typename... Ts>
constexpr std::size_t aligned_union<Len, Ts...>::alignment_value;
#endif

template <typename T>
struct remove_cvref {
  using type =
      typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

using std::decay;
using std::enable_if;
using std::conditional;

// TODO: C++14 (SFINAE) / C++17 (recursive commutative `std::decay_t`)
using std::common_type;

using std::underlying_type;

namespace detail {

template <typename>
struct default_alignment;

template <std::size_t Len, std::size_t Align>
struct default_alignment<slb::aligned_storage<Len, Align>>
    : slb::integral_constant<std::size_t, Align> {};

} // namespace detail

template <std::size_t Len,
          std::size_t Align =
              detail::default_alignment<slb::aligned_storage<Len>>::value>
using aligned_storage_t = typename slb::aligned_storage<Len, Align>::type;

template <std::size_t Len, typename... Ts>
using aligned_union_t = typename slb::aligned_union<Len, Ts...>::type;

template <typename T>
using remove_cvref_t = typename slb::remove_cvref<T>::type;

template <typename T>
using decay_t = typename slb::decay<T>::type;

template <bool B, typename T = void>
using enable_if_t = typename slb::enable_if<B, T>::type;

template <bool B, typename T, typename F>
using conditional_t = typename slb::conditional<B, T, F>::type;

template <typename... Ts>
using common_type_t = typename slb::common_type<Ts...>::type;

template <typename T>
using underlying_type_t = typename slb::underlying_type<T>::type;

// We only enable the C++17 implementation under C++2a here to account for
// P0704: "Fixing const-qualified pointers to members".

#if __cpp_lib_is_invocable /* C++17 */ && __cplusplus > 201703L /* C++2a */
using std::invoke_result;
using std::invoke_result_t;
#else
namespace detail {

template <typename F, typename Enable = void>
struct invoke_result_impl {};

template <typename F, typename... Args>
struct invoke_result_impl<F(Args...),
                          decltype((void)detail::invoke(
                              std::declval<F>(), std::declval<Args>()...))> {
  using type =
      decltype(detail::invoke(std::declval<F>(), std::declval<Args>()...));
};

} // namespace detail

template <typename F, typename... Args>
struct invoke_result : detail::invoke_result_impl<F && (Args && ...)> {};

template <typename F, typename... Args>
using invoke_result_t = typename slb::invoke_result<F, Args...>::type;
#endif

// gcc did not implement CWG1558: "Unused arguments in alias template
// specializations" until version 5.
#if !defined(__GNUC__) || __GNUC__ >= 5
template <typename... Ts>
using void_t = void;
#else
namespace detail {
template <typename... Ts>
struct dependent_void {
  using type = void;
};
} // namespace detail

template <typename... Ts>
using void_t = typename detail::dependent_void<Ts...>::type;
#endif

// [meta.logical], logical operator traits

// We don't enable the C++17 implementations via `__cpp_lib_logical_traits`
// here since none of the implementations (i.e., libstdc++, libc++, MSVC)
// are currently strictly conforming.

template <typename... Bs>
struct conjunction : slb::true_type {};

template <typename B>
struct conjunction<B> : B {};

// We deviate slightly here from the standard by performing a `static_assert` to
// `bool` rather than a C-style cast. Reconsider this if it turns out to matter.
template <typename B, typename... Bs>
struct conjunction<B, Bs...>
    : slb::conditional_t<static_cast<bool>(B::value), conjunction<Bs...>, B> {
  static_assert(std::is_convertible<decltype((B::value)), bool>::value,
                "Member `value` shall be convertible to `bool`.");
};

template <typename... Bs>
struct disjunction : slb::false_type {};

template <typename B>
struct disjunction<B> : B {};

// We deviate slightly here from the standard by performing a `static_assert` to
// `bool` rather than a C-style cast. Reconsider this if it turns out to matter.
template <typename B, typename... Bs>
struct disjunction<B, Bs...>
    : slb::conditional_t<static_cast<bool>(B::value), B, disjunction<Bs...>> {
  static_assert(std::is_convertible<decltype((B::value)), bool>::value,
                "Member `value` shall be convertible to `bool`.");
};

template <typename B>
struct negation : slb::bool_constant<!static_cast<bool>(B::value)> {};

// [meta.endian], endian

enum class endian {
#ifdef _WIN32
  little = 0,
  big = 1,
  native = little
#else
  little = __ORDER_LITTLE_ENDIAN__,
  big = __ORDER_BIG_ENDIAN__,
  native = __BYTE_ORDER__
#endif
};

} // namespace slb

#endif // SLB_TYPE_TRAITS_HPP
