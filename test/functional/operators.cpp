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

template <int Value>
struct C {};

#define SLB_DEFINE_UNARY_OPERATOR(Op)                                          \
  template <int Value>                                                         \
  constexpr C<(Op Value)> operator Op(C<Value>) {                              \
    return {};                                                                 \
  }
#define SLB_DEFINE_BINARY_OPERATOR(Op)                                         \
  template <int Left, int Right>                                               \
  constexpr C<(Left Op Right)> operator Op(C<Left>, C<Right>) {                \
    return {};                                                                 \
  }

SLB_DEFINE_BINARY_OPERATOR(+)
SLB_DEFINE_BINARY_OPERATOR(-)
SLB_DEFINE_BINARY_OPERATOR(*)
SLB_DEFINE_BINARY_OPERATOR(/)
SLB_DEFINE_BINARY_OPERATOR(%)
SLB_DEFINE_UNARY_OPERATOR(-)

SLB_DEFINE_BINARY_OPERATOR(==)
SLB_DEFINE_BINARY_OPERATOR(!=)
SLB_DEFINE_BINARY_OPERATOR(>)
SLB_DEFINE_BINARY_OPERATOR(<)
SLB_DEFINE_BINARY_OPERATOR(>=)
SLB_DEFINE_BINARY_OPERATOR(<=)

SLB_DEFINE_BINARY_OPERATOR(&&)
SLB_DEFINE_BINARY_OPERATOR(||)
SLB_DEFINE_UNARY_OPERATOR(!)

SLB_DEFINE_BINARY_OPERATOR(&)
SLB_DEFINE_BINARY_OPERATOR(|)
SLB_DEFINE_BINARY_OPERATOR (^)
SLB_DEFINE_UNARY_OPERATOR(~)

template <typename T>
struct _void {
  using type = void;
};

template <typename T, typename Enable = void>
struct has_is_transparent : std::false_type {};

template <typename T>
struct has_is_transparent<T, typename _void<typename T::is_transparent>::type>
    : std::true_type {};

// [arithmetic.operations], arithmetic operations

TEST_CASE("plus", "[arithmetic.operations]") {
  constexpr slb::plus<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), int>::value);
  CHECK(op(4, 2) == (4 + 2));
  constexpr int r = op(4, 2);
  (void)r;
}

TEST_CASE("plus<void>", "[arithmetic.operations]") {
  CHECK(std::is_same<slb::plus<>, slb::plus<void>>::value);

  constexpr slb::plus<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 + 2)>>::value);
  constexpr C<(4 + 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::plus<>>::value);
  }
}

TEST_CASE("minus", "[arithmetic.operations]") {
  constexpr slb::minus<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), int>::value);
  CHECK(op(4, 2) == (4 - 2));
  constexpr int r = op(4, 2);
  (void)r;
}

TEST_CASE("minus<void>", "[arithmetic.operations]") {
  CHECK(std::is_same<slb::minus<void>, slb::minus<>>::value);

  constexpr slb::minus<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 - 2)>>::value);
  constexpr C<(4 - 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::minus<>>::value);
  }
}

TEST_CASE("multiplies", "[arithmetic.operations]") {
  constexpr slb::multiplies<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), int>::value);
  CHECK(op(4, 2) == (4 * 2));
  constexpr int r = op(4, 2);
  (void)r;
}

TEST_CASE("multiplies<void>", "[arithmetic.operations]") {
  CHECK(std::is_same<slb::multiplies<void>, slb::multiplies<>>::value);

  constexpr slb::multiplies<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 * 2)>>::value);
  constexpr C<(4 * 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::multiplies<>>::value);
  }
}

TEST_CASE("divides", "[arithmetic.operations]") {
  constexpr slb::divides<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), int>::value);
  CHECK(op(4, 2) == (4 / 2));
  constexpr int r = op(4, 2);
  (void)r;
}

TEST_CASE("divides<void>", "[arithmetic.operations]") {
  CHECK(std::is_same<slb::divides<void>, slb::divides<>>::value);

  constexpr slb::divides<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 / 2)>>::value);
  constexpr C<(4 / 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::divides<>>::value);
  }
}

TEST_CASE("modulus", "[arithmetic.operations]") {
  constexpr slb::modulus<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), int>::value);
  CHECK(op(4, 2) == (4 % 2));
  constexpr int r = op(4, 2);
  (void)r;
}

TEST_CASE("modulus<void>", "[arithmetic.operations]") {
  CHECK(std::is_same<slb::modulus<void>, slb::modulus<>>::value);

  constexpr slb::modulus<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 % 2)>>::value);
  constexpr C<(4 % 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::modulus<>>::value);
  }
}

TEST_CASE("negate", "[arithmetic.operations]") {
  constexpr slb::negate<int> op{};
  CHECK(std::is_same<decltype(op(4)), int>::value);
  CHECK(op(4) == (-4));
  constexpr int r = op(4);
  (void)r;
}

TEST_CASE("negate<void>", "[arithmetic.operations]") {
  CHECK(std::is_same<slb::negate<void>, slb::negate<>>::value);

  constexpr slb::negate<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{})), C<(-4)>>::value);
  constexpr C<(-4)> r = op(C<4>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::negate<>>::value);
  }
}

// [comparisons], comparisons

template <typename T>
struct Convertible {
  T val;
  operator T() const { return val; }
};

struct MemberPtrComparison {
  void* ptr;
  bool operator>(void* uptr) const { return ptr > uptr; }
  bool operator<(void* uptr) const { return ptr < uptr; }
  bool operator>=(void* uptr) const { return ptr >= uptr; }
  bool operator<=(void* uptr) const { return ptr <= uptr; }
};

struct FreePtrComparison {
  void* ptr;
};
bool operator>(FreePtrComparison x, void* uptr) { return x.ptr > uptr; }
bool operator<(FreePtrComparison x, void* uptr) { return x.ptr < uptr; }
bool operator>=(FreePtrComparison x, void* uptr) { return x.ptr >= uptr; }
bool operator<=(FreePtrComparison x, void* uptr) { return x.ptr <= uptr; }

TEST_CASE("equal_to", "[comparisons]") {
  constexpr slb::equal_to<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), bool>::value);
  CHECK(op(4, 2) == (4 == 2));
  constexpr bool r = op(4, 2);
  (void)r;
}

TEST_CASE("equal_to<void>", "[comparisons]") {
  CHECK(std::is_same<slb::equal_to<void>, slb::equal_to<>>::value);

  constexpr slb::equal_to<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 == 2)>>::value);
  constexpr C<(4 == 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::equal_to<>>::value);
  }
}

TEST_CASE("not_equal_to", "[comparisons]") {
  constexpr slb::not_equal_to<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), bool>::value);
  CHECK(op(4, 2) == (4 != 2));
  constexpr bool r = op(4, 2);
  (void)r;
}

TEST_CASE("not_equal_to<void>", "[comparisons]") {
  CHECK(std::is_same<slb::not_equal_to<void>, slb::not_equal_to<>>::value);

  constexpr slb::not_equal_to<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 != 2)>>::value);
  constexpr C<(4 != 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::not_equal_to<>>::value);
  }
}

TEST_CASE("greater", "[comparisons]") {
  constexpr slb::greater<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), bool>::value);
  CHECK(op(4, 2) == (4 > 2));
  constexpr bool r = op(4, 2);
  (void)r;

  /* pointer type */ {
    static struct { int x, y; } s{0, 0};

    constexpr slb::greater<int*> pop{};
    CHECK(std::is_same<decltype(pop(&s.x, &s.y)), bool>::value);
    CHECK(pop(&s.x, &s.y) == (&s.x > &s.y));
  }
}

TEST_CASE("greater<void>", "[comparisons]") {
  CHECK(std::is_same<slb::greater<void>, slb::greater<>>::value);

  constexpr slb::greater<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 > 2)>>::value);
  constexpr C<(4 > 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* pointer type */ {
    static struct { int x, y; } s{0, 0};

    constexpr slb::greater<> pop{};
    CHECK(std::is_same<decltype(pop(&s.x, &s.y)), bool>::value);
    CHECK(pop(&s.x, &s.y) == (&s.x > &s.y));

    Convertible<int*> const cx{&s.x};
    Convertible<int*> const cy{&s.y};
    CHECK(pop(cx, cy) == (&s.x > &s.y));

    MemberPtrComparison const mx{&s.x};
    CHECK(pop(mx, &s.y) == (&s.x > &s.y));

    FreePtrComparison const fx{&s.x};
    CHECK(pop(fx, &s.y) == (&s.x > &s.y));
  }

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::greater<>>::value);
  }
}

TEST_CASE("less", "[comparisons]") {
  constexpr slb::less<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), bool>::value);
  CHECK(op(4, 2) == (4 < 2));
  constexpr bool r = op(4, 2);
  (void)r;

  /* pointer type */ {
    static struct { int x, y; } s{0, 0};

    constexpr slb::less<int*> pop{};
    CHECK(std::is_same<decltype(pop(&s.x, &s.y)), bool>::value);
  }
}

TEST_CASE("less<void>", "[comparisons]") {
  CHECK(std::is_same<slb::less<void>, slb::less<>>::value);

  constexpr slb::less<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 < 2)>>::value);
  constexpr C<(4 < 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* pointer type */ {
    static struct { int x, y; } s{0, 0};

    constexpr slb::less<> pop{};
    CHECK(std::is_same<decltype(pop(&s.x, &s.y)), bool>::value);
    CHECK(pop(&s.x, &s.y) == (&s.x < &s.y));

    Convertible<int*> const cx{&s.x};
    Convertible<int*> const cy{&s.y};
    CHECK(pop(cx, cy) == (&s.x < &s.y));

    MemberPtrComparison const mx{&s.x};
    CHECK(pop(mx, &s.y) == (&s.x < &s.y));

    FreePtrComparison const fx{&s.x};
    CHECK(pop(fx, &s.y) == (&s.x < &s.y));
  }

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::less<>>::value);
  }
}

TEST_CASE("greater_equal", "[comparisons]") {
  constexpr slb::greater_equal<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), bool>::value);
  CHECK(op(4, 2) == (4 >= 2));
  constexpr bool r = op(4, 2);
  (void)r;

  /* pointer type */ {
    static struct { int x, y; } s{0, 0};

    constexpr slb::greater_equal<int*> pop{};
    CHECK(std::is_same<decltype(pop(&s.x, &s.y)), bool>::value);
  }
}

TEST_CASE("greater_equal<void>", "[comparisons]") {
  CHECK(std::is_same<slb::greater_equal<void>, slb::greater_equal<>>::value);

  constexpr slb::greater_equal<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 >= 2)>>::value);
  constexpr C<(4 >= 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* pointer type */ {
    static struct { int x, y; } s{0, 0};

    constexpr slb::greater_equal<> pop{};
    CHECK(std::is_same<decltype(pop(&s.x, &s.y)), bool>::value);
    CHECK(pop(&s.x, &s.y) == (&s.x >= &s.y));

    Convertible<int*> const cx{&s.x};
    Convertible<int*> const cy{&s.y};
    CHECK(pop(cx, cy) == (&s.x >= &s.y));

    MemberPtrComparison const mx{&s.x};
    CHECK(pop(mx, &s.y) == (&s.x >= &s.y));

    FreePtrComparison const fx{&s.x};
    CHECK(pop(fx, &s.y) == (&s.x >= &s.y));
  }

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::greater_equal<>>::value);
  }
}

TEST_CASE("less_equal", "[comparisons]") {
  constexpr slb::less_equal<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), bool>::value);
  CHECK(op(4, 2) == (4 <= 2));
  constexpr bool r = op(4, 2);
  (void)r;

  /* pointer type */ {
    static struct { int x, y; } s{0, 0};

    constexpr slb::less_equal<int*> pop{};
    CHECK(std::is_same<decltype(pop(&s.x, &s.y)), bool>::value);
  }
}

TEST_CASE("less_equal<void>", "[comparisons]") {
  CHECK(std::is_same<slb::less_equal<void>, slb::less_equal<>>::value);

  constexpr slb::less_equal<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 <= 2)>>::value);
  constexpr C<(4 <= 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* pointer type */ {
    static struct { int x, y; } s{0, 0};

    constexpr slb::less_equal<> pop{};
    CHECK(std::is_same<decltype(pop(&s.x, &s.y)), bool>::value);
    CHECK(pop(&s.x, &s.y) == (&s.x <= &s.y));

    Convertible<int*> const cx{&s.x};
    Convertible<int*> const cy{&s.y};
    CHECK(pop(cx, cy) == (&s.x <= &s.y));

    MemberPtrComparison const mx{&s.x};
    CHECK(pop(mx, &s.y) == (&s.x <= &s.y));

    FreePtrComparison const fx{&s.x};
    CHECK(pop(fx, &s.y) == (&s.x <= &s.y));
  }

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::less_equal<>>::value);
  }
}

// [logical.operations], logical operations

TEST_CASE("logical_and", "[logical.operations]") {
  constexpr slb::logical_and<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), bool>::value);
  CHECK(op(4, 2) == true);     // -Wconstant-logical-operand
  constexpr bool r = op(4, 2); // -Wconstant-logical-operand
  (void)r;
}

TEST_CASE("logical_and<void>", "[logical.operations]") {
  CHECK(std::is_same<slb::logical_and<void>, slb::logical_and<>>::value);

  constexpr slb::logical_and<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 && 2)>>::value);
  constexpr C<true> r = op(C<4>{}, C<2>{}); // -Wconstant-logical-operand
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::logical_and<>>::value);
  }
}

TEST_CASE("logical_or", "[logical.operations]") {
  constexpr slb::logical_or<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), bool>::value);
  CHECK(op(4, 2) == (true));   // -Wconstant-logical-operand
  constexpr bool r = op(4, 2); // -Wconstant-logical-operand
  (void)r;
}

TEST_CASE("logical_or<void>", "[logical.operations]") {
  CHECK(std::is_same<slb::logical_or<void>, slb::logical_or<>>::value);

  constexpr slb::logical_or<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 || 2)>>::value);
  constexpr C<true> r = op(C<4>{}, C<2>{}); // -Wconstant-logical-operand
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::logical_or<>>::value);
  }
}

TEST_CASE("logical_not", "[logical.operations]") {
  constexpr slb::logical_not<int> op{};
  CHECK(std::is_same<decltype(op(4)), bool>::value);
  CHECK(op(4) == (!4));
  constexpr bool r = op(4);
  (void)r;
}

TEST_CASE("logical_not<void>", "[logical.operations]") {
  CHECK(std::is_same<slb::logical_not<void>, slb::logical_not<>>::value);

  constexpr slb::logical_not<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{})), C<(!4)>>::value);
  constexpr C<(!4)> r = op(C<4>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::logical_not<>>::value);
  }
}

// [bitwise.operations], bitwise operations

TEST_CASE("bit_and", "[bitwise.operations]") {
  constexpr slb::bit_and<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), int>::value);
  CHECK(op(4, 2) == (4 & 2));
  constexpr int r = op(4, 2);
  (void)r;
}

TEST_CASE("bit_and<void>", "[bitwise.operations]") {
  CHECK(std::is_same<slb::bit_and<void>, slb::bit_and<>>::value);

  constexpr slb::bit_and<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 & 2)>>::value);
  constexpr C<(4 & 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::bit_and<>>::value);
  }
}

TEST_CASE("bit_or", "[bitwise.operations]") {
  constexpr slb::bit_or<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), int>::value);
  CHECK(op(4, 2) == (4 | 2));
  constexpr int r = op(4, 2);
  (void)r;
}

TEST_CASE("bit_or<void>", "[bitwise.operations]") {
  CHECK(std::is_same<slb::bit_or<void>, slb::bit_or<>>::value);

  constexpr slb::bit_or<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 | 2)>>::value);
  constexpr C<(4 | 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::bit_or<>>::value);
  }
}

TEST_CASE("bit_xor", "[bitwise.operations]") {
  constexpr slb::bit_xor<int> op{};
  CHECK(std::is_same<decltype(op(4, 2)), int>::value);
  CHECK(op(4, 2) == (4 ^ 2));
  constexpr int r = op(4, 2);
  (void)r;
}

TEST_CASE("bit_xor<void>", "[bitwise.operations]") {
  CHECK(std::is_same<slb::bit_xor<void>, slb::bit_xor<>>::value);

  constexpr slb::bit_xor<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{}, C<2>{})), C<(4 ^ 2)>>::value);
  constexpr C<(4 ^ 2)> r = op(C<4>{}, C<2>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::bit_xor<>>::value);
  }
}

TEST_CASE("bit_not", "[bitwise.operations]") {
  constexpr slb::bit_not<int> op{};
  CHECK(std::is_same<decltype(op(4)), int>::value);
  CHECK(op(4) == ~4);
  constexpr int r = op(4);
  (void)r;
}

TEST_CASE("bit_not<void>", "[bitwise.operations]") {
  CHECK(std::is_same<slb::bit_not<void>, slb::bit_not<>>::value);

  constexpr slb::bit_not<void> op{};
  CHECK(std::is_same<decltype(op(C<4>{})), C<(~4)>>::value);
  constexpr C<(~4)> r = op(C<4>{});
  (void)r;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::bit_not<>>::value);
  }
}

TEST_CASE("identity", "[func.identity]") {
  constexpr slb::identity op{};

  int i = 4;
  CHECK(std::is_same<decltype(op(i)), int&>::value);
  CHECK(noexcept(op(i)));
  CHECK(&op(i) == &i);
  constexpr int rl = op(4);
  (void)rl;

  CHECK(std::is_same<decltype(op(4)), int&&>::value);
  CHECK(noexcept(op(4)));
  CHECK(op(4) == 4);
  constexpr int rr = op(4);
  (void)rr;

  /* using is_transparent = unspecified; */ {
    CHECK(has_is_transparent<slb::identity>::value);
  }
}
