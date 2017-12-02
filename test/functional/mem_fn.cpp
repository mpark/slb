/*
  SLB.Functional

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <slb/functional.hpp>

#include <type_traits>
#include <utility>

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#define CHECK_NESTED(...)                                                      \
  do {                                                                         \
    INFO(__FILE__ "(" << __LINE__ << "): " #__VA_ARGS__);                      \
    check_##__VA_ARGS__;                                                       \
  } while (false)

// [func.memfn], member function adaptors

// template<class R, class T>
//   unspecified mem_fn(R T::*) noexcept;

// Account for P0012: "Make exception specifications be part of the type
// system".
static constexpr bool p0012 = !std::is_same<void(), void() noexcept>::value;

struct C {
  int obj;
  C(int val) : obj(val) {}

  int fun(int base) noexcept(p0012) { return base + 0; }
  int cfun(int base) const noexcept(p0012) { return base + 1; }
  int lfun(int base) & noexcept(p0012) { return base + 2; }
  int rfun(int base) && noexcept(p0012) { return base + 3; }
  int clfun(int base) const& noexcept(p0012) { return base + 4; }
  int crfun(int base) const&& noexcept(p0012) { return base + 5; }
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

template <typename T>
T const* addressof(T const& ref) {
  return &ref;
}

template <typename R, bool IsNothrow, typename F, typename T1>
void check_mem_obj(R&& r,
                   std::integral_constant<bool, IsNothrow>,
                   F mem,
                   T1&& t1) {
  CHECK(::addressof(mem(std::forward<T1>(t1))) == ::addressof(r));
  CHECK(std::is_same<decltype(mem(std::forward<T1>(t1))), R&&>::value);
  CHECK(noexcept(mem(std::forward<T1>(t1))) == IsNothrow);
}

template <typename R, bool IsNothrow, typename F, typename... Tn>
void check_mem_fun(R const& r,
                   std::integral_constant<bool, IsNothrow>,
                   F mem,
                   Tn&&... tn) {
  CHECK(mem(std::forward<Tn>(tn)...) == r);
  CHECK(std::is_same<decltype(mem(std::forward<Tn>(tn)...)), R>::value);
  CHECK(noexcept(mem(std::forward<Tn>(tn)...)) == IsNothrow);
}

TEST_CASE("mem_fn(mem-obj-ptr)", "[func.memfn]") {
  auto mem = slb::mem_fn(&C::obj);
  CHECK(noexcept(slb::mem_fn(&C::obj)));

  /* simple call wrapper */ {
    using call_wrapper = decltype(mem);

    CHECK(std::is_move_constructible<call_wrapper>::value);
    CHECK(std::is_copy_constructible<call_wrapper>::value);
    CHECK(std::is_move_assignable<call_wrapper>::value);
    CHECK(std::is_copy_assignable<call_wrapper>::value);

    CHECK(std::is_nothrow_move_constructible<call_wrapper>::value);
    CHECK(std::is_nothrow_copy_constructible<call_wrapper>::value);
    CHECK(std::is_nothrow_move_assignable<call_wrapper>::value);
    CHECK(std::is_nothrow_copy_assignable<call_wrapper>::value);
  }

  /* reference */ {
    C x = {42};
    C& r = x;
    C const& cr = x;

    CHECK_NESTED(mem_obj(r.obj, nothrows, mem, r));
    CHECK_NESTED(mem_obj(cr.obj, nothrows, mem, cr));
    CHECK_NESTED(mem_obj(std::move(r.obj), nothrows, mem, std::move(r)));
    CHECK_NESTED(mem_obj(std::move(cr.obj), nothrows, mem, std::move(cr)));

    D d = {42};
    D& rd = d;
    D const& crd = d;

    CHECK_NESTED(mem_obj(rd.obj, nothrows, mem, rd));
    CHECK_NESTED(mem_obj(crd.obj, nothrows, mem, crd));
    CHECK_NESTED(mem_obj(std::move(rd.obj), nothrows, mem, std::move(rd)));
    CHECK_NESTED(mem_obj(std::move(crd.obj), nothrows, mem, std::move(crd)));
  }

  /* reference wrapper */ {
    C x = {42};
    std::reference_wrapper<C> r = x;
    std::reference_wrapper<C const> cr = x;

    CHECK_NESTED(mem_obj(r.get().obj, nothrows, mem, r));
    CHECK_NESTED(mem_obj(cr.get().obj, nothrows, mem, cr));
  }

  /* pointer */ {
    C x = {42};
    C* p = &x;
    C const* cp = &x;

    CHECK_NESTED(mem_obj((*p).obj, nothrows, mem, p));
    CHECK_NESTED(mem_obj((*cp).obj, nothrows, mem, cp));
  }

  /* smart pointer */ {
    C x = {42};
    smart_ptr<C> p = &x;
    smart_ptr<C const> cp = &x;

    CHECK_NESTED(mem_obj((*p).obj, nothrows, mem, p));
    CHECK_NESTED(mem_obj((*cp).obj, nothrows, mem, cp));

    smart_ptr_throws<C> tp = &x;
    smart_ptr_throws<C const> tcp = &x;

    CHECK_NESTED(mem_obj((*tp).obj, throws, mem, tp));
    CHECK_NESTED(mem_obj((*tcp).obj, throws, mem, tcp));
  }
}

TEST_CASE("mem_fn(mem-fun-ptr)", "[func.memfn]") {
  auto mem = slb::mem_fn(&C::fun);
  CHECK(noexcept(slb::mem_fn(&C::fun)));

  /* simple call wrapper */ {
    using call_wrapper = decltype(mem);

    CHECK(std::is_move_constructible<call_wrapper>::value);
    CHECK(std::is_copy_constructible<call_wrapper>::value);
    CHECK(std::is_move_assignable<call_wrapper>::value);
    CHECK(std::is_copy_assignable<call_wrapper>::value);

    CHECK(std::is_nothrow_move_constructible<call_wrapper>::value);
    CHECK(std::is_nothrow_copy_constructible<call_wrapper>::value);
    CHECK(std::is_nothrow_move_assignable<call_wrapper>::value);
    CHECK(std::is_nothrow_copy_assignable<call_wrapper>::value);
  }

  auto cmem = slb::mem_fn(&C::cfun);
  auto lmem = slb::mem_fn(&C::lfun);
  auto rmem = slb::mem_fn(&C::rfun);
  auto clmem = slb::mem_fn(&C::clfun);
  auto crmem = slb::mem_fn(&C::crfun);

  /* reference */ {
    C x = {42};
    C& r = x;
    C const& cr = x;

    CHECK_NESTED(mem_fun(r.fun(40), p0012_nothrows, mem, r, 40));
    CHECK_NESTED(mem_fun(r.cfun(40), p0012_nothrows, cmem, r, 40));
    CHECK_NESTED(mem_fun(r.lfun(40), p0012_nothrows, lmem, r, 40));
    CHECK_NESTED(mem_fun(r.clfun(40), p0012_nothrows, clmem, r, 40));

    CHECK_NESTED(mem_fun(cr.cfun(40), p0012_nothrows, cmem, cr, 40));
    CHECK_NESTED(mem_fun(cr.clfun(40), p0012_nothrows, clmem, cr, 40));

    CHECK_NESTED(
        mem_fun(std::move(r).fun(40), p0012_nothrows, mem, std::move(r), 40));
    CHECK_NESTED(
        mem_fun(std::move(r).cfun(40), p0012_nothrows, cmem, std::move(r), 40));
    CHECK_NESTED(mem_fun(
        std::move(r).clfun(40), p0012_nothrows, clmem, std::move(r), 40));
    CHECK_NESTED(
        mem_fun(std::move(r).rfun(40), p0012_nothrows, rmem, std::move(r), 40));
    CHECK_NESTED(mem_fun(
        std::move(r).crfun(40), p0012_nothrows, crmem, std::move(r), 40));

    CHECK_NESTED(mem_fun(
        std::move(cr).cfun(40), p0012_nothrows, cmem, std::move(cr), 40));
    CHECK_NESTED(mem_fun(
        std::move(cr).clfun(40), p0012_nothrows, clmem, std::move(cr), 40));
    CHECK_NESTED(mem_fun(
        std::move(cr).crfun(40), p0012_nothrows, crmem, std::move(cr), 40));
  }

  /* reference wrapper */ {
    C x = {42};
    std::reference_wrapper<C> r = x;
    std::reference_wrapper<C const> cr = x;

    CHECK_NESTED(mem_fun(r.get().fun(40), p0012_nothrows, mem, r, 40));
    CHECK_NESTED(mem_fun(r.get().cfun(40), p0012_nothrows, cmem, r, 40));
    CHECK_NESTED(mem_fun(r.get().lfun(40), p0012_nothrows, lmem, r, 40));
    CHECK_NESTED(mem_fun(r.get().clfun(40), p0012_nothrows, clmem, r, 40));

    CHECK_NESTED(mem_fun(cr.get().cfun(40), p0012_nothrows, cmem, cr, 40));
    CHECK_NESTED(mem_fun(cr.get().clfun(40), p0012_nothrows, clmem, cr, 40));
  }

  /* pointer */ {
    C x = {42};
    C* p = &x;
    C const* cp = &x;

    CHECK_NESTED(mem_fun((*p).fun(40), p0012_nothrows, mem, p, 40));
    CHECK_NESTED(mem_fun((*p).cfun(40), p0012_nothrows, cmem, p, 40));
    CHECK_NESTED(mem_fun((*p).lfun(40), p0012_nothrows, lmem, p, 40));
    CHECK_NESTED(mem_fun((*p).clfun(40), p0012_nothrows, clmem, p, 40));

    CHECK_NESTED(mem_fun((*cp).cfun(40), p0012_nothrows, cmem, cp, 40));
    CHECK_NESTED(mem_fun((*cp).clfun(40), p0012_nothrows, clmem, cp, 40));
  }

  /* smart pointer */ {
    C x = {42};
    smart_ptr<C> p = &x;
    smart_ptr<C const> cp = &x;

    CHECK_NESTED(mem_fun((*p).fun(40), p0012_nothrows, mem, p, 40));
    CHECK_NESTED(mem_fun((*p).cfun(40), p0012_nothrows, cmem, p, 40));
    CHECK_NESTED(mem_fun((*p).lfun(40), p0012_nothrows, lmem, p, 40));
    CHECK_NESTED(mem_fun((*p).clfun(40), p0012_nothrows, clmem, p, 40));

    CHECK_NESTED(mem_fun((*cp).cfun(40), p0012_nothrows, cmem, cp, 40));
    CHECK_NESTED(mem_fun((*cp).clfun(40), p0012_nothrows, clmem, cp, 40));

    smart_ptr_throws<C> tp = &x;
    smart_ptr_throws<C const> tcp = &x;

    CHECK_NESTED(mem_fun((*tp).fun(40), throws, mem, tp, 40));
    CHECK_NESTED(mem_fun((*tp).cfun(40), throws, cmem, tp, 40));
    CHECK_NESTED(mem_fun((*tp).lfun(40), throws, lmem, tp, 40));
    CHECK_NESTED(mem_fun((*tp).clfun(40), throws, clmem, tp, 40));

    CHECK_NESTED(mem_fun((*tcp).cfun(40), throws, cmem, tcp, 40));
    CHECK_NESTED(mem_fun((*tcp).clfun(40), throws, clmem, tcp, 40));
  }
}
