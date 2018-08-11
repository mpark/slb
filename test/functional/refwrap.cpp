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

#include "../catch.hpp"

#define CHECK_NESTED(...)                                                      \
  do {                                                                         \
    INFO(__FILE__ "(" << __LINE__ << "): " #__VA_ARGS__);                      \
    check_##__VA_ARGS__;                                                       \
  } while (false)

// [refwrap], reference_wrapper
// template <class T> class reference_wrapper;
TEST_CASE("reference_wrapper", "[refwrap]") {
  CHECK(std::is_same<slb::reference_wrapper<int>::type, int>::value);

  // reference_wrapper<T> is a CopyConstructible and CopyAssignable wrapper
  // around a reference to an object or function of type T.
  {
    using ref_wrapper = slb::reference_wrapper<int>;

    CHECK(std::is_move_constructible<ref_wrapper>::value);
    CHECK(std::is_copy_constructible<ref_wrapper>::value);
    CHECK(std::is_move_assignable<ref_wrapper>::value);
    CHECK(std::is_copy_assignable<ref_wrapper>::value);

    CHECK(std::is_nothrow_move_constructible<ref_wrapper>::value);
    CHECK(std::is_nothrow_copy_constructible<ref_wrapper>::value);
    CHECK(std::is_nothrow_move_assignable<ref_wrapper>::value);
    CHECK(std::is_nothrow_copy_assignable<ref_wrapper>::value);

#if SLB_TRIVIALITY_TRAITS
    CHECK(std::is_trivially_move_constructible<ref_wrapper>::value);
    CHECK(std::is_trivially_copy_constructible<ref_wrapper>::value);
    CHECK(std::is_trivially_move_assignable<ref_wrapper>::value);
    CHECK(std::is_trivially_copy_assignable<ref_wrapper>::value);
#endif
  }

  // reference_wrapper<T> is a trivially copyable type
  {
    using ref_wrapper = slb::reference_wrapper<int>;

#if SLB_TRIVIALITY_TRAITS
    CHECK(std::is_trivially_copyable<ref_wrapper>::value);
#endif
    CHECK(std::is_trivially_destructible<ref_wrapper>::value);
  }
}

// template<class T>
// template<class U>
// reference_wrapper<T>::reference_wrapper(U&&) noexcept(see below);
template <typename T, bool IsNothrow = true>
struct conv_to {
  T val;
  operator T() const noexcept(IsNothrow) { return val; }
};
template <typename T>
using conv_to_throws = conv_to<T, false>;

template <typename T>
struct conv_from {
  conv_from(T) {}
};

TEST_CASE("reference_wrapper::reference_wrapper(U&&)", "[refwrap.const]") {
  int i = 0;
  CHECK(noexcept(slb::reference_wrapper<int>(i)));
  slb::reference_wrapper<int> ri(i);

  CHECK(noexcept(slb::reference_wrapper<int>(conv_to<int&>{i})));
  slb::reference_wrapper<int> rconv_to(conv_to<int&>{i});

  CHECK(!noexcept(slb::reference_wrapper<int>(conv_to_throws<int&>{i})));
  slb::reference_wrapper<int> rconv_to_throws(conv_to_throws<int&>{i});

  /* sfinae */ {
    CHECK(!std::is_constructible<slb::reference_wrapper<int>, int>::value);
  }

  /* implicit */ {
    CHECK(std::is_convertible<slb::reference_wrapper<int>, int&>::value);
    CHECK(!std::is_convertible<slb::reference_wrapper<int>, int&&>::value);

    CHECK(std::is_convertible<slb::reference_wrapper<int>,
                              slb::reference_wrapper<int const>>::value);

    CHECK(std::is_same<std::common_type<slb::reference_wrapper<int>, int>::type,
                       int>::value);
    CHECK(
        std::is_same<std::common_type<slb::reference_wrapper<int>,
                                      slb::reference_wrapper<int const>>::type,
                     slb::reference_wrapper<int const>>::value);

    struct S {
      static void f(slb::reference_wrapper<int>) {}
      static void f(conv_from<int>) {}
    };
    S::f(0);
  }
}

// template<class T>
// reference_wrapper<T>::operator=(const reference_wrapper&) noexcept
TEST_CASE("reference_wrapper::operator=(reference_wrapper const&)",
          "[refwrap.assign]") {
  int i = 0, none = 0;
  slb::reference_wrapper<int> ri(none);
  CHECK(noexcept(ri = i));
  ri = i;

  slb::reference_wrapper<int> rconv_to(none);
  CHECK(noexcept(rconv_to = conv_to<int&>{i}));
  rconv_to = conv_to<int&>{i};

  slb::reference_wrapper<int> rconv_to_throws(none);
  CHECK(!noexcept(rconv_to_throws = conv_to_throws<int&>{i}));
  rconv_to_throws = conv_to_throws<int&>{i};

  /* sfinae */ {
    CHECK(!std::is_assignable<slb::reference_wrapper<int>, int>::value);
  }
}

// template<class T>
// reference_wrapper<T>::operator T&() const noexcept
template <typename T>
T& implicit_conv(T& t) noexcept {
  return t;
}

TEST_CASE("reference_wrapper::operator T&() const noexcept",
          "[refwrap.access]") {
  int i = 0;
  slb::reference_wrapper<int> const ri(i);
  CHECK(noexcept(implicit_conv<int&>(ri)));
  CHECK(&implicit_conv<int&>(ri) == &i);
}

// template<class T>
// T& reference_wrapper<T>::get() const noexcept
TEST_CASE("T& reference_wrapper::get() const noexcept", "[refwrap.access]") {
  int i = 0;
  slb::reference_wrapper<int> const ri(i);
  CHECK(std::is_same<decltype(ri.get()), int&>::value);
  CHECK(noexcept(ri.get()));
  CHECK(&ri.get() == &i);
}

// template<class T>
// template<class... ArgTypes>
// invoke_result_t<T&, ArgTypes...>
//   reference_wrapper<T>::operator()(ArgTypes&&...) const;
struct C {
  int obj;
  C(int val) : obj(val) {}

  int fun(int base) noexcept { return base + 0; }
  int cfun(int base) const noexcept { return base + 1; }
  int lfun(int base) & noexcept { return base + 2; }
  int rfun(int base) && noexcept { return base + 3; }
  int clfun(int base) const& noexcept { return base + 4; }
  int crfun(int base) const&& noexcept { return base + 5; }
};

struct D : C {
  D(int val) : C(val) {}
};

template <typename T>
struct smart_ptr {
  T* ptr;
  smart_ptr(T* ptr) : ptr(ptr) {}
  T& operator*() const noexcept { return *ptr; }
};

template <typename T>
T const* addressof(T const& ref) {
  return &ref;
}

template <typename R, typename F, typename T1>
void check_ref_obj(R&& r, F mem, T1&& t1) {
  CHECK(::addressof(mem(std::forward<T1>(t1))) == ::addressof(r));
  CHECK(std::is_same<decltype(mem(std::forward<T1>(t1))), R&&>::value);
}

template <typename R, typename F, typename... Tn>
void check_ref_fun(R const& r, F mem, Tn&&... tn) {
  CHECK(mem(std::forward<Tn>(tn)...) == r);
  CHECK(std::is_same<decltype(mem(std::forward<Tn>(tn)...)), R>::value);
}

TEST_CASE("reference_wrapper::operator()(mem-obj-ptr)", "[refwrap.invoke]") {
  auto mem = &C::obj;
  auto const ref = slb::ref(mem);

  /* reference */ {
    C x = {42};
    C& r = x;
    C const& cr = x;

    CHECK_NESTED(ref_obj(r.obj, ref, r));
    CHECK_NESTED(ref_obj(cr.obj, ref, cr));
    CHECK_NESTED(ref_obj(std::move(r.obj), ref, std::move(r)));
    CHECK_NESTED(ref_obj(std::move(cr.obj), ref, std::move(cr)));

    D d = {42};
    D& rd = d;
    D const& crd = d;

    CHECK_NESTED(ref_obj(rd.obj, ref, rd));
    CHECK_NESTED(ref_obj(crd.obj, ref, crd));
    CHECK_NESTED(ref_obj(std::move(rd.obj), ref, std::move(rd)));
    CHECK_NESTED(ref_obj(std::move(crd.obj), ref, std::move(crd)));
  }

  /* reference wrapper */ {
    C x = {42};
    std::reference_wrapper<C> std_r = x;
    std::reference_wrapper<C const> std_cr = x;

    CHECK_NESTED(ref_obj(std_r.get().obj, ref, std_r));
    CHECK_NESTED(ref_obj(std_cr.get().obj, ref, std_cr));

    slb::reference_wrapper<C> slb_r = x;
    slb::reference_wrapper<C const> slb_cr = x;

    CHECK_NESTED(ref_obj(slb_r.get().obj, ref, slb_r));
    CHECK_NESTED(ref_obj(slb_cr.get().obj, ref, slb_cr));
  }

  /* pointer */ {
    C x = {42};
    C* p = &x;
    C const* cp = &x;

    CHECK_NESTED(ref_obj((*p).obj, ref, p));
    CHECK_NESTED(ref_obj((*cp).obj, ref, cp));
  }

  /* smart pointer */ {
    C x = {42};
    smart_ptr<C> p = &x;
    smart_ptr<C const> cp = &x;

    CHECK_NESTED(ref_obj((*p).obj, ref, p));
    CHECK_NESTED(ref_obj((*cp).obj, ref, cp));
  }
}

TEST_CASE("reference_wrapper::operator()(mem-fun-ptr)", "[refwrap.invoke]") {
  auto mem = &C::fun;
  auto const ref = slb::ref(mem);

  auto cmem = &C::cfun;
  auto const cref = slb::ref(cmem);
  auto lmem = &C::lfun;
  auto const lref = slb::ref(lmem);
  auto rmem = &C::rfun;
  auto const rref = slb::ref(rmem);
  auto clmem = &C::clfun;
  auto const clref = slb::ref(clmem);
  auto crmem = &C::crfun;
  auto const crref = slb::ref(crmem);

  /* reference */ {
    C x = {42};
    C& r = x;
    C const& cr = x;

    CHECK_NESTED(ref_fun(r.fun(40), ref, r, 40));
    CHECK_NESTED(ref_fun(r.cfun(40), cref, r, 40));
    CHECK_NESTED(ref_fun(r.lfun(40), lref, r, 40));
    CHECK_NESTED(ref_fun(r.clfun(40), clref, r, 40));

    CHECK_NESTED(ref_fun(cr.cfun(40), cref, cr, 40));
    CHECK_NESTED(ref_fun(cr.clfun(40), clref, cr, 40));

    CHECK_NESTED(ref_fun(std::move(r).fun(40), ref, std::move(r), 40));
    CHECK_NESTED(ref_fun(std::move(r).cfun(40), cref, std::move(r), 40));
    CHECK_NESTED(ref_fun(std::move(r).clfun(40), clref, std::move(r), 40));
    CHECK_NESTED(ref_fun(std::move(r).rfun(40), rref, std::move(r), 40));
    CHECK_NESTED(ref_fun(std::move(r).crfun(40), crref, std::move(r), 40));

    CHECK_NESTED(ref_fun(std::move(cr).cfun(40), cref, std::move(cr), 40));
    CHECK_NESTED(ref_fun(std::move(cr).clfun(40), clref, std::move(cr), 40));
    CHECK_NESTED(ref_fun(std::move(cr).crfun(40), crref, std::move(cr), 40));
  }

  /* reference wrapper */ {
    C x = {42};
    std::reference_wrapper<C> std_r = x;
    std::reference_wrapper<C const> std_cr = x;

    CHECK_NESTED(ref_fun(std_r.get().fun(40), ref, std_r, 40));
    CHECK_NESTED(ref_fun(std_r.get().cfun(40), cref, std_r, 40));
    CHECK_NESTED(ref_fun(std_r.get().lfun(40), lref, std_r, 40));
    CHECK_NESTED(ref_fun(std_r.get().clfun(40), clref, std_r, 40));

    CHECK_NESTED(ref_fun(std_cr.get().cfun(40), cref, std_cr, 40));
    CHECK_NESTED(ref_fun(std_cr.get().clfun(40), clref, std_cr, 40));

    slb::reference_wrapper<C> slb_r = x;
    slb::reference_wrapper<C const> slb_cr = x;

    CHECK_NESTED(ref_fun(slb_r.get().fun(40), ref, slb_r, 40));
    CHECK_NESTED(ref_fun(slb_r.get().cfun(40), cref, slb_r, 40));
    CHECK_NESTED(ref_fun(slb_r.get().lfun(40), lref, slb_r, 40));
    CHECK_NESTED(ref_fun(slb_r.get().clfun(40), clref, slb_r, 40));

    CHECK_NESTED(ref_fun(slb_cr.get().cfun(40), cref, slb_cr, 40));
    CHECK_NESTED(ref_fun(slb_cr.get().clfun(40), clref, slb_cr, 40));
  }

  /* pointer */ {
    C x = {42};
    C* p = &x;
    C const* cp = &x;

    CHECK_NESTED(ref_fun((*p).fun(40), ref, p, 40));
    CHECK_NESTED(ref_fun((*p).cfun(40), cref, p, 40));
    CHECK_NESTED(ref_fun((*p).lfun(40), lref, p, 40));
    CHECK_NESTED(ref_fun((*p).clfun(40), clref, p, 40));

    CHECK_NESTED(ref_fun((*cp).cfun(40), cref, cp, 40));
    CHECK_NESTED(ref_fun((*cp).clfun(40), clref, cp, 40));
  }

  /* smart pointer */ {
    C x = {42};
    smart_ptr<C> p = &x;
    smart_ptr<C const> cp = &x;

    CHECK_NESTED(ref_fun((*p).fun(40), ref, p, 40));
    CHECK_NESTED(ref_fun((*p).cfun(40), cref, p, 40));
    CHECK_NESTED(ref_fun((*p).lfun(40), lref, p, 40));
    CHECK_NESTED(ref_fun((*p).clfun(40), clref, p, 40));

    CHECK_NESTED(ref_fun((*cp).cfun(40), cref, cp, 40));
    CHECK_NESTED(ref_fun((*cp).clfun(40), clref, cp, 40));
  }
}

TEST_CASE("reference_wrapper::operator()(fun-obj)", "[refwrap.invoke]") {
  /* call-op */ {
    struct Fn {
      int operator()(int base) noexcept { return base + 6; }
      int operator()(int base) const noexcept { return base + 7; }
    };
    auto f = Fn{};
    auto const& fc = f;
    auto const ref = slb::ref(f);
    auto const cref = slb::ref(fc);

    CHECK_NESTED(ref_fun(f(40), ref, 40));
    CHECK_NESTED(ref_fun(fc(40), cref, 40));
  }

  /* fun-ptr */ {
    struct S {
      static int f(int base) noexcept { return base + 7; }
    };
    auto f = &S::f;
    auto const ref = slb::ref(f);

    CHECK_NESTED(ref_fun(f(40), ref, 40));
  }
}

// template <typename T>
// reference_wrapper(T&) -> reference_wrapper<T>;
#if SLB_HAS_CXX17_DEDUCTION_GUIDES
TEST_CASE("reference_wrapper(guide)", "[refwrap]") {
  int i = 0;
  CHECK(std::is_same<decltype(slb::reference_wrapper(i)),
                     slb::reference_wrapper<int>>::value);
  slb::reference_wrapper ri(i);
  CHECK(&ri.get() == &i);

  int const ci = 0;
  CHECK(std::is_same<decltype(slb::reference_wrapper(ci)),
                     slb::reference_wrapper<int const>>::value);
  slb::reference_wrapper rci(ci);
  CHECK(&rci.get() == &ci);
}
#endif

// template <class T> reference_wrapper<T> ref(T&) noexcept;
// template <class T> void ref(const T&&) = delete;
// template <class T> reference_wrapper<T> ref(reference_wrapper<T>) noexcept;
namespace ref_deleted {
void ref(...);
using slb::ref;

template <typename T, typename Enable = void>
struct no_ref : std::true_type {};

template <typename T>
struct no_ref<T, decltype((void)(ref_deleted::ref(std::declval<T>())))>
    : std::false_type {};
} // namespace ref_deleted

TEST_CASE("ref", "[refwrap.helpers]") {
  int i = 0;
  CHECK(
      std::is_same<decltype(slb::ref(i)), slb::reference_wrapper<int>>::value);
  CHECK(noexcept(slb::ref(i)));
  slb::reference_wrapper<int> ri = slb::ref(i);
  CHECK(&ri.get() == &i);

  int const ci = 0;
  CHECK(std::is_same<decltype(slb::ref(ci)),
                     slb::reference_wrapper<int const>>::value);
  CHECK(noexcept(slb::ref(ci)));
  slb::reference_wrapper<int const> rci = slb::ref(ci);
  CHECK(&rci.get() == &ci);

  using ref_deleted::no_ref;
  CHECK(no_ref<int&&>::value);
  CHECK(no_ref<int const&&>::value);

  /* unwrap */ {
    CHECK(std::is_same<decltype(slb::ref(ri)),
                       slb::reference_wrapper<int>>::value);
    CHECK(noexcept(slb::ref(ri)));
    slb::reference_wrapper<int> uri = slb::ref(ri);
    CHECK(&uri.get() == &i);

    slb::reference_wrapper<slb::reference_wrapper<int>> r2i(uri);
    CHECK(std::is_same<
          decltype(slb::ref(r2i)),
          slb::reference_wrapper<slb::reference_wrapper<int>>>::value);
    //! CHECK(&slb::ref(r2i).get() == &uri); // LWG3146

    CHECK(std::is_same<decltype(slb::ref(rci)),
                       slb::reference_wrapper<int const>>::value);
    CHECK(noexcept(slb::ref(rci)));
    slb::reference_wrapper<int const> urci = slb::ref(rci);
    CHECK(&urci.get() == &ci);

    slb::reference_wrapper<slb::reference_wrapper<int const>> r2ci(urci);
    CHECK(std::is_same<
          decltype(slb::ref(r2ci)),
          slb::reference_wrapper<slb::reference_wrapper<int const>>>::value);
    //! CHECK(&slb::ref(r2ci).get() == &urci); // LWG3146
  }
}

// template <class T> reference_wrapper<const T> cref(const T&) noexcept;
// template <class T> void cref(const T&&) = delete;
// template <class T> reference_wrapper<const T> cref(reference_wrapper<T>)
//   noexcept;
namespace cref_deleted {
void cref(...);
using slb::cref;

template <typename T, typename Enable = void>
struct no_cref : std::true_type {};

template <typename T>
struct no_cref<T, decltype((void)(cref_deleted::cref(std::declval<T>())))>
    : std::false_type {};
} // namespace cref_deleted

TEST_CASE("cref", "[refwrap.helpers]") {
  int i = 0;
  CHECK(std::is_same<decltype(slb::cref(i)),
                     slb::reference_wrapper<int const>>::value);
  CHECK(noexcept(slb::cref(i)));
  slb::reference_wrapper<int const> ri = slb::cref(i);
  CHECK(&ri.get() == &i);

  int const ci = 0;
  CHECK(std::is_same<decltype(slb::cref(ci)),
                     slb::reference_wrapper<int const>>::value);
  CHECK(noexcept(slb::cref(ci)));
  slb::reference_wrapper<int const> rci = slb::cref(ci);
  CHECK(&rci.get() == &ci);

  using cref_deleted::no_cref;
  CHECK(no_cref<int&&>::value);
  CHECK(no_cref<int const&&>::value);

  /* unwrap */ {
    CHECK(std::is_same<decltype(slb::cref(ri)),
                       slb::reference_wrapper<int const>>::value);
    CHECK(noexcept(slb::cref(ri)));
    slb::reference_wrapper<int const> uri = slb::cref(ri);
    CHECK(&uri.get() == &i);

    slb::reference_wrapper<slb::reference_wrapper<int const>> r2i(uri);
    CHECK(std::is_same<decltype(slb::cref(r2i)),
                       slb::reference_wrapper<
                           slb::reference_wrapper<int const> const>>::value);
    //! CHECK(&slb::cref(r2i).get() == &uri); // LWG3146

    CHECK(std::is_same<decltype(slb::cref(rci)),
                       slb::reference_wrapper<int const>>::value);
    CHECK(noexcept(slb::cref(rci)));
    slb::reference_wrapper<int const> urci = slb::cref(rci);
    CHECK(&urci.get() == &ci);

    slb::reference_wrapper<slb::reference_wrapper<int const>> r2ci(urci);
    CHECK(std::is_same<decltype(slb::cref(r2ci)),
                       slb::reference_wrapper<
                           slb::reference_wrapper<int const> const>>::value);
    //! CHECK(&slb::cref(r2ci).get() == &urci); // LWG3146
  }
}
