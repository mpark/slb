/*
  SLB.Functional

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <slb/functional.hpp>

#include <functional>
#include <type_traits>
#include <utility>

#include "../catch.hpp"

#define CHECK_NESTED(...)                                                      \
  do {                                                                         \
    INFO(__FILE__ "(" << __LINE__ << "): " #__VA_ARGS__);                      \
    check_##__VA_ARGS__;                                                       \
  } while (false)

// [func.not_fn], function template not_fn

// template <class F> unspecified not_fn(F&& f);

enum lvalue : int {};
enum rvalue : int {};
enum const_lvalue : int {};
enum const_rvalue : int {};

template <typename T = int>
struct Boolean {
  int ret;
  Boolean(int val) : ret(val) {}

  T operator!() const noexcept { return static_cast<T>(ret); }
};

struct BooleanObj {
  int base;
  BooleanObj(int val) : base(val) {}

  int operator!() & noexcept { return base - 1; }
  int operator!() && noexcept { return base - 2; }
  int operator!() const & noexcept { return base - 3; }
// gcc finds calls on const rvalues ambiguous up to version 4.8.
#if !defined(__GNUC__) || (__GNUC__ > 4) ||                                    \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))
  int operator!() const && noexcept { return base - 4; }
#endif
};

// Account for P0012: "Make exception specifications be part of the type
// system".
static constexpr bool p0012 = !std::is_same<void(), void() noexcept>::value;

struct C {
  BooleanObj obj;
  C(int val) : obj(val) {}

  Boolean<> fun(int base) noexcept(p0012) { return {base + 0}; }
  Boolean<> cfun(int base) const noexcept(p0012) { return {base + 1}; }
  Boolean<> lfun(int base) & noexcept(p0012) { return {base + 2}; }
  Boolean<> rfun(int base) && noexcept(p0012) { return {base + 3}; }
  Boolean<> clfun(int base) const& noexcept(p0012) { return {base + 4}; }
// gcc finds calls on const rvalues ambiguous up to version 4.8.
#if !defined(__GNUC__) || (__GNUC__ > 4) ||                                    \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))
  Boolean<> crfun(int base) const&& noexcept(p0012) { return {base + 5}; }
#endif
};

struct D : C {
  D(int val) : C(val) {}
};

template <typename T, bool IsNothrow = true>
struct smart_ptr {
  T* ptr;
  smart_ptr(T* ptr) : ptr(ptr) {}
  T& operator*() const noexcept(IsNothrow) { return *ptr; }
};
template <typename T>
using smart_ptr_throws = smart_ptr<T, false>;

std::true_type const nothrows{};
std::false_type const throws{};
std::integral_constant<bool, p0012> const p0012_nothrows{};

template <typename R, bool IsNothrow, typename F, typename... Args>
void check_not_fn(R&& r,
                  std::integral_constant<bool, IsNothrow>,
                  F&& nf,
                  Args&&... args) {
  CHECK(std::forward<F>(nf)(std::forward<Args>(args)...) == r);
  CHECK(std::is_same<decltype(std::forward<F>(nf)(std::forward<Args>(args)...)),
                     R>::value);
  CHECK(noexcept(std::forward<F>(nf)(std::forward<Args>(args)...)) ==
        IsNothrow);
}

TEST_CASE("not_fn(mem-obj-ptr)", "[func.not_fn]") {
  auto nf = slb::not_fn(&C::obj);

  /* call wrapper */ {
    using call_wrapper = decltype(nf);

    CHECK(std::is_move_constructible<call_wrapper>::value);
    CHECK(std::is_copy_constructible<call_wrapper>::value);
    CHECK(!std::is_move_assignable<call_wrapper>::value);
    CHECK(!std::is_copy_assignable<call_wrapper>::value);

    CHECK(std::is_nothrow_move_constructible<call_wrapper>::value);
    CHECK(std::is_nothrow_copy_constructible<call_wrapper>::value);
  }

  /* reference */ {
    C x = {42};
    C& r = x;
    C const& cr = x;

    CHECK_NESTED(not_fn(!r.obj, nothrows, nf, r));
    CHECK_NESTED(not_fn(!cr.obj, nothrows, nf, cr));
    CHECK_NESTED(not_fn(!std::move(r.obj), nothrows, nf, std::move(r)));
    CHECK_NESTED(not_fn(!std::move(cr.obj), nothrows, nf, std::move(cr)));

    D d = {42};
    D& rd = d;
    D const& crd = d;

    CHECK_NESTED(not_fn(!rd.obj, nothrows, nf, rd));
    CHECK_NESTED(not_fn(!crd.obj, nothrows, nf, crd));
    CHECK_NESTED(not_fn(!std::move(rd.obj), nothrows, nf, std::move(rd)));
    CHECK_NESTED(not_fn(!std::move(crd.obj), nothrows, nf, std::move(crd)));
  }

  /* reference wrapper */ {
    C x = {42};
    std::reference_wrapper<C> r = x;
    std::reference_wrapper<C const> cr = x;

    CHECK_NESTED(not_fn(!r.get().obj, nothrows, nf, r));
    CHECK_NESTED(not_fn(!cr.get().obj, nothrows, nf, cr));
  }

  /* pointer */ {
    C x = {42};
    C* p = &x;
    C const* cp = &x;

    CHECK_NESTED(not_fn(!(*p).obj, nothrows, nf, p));
    CHECK_NESTED(not_fn(!(*cp).obj, nothrows, nf, cp));
  }

  /* smart pointer */ {
    C x = {42};
    smart_ptr<C> p = &x;
    smart_ptr<C const> cp = &x;

    CHECK_NESTED(not_fn(!(*p).obj, nothrows, nf, p));
    CHECK_NESTED(not_fn(!(*cp).obj, nothrows, nf, cp));

    smart_ptr_throws<C> tp = &x;
    smart_ptr_throws<C const> tcp = &x;

    CHECK_NESTED(not_fn(!(*tp).obj, throws, nf, tp));
    CHECK_NESTED(not_fn(!(*tcp).obj, throws, nf, tcp));
  }
}

TEST_CASE("not_fn(mem-fun-ptr)", "[func.not_fn]") {
  auto nf = slb::not_fn(&C::fun);

  /* call wrapper */ {
    using call_wrapper = decltype(nf);

    CHECK(std::is_move_constructible<call_wrapper>::value);
    CHECK(std::is_copy_constructible<call_wrapper>::value);
    CHECK(!std::is_move_assignable<call_wrapper>::value);
    CHECK(!std::is_copy_assignable<call_wrapper>::value);

    CHECK(std::is_nothrow_move_constructible<call_wrapper>::value);
    CHECK(std::is_nothrow_copy_constructible<call_wrapper>::value);
  }

  auto ncf = slb::not_fn(&C::cfun);
  auto nlf = slb::not_fn(&C::lfun);
  auto nrf = slb::not_fn(&C::rfun);
  auto nclf = slb::not_fn(&C::clfun);
// gcc finds calls on const rvalues ambiguous up to version 4.8.
#if !defined(__GNUC__) || (__GNUC__ > 4) ||                                    \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))
  auto ncrf = slb::not_fn(&C::crfun);
#endif

  /* reference */ {
    C x = {42};
    C& r = x;
    C const& cr = x;

    CHECK_NESTED(not_fn(!r.fun(40), p0012_nothrows, nf, r, 40));
    CHECK_NESTED(not_fn(!r.cfun(40), p0012_nothrows, ncf, r, 40));
    CHECK_NESTED(not_fn(!r.lfun(40), p0012_nothrows, nlf, r, 40));
    CHECK_NESTED(not_fn(!r.clfun(40), p0012_nothrows, nclf, r, 40));

    CHECK_NESTED(not_fn(!cr.cfun(40), p0012_nothrows, ncf, cr, 40));
    CHECK_NESTED(not_fn(!cr.clfun(40), p0012_nothrows, nclf, cr, 40));

    CHECK_NESTED(
        not_fn(!std::move(r).fun(40), p0012_nothrows, nf, std::move(r), 40));
    CHECK_NESTED(
        not_fn(!std::move(r).cfun(40), p0012_nothrows, ncf, std::move(r), 40));
    CHECK_NESTED(not_fn(
        !std::move(r).clfun(40), p0012_nothrows, nclf, std::move(r), 40));
    CHECK_NESTED(
        not_fn(!std::move(r).rfun(40), p0012_nothrows, nrf, std::move(r), 40));
// gcc finds calls on const rvalues ambiguous up to version 4.8.
#if !defined(__GNUC__) || (__GNUC__ > 4) ||                                    \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))
    CHECK_NESTED(not_fn(
        !std::move(r).crfun(40), p0012_nothrows, ncrf, std::move(r), 40));
#endif

    CHECK_NESTED(not_fn(
        !std::move(cr).cfun(40), p0012_nothrows, ncf, std::move(cr), 40));
    CHECK_NESTED(not_fn(
        !std::move(cr).clfun(40), p0012_nothrows, nclf, std::move(cr), 40));
// gcc finds calls on const rvalues ambiguous up to version 4.8.
#if !defined(__GNUC__) || (__GNUC__ > 4) ||                                    \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))
    CHECK_NESTED(not_fn(
        !std::move(cr).crfun(40), p0012_nothrows, ncrf, std::move(cr), 40));
#endif
  }

  /* reference wrapper */ {
    C x = {42};
    std::reference_wrapper<C> r = x;
    std::reference_wrapper<C const> cr = x;

    CHECK_NESTED(not_fn(!r.get().fun(40), p0012_nothrows, nf, r, 40));
    CHECK_NESTED(not_fn(!r.get().cfun(40), p0012_nothrows, ncf, r, 40));
    CHECK_NESTED(not_fn(!r.get().lfun(40), p0012_nothrows, nlf, r, 40));
    CHECK_NESTED(not_fn(!r.get().clfun(40), p0012_nothrows, nclf, r, 40));

    CHECK_NESTED(not_fn(!cr.get().cfun(40), p0012_nothrows, ncf, cr, 40));
    CHECK_NESTED(not_fn(!cr.get().clfun(40), p0012_nothrows, nclf, cr, 40));
  }

  /* pointer */ {
    C x = {42};
    C* p = &x;
    C const* cp = &x;

    CHECK_NESTED(not_fn(!(*p).fun(40), p0012_nothrows, nf, p, 40));
    CHECK_NESTED(not_fn(!(*p).cfun(40), p0012_nothrows, ncf, p, 40));
    CHECK_NESTED(not_fn(!(*p).lfun(40), p0012_nothrows, nlf, p, 40));
    CHECK_NESTED(not_fn(!(*p).clfun(40), p0012_nothrows, nclf, p, 40));

    CHECK_NESTED(not_fn(!(*cp).cfun(40), p0012_nothrows, ncf, cp, 40));
    CHECK_NESTED(not_fn(!(*cp).clfun(40), p0012_nothrows, nclf, cp, 40));
  }

  /* smart pointer */ {
    C x = {42};
    smart_ptr<C> p = &x;
    smart_ptr<C const> cp = &x;

    CHECK_NESTED(not_fn(!(*p).fun(40), p0012_nothrows, nf, p, 40));
    CHECK_NESTED(not_fn(!(*p).cfun(40), p0012_nothrows, ncf, p, 40));
    CHECK_NESTED(not_fn(!(*p).lfun(40), p0012_nothrows, nlf, p, 40));
    CHECK_NESTED(not_fn(!(*p).clfun(40), p0012_nothrows, nclf, p, 40));

    CHECK_NESTED(not_fn(!(*cp).cfun(40), p0012_nothrows, ncf, cp, 40));
    CHECK_NESTED(not_fn(!(*cp).clfun(40), p0012_nothrows, nclf, cp, 40));

    smart_ptr_throws<C> tp = &x;
    smart_ptr_throws<C const> tcp = &x;

    CHECK_NESTED(not_fn(!(*tp).fun(40), throws, nf, tp, 40));
    CHECK_NESTED(not_fn(!(*tp).cfun(40), throws, ncf, tp, 40));
    CHECK_NESTED(not_fn(!(*tp).lfun(40), throws, nlf, tp, 40));
    CHECK_NESTED(not_fn(!(*tp).clfun(40), throws, nclf, tp, 40));

    CHECK_NESTED(not_fn(!(*tcp).cfun(40), throws, ncf, tcp, 40));
    CHECK_NESTED(not_fn(!(*tcp).clfun(40), throws, nclf, tcp, 40));
  }
}

TEST_CASE("not_fn(fun-obj)", "[func.not_fn]") {
  /* call wrapper */ {
    struct Fn {
      Fn() {}
      Fn(Fn&&) = default;
      Fn(Fn const&) = delete;
      Fn& operator=(Fn&&) = default;
      void operator()(){};
    };
    using call_wrapper = decltype(slb::not_fn(Fn{}));

    CHECK(std::is_move_constructible<call_wrapper>::value);
    CHECK(!std::is_copy_constructible<call_wrapper>::value);
    CHECK(!std::is_move_assignable<call_wrapper>::value);
    CHECK(!std::is_copy_assignable<call_wrapper>::value);

    CHECK(std::is_nothrow_move_constructible<call_wrapper>::value);
  }

  /* call-op */ {
    struct Fn {
      Boolean<lvalue> operator()(int base) & noexcept { return {base + 6}; }
      Boolean<const_lvalue> operator()(int base) const& noexcept {
        return {base + 7};
      }
      Boolean<rvalue> operator()(int base) && noexcept { return {base + 8}; }
// gcc finds calls on const rvalues ambiguous up to version 4.8.
#if !defined(__GNUC__) || (__GNUC__ > 4) ||                                    \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))
      Boolean<const_rvalue> operator()(int base) const&& noexcept {
        return {base + 9};
      }
#endif
    };
    auto f = Fn{};
    auto const& cf = f;
    auto nf = slb::not_fn(f);
    auto const& ncf = nf;

    CHECK_NESTED(not_fn(!f(40), nothrows, nf, 40));
    CHECK_NESTED(not_fn(!cf(40), nothrows, ncf, 40));
    CHECK_NESTED(not_fn(!std::move(f)(40), nothrows, std::move(nf), 40));
// gcc finds calls on const rvalues ambiguous up to version 4.8.
#if !defined(__GNUC__) || (__GNUC__ > 4) ||                                    \
    ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))
    CHECK_NESTED(not_fn(!std::move(cf)(40), nothrows, std::move(ncf), 40));
#endif
  }

  /* fun-ptr */ {
    struct S {
      static Boolean<> f(int base) noexcept(p0012) { return {base + 10}; }
    };
    auto f = &S::f;
    auto nf = slb::not_fn(f);

    CHECK_NESTED(not_fn(!f(40), p0012_nothrows, nf, 40));
  }

  /* sfinae */ {
    struct Fn {
      void operator()(int base) = delete;
      Boolean<> operator()(int base) const noexcept { return {base + 11}; }
    };
    auto f = Fn{};
    auto const& cf = f;
    auto nf = slb::not_fn(f);

    CHECK_NESTED(not_fn(!cf(40), nothrows, nf, 40));
  }
}
