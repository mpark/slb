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

// [func.bind], bind

// template<class T> struct is_bind_expression;
TEST_CASE("is_bind_expression", "[func.bind.isbind]") {
  struct S {
    static int f(int base) { return base; }
  };

  // The implementation shall provide a definition that has a base
  // characteristic of `true_type` if `T` is a type returned from `bind`,
  // otherwise it shall have a base characteristic of `false_type`.
  using slb_bind_t = decltype(slb::bind(&S::f, 42));
  CHECK(slb::is_base_of<slb::true_type,
                        slb::is_bind_expression<slb_bind_t>>::value);
  CHECK(std::is_bind_expression<slb_bind_t>::value);

  using std_bind_t = decltype(std::bind(&S::f, 42));
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<std_bind_t>>::value);

  using slb_bind_r_t = decltype(slb::bind<int>(&S::f, 42));
  CHECK(slb::is_base_of<slb::true_type,
                        slb::is_bind_expression<slb_bind_r_t>>::value);
  CHECK(std::is_bind_expression<slb_bind_r_t>::value);

  using std_bind_r_t = decltype(std::bind<int>(&S::f, 42));
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<std_bind_r_t>>::value);

  CHECK(std::is_base_of<slb::false_type, slb::is_bind_expression<int>>::value);
}

// template<class T> struct is_placeholder;
TEST_CASE("is_placeholder", "[func.bind.isplace]") {

  // The implementation shall provide a definition that has the base
  // characteristic of `integral_constant<int, J>` if `T` is the type of
  // `std::placeholders::_J`, otherwise it shall have a base characteristic of
  // `integral_constant<int, 0>`.
  using slb_ph1_t = decltype(slb::placeholders::_1);
  CHECK(std::is_base_of<slb::integral_constant<int, 1>,
                        slb::is_placeholder<slb_ph1_t>>::value);
  CHECK(slb::is_placeholder<slb_ph1_t>::value);

  using std_ph1_t = decltype(std::placeholders::_1);
  CHECK(std::is_base_of<slb::integral_constant<int, 1>,
                        slb::is_placeholder<std_ph1_t>>::value);

  using slb_ph2_t = decltype(slb::placeholders::_2);
  CHECK(std::is_base_of<slb::integral_constant<int, 2>,
                        slb::is_placeholder<slb_ph2_t>>::value);
  CHECK(slb::is_placeholder<slb_ph2_t>::value);

  using std_ph2_t = decltype(std::placeholders::_2);
  CHECK(std::is_base_of<slb::integral_constant<int, 2>,
                        slb::is_placeholder<std_ph2_t>>::value);

  CHECK(std::is_base_of<slb::integral_constant<int, 0>,
                        slb::is_placeholder<int>>::value);
}

// template<class F, class... BoundArgs>
//   unspecified bind(F&&, BoundArgs&&...);
// template<class R, class F, class... BoundArgs>
//   unspecified bind(F&&, BoundArgs&&...);
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

std::true_type const byval{};
std::false_type const byref{};

template <typename T>
T const* addressof(T const& ref) {
  return &ref;
}

template <typename R,
          typename Bound,
          typename BoundR,
          typename BoundVoid,
          typename... Args>
void check_bound_obj(
    R&& r, Bound bound, BoundR bound_r, BoundVoid bound_void, Args&&... args) {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound)>>::value);
  CHECK(std::is_same<decltype(bound(std::forward<Args>(args)...)), R&&>::value);
  CHECK(::addressof(bound(std::forward<Args>(args)...)) == ::addressof(r));

  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound_r)>>::value);
  CHECK(
      std::is_same<decltype(bound_r(std::forward<Args>(args)...)), R&&>::value);
  CHECK(::addressof(bound_r(std::forward<Args>(args)...)) == ::addressof(r));

  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound_void)>>::value);
  CHECK(std::is_same<decltype(bound_void(std::forward<Args>(args)...)),
                     void>::value);
  bound_void(std::forward<Args>(args)...);
}

template <typename R, bool ByVal, typename Bound>
void check_captured_bound_obj(R&& r,
                              std::integral_constant<bool, ByVal>,
                              Bound bound) {
  using RD =
      typename std::remove_cv<typename std::remove_reference<R>::type>::type;

  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound)>>::value);
  if (ByVal) {
    CHECK(std::is_same<decltype(bound()), RD&>::value);
    CHECK(::addressof(bound()) != ::addressof(r));
    CHECK(bound() == r);
  } else {
    CHECK(std::is_same<decltype(bound()), R&&>::value);
    CHECK(::addressof(bound()) == ::addressof(r));
  }

  Bound const bound_const(bound);
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound_const)>>::value);
  if (ByVal) {
    CHECK(std::is_same<decltype(bound_const()), RD const&>::value);
    CHECK(::addressof(bound_const()) != ::addressof(r));
    CHECK(bound_const() == r);
  } else {
    CHECK(std::is_same<decltype(bound_const()), R&&>::value);
    CHECK(::addressof(bound_const()) == ::addressof(r));
  }
}

template <typename R, bool ByVal, typename F, typename Arg1>
void check_bind_obj(R&& r,
                    std::integral_constant<bool, ByVal>,
                    F&& f,
                    Arg1&& arg1) {
  namespace ph = slb::placeholders;
  check_bound_obj(std::forward<R>(r),
                  slb::bind(std::forward<F>(f), ph::_1),
                  slb::bind<R&&>(std::forward<F>(f), ph::_1),
                  slb::bind<void>(std::forward<F>(f), ph::_1),
                  std::forward<Arg1>(arg1));
  check_captured_bound_obj(
      std::forward<R>(r),
      std::integral_constant<bool, ByVal>{},
      slb::bind(std::forward<F>(f), std::forward<Arg1>(arg1)));
}

template <typename R,
          typename Bound,
          typename BoundR,
          typename BoundVoid,
          typename... Args>
void check_bound_fun(
    R&& r, Bound bound, BoundR bound_r, BoundVoid bound_void, Args&&... args) {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound)>>::value);
  CHECK(std::is_same<decltype(bound(std::forward<Args>(args)...)), R>::value);
  CHECK(bound(std::forward<Args>(args)...) == r);

  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound_r)>>::value);
  CHECK(std::is_same<decltype(bound_r(std::forward<Args>(args)...)), R>::value);
  CHECK(bound_r(std::forward<Args>(args)...) == r);

  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound_void)>>::value);
  CHECK(std::is_same<decltype(bound_void(std::forward<Args>(args)...)),
                     void>::value);
  bound_void(std::forward<Args>(args)...);
}

template <typename R,
          typename Bound,
          typename BoundR,
          typename BoundVoid,
          typename... Args>
void check_bound_fun_const(R&& r,
                           Bound const bound,
                           BoundR const bound_r,
                           BoundVoid const bound_void,
                           Args&&... args) {
  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound)>>::value);
  CHECK(std::is_same<decltype(bound(std::forward<Args>(args)...)), R>::value);
  CHECK(bound(std::forward<Args>(args)...) == r);

  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound_r)>>::value);
  CHECK(std::is_same<decltype(bound_r(std::forward<Args>(args)...)), R>::value);
  CHECK(bound_r(std::forward<Args>(args)...) == r);

  CHECK(std::is_base_of<slb::true_type,
                        slb::is_bind_expression<decltype(bound_void)>>::value);
  CHECK(std::is_same<decltype(bound_void(std::forward<Args>(args)...)),
                     void>::value);
  bound_void(std::forward<Args>(args)...);
}

template <typename R, typename F, typename Arg1>
void check_bind_fun(R&& r, F&& f, Arg1&& arg1) {
  namespace ph = slb::placeholders;
  check_bound_fun(std::forward<R>(r),
                  slb::bind(std::forward<F>(f), ph::_1),
                  slb::bind<R>(std::forward<F>(f), ph::_1),
                  slb::bind<void>(std::forward<F>(f), ph::_1),
                  std::forward<Arg1>(arg1));
}

template <typename R, typename F, typename Arg1>
void check_bind_fun_const(R&& r, F&& f, Arg1&& arg1) {
  namespace ph = slb::placeholders;
  check_bound_fun_const(std::forward<R>(r),
                        slb::bind(std::forward<F>(f), ph::_1),
                        slb::bind<R>(std::forward<F>(f), ph::_1),
                        slb::bind<void>(std::forward<F>(f), ph::_1),
                        std::forward<Arg1>(arg1));
}

template <typename R, typename F, typename Arg1, typename Arg2>
void check_bind_fun(R&& r, F&& f, Arg1&& arg1, Arg2&& arg2) {
  namespace ph = slb::placeholders;
  check_bound_fun(std::forward<R>(r),
                  slb::bind(std::forward<F>(f), ph::_1, ph::_2),
                  slb::bind<R>(std::forward<F>(f), ph::_1, ph::_2),
                  slb::bind<void>(std::forward<F>(f), ph::_1, ph::_2),
                  std::forward<Arg1>(arg1),
                  std::forward<Arg2>(arg2));
}

TEST_CASE("bind(mem-obj-ptr)", "[func.bind.bind]") {
  auto f = &C::obj;

  /* forwarding call wrapper */ {
    using call_wrapper = decltype(slb::bind(f, C{42}));

    CHECK(std::is_move_constructible<call_wrapper>::value);
    CHECK(std::is_copy_constructible<call_wrapper>::value);
  }

  /* reference */ {
    C x = {42};
    C& r = x;
    C const& cr = x;

    CHECK_NESTED(bind_obj(r.obj, byval, f, r));
    CHECK_NESTED(bind_obj(cr.obj, byval, f, cr));
    CHECK_NESTED(bind_obj(std::move(r.obj), byval, f, std::move(r)));
    CHECK_NESTED(bind_obj(std::move(cr.obj), byval, f, std::move(cr)));

    D d = {42};
    D& rd = d;
    D const& crd = d;

    CHECK_NESTED(bind_obj(rd.obj, byval, f, rd));
    CHECK_NESTED(bind_obj(crd.obj, byval, f, crd));
    CHECK_NESTED(bind_obj(std::move(rd.obj), byval, f, std::move(rd)));
    CHECK_NESTED(bind_obj(std::move(crd.obj), byval, f, std::move(crd)));
  }

  /* reference wrapper */ {
    C x = {42};
    std::reference_wrapper<C> r = x;
    std::reference_wrapper<C const> cr = x;

    CHECK_NESTED(bind_obj(r.get().obj, byref, f, r));
    CHECK_NESTED(bind_obj(cr.get().obj, byref, f, cr));
  }

  /* pointer */ {
    C x = {42};
    C* p = &x;
    C const* cp = &x;

    CHECK_NESTED(bind_obj((*p).obj, byref, f, p));
    CHECK_NESTED(bind_obj((*cp).obj, byref, f, cp));
  }

  /* smart pointer */ {
    C x = {42};
    smart_ptr<C> p = &x;
    smart_ptr<C const> cp = &x;

    CHECK_NESTED(bind_obj((*p).obj, byref, f, p));
    CHECK_NESTED(bind_obj((*cp).obj, byref, f, cp));
  }
}

TEST_CASE("bind(mem-fun-ptr)", "[func.bind.bind]") {
  auto f = &C::fun;

  /* forwarding call wrapper */ {
    using call_wrapper = decltype(slb::bind(f, C{42}));

    CHECK(std::is_move_constructible<call_wrapper>::value);
    CHECK(std::is_copy_constructible<call_wrapper>::value);
  }

  auto cf = &C::cfun;
  auto lf = &C::lfun;
  auto rf = &C::rfun;
  auto clf = &C::clfun;
  auto crf = &C::crfun;

  /* reference */ {
    C x = {42};
    C& r = x;
    C const& cr = x;

    CHECK_NESTED(bind_fun(r.fun(40), f, r, 40));
    CHECK_NESTED(bind_fun(r.cfun(40), cf, r, 40));
    CHECK_NESTED(bind_fun(r.lfun(40), lf, r, 40));
    CHECK_NESTED(bind_fun(r.clfun(40), clf, r, 40));

    CHECK_NESTED(bind_fun(cr.cfun(40), cf, cr, 40));
    CHECK_NESTED(bind_fun(cr.clfun(40), clf, cr, 40));

    CHECK_NESTED(bind_fun(std::move(r).fun(40), f, std::move(r), 40));
    CHECK_NESTED(bind_fun(std::move(r).cfun(40), cf, std::move(r), 40));
    CHECK_NESTED(bind_fun(std::move(r).clfun(40), clf, std::move(r), 40));
    CHECK_NESTED(bind_fun(std::move(r).rfun(40), rf, std::move(r), 40));
    CHECK_NESTED(bind_fun(std::move(r).crfun(40), crf, std::move(r), 40));

    CHECK_NESTED(bind_fun(std::move(cr).cfun(40), cf, std::move(cr), 40));
    CHECK_NESTED(bind_fun(std::move(cr).clfun(40), clf, std::move(cr), 40));
    CHECK_NESTED(bind_fun(std::move(cr).crfun(40), crf, std::move(cr), 40));
  }

  /* reference wrapper */ {
    C x = {42};
    std::reference_wrapper<C> r = x;
    std::reference_wrapper<C const> cr = x;

    CHECK_NESTED(bind_fun(r.get().fun(40), f, r, 40));
    CHECK_NESTED(bind_fun(r.get().cfun(40), cf, r, 40));
    CHECK_NESTED(bind_fun(r.get().lfun(40), lf, r, 40));
    CHECK_NESTED(bind_fun(r.get().clfun(40), clf, r, 40));

    CHECK_NESTED(bind_fun(cr.get().cfun(40), cf, cr, 40));
    CHECK_NESTED(bind_fun(cr.get().clfun(40), clf, cr, 40));
  }

  /* pointer */ {
    C x = {42};
    C* p = &x;
    C const* cp = &x;

    CHECK_NESTED(bind_fun((*p).fun(40), f, p, 40));
    CHECK_NESTED(bind_fun((*p).cfun(40), cf, p, 40));
    CHECK_NESTED(bind_fun((*p).lfun(40), lf, p, 40));
    CHECK_NESTED(bind_fun((*p).clfun(40), clf, p, 40));

    CHECK_NESTED(bind_fun((*cp).cfun(40), cf, cp, 40));
    CHECK_NESTED(bind_fun((*cp).clfun(40), clf, cp, 40));
  }

  /* smart pointer */ {
    C x = {42};
    smart_ptr<C> p = &x;
    smart_ptr<C const> cp = &x;

    CHECK_NESTED(bind_fun((*p).fun(40), f, p, 40));
    CHECK_NESTED(bind_fun((*p).cfun(40), cf, p, 40));
    CHECK_NESTED(bind_fun((*p).lfun(40), lf, p, 40));
    CHECK_NESTED(bind_fun((*p).clfun(40), clf, p, 40));

    CHECK_NESTED(bind_fun((*cp).cfun(40), cf, cp, 40));
    CHECK_NESTED(bind_fun((*cp).clfun(40), clf, cp, 40));
  }
}

TEST_CASE("bind(fun-obj)", "[func.bind.bind]") {
  /* forwarding call wrapper */ {
    struct Fn {
      Fn() {}
      Fn(Fn&&) = default;
      Fn(Fn const&) = delete;
      void operator()(){};
    };
    using call_wrapper = decltype(slb::bind(Fn{}));

    CHECK(std::is_move_constructible<call_wrapper>::value);
    CHECK(!std::is_copy_constructible<call_wrapper>::value);
  }

  /* call-op */ {
    struct Fn {
      int operator()(int base) noexcept { return base + 6; }
      int operator()(int base) const noexcept { return base + 7; }
    };
    auto f = Fn{};
    auto const& fc = f;

    CHECK_NESTED(bind_fun(f(40), f, 40));
    CHECK_NESTED(bind_fun_const(fc(40), f, 40));
  }

  /* fun-ptr */ {
    struct S {
      static int f(int base) noexcept { return base + 8; }
    };
    auto f = &S::f;

    CHECK_NESTED(bind_fun(f(40), f, 40));
  }
}

#if SLB_HAS_CXX14_VARIABLE_TEMPLATES
// template <class T>
//   inline constexpr bool is_bind_expression_v = is_bind_expression<T>::value;
TEST_CASE("is_bind_expression_v", "[functional.syn]") {
  struct S {
    static int f(int base) { return base; }
  };

  using slb_bind_t = decltype(slb::bind(&S::f, 42));
  CHECK(slb::is_same<decltype(slb::is_bind_expression_v<slb_bind_t>),
                     const bool>::value);
  CHECK(slb::is_bind_expression_v<slb_bind_t> ==
        slb::is_bind_expression<slb_bind_t>::value);
  constexpr bool slb_bind_v = slb::is_bind_expression_v<slb_bind_t>;
  (void)slb_bind_v;

  using std_bind_t = decltype(std::bind(&S::f, 42));
  CHECK(std::is_same<decltype(slb::is_bind_expression_v<std_bind_t>),
                     const bool>::value);
  CHECK(slb::is_bind_expression_v<std_bind_t> ==
        slb::is_bind_expression<std_bind_t>::value);
  constexpr bool std_bind_v = slb::is_bind_expression_v<std_bind_t>;
  (void)std_bind_v;

  using slb_bind_r_t = decltype(slb::bind<int>(&S::f, 42));
  CHECK(slb::is_same<decltype(slb::is_bind_expression_v<slb_bind_r_t>),
                     const bool>::value);
  CHECK(slb::is_bind_expression_v<slb_bind_r_t> ==
        slb::is_bind_expression<slb_bind_r_t>::value);
  constexpr bool slb_bind_r_v = slb::is_bind_expression_v<slb_bind_r_t>;
  (void)slb_bind_r_v;

  using std_bind_r_t = decltype(std::bind<int>(&S::f, 42));
  CHECK(std::is_same<decltype(slb::is_bind_expression_v<std_bind_r_t>),
                     const bool>::value);
  CHECK(slb::is_bind_expression_v<std_bind_r_t> ==
        slb::is_bind_expression<std_bind_r_t>::value);
  constexpr bool std_bind_r_v = slb::is_bind_expression_v<std_bind_r_t>;
  (void)std_bind_r_v;

  CHECK(slb::is_bind_expression_v<int> == slb::is_bind_expression<int>::value);
  CHECK(std::is_same<decltype(slb::is_bind_expression_v<int>),
                     const bool>::value);
  CHECK(slb::is_bind_expression_v<int> == slb::is_bind_expression<int>::value);
  constexpr bool int_v = slb::is_bind_expression_v<int>;
  (void)int_v;
}

// template <class T>
//   inline constexpr int is_placeholder_v = is_placeholder<T>::value;
TEST_CASE("is_placeholder_v", "[functional.syn]") {
  using slb_ph1_t = decltype(slb::placeholders::_1);
  CHECK(std::is_same<decltype(slb::is_placeholder_v<slb_ph1_t>),
                     const int>::value);
  CHECK(slb::is_placeholder_v<slb_ph1_t> ==
        slb::is_placeholder<slb_ph1_t>::value);
  constexpr int slb_ph1_v = slb::is_placeholder_v<slb_ph1_t>;
  (void)slb_ph1_v;

  using std_ph1_t = decltype(std::placeholders::_1);
  CHECK(std::is_same<decltype(slb::is_placeholder_v<std_ph1_t>),
                     const int>::value);
  CHECK(slb::is_placeholder_v<std_ph1_t> ==
        slb::is_placeholder<std_ph1_t>::value);
  constexpr int std_ph1_v = slb::is_placeholder_v<std_ph1_t>;
  (void)std_ph1_v;

  using slb_ph2_t = decltype(slb::placeholders::_2);
  CHECK(std::is_same<decltype(slb::is_placeholder_v<slb_ph2_t>),
                     const int>::value);
  CHECK(slb::is_placeholder_v<slb_ph2_t> ==
        slb::is_placeholder<slb_ph2_t>::value);
  constexpr int slb_ph2_v = slb::is_placeholder_v<slb_ph2_t>;
  (void)slb_ph2_v;

  using std_ph2_t = decltype(std::placeholders::_2);
  CHECK(std::is_same<decltype(slb::is_placeholder_v<std_ph2_t>),
                     const int>::value);
  CHECK(slb::is_placeholder_v<std_ph2_t> ==
        slb::is_placeholder<std_ph2_t>::value);
  constexpr int std_ph2_v = slb::is_placeholder_v<std_ph2_t>;
  (void)std_ph2_v;

  CHECK(std::is_same<decltype(slb::is_placeholder_v<int>), const int>::value);
  CHECK(slb::is_placeholder_v<int> == slb::is_placeholder<int>::value);
  constexpr int int_v = slb::is_placeholder_v<int>;
  (void)int_v;
}
#endif
