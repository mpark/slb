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

23.15.2 Header <type_traits> synopsis [meta.type.synop]

namespace std {
  // 23.15.3, helper class
  template <class T, T v> struct integral_constant;

  template <bool B>
  using bool_constant = integral_constant<bool, B>;
  using true_type = bool_constant<true>;
  using false_type = bool_constant<false>;

  // 23.15.4.1, primary type categories
  template <class T> struct is_void;
  template <class T> struct is_null_pointer;
  template <class T> struct is_integral;
  template <class T> struct is_floating_point;
  template <class T> struct is_array;
  template <class T> struct is_pointer;
  template <class T> struct is_lvalue_reference;
  template <class T> struct is_rvalue_reference;
  template <class T> struct is_member_object_pointer;
  template <class T> struct is_member_function_pointer;
  template <class T> struct is_enum;
  template <class T> struct is_union;
  template <class T> struct is_class;
  template <class T> struct is_function;

  // 23.15.4.2, composite type categories
  template <class T> struct is_reference;
  template <class T> struct is_arithmetic;
  template <class T> struct is_fundamental;
  template <class T> struct is_object;
  template <class T> struct is_scalar;
  template <class T> struct is_compound;
  template <class T> struct is_member_pointer;

  // 23.15.4.3, type properties
  template <class T> struct is_const;
  template <class T> struct is_volatile;
  template <class T> struct is_trivial;
  template <class T> struct is_trivially_copyable;
  template <class T> struct is_standard_layout;
  template <class T> struct is_empty;
  template <class T> struct is_polymorphic;
  template <class T> struct is_abstract;
  template <class T> struct is_final;
  template <class T> struct is_aggregate;

  template <class T> struct is_signed;
  template <class T> struct is_unsigned;

  template <class T, class... Args> struct is_constructible;
  template <class T> struct is_default_constructible;
  template <class T> struct is_copy_constructible;
  template <class T> struct is_move_constructible;

  template <class T, class... Args> struct is_trivially_constructible;
  template <class T> struct is_trivially_default_constructible;
  template <class T> struct is_trivially_copy_constructible;
  template <class T> struct is_trivially_move_constructible;

  template <class T, class... Args> struct is_nothrow_constructible;
  template <class T> struct is_nothrow_default_constructible;
  template <class T> struct is_nothrow_copy_constructible;
  template <class T> struct is_nothrow_move_constructible;

  template <class T> struct is_destructible;
  template <class T> struct is_trivially_destructible;
  template <class T> struct is_nothrow_destructible;

  template <class T, class U> struct is_assignable;
  template <class T> struct is_copy_assignable;
  template <class T> struct is_move_assignable;

  template <class T, class U> struct is_trivially_assignable;
  template <class T> struct is_trivially_copy_assignable;
  template <class T> struct is_trivially_move_assignable;

  template <class T, class U> struct is_nothrow_assignable;
  template <class T> struct is_nothrow_copy_assignable;
  template <class T> struct is_nothrow_move_assignable;

  template <class T, class U> struct is_swappable_with;
  template <class T> struct is_swappable;
  template <class T, class U> struct is_nothrow_swappable_with;
  template <class T> struct is_nothrow_swappable;

  template <class T> struct has_virtual_destructor;

  template <class T> struct has_unique_object_representations;

  // 23.15.5, type property queries
  template <class T> struct alignment_of;
  template <class T> struct rank;
  template <class T, unsigned I = 0> struct extent;

  // 23.15.6, type relations
  template <class T, class U> struct is_same;
  template <class Base, class Derived> struct is_base_of;
  template <class From, class To> struct is_convertible;

  template <class Fn, class... ArgTypes> struct is_invocable;
  template <class R, class Fn, class... ArgTypes> struct is_invocable_r;
  template <class Fn, class... ArgTypes> struct is_nothrow_invocable;
  template <class R, class Fn, class... ArgTypes> struct is_nothrow_invocable_r;

  // 23.15.7.1, const-volatile modifications
  template <class T> struct remove_const;
  template <class T> struct remove_volatile;
  template <class T> struct remove_cv;
  template <class T> struct add_const;
  template <class T> struct add_volatile;
  template <class T> struct add_cv;

  template <class T>
  using remove_const_t = typename remove_const<T>::type;
  template <class T>
  using remove_volatile_t = typename remove_volatile<T>::type;
  template <class T>
  using remove_cv_t = typename remove_cv<T>::type;
  template <class T>
  using add_const_t = typename add_const<T>::type;
  template <class T>
  using add_volatile_t = typename add_volatile<T>::type;
  template <class T>
  using add_cv_t = typename add_cv<T>::type;

  // 23.15.7.2, reference modifications
  template <class T> struct remove_reference;
  template <class T> struct add_lvalue_reference;
  template <class T> struct add_rvalue_reference;

  template <class T>
  using remove_reference_t = typename remove_reference<T>::type;
  template <class T>
  using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
  template <class T>
  using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

  // 23.15.7.3, sign modifications
  template <class T> struct make_signed;
  template <class T> struct make_unsigned;

  template <class T>
  using make_signed_t = typename make_signed<T>::type;
  template <class T>
  using make_unsigned_t = typename make_unsigned<T>::type;

  // 23.15.7.4, array modifications
  template <class T> struct remove_extent;
  template <class T> struct remove_all_extents;

  template <class T>
  using remove_extent_t = typename remove_extent<T>::type;
  template <class T>
  using remove_all_extents_t = typename remove_all_extents<T>::type;

  // 23.15.7.5, pointer modifications
  template <class T> struct remove_pointer;
  template <class T> struct add_pointer;

  template <class T>
  using remove_pointer_t = typename remove_pointer<T>::type;
  template <class T>
  using add_pointer_t = typename add_pointer<T>::type;

  // 23.15.7.6, other transformations
  template <size_t Len, size_t Align = default-alignment > // see 23.15.7.6
  struct aligned_storage;
  template <size_t Len, class... Types> struct aligned_union;
  template <class T> struct decay;
  template <bool, class T = void> struct enable_if;
  template <bool, class T, class F> struct conditional;
  template <class... T> struct common_type;
  template <class T> struct underlying_type;
  template <class Fn, class... ArgTypes> struct invoke_result;

  template <size_t Len, size_t Align = default-alignment > // see 23.15.7.6
  using aligned_storage_t = typename aligned_storage<Len, Align>::type;
  template <size_t Len, class... Types>
  using aligned_union_t = typename aligned_union<Len, Types...>::type;
  template <class T>
  using decay_t = typename decay<T>::type;
  template <bool B, class T = void>
  using enable_if_t = typename enable_if<b, T>::type;
  template <bool B, class T, class F>
  using conditional_t = typename conditional<b, T, F>::type;
  template <class... T>
  using common_type_t = typename common_type<T...>::type;
  template <class T>
  using underlying_type_t = typename underlying_type<T>::type;
  template <class Fn, class... ArgTypes>
  using invoke_result_t = typename invoke_result<Fn, ArgTypes...>::type;
  template <class...>
  using void_t = void;

  // 23.15.8, logical operator traits
  template<class... B> struct conjunction;
  template<class... B> struct disjunction;
  template<class B> struct negation;

  // 23.15.9, endian
  enum class endian {
    little = see below,
    big = see below,
    native = see below
  };

  // 23.15.4.1, primary type categories
  template <class T> inline constexpr bool is_void_v
    = is_void<T>::value;
  template <class T> inline constexpr bool is_null_pointer_v
    = is_null_pointer<T>::value;
  template <class T> inline constexpr bool is_integral_v
    = is_integral<T>::value;
  template <class T> inline constexpr bool is_floating_point_v
    = is_floating_point<T>::value;
  template <class T> inline constexpr bool is_array_v
    = is_array<T>::value;
  template <class T> inline constexpr bool is_pointer_v
    = is_pointer<T>::value;
  template <class T> inline constexpr bool is_lvalue_reference_v
    = is_lvalue_reference<T>::value;
  template <class T> inline constexpr bool is_rvalue_reference_v
    = is_rvalue_reference<T>::value;
  template <class T> inline constexpr bool is_member_object_pointer_v
    = is_member_object_pointer<T>::value;
  template <class T> inline constexpr bool is_member_function_pointer_v
    = is_member_function_pointer<T>::value;
  template <class T> inline constexpr bool is_enum_v
    = is_enum<T>::value;
  template <class T> inline constexpr bool is_union_v
    = is_union<T>::value;
  template <class T> inline constexpr bool is_class_v
    = is_class<T>::value;
  template <class T> inline constexpr bool is_function_v
    = is_function<T>::value;

  // 23.15.4.2, composite type categories
  template <class T> inline constexpr bool is_reference_v
    = is_reference<T>::value;
  template <class T> inline constexpr bool is_arithmetic_v
    = is_arithmetic<T>::value;
  template <class T> inline constexpr bool is_fundamental_v
    = is_fundamental<T>::value;
  template <class T> inline constexpr bool is_object_v
    = is_object<T>::value;
  template <class T> inline constexpr bool is_scalar_v
    = is_scalar<T>::value;
  template <class T> inline constexpr bool is_compound_v
    = is_compound<T>::value;
  template <class T> inline constexpr bool is_member_pointer_v
    = is_member_pointer<T>::value;

  // 23.15.4.3, type properties
  template <class T> inline constexpr bool is_const_v
    = is_const<T>::value;
  template <class T> inline constexpr bool is_volatile_v
    = is_volatile<T>::value;
  template <class T> inline constexpr bool is_trivial_v
    = is_trivial<T>::value;
  template <class T> inline constexpr bool is_trivially_copyable_v
    = is_trivially_copyable<T>::value;
  template <class T> inline constexpr bool is_standard_layout_v
    = is_standard_layout<T>::value;
  template <class T> inline constexpr bool is_empty_v
    = is_empty<T>::value;
  template <class T> inline constexpr bool is_polymorphic_v
    = is_polymorphic<T>::value;
  template <class T> inline constexpr bool is_abstract_v
    = is_abstract<T>::value;
  template <class T> inline constexpr bool is_final_v
    = is_final<T>::value;
  template <class T> inline constexpr bool is_aggregate_v
    = is_aggregate<T>::value;
  template <class T> inline constexpr bool is_signed_v
    = is_signed<T>::value;
  template <class T> inline constexpr bool is_unsigned_v
    = is_unsigned<T>::value;
  template <class T, class... Args> inline constexpr bool is_constructible_v
    = is_constructible<T, Args...>::value;
  template <class T> inline constexpr bool is_default_constructible_v
    = is_default_constructible<T>::value;

  template <class T> inline constexpr bool is_copy_constructible_v
    = is_copy_constructible<T>::value;
  template <class T> inline constexpr bool is_move_constructible_v
    = is_move_constructible<T>::value;
  template <class T, class U> inline constexpr bool is_assignable_v
    = is_assignable<T, U>::value;
  template <class T> inline constexpr bool is_copy_assignable_v
    = is_copy_assignable<T>::value;
  template <class T> inline constexpr bool is_move_assignable_v
    = is_move_assignable<T>::value;
  template <class T, class U> inline constexpr bool is_swappable_with_v
    = is_swappable_with<T, U>::value;
  template <class T> inline constexpr bool is_swappable_v
    = is_swappable<T>::value;
  template <class T> inline constexpr bool is_destructible_v
    = is_destructible<T>::value;
  template <class T, class... Args>
  inline constexpr bool is_trivially_constructible_v
    = is_trivially_constructible<T, Args...>::value;
  template <class T> inline constexpr bool is_trivially_default_constructible_v
    = is_trivially_default_constructible<T>::value;
  template <class T> inline constexpr bool is_trivially_copy_constructible_v
    = is_trivially_copy_constructible<T>::value;
  template <class T> inline constexpr bool is_trivially_move_constructible_v
    = is_trivially_move_constructible<T>::value;
  template <class T, class U> inline constexpr bool is_trivially_assignable_v
    = is_trivially_assignable<T, U>::value;
  template <class T> inline constexpr bool is_trivially_copy_assignable_v
    = is_trivially_copy_assignable<T>::value;
  template <class T> inline constexpr bool is_trivially_move_assignable_v
    = is_trivially_move_assignable<T>::value;
  template <class T> inline constexpr bool is_trivially_destructible_v
    = is_trivially_destructible<T>::value;
  template <class T, class... Args>
  inline constexpr bool is_nothrow_constructible_v
    = is_nothrow_constructible<T, Args...>::value;
  template <class T> inline constexpr bool is_nothrow_default_constructible_v
    = is_nothrow_default_constructible<T>::value;
  template <class T> inline constexpr bool is_nothrow_copy_constructible_v
    = is_nothrow_copy_constructible<T>::value;
  template <class T> inline constexpr bool is_nothrow_move_constructible_v
    = is_nothrow_move_constructible<T>::value;
  template <class T, class U> inline constexpr bool is_nothrow_assignable_v
    = is_nothrow_assignable<T, U>::value;
  template <class T> inline constexpr bool is_nothrow_copy_assignable_v
    = is_nothrow_copy_assignable<T>::value;
  template <class T> inline constexpr bool is_nothrow_move_assignable_v
    = is_nothrow_move_assignable<T>::value;
  template <class T, class U> inline constexpr bool is_nothrow_swappable_with_v
    = is_nothrow_swappable_with<T, U>::value;
  template <class T> inline constexpr bool is_nothrow_swappable_v
    = is_nothrow_swappable<T>::value;
  template <class T> inline constexpr bool is_nothrow_destructible_v
    = is_nothrow_destructible<T>::value;
  template <class T> inline constexpr bool has_virtual_destructor_v
    = has_virtual_destructor<T>::value;
  template <class T> inline constexpr bool has_unique_object_representations_v
    = has_unique_object_representations<T>::value;

  // 23.15.5, type property queries
  template <class T> inline constexpr size_t alignment_of_v
    = alignment_of<T>::value;
  template <class T> inline constexpr size_t rank_v
    = rank<T>::value;
  template <class T, unsigned I = 0> inline constexpr size_t extent_v
    = extent<T, I>::value;
  // 23.15.6, type relations
  template <class T, class U> inline constexpr bool is_same_v
    = is_same<T, U>::value;
  template <class Base, class Derived> inline constexpr bool is_base_of_v
    = is_base_of<Base, Derived>::value;
  template <class From, class To> inline constexpr bool is_convertible_v
    = is_convertible<From, To>::value;
  template <class Fn, class... ArgTypes> inline constexpr bool is_invocable_v
    = is_invocable<Fn, ArgTypes...>::value;
  template <class R, class Fn, class... ArgTypes>
  inline constexpr bool is_invocable_r_v
    = is_invocable_r<R, Fn, ArgTypes...>::value;
  template <class Fn, class... ArgTypes>
  inline constexpr bool is_nothrow_invocable_v
    = is_nothrow_invocable<Fn, ArgTypes...>::value;
  template <class R, class Fn, class... ArgTypes>
  inline constexpr bool is_nothrow_invocable_r_v
    = is_nothrow_invocable_r<R, Fn, ArgTypes...>::value;

  // 23.15.8, logical operator traits
  template<class... B>
  inline constexpr bool conjunction_v = conjunction<B...>::value;
  template<class... B>
  inline constexpr bool disjunction_v = disjunction<B...>::value;
  template<class B>
  inline constexpr bool negation_v = negation<B>::value;
}

*/

#include <cstddef>
#include <type_traits>

#include "detail/config.hpp"

namespace slb {

// 23.15.3, helper class

#if __cpp_lib_integral_constant_callable // C++14
using std::integral_constant;
#else
template <typename T, T v>
struct integral_constant : std::integral_constant<T, v> {
  using type = integral_constant;

  // C++14
  inline constexpr value_type operator()() const noexcept { return value; }
};
#endif

#if __cpp_lib_integral_constant_callable && __cpp_lib_bool_constant // C++17
using std::bool_constant;
#else
template <bool B>
using bool_constant = slb::integral_constant<bool, B>;
#endif

#if __cpp_lib_integral_constant_callable // C++14
using std::true_type;
using std::false_type;
#else
using true_type = slb::bool_constant<true>;
using false_type = slb::bool_constant<false>;
#endif

// 23.15.4.1, primary type categories

#if __cpp_lib_integral_constant_callable // C++14
using std::is_void;
using std::is_integral;
using std::is_floating_point;
using std::is_array;
using std::is_pointer;
using std::is_lvalue_reference;
using std::is_rvalue_reference;
using std::is_member_object_pointer;
using std::is_member_function_pointer;
using std::is_enum;
using std::is_union;
using std::is_class;
using std::is_function;
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
struct is_member_object_pointer
    : slb::bool_constant<std::is_member_object_pointer<T>::value> {};

template <typename T>
struct is_member_function_pointer
    : slb::bool_constant<std::is_member_function_pointer<T>::value> {};

template <typename T>
struct is_enum : slb::bool_constant<std::is_enum<T>::value> {};

template <typename T>
struct is_union : slb::bool_constant<std::is_union<T>::value> {};

template <typename T>
struct is_class : slb::bool_constant<std::is_class<T>::value> {};

template <typename T>
struct is_function : slb::bool_constant<std::is_function<T>::value> {};
#endif

#if __cpp_lib_integral_constant_callable && __cpp_lib_is_null_pointer // C++14
using std::is_null_pointer;
#else
template <typename T>
struct is_null_pointer
    : slb::bool_constant<std::is_same<
          std::nullptr_t, typename std::remove_cv<T>::type>::value> {};
#endif

// 23.15.4.2, composite type categories

#if __cpp_lib_integral_constant_callable // C++14
using std::is_reference;
using std::is_arithmetic;
using std::is_fundamental;
using std::is_object;
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
struct is_object : slb::bool_constant<std::is_object<T>::value> {};

template <typename T>
struct is_scalar : slb::bool_constant<std::is_scalar<T>::value> {};

template <typename T>
struct is_compound : slb::bool_constant<std::is_compound<T>::value> {};

template <typename T>
struct is_member_pointer
    : slb::bool_constant<std::is_member_pointer<T>::value> {};
#endif

// 23.15.4.3, type properties

#if __cpp_lib_integral_constant_callable // C++14
using std::is_const;
using std::is_volatile;
using std::is_trivial;
using std::is_trivially_copyable;
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
struct is_trivially_copyable
    : slb::bool_constant<std::is_trivially_copyable<T>::value> {};

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

#if __cpp_lib_integral_constant_callable && __cpp_lib_is_final // C++14
using std::is_final;
#elif __has_feature(is_final) || (__GNUC__ > 4) ||                             \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))
template <typename T>
struct is_final : slb::bool_constant<__is_final(T)> {};
#else
template <typename T>
struct is_final {
  static_assert(false_v<T>, "`is_final` is not available.");
};
#endif

#if __cpp_lib_integral_constant_callable && __cpp_lib_is_aggregate // C++17
using std::is_aggregate;
#elif __has_feature(is_aggregate) || (__GNUC__ >= 7)
template <typename T>
struct is_aggregate : slb::bool_constant<__is_aggregate(T)> {};
#else
template <typename T>
struct is_aggregate {
  static_assert(false_v<T>, "`is_aggregate` is not available.");
}
#endif

#if __cpp_lib_integral_constant_callable // C++14
using is_signed;
using is_unsigned;

using is_constructible;
using is_default_constructible;
using is_copy_constructible;
using is_move_constructible;

using is_trivially_constructible;
using is_trivially_default_constructible;
using is_trivially_copy_constructible;
using is_trivially_move_constructible;

using is_nothrow_constructible;
using is_nothrow_default_constructible;
using is_nothrow_copy_constructible;
using is_nothrow_move_constructible;

using is_destructible;
using is_trivially_destructible;
using is_nothrow_destructible;

using is_assignable;
using is_copy_assignable;
using is_move_assignable;

using is_trivially_assignable;
using is_trivially_copy_assignable;
using is_trivially_move_assignable;

using is_nothrow_assignable;
using is_nothrow_copy_assignable;
using is_nothrow_move_assignable;
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
struct is_trivially_assignable
    : slb::bool_constant<std::is_trivially_assignable<T, U>::value> {};

template <typename T>
struct is_trivially_copy_assignable
    : slb::bool_constant<std::is_trivially_copy_assignable<T>::value> {};

template <typename T>
struct is_trivially_move_assignable
    : slb::bool_constant<std::is_trivially_move_assignable<T>::value> {};

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

#if __cpp_lib_integral_constant_callable && __cpp_lib_is_swappable // C++17
using is_swappable_with;
using is_swappable;
using is_nothrow_swappable_with;
using is_nothrow_swappable;
#else
// TODO
// template <typename T, typename U> struct is_swappable_with;
// template <typename T> struct is_swappable;
// template <typename T, typename U> struct is_nothrow_swappable_with;
// template <typename T> struct is_nothrow_swappable;
#endif

#if __cpp_lib_integral_constant_callable // C++14
using std::has_virtual_destructor;
#else
template <typename T>
struct has_virtual_destructor
    : slb::bool_constant<std::has_virtual_destructor<T>::value> {};
#endif

#if __cpp_lib_integral_constant_callable &&                                    \
    __cpp_lib_has_unique_object_representations // C++17
using std::has_unique_object_representations;
#elif __has_feature(has_unique_object_representations) || (__GNUC__ >= 7)
template <typename T>
struct has_unique_object_representations
    : slb::bool_constant<__has_unique_object_representations(T)> {};
#else
template <typename T>
struct has_unique_object_representations {
  static_assert(false_v<T>,
                "`has_unique_object_representations` is not available.");
};
#endif

// 23.15.5, type property queries

#if __cpp_lib_integral_constant_callable // C++14
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
struct extent : slb::integral_constant<std::size_t, std::extent<T>::value> {};
#endif

// 23.15.6, type relations

#if __cpp_lib_integral_constant_callable // C++14
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

#if __cpp_lib_integral_constant_callable && __cpp_lib_is_invocable // C++17
using std::is_invocable;
using std::is_invocable_r;
using std::is_nothrow_invocable;
using std::is_nothrow_invocable_r;
#else
// TODO

// template <typename F, typename... Args>
// struct is_invocable;

// template <typename R, typename F, typename... Args>
// struct is_invocable_r;

// template <typename F, typename... Args>
// struct is_nothrow_invocable;

// template <typename R, typename F, typename... Args>
// struct is_nothrow_invocable_r;
#endif

// 23.15.7.1, const-volatile modifications

using std::remove_const;
using std::remove_volatile;
using std::remove_cv;
using std::add_const;
using std::add_volatile;
using std::add_cv;

#if __cpp_lib_transformation_trait_aliases // C++14
using std::remove_const_t;
using std::remove_volatile_t;
using std::remove_cv_t;
using std::add_const_t;
using std::add_volatile_t;
using std::add_cv_t;
#else
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
#endif

// 23.15.7.2, reference modifications

using std::remove_reference;
using std::add_lvalue_reference;
using std::add_rvalue_reference;

#if __cpp_lib_transformation_trait_aliases // C++14
using std::remove_reference_t;
using std::add_lvalue_reference_t;
using std::add_rvalue_reference_t;
#else
template <typename T>
using remove_reference_t = typename slb::remove_reference<T>::type;

template <typename T>
using add_lvalue_reference_t = typename slb::add_lvalue_reference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename slb::add_rvalue_reference<T>::type;
#endif

// 23.15.7.3, sign modifications

using std::make_signed;
using std::make_unsigned;

#if __cpp_lib_transformation_trait_aliases // C++14
using std::make_signed_t;
using std::make_unsigned_t;
#else
template <typename T>
using make_signed_t = typename slb::make_signed<T>::type;

template <typename T>
using make_unsigned_t = typename slb::make_unsigned<T>::type;
#endif

// 23.15.7.4, array modifications

using std::remove_extent;
using std::remove_all_extents;

#if __cpp_lib_transformation_trait_aliases // C++14
using std::remove_extent_t;
using std::remove_all_extents_t;
#else
template <typename T>
using remove_extent_t = typename slb::remove_extent<T>::type;

template <typename T>
using remove_all_extents_t = typename slb::remove_all_extents<T>::type;
#endif

// 23.15.7.5, pointer modifications

using std::remove_pointer;
using std::add_pointer;

#if __cpp_lib_transformation_trait_aliases // C++14
using std::remove_pointer_t;
using std::add_pointer_t;
#else
template <typename T>
using remove_pointer_t = typename slb::remove_pointer<T>::type;

template <typename T>
using add_pointer_t = typename slb::add_pointer<T>::type;
#endif

// 23.15.7.6, other transformations

using std::aligned_storage;
using std::aligned_union;
using std::decay;
using std::enable_if;
using std::conditional;

// TODO: C++14 (SFINAE) / C++17 (recursive commutative `std::decay_t`)
using std::common_type;

using std::underlying_type;

#if __cpp_lib_transformation_trait_aliases // C++14
using std::aligned_storage_t;
using std::aligned_union_t;
using std::decay_t;
using std::enable_if_t;
using std::conditional_t;
using std::common_type_t;
using std::underlying_type_t;
#else
namespace detail {

template <typename>
struct default_alignment;

template <std::size_t Len, std::size_t Align>
struct default_alignment<slb::aligned_storage<Len, Align>>
    : slb::integral_constant<std::size_t, Align> {};

} // namespace detail

template <std::size_t Len,
          std::size_t Align =
              slb::detail::default_alignment<slb::aligned_storage<Len>>::value>
using aligned_storage_t = typename slb::aligned_storage<Len, Align>::type;

template <std::size_t Len, typename... Ts>
using aligned_union_t = typename slb::aligned_union<Len, Ts...>::type;

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
#endif

#if __cpp_lib_is_invocable // C++17
using std::invoke_result;
using std::invoke_result_t;
#else
// TODO
// template <typename F, typename... Args> struct invoke_result;

template <typename F, typename... Args>
using invoke_result_t = typename slb::invoke_result<F, Args...>::type;
#endif

#if __cpp_lib_void_t // C++17
using std::void_t;
#else
template <typename... Ts>
using void_t = void;
#endif

// 23.15.8, logical operator traits

#if __cpp_lib_logical_traits // C++17
using std::conjunction;
using std::disjunction;
using std::negation;
#else
// TODO
// template <typename... Bs> struct conjunction;
// template <typename... Bs> struct disjunction;
// template <typename B> struct negation;
#endif

// 23.15.9, endian

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
