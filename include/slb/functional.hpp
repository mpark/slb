/*
  SLB.Functional

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_FUNCTIONAL_HPP
#define SLB_FUNCTIONAL_HPP

/*

Header <functional> synopsis [functional.syn]

namespace std {
  // [func.invoke], invoke
  template <class F, class... Args>
    invoke_result_t<F, Args...> invoke(F&& f, Args&&... args)
      noexcept(is_nothrow_invocable_v<F, Args...>);

  // [refwrap], reference_wrapper
  template <class T> class reference_wrapper;

  template <class T> reference_wrapper<T> ref(T&) noexcept;
  template <class T> reference_wrapper<const T> cref(const T&) noexcept;
  template <class T> void ref(const T&&) = delete;
  template <class T> void cref(const T&&) = delete;

  template <class T> reference_wrapper<T> ref(reference_wrapper<T>) noexcept;
  template <class T> reference_wrapper<const T> cref(reference_wrapper<T>)
      noexcept;

  // [arithmetic.operations], arithmetic operations
  template <class T = void> struct plus;
  template <class T = void> struct minus;
  template <class T = void> struct multiplies;
  template <class T = void> struct divides;
  template <class T = void> struct modulus;
  template <class T = void> struct negate;
  template <> struct plus<void>;
  template <> struct minus<void>;
  template <> struct multiplies<void>;
  template <> struct divides<void>;
  template <> struct modulus<void>;
  template <> struct negate<void>;

  // [comparisons], comparisons
  template <class T = void> struct equal_to;
  template <class T = void> struct not_equal_to;
  template <class T = void> struct greater;
  template <class T = void> struct less;
  template <class T = void> struct greater_equal;
  template <class T = void> struct less_equal;
  template <> struct equal_to<void>;
  template <> struct not_equal_to<void>;
  template <> struct greater<void>;
  template <> struct less<void>;
  template <> struct greater_equal<void>;
  template <> struct less_equal<void>;

  // [logical.operations], logical operations
  template <class T = void> struct logical_and;
  template <class T = void> struct logical_or;
  template <class T = void> struct logical_not;
  template <> struct logical_and<void>;
  template <> struct logical_or<void>;
  template <> struct logical_not<void>;

  // [bitwise.operations], bitwise operations
  template <class T = void> struct bit_and;
  template <class T = void> struct bit_or;
  template <class T = void> struct bit_xor;
  template <class T = void> struct bit_not;
  template <> struct bit_and<void>;
  template <> struct bit_or<void>;
  template <> struct bit_xor<void>;
  template <> struct bit_not<void>;

  // [func.not_fn], function template not_fn
  template <class F> unspecified not_fn(F&& f);

  // [func.bind], bind
  template<class T> struct is_bind_expression;
  template<class T> struct is_placeholder;

  template<class F, class... BoundArgs>
    unspecified bind(F&&, BoundArgs&&...);
  template<class R, class F, class... BoundArgs>
    unspecified bind(F&&, BoundArgs&&...);

  namespace placeholders {
    // M is the implementation-defined number of placeholders
    see below _1;
    see below _2;
    ...
    see below _M;
  }

  // [func.memfn], member function adaptors
  template<class R, class T>
    unspecified mem_fn(R T::*) noexcept;

  // [func.wrap], polymorphic function wrappers
  class bad_function_call;

  template<class> class function; // not defined
  template<class R, class... ArgTypes> class function<R(ArgTypes...)>;

  template<class R, class... ArgTypes>
    void swap(function<R(ArgTypes...)>&, function<R(ArgTypes...)>&) noexcept;

  template<class R, class... ArgTypes>
    bool operator==(const function<R(ArgTypes...)>&, nullptr_t) noexcept;
  template<class R, class... ArgTypes>
    bool operator==(nullptr_t, const function<R(ArgTypes...)>&) noexcept;
  template<class R, class... ArgTypes>
    bool operator!=(const function<R(ArgTypes...)>&, nullptr_t) noexcept;
  template<class R, class... ArgTypes>
    bool operator!=(nullptr_t, const function<R(ArgTypes...)>&) noexcept;

  // [func.search], searchers
  template<class ForwardIterator, class BinaryPredicate = equal_to<>>
    class default_searcher;

  template<class RandomAccessIterator,
           class Hash = hash<
               typename iterator_traits<RandomAccessIterator>::value_type>,
           class BinaryPredicate = equal_to<>>
    class boyer_moore_searcher;

  template<class RandomAccessIterator,
           class Hash = hash<
               typename iterator_traits<RandomAccessIterator>::value_type>,
           class BinaryPredicate = equal_to<>>
    class boyer_moore_horspool_searcher;

  // [unord.hash], hash function primary template
  template <class T>
    struct hash;

  // [func.bind], function object binders
  template <class T>
    inline constexpr bool is_bind_expression_v = is_bind_expression<T>::value;
  template <class T>
    inline constexpr int is_placeholder_v = is_placeholder<T>::value;
}

*/

#include <functional>
#include <type_traits>
#include <utility>

#include "detail/config.hpp"
#include "detail/invoke.hpp"
#include "type_traits.hpp"

namespace slb {

// [func.invoke], invoke

// We only enable the C++17 implementation under C++2a here to account for
// P0704: "Fixing const-qualified pointers to members".

#if __cpp_lib_invoke /* C++17 */ && __cplusplus > 201703L /* C++2a */
using std::invoke;
#else
template <typename F, typename... Args>
typename slb::invoke_result<F, Args...>::type
invoke(F&& f,
       Args&&... args) noexcept(slb::is_nothrow_invocable<F, Args...>::value) {
  return detail::invoke(std::forward<F>(f), std::forward<Args>(args)...);
}
#endif

// [func.not_fn], function template not_fn

// We only enable the C++17 implementation under C++2a here to account for
// P0704: "Fixing const-qualified pointers to members" and LWG2210: "INVOKE-ing
// a pointer to member with a `reference_wrapper` as the object expression".

#if __cpp_lib_not_fn /* C++17 */ && __cplusplus > 201703L /* C++2a */
using std::not_fn;
#else
namespace detail {
struct not_fn_tag {
  explicit not_fn_tag() = default;
};

template <typename FD>
class not_fn_result {
  FD fd;

public:
  template <typename F>
  not_fn_result(not_fn_tag, F&& f) : fd(std::forward<F>(f)) {}

  not_fn_result(not_fn_result&&) = default;
  not_fn_result(not_fn_result const&) = default;

  template <typename... Args>
      auto operator()(Args&&... args) &
      noexcept(noexcept(!detail::invoke(std::declval<FD&>(),
                                        std::forward<Args>(args)...)))
          -> decltype(!detail::invoke(std::declval<FD&>(),
                                      std::forward<Args>(args)...)) {
    return !detail::invoke(fd, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto operator()(Args&&... args) const& noexcept(noexcept(
      !detail::invoke(std::declval<FD const&>(), std::forward<Args>(args)...)))
      -> decltype(!detail::invoke(std::declval<FD const&>(),
                                  std::forward<Args>(args)...)) {
    return !detail::invoke(fd, std::forward<Args>(args)...);
  }

  template <typename... Args>
      auto operator()(Args&&... args) &&
      noexcept(noexcept(!detail::invoke(std::declval<FD&&>(),
                                        std::forward<Args>(args)...)))
          -> decltype(!detail::invoke(std::declval<FD&&>(),
                                      std::forward<Args>(args)...)) {
    return !detail::invoke(std::move(fd), std::forward<Args>(args)...);
  }

// gcc finds calls on const rvalues ambiguous up to version 4.8.
#if !defined(__GNUC__) || (__GNUC__ > 4) ||                                    \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))
  template <typename... Args>
  auto operator()(Args&&... args) const&& noexcept(noexcept(
      !detail::invoke(std::declval<FD const&&>(), std::forward<Args>(args)...)))
      -> decltype(!detail::invoke(std::declval<FD const&&>(),
                                  std::forward<Args>(args)...)) {
    return !detail::invoke(std::move(fd), std::forward<Args>(args)...);
  }
#endif
};
} // namespace detail

template <typename F, typename FD = typename std::decay<F>::type>
detail::not_fn_result<FD> not_fn(F&& f) {
  static_assert(std::is_move_constructible<FD>::value &&
                    std::is_convertible<FD&&, FD>::value,
                "FD shall satisfy the requirements of MoveConstructible");
  static_assert(std::is_constructible<FD, F>::value,
                "is_constructible_v<FD, F> shall be true");
  return {detail::not_fn_tag{}, std::forward<F>(f)};
}
#endif

// [func.memfn], member function adaptors

// We only enable the C++17 implementation under C++2a here to account for
// P0704: "Fixing const-qualified pointers to members" and LWG2210: "INVOKE-ing
// a pointer to member with a `reference_wrapper` as the object expression".

#if __cpp_lib_invoke /* C++17 */ && __cplusplus > 201703L /* C++2a */
using std::mem_fn;
#else
template <typename T, typename C>
typename detail::mem_fn_result<T C::*>::type mem_fn(T C::*pm) noexcept {
  return pm;
}
#endif

} // namespace slb

#endif // SLB_FUNCTIONAL_HPP
