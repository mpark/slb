/*
  SLB.TypeTraits

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_DETAIL_LIB_HPP
#define SLB_DETAIL_LIB_HPP

#include <type_traits>

namespace slb {
namespace detail {
namespace lib {

template <typename>
struct always_false : std::false_type {};

////////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4180) // qualifier applied to function type has no
                                // meaning; ignored
#endif
template <typename T>
struct is_function : std::integral_constant<bool,
                                            !std::is_const<T const>::value &&
                                                !std::is_reference<T>::value> {
};
#ifdef _MSC_VER
#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////
template <typename T>
constexpr T(min)(T a, T b) {
  return a < b ? a : b;
}
template <typename T, typename... Ts>
constexpr T(min)(T a, T b, Ts... cs) {
  return (min)((min)(a, b), cs...);
}

template <typename T>
constexpr T(max)(T a, T b) {
  return a < b ? b : a;
}
template <typename T, typename... Ts>
constexpr T(max)(T a, T b, Ts... cs) {
  return (max)((max)(a, b), cs...);
}

////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct remove_cvref {
  using type =
      typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

} // namespace lib
} // namespace detail
} // namespace slb

#endif // SLB_DETAIL_LIB_HPP
