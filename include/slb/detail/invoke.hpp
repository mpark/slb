/*
  SLB.Detail.Invoke

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_DETAIL_INVOKE_HPP
#define SLB_DETAIL_INVOKE_HPP

#include "config.hpp"
#include "lib.hpp"

#include <functional>
#include <type_traits>
#include <utility>

namespace slb {
namespace detail {
namespace invoke_impl {

////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct is_reference_wrapper : std::false_type {};

template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

////////////////////////////////////////////////////////////////////////////////
struct ref_tag {};
struct ref_wrapper_tag {};
struct ptr_tag {};

template <typename C, typename T, typename Enable = void>
struct dispatch_mem_ptr;

// `ref_tag` when `pm` is a pointer to member of a class `C` and
// `is_base_of_v<C, remove_reference_t<T>>` is `true`;
template <typename C, typename T>
struct dispatch_mem_ptr<
    C,
    T,
    typename std::enable_if<
        std::is_base_of<C, typename std::remove_reference<T>::type>::value>::
        type> {
  using type = ref_tag;
};

// `ref_wrapper_tag` when `pm` is a pointer to member of a class `C` and
// `remove_cvref_t<T>` is a specialization of `reference_wrapper`;
template <typename C, typename T>
struct dispatch_mem_ptr<
    C,
    T,
    typename std::enable_if<is_reference_wrapper<
        typename detail::lib::remove_cvref<T>::type>::value>::type> {
  using type = ref_wrapper_tag;
};

// `ptr_tag` when `pm` is a pointer to member of a class `C` and `T` does not
// satisfy the previous two items;
template <typename C, typename T, typename Enable>
struct dispatch_mem_ptr {
  using type = ptr_tag;
};

////////////////////////////////////////////////////////////////////////////////
template <typename Tag, typename T, typename Enable = void>
struct mem_obj_ptr_result {};

template <typename F, typename T1>
struct mem_obj_ptr_result<ref_tag,
                          F(T1),
                          decltype(
                              (void)(std::declval<T1>().*std::declval<F&>()))> {
  using type = decltype(std::declval<T1>().*std::declval<F&>());
};

template <typename F, typename T1>
struct mem_obj_ptr_result<ref_wrapper_tag,
                          F(T1),
                          decltype((void)(std::declval<T1>().get().*
                                          std::declval<F&>()))> {
  using type = decltype(std::declval<T1>().get().*std::declval<F&>());
};

template <typename F, typename T1>
struct mem_obj_ptr_result<ptr_tag,
                          F(T1),
                          decltype((void)((*std::declval<T1>()).*
                                          std::declval<F&>()))> {
  using type = decltype((*std::declval<T1>()).*std::declval<F&>());
};

////////////////////////////////////////////////////////////////////////////////
template <typename T>
class mem_obj_ptr;

template <typename T, typename C>
class mem_obj_ptr<T C::*> {
  T C::*pm;

private:
  // `t1.*pm` when `N == 1` and `pm` is a pointer to data member of a class `C`
  // and `is_base_of_v<C, remove_reference_t<decltype(t1)>>` is `true`;
  template <typename R, typename T1>
  static constexpr R call(ref_tag, T C::*pm, T1&& t1) noexcept {
    return std::forward<T1>(t1).*pm;
  }

  // `t1.get().*pm` when `N == 1` and `pm` is a pointer to data member of a
  // class `C` and `remove_cvref_t<decltype(t1)>` is a specialization of
  // `reference_wrapper`;
  template <typename R, typename T1>
  static constexpr R call(ref_wrapper_tag, T C::*pm, T1&& t1) noexcept {
    return t1.get().*pm;
  }

  // `(*t1).*pm` when `N == 1` and `pm` is a pointer to data member of a class
  // `C` and `t1` does not satisfy the previous two items;
  template <typename R, typename T1>
  static constexpr R
  call(ptr_tag, T C::*pm, T1&& t1) noexcept(noexcept(*std::forward<T1>(t1))) {
    return (*std::forward<T1>(t1)).*pm;
  }

public:
  constexpr mem_obj_ptr(T C::*pm) noexcept : pm(pm) {}

  template <typename T1,
            typename Tag = typename dispatch_mem_ptr<C, T1>::type,
            typename R = typename mem_obj_ptr_result<Tag, T C::*(T1&&)>::type>
  constexpr R operator()(T1&& t1) const
      noexcept(noexcept(call<R>(Tag{}, pm, std::forward<T1>(t1)))) {
    return call<R>(Tag{}, pm, std::forward<T1>(t1));
  }
};

////////////////////////////////////////////////////////////////////////////////
template <typename F, typename T1>
struct p0704_noexcept {
  using type = T1;
};
template <typename F, typename T1>
struct p0704 {
  using type = typename p0704_noexcept<F, T1>::type;
};

// P0704: Fixing const-qualified pointers to members
// In a .* expression whose object expression is an rvalue, the program is
// ill-formed if the second operand is a pointer to member function with whose
// ref-qualifier is &, unless its cv-qualifier-seq is const.
#if __cplusplus <= 201703L // C++2a
template <typename R, typename... Args, typename T1>
struct p0704_noexcept<R(Args...) const& noexcept, T1> {
  using type = T1&;
};
template <typename R, typename... Args, typename T1>
struct p0704_noexcept<R(Args..., ...) const& noexcept, T1> {
  using type = T1&;
};
template <typename R, typename... Args, typename T1>
struct p0704<R(Args...) const&, T1> {
  using type = T1&;
};
template <typename R, typename... Args, typename T1>
struct p0704<R(Args..., ...) const&, T1> {
  using type = T1&;
};
#endif

////////////////////////////////////////////////////////////////////////////////
template <typename C, typename F, typename T1>
struct apply_clang_workaround_noexcept {
  using type = void;
};
template <typename C, typename F, typename T1>
struct apply_clang_workaround : apply_clang_workaround_noexcept<C, F, T1> {};

// clang accepts invalid calls to non-const members on references to const
// derived class up to version 5.0.
#if defined(__clang__)
template <typename C, typename T1>
struct disable_if_const_derived
    : std::enable_if<
          !std::is_const<typename std::remove_reference<T1>::type>::value ||
          !std::is_base_of<C,
                           typename std::remove_reference<T1>::type>::value> {};

template <typename C, typename R, typename... Args, typename T1>
struct apply_clang_workaround_noexcept<C, R(Args...) noexcept, T1>
    : disable_if_const_derived<C, T1> {};
template <typename C, typename R, typename... Args, typename T1>
struct apply_clang_workaround_noexcept<C, R(Args..., ...) noexcept, T1>
    : disable_if_const_derived<C, T1> {};
template <typename C, typename R, typename... Args, typename T1>
struct apply_clang_workaround<C, R(Args...), T1>
    : disable_if_const_derived<C, T1> {};
template <typename C, typename R, typename... Args, typename T1>
struct apply_clang_workaround<C, R(Args..., ...), T1>
    : disable_if_const_derived<C, T1> {};
#endif

////////////////////////////////////////////////////////////////////////////////
template <typename Tag, typename T, typename Enable = void>
struct mem_fun_ptr_result {};

template <typename F, typename T1, typename... Tn>
struct mem_fun_ptr_result<ref_tag,
                          F(T1, Tn...),
                          decltype(
                              (void)((std::declval<T1>().*std::declval<F&>())(
                                  std::declval<Tn>()...)))> {
  using type =
      decltype((std::declval<T1>().*std::declval<F&>())(std::declval<Tn>()...));
};

template <typename F, typename T1, typename... Tn>
struct mem_fun_ptr_result<ref_wrapper_tag,
                          F(T1, Tn...),
                          decltype((void)((std::declval<T1>().get().*
                                           std::declval<F&>())(
                              std::declval<Tn>()...)))> {
  using type = decltype(
      (std::declval<T1>().get().*std::declval<F&>())(std::declval<Tn>()...));
};

template <typename F, typename T1, typename... Tn>
struct mem_fun_ptr_result<ptr_tag,
                          F(T1, Tn...),
                          decltype((
                              void)(((*std::declval<T1>()).*std::declval<F&>())(
                              std::declval<Tn>()...)))> {
  using type = decltype(
      ((*std::declval<T1>()).*std::declval<F&>())(std::declval<Tn>()...));
};

////////////////////////////////////////////////////////////////////////////////
template <typename T>
class mem_fun_ptr;

template <typename T, typename C>
class mem_fun_ptr<T C::*> {
  T C::*pm;

private:
  // `(t1.*pm)(t2, ..., tN)` when `pm` is a pointer to a member function of a
  // class `C` and `is_base_of_v<C, remove_reference_t<decltype(t1)>>` is
  // `true`;
  template <typename R, typename T1, typename... Tn>
  static constexpr R call(ref_tag, T C::*pm, T1&& t1, Tn&&... tn) noexcept(
      noexcept((std::forward<T1>(t1).*pm)(std::forward<Tn>(tn)...))) {
    return (std::forward<T1>(t1).*pm)(std::forward<Tn>(tn)...);
  }

  // `(t1.get().*pm)(t2, ..., tN)` when `pm` is a pointer to a member function
  // of a class `C` and `remove_cvref_t<decltype(t1)>` is a specialization of
  // `reference_wrapper`;
  template <typename R, typename T1, typename... Tn>
  static constexpr R
  call(ref_wrapper_tag,
       T C::*pm,
       T1&& t1,
       Tn&&... tn) noexcept(noexcept((t1.get().*pm)(std::forward<Tn>(tn)...))) {
    return (t1.get().*pm)(std::forward<Tn>(tn)...);
  }

  // `((*t1).*pm)(t2, ..., tN)` when `pm` is a pointer to a member function of a
  // class `C` and `t1` does not satisfy the previous two items;
  template <typename R, typename T1, typename... Tn>
  static constexpr R call(ptr_tag, T C::*pm, T1&& t1, Tn&&... tn) noexcept(
      noexcept(((*std::forward<T1>(t1)).*pm)(std::forward<Tn>(tn)...))) {
    return ((*std::forward<T1>(t1)).*pm)(std::forward<Tn>(tn)...);
  }

public:
  constexpr mem_fun_ptr(T C::*pm) noexcept : pm(pm) {}

  template <typename T1,
            typename... Tn,
            typename PT1 = typename p0704<T, T1>::type,
            typename Tag = typename dispatch_mem_ptr<C, PT1>::type,
            typename R =
                typename mem_fun_ptr_result<Tag, T C::*(PT1&&, Tn&&...)>::type,
            typename = typename apply_clang_workaround<C, T, T1>::type>
  constexpr R operator()(T1&& t1, Tn&&... tn) const noexcept(noexcept(
      call<R>(Tag{}, pm, std::forward<PT1>(t1), std::forward<Tn>(tn)...))) {
    return call<R>(Tag{}, pm, std::forward<PT1>(t1), std::forward<Tn>(tn)...);
  }
};

} // namespace invoke_impl

// We don't use `std::is_function<T>` here since some libstdc++ versions fail to
// classify function types with noexcept specifiers.

template <typename T>
struct mem_fn_result {};

template <typename T, typename C>
struct mem_fn_result<T C::*> {
  using type =
      typename std::conditional<detail::lib::is_function<T>::value,
                                invoke_impl::mem_fun_ptr<T C::*>,
                                invoke_impl::mem_obj_ptr<T C::*>>::type;
};

////////////////////////////////////////////////////////////////////////////////
namespace invoke_impl {

template <typename F, typename FD = typename detail::lib::remove_cvref<F>::type>
struct dispatch_invoke {
  using type = F&&;
};

template <typename F, typename T, typename C>
struct dispatch_invoke<F, T C::*> {
  using type = typename mem_fn_result<T C::*>::type;
};

} // namespace invoke_impl

template <typename F, typename... Args>
constexpr auto invoke(F&& f, Args&&... args) noexcept(
    noexcept(typename invoke_impl::dispatch_invoke<F>::type(std::forward<F>(f))(
        std::forward<Args>(args)...)))
    -> decltype(typename invoke_impl::dispatch_invoke<F>::type(
        std::forward<F>(f))(std::forward<Args>(args)...)) {
  return typename invoke_impl::dispatch_invoke<F>::type(std::forward<F>(f))(
      std::forward<Args>(args)...);
}

} // namespace detail
} // namespace slb

#endif // SLB_DETAIL_INVOKE_HPP
