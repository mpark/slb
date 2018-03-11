/*
  SLB.Utility

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_UTILITY_HPP
#define SLB_UTILITY_HPP

/*

Header <utility> synopsis [utility.syn]

#include <initializer_list>     // see [initializer_list.syn]

namespace std {
  // [utility.swap], swap
  template<class T>
    void swap(T& a, T& b) noexcept(see below);
  template<class T, size_t N>
    void swap(T (&a)[N], T (&b)[N]) noexcept(is_nothrow_swappable_v<T>);

  // [utility.exchange], exchange
  template<class T, class U = T>
    constexpr T exchange(T& obj, U&& new_val);

  // [forward], forward/move
  template<class T>
    constexpr T&& forward(remove_reference_t<T>& t) noexcept;
  template<class T>
    constexpr T&& forward(remove_reference_t<T>&& t) noexcept;
  template<class T>
    constexpr remove_reference_t<T>&& move(T&&) noexcept;
  template<class T>
    constexpr conditional_t<
        !is_nothrow_move_constructible_v<T> && is_copy_constructible_v<T>,
        const T&,
        T&&>
      move_if_noexcept(T& x) noexcept;

  // [utility.as_const], as_const
  template<class T>
    constexpr add_const_t<T>& as_const(T& t) noexcept;
  template<class T>
    void as_const(const T&&) = delete;

  // [declval], declval
  template<class T>
    add_rvalue_reference_t<T> declval() noexcept;  // as unevaluated operand

  // [intseq], Compile-time integer sequences
  template<class T, T...>
    struct integer_sequence;
  template<size_t... I>
    using index_sequence = integer_sequence<size_t, I...>;

  template<class T, T N>
    using make_integer_sequence = integer_sequence<T, see below>;
  template<size_t N>
    using make_index_sequence = make_integer_sequence<size_t, N>;

  template<class... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;

  // [pairs], class template pair
  template<class T1, class T2>
    struct pair;

  // [pairs.spec], pair specialized algorithms
  template<class T1, class T2>
    constexpr bool operator==(const pair<T1, T2>&, const pair<T1, T2>&);
  template<class T1, class T2>
    constexpr bool operator< (const pair<T1, T2>&, const pair<T1, T2>&);
  template<class T1, class T2>
    constexpr bool operator!=(const pair<T1, T2>&, const pair<T1, T2>&);
  template<class T1, class T2>
    constexpr bool operator> (const pair<T1, T2>&, const pair<T1, T2>&);
  template<class T1, class T2>
    constexpr bool operator>=(const pair<T1, T2>&, const pair<T1, T2>&);
  template<class T1, class T2>
    constexpr bool operator<=(const pair<T1, T2>&, const pair<T1, T2>&);

  template<class T1, class T2>
    void swap(pair<T1, T2>& x, pair<T1, T2>& y) noexcept(noexcept(x.swap(y)));

  template<class T1, class T2>
    constexpr see below make_pair(T1&&, T2&&);

  // [pair.astuple], tuple-like access to pair
  template<class T> class tuple_size;
  template<size_t I, class T> class tuple_element;

  template<class T1, class T2> struct tuple_size<pair<T1, T2>>;
  template<size_t I, class T1, class T2> struct tuple_element<I, pair<T1, T2>>;

  template<size_t I, class T1, class T2>
    constexpr tuple_element_t<I, pair<T1, T2>>& get(pair<T1, T2>&) noexcept;
  template<size_t I, class T1, class T2>
    constexpr tuple_element_t<I, pair<T1, T2>>&& get(pair<T1, T2>&&) noexcept;
  template<size_t I, class T1, class T2>
    constexpr const tuple_element_t<I, pair<T1, T2>>&
      get(const pair<T1, T2>&) noexcept;
  template<size_t I, class T1, class T2>
    constexpr const tuple_element_t<I, pair<T1, T2>>&&
      get(const pair<T1, T2>&&) noexcept;
  template<class T1, class T2>
    constexpr T1& get(pair<T1, T2>& p) noexcept;
  template<class T1, class T2>
    constexpr const T1& get(const pair<T1, T2>& p) noexcept;
  template<class T1, class T2>
    constexpr T1&& get(pair<T1, T2>&& p) noexcept;
  template<class T1, class T2>
    constexpr const T1&& get(const pair<T1, T2>&& p) noexcept;
  template<class T2, class T1>
    constexpr T2& get(pair<T1, T2>& p) noexcept;
  template<class T2, class T1>
    constexpr const T2& get(const pair<T1, T2>& p) noexcept;
  template<class T2, class T1>
    constexpr T2&& get(pair<T1, T2>&& p) noexcept;
  template<class T2, class T1>
    constexpr const T2&& get(const pair<T1, T2>&& p) noexcept;

  // [pair.piecewise], pair piecewise construction
  struct piecewise_construct_t {
    explicit piecewise_construct_t() = default;
  };
  inline constexpr piecewise_construct_t piecewise_construct{};
  template<class... Types> class tuple;        // defined in <tuple>

  // in-place construction
  struct in_place_t {
    explicit in_place_t() = default;
  };
  inline constexpr in_place_t in_place{};
  template<class T>
    struct in_place_type_t {
      explicit in_place_type_t() = default;
    };
  template<class T> inline constexpr in_place_type_t<T> in_place_type{};
  template<size_t I>
    struct in_place_index_t {
      explicit in_place_index_t() = default;
    };
  template<size_t I> inline constexpr in_place_index_t<I> in_place_index{};
}

*/

#include <utility>

#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "detail/config.hpp"
#include "detail/lib.hpp"

namespace slb {

// [utility.exchange], exchange

template <typename T, typename U = T>
SLB_CXX14_CONSTEXPR T exchange(T& obj, U&& new_val) {
  T old_val = std::move(obj);
  obj = std::forward<U>(new_val);
  return old_val;
}

// [utility.as_const], as_const

template <typename T>
constexpr typename std::add_const<T>::type& as_const(T& t) noexcept {
  return t;
}

template <typename T>
void as_const(T const&&) = delete;

// [intseq], Compile-time integer sequences

#if __cpp_lib_integer_sequence ||                                              \
    (_LIBCPP_VERSION >= 1101 && _LIBCPP_STD_VER > 11) || defined(_MSC_VER)
// libstdc++ did not mark `size()` as `noexcept` until version 8.
#if defined(__GLIBCXX__) && __has_include(<filesystem>)
#define SLB_INTEGER_SEQUENCE 2 // available / conforming
#else
#define SLB_INTEGER_SEQUENCE 1 // available / non-conforming
#endif
#else
#define SLB_INTEGER_SEQUENCE 0 // not available
#endif

#if SLB_INTEGER_SEQUENCE == 2
using std::integer_sequence;
#else
template <typename T, T... Is>
struct integer_sequence
#if SLB_INTEGER_SEQUENCE == 1
    : std::integer_sequence<T, Is...>
#endif
{
  static_assert(std::is_integral<T>::value, "`T` shall be an integer type.");
  using value_type = T;
  static constexpr std::size_t size() noexcept { return sizeof...(Is); }
};
#endif

template <std::size_t... Is>
using index_sequence = slb::integer_sequence<std::size_t, Is...>;

#if __GNUC__ >= 8
template <typename T, T N>
using make_integer_sequence = slb::integer_sequence<T, __integer_pack(N)...>;

template <std::size_t N>
using make_index_sequence = slb::make_integer_sequence<std::size_t, N>;
#elif __has_builtin(__make_integer_seq) || defined(_MSC_VER)
template <typename T, T N>
using make_integer_sequence = __make_integer_seq<slb::integer_sequence, T, N>;

template <std::size_t N>
using make_index_sequence = slb::make_integer_sequence<std::size_t, N>;
#else
namespace detail {

template <typename Is, bool Even>
struct expand_index_sequence;

template <std::size_t... Is>
struct expand_index_sequence<slb::index_sequence<Is...>, true> {
  using type = slb::index_sequence<Is..., (sizeof...(Is) + Is)...>;
};

template <std::size_t... Is>
struct expand_index_sequence<slb::index_sequence<Is...>, false> {
  using type =
      slb::index_sequence<Is..., (sizeof...(Is) + Is)..., sizeof...(Is) * 2>;
};

template <std::size_t N>
struct make_index_sequence_impl
    : expand_index_sequence<typename make_index_sequence_impl<N / 2>::type,
                            N % 2 == 0> {
  static_assert(N >= 0, "`N` cannot be negative.");
};

template <>
struct make_index_sequence_impl<0> {
  using type = slb::index_sequence<>;
};

template <>
struct make_index_sequence_impl<1> {
  using type = slb::index_sequence<0>;
};

template <typename T, typename Is>
struct make_integer_sequence_impl;

template <typename T, std::size_t... Is>
struct make_integer_sequence_impl<T, slb::index_sequence<Is...>> {
  using type = slb::integer_sequence<T, static_cast<T>(Is)...>;
};

} // namespace detail

template <std::size_t N>
using make_index_sequence = typename detail::make_index_sequence_impl<N>::type;

template <typename T, T N>
using make_integer_sequence = typename detail::make_integer_sequence_impl<
    T,
    slb::make_index_sequence<static_cast<std::size_t>(N)>>::type;
#endif

template <typename... Ts>
using index_sequence_for = slb::make_index_sequence<sizeof...(Ts)>;

} // namespace slb

#endif // SLB_UTILITY_HPP
