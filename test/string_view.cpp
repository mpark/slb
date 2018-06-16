/*
  SLB.String_View

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_INSTANTIATION

#include <slb/string_view.hpp>

#include <cstddef>
#include <functional>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "catch.hpp"

#if (__cpp_lib_array_constexpr && __cplusplus >= 201703L) ||                   \
    (_LIBCPP_VERSION >= 4000 && _LIBCPP_STD_VER > 17) ||                       \
    (defined(_MSC_VER) && _MSC_VER >= 1914 && _HAS_CXX17)
#define SLB_CONSTEXPR_REVERSE_ITERATOR constexpr
#else
#define SLB_CONSTEXPR_REVERSE_ITERATOR
#endif

#define CHECK_CXX14_CONSTEXPR(Tag, ...)                                        \
  struct check_cxx14_constexpr_##Tag {                                         \
    static SLB_CXX14_CONSTEXPR bool check(){__VA_ARGS__};                      \
  };                                                                           \
  CHECK(check_cxx14_constexpr_##Tag::check())

#define CHECK_NESTED(...)                                                      \
  do {                                                                         \
    INFO(__FILE__ "(" << __LINE__ << "): " #__VA_ARGS__);                      \
    check_##__VA_ARGS__;                                                       \
  } while (false)

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept {
  return N;
}

// [string.view.io], inserters and extractors
template <typename CharT, typename string_view = slb::basic_string_view<CharT>>
void check_inserter() {
  static constexpr CharT str[] = {'f', 'o', 'o'};

  string_view const sv(str, countof(str));
  std::basic_ostringstream<CharT> stream;
  stream << sv;

  auto const s = stream.str();
  CHECK(string_view(s.c_str()) == sv);

  /* padding */
  {
    static constexpr CharT lstr[] = {'.', '.', 'f', 'o', 'o'};
    static constexpr CharT rstr[] = {'f', 'o', 'o', '.', '.'};

    std::basic_ostringstream<CharT> lstream;
    lstream.setf(std::ios_base::left, std::ios_base::adjustfield);
    lstream.fill('.');
    lstream.width(5);
    lstream << sv;

    auto const ls = lstream.str();
    CHECK(string_view(ls.c_str()) == string_view(lstr, 5));
    CHECK(lstream.width() == 0);

    std::basic_ostringstream<CharT> rstream;
    rstream.setf(std::ios_base::right, std::ios_base::adjustfield);
    rstream.fill('.');
    rstream.width(5);
    rstream << sv;

    auto const rs = rstream.str();
    CHECK(string_view(rs.c_str()) == string_view(rstr, 5));
    CHECK(rstream.width() == 0);

    std::basic_ostringstream<CharT> istream;
    istream.setf(std::ios_base::internal, std::ios_base::adjustfield);
    istream.fill('.');
    istream.width(5);
    istream << sv;

    auto const is = istream.str();
    CHECK(string_view(is.c_str()) == string_view(rstr, 5));
    CHECK(istream.width() == 0);
  }
}

TEST_CASE("basic_string_view(inserters-and-extractors)", "[string.view.io]") {
  CHECK_NESTED(inserter<char>());
  //! CHECK_NESTED(inserter<char16_t>());
  //! CHECK_NESTED(inserter<char32_t>());
  CHECK_NESTED(inserter<wchar_t>());
}

// basic_string_view typedef names
TEST_CASE("basic_string_view(typedef-names)", "[string.view.synop]") {
  /* using string_view = basic_string_view<char>; */ {
    CHECK(std::is_same<slb::string_view, slb::basic_string_view<char>>::value);
    CHECK(std::is_same<
          slb::string_view,
          slb::basic_string_view<char, std::char_traits<char>>>::value);
  }

  /* using u16string_view = basic_string_view<char16_t>; */ {
    CHECK(std::is_same<slb::u16string_view,
                       slb::basic_string_view<char16_t>>::value);
    CHECK(std::is_same<
          slb::u16string_view,
          slb::basic_string_view<char16_t, std::char_traits<char16_t>>>::value);
  }

  /* using u32string_view = basic_string_view<char32_t>; */ {
    CHECK(std::is_same<slb::u32string_view,
                       slb::basic_string_view<char32_t>>::value);
    CHECK(std::is_same<
          slb::u32string_view,
          slb::basic_string_view<char32_t, std::char_traits<char32_t>>>::value);
  }

  /* using wstring_view = basic_string_view<wchar_t>; */ {
    CHECK(std::is_same<slb::wstring_view,
                       slb::basic_string_view<wchar_t>>::value);
    CHECK(std::is_same<
          slb::wstring_view,
          slb::basic_string_view<wchar_t, std::char_traits<wchar_t>>>::value);
  }
}

// [string.view.hash], hash support
template <typename CharT, typename string_view = slb::basic_string_view<CharT>>
void check_hash() {
  static constexpr CharT str[] = {'f', 'o', 'o'};

  slb::hash<string_view> const h{};
  string_view const sv1(str, countof(str));
  string_view const sv2(str, countof(str));

  CHECK(std::is_same<decltype(h(sv1)), std::size_t>::value);
  CHECK(noexcept(h(sv1)));
  std::size_t const r = h(sv1);
  CHECK(h(sv2) == r);

  /* enabled */ {
    using hasher = slb::hash<string_view>;

    CHECK(std::is_default_constructible<hasher>::value);
    CHECK(std::is_copy_constructible<hasher>::value);
    CHECK(std::is_move_constructible<hasher>::value);
    CHECK(std::is_copy_assignable<hasher>::value);
    CHECK(std::is_move_assignable<hasher>::value);
    CHECK(std::is_destructible<hasher>::value);
  }

  /* std-compatible */ {
    std::hash<string_view> const std_h{};

    CHECK(std_h(sv2) == r);
  }
}

TEST_CASE("basic_string_view(hash-support)", "[string.view.hash]") {
  CHECK_NESTED(hash<char>());
  CHECK_NESTED(hash<char16_t>());
  CHECK_NESTED(hash<char32_t>());
  CHECK_NESTED(hash<wchar_t>());
}

// [string.view.literals], suffix for basic_string_view literals
TEST_CASE("basic_string_view(literals)", "[string.view.literals]") {
  using namespace slb::literals::string_view_literals;

  CHECK(std::is_same<decltype("foo"_sv), slb::string_view>::value);
  CHECK("foo"_sv == slb::string_view{"foo", 3});
  constexpr slb::string_view sv = "foo"_sv;
  (void)sv;

  CHECK(std::is_same<decltype(u"foo"_sv), slb::u16string_view>::value);
  CHECK(u"foo"_sv == slb::u16string_view{u"foo", 3});
  constexpr slb::u16string_view u16sv = u"foo"_sv;
  (void)u16sv;

  CHECK(std::is_same<decltype(U"foo"_sv), slb::u32string_view>::value);
  CHECK(U"foo"_sv == slb::u32string_view{U"foo", 3});
  constexpr slb::u32string_view u32sv = U"foo"_sv;
  (void)u32sv;

  CHECK(std::is_same<decltype(L"foo"_sv), slb::wstring_view>::value);
  CHECK(L"foo"_sv == slb::wstring_view{L"foo", 3});
  constexpr slb::wstring_view wsv = L"foo"_sv;
  (void)wsv;
}

// [string.view.template], class template basic_string_view

struct CharT {
  char val;
  constexpr CharT(char val = 0) : val(val) {}
};

struct Traits {
  using char_type = CharT;

  static SLB_CXX14_CONSTEXPR void assign(CharT& r, CharT const& d) noexcept {
    r = d;
  }

  static constexpr bool eq(CharT c, CharT d) noexcept {
    return static_cast<unsigned char>(c.val) ==
           static_cast<unsigned char>(d.val);
  }

  static constexpr bool lt(CharT c, CharT d) noexcept {
    return static_cast<unsigned char>(c.val) <
           static_cast<unsigned char>(d.val);
  }

  static constexpr int compare(CharT const* p, CharT const* q, std::size_t n) {
    return n == 0 ? 0
                  : eq(*p, *q) ? compare(p + 1, q + 1, n - 1)
                               : lt(*p, *q) ? -1 : 1;
  }

  static constexpr std::size_t length(CharT const* p) {
    return eq(*p, CharT()) ? 0 : length(p + 1) + 1;
  }

  static CharT* copy(CharT* s, CharT const* p, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
      assign(s[i], p[i]);
    return s;
  }
};

#define SLB_INSTANTIATION "basic_string_view<CharT, Traits>"
#define SLB_CONSTEXPR_CHAR_TRAITS constexpr
#define SLB_CXX14_CONSTEXPR_CHAR_TRAITS SLB_CXX14_CONSTEXPR
#include "string_view.cpp"
#undef SLB_INSTANTIATION
#undef SLB_CONSTEXPR_CHAR_TRAITS
#undef SLB_CXX14_CONSTEXPR_CHAR_TRAITS

#define SLB_CONSTEXPR_CHAR_TRAITS
#define SLB_CXX14_CONSTEXPR_CHAR_TRAITS

// basic_string_view<char, std::char_traits<char>>
#define SLB_INSTANTIATION "string_view"
namespace test_string_view {
using CharT = char;
using Traits = std::char_traits<char>;
#include "string_view.cpp"
} // namespace test_string_view
#undef SLB_INSTANTIATION

// basic_string_view<char16_t, std::char_traits<char16_t>>
#define SLB_INSTANTIATION "u16string_view"
namespace test_u16string_view {
using CharT = char16_t;
using Traits = std::char_traits<char16_t>;
#include "string_view.cpp"
} // namespace test_u16string_view
#undef SLB_INSTANTIATION

// basic_string_view<char32_t, std::char_traits<char32_t>>
#define SLB_INSTANTIATION "u32string_view"
namespace test_u32string_view {
using CharT = char32_t;
using Traits = std::char_traits<char32_t>;
#include "string_view.cpp"
} // namespace test_u32string_view
#undef SLB_INSTANTIATION

// basic_string_view<wchar_t, std::char_traits<wchar_t>>
#define SLB_INSTANTIATION "wstring_view"
namespace test_wstring_view {
using CharT = wchar_t;
using Traits = std::char_traits<wchar_t>;
#include "string_view.cpp"
} // namespace test_wstring_view
#undef SLB_INSTANTIATION

#else

using string_view = slb::basic_string_view<CharT, Traits>;

TEST_CASE(SLB_INSTANTIATION, "[string.view.template]") {
  /* using traits_type = traits; */ {
    CHECK(std::is_same<string_view::traits_type, Traits>::value);
    CHECK(std::is_same<slb::basic_string_view<CharT>::traits_type,
                       std::char_traits<CharT>>::value);
  }

  /* using value_type = charT; */ {
    CHECK(std::is_same<string_view::value_type, CharT>::value);
  }

  /* using pointer = value_type*; */
  /* using const_pointer = const value_type*; */ {
    CHECK(std::is_same<string_view::pointer, CharT*>::value);
    CHECK(std::is_same<string_view::const_pointer, CharT const*>::value);
  }

  /* using reference = value_type&; */
  /* using const_reference = const value_type&; */ {
    CHECK(std::is_same<string_view::reference, CharT&>::value);
    CHECK(std::is_same<string_view::const_reference, CharT const&>::value);
  }

  /* using const_iterator = implementation-defined; */
  /* using iterator = const_iterator; */ {
    // A type that meets the requirements of a constant random access iterator
    // (27.2.7) and of a contiguous iterator (27.2.1) whose value_type is the
    // template parameter charT.
    using iter_traits = std::iterator_traits<string_view::const_iterator>;
    CHECK(std::is_same<iter_traits::value_type, CharT>::value);
    CHECK(std::is_same<iter_traits::pointer, CharT const*>::value);
    CHECK(std::is_same<iter_traits::reference, CharT const&>::value);
    CHECK(std::is_same<iter_traits::iterator_category,
                       std::random_access_iterator_tag>::value);

    CHECK(std::is_same<string_view::iterator,
                       string_view::const_iterator>::value);

    // CHECK(contiguous iterator?)
  }

  /* using const_reverse_iterator = reverse_iterator<const_iterator>; */
  /* using reverse_iterator = const_reverse_iterator; */ {
    CHECK(std::is_same<
          string_view::const_reverse_iterator,
          std::reverse_iterator<string_view::const_iterator>>::value);
    CHECK(std::is_same<string_view::reverse_iterator,
                       string_view::const_reverse_iterator>::value);
  }

  /* using size_type = size_t; */ {
    CHECK(std::is_same<string_view::size_type, std::size_t>::value);
  }

  /* using difference_type = ptrdiff_t; */ {
    CHECK(std::is_same<string_view::difference_type, std::ptrdiff_t>::value);
  }

  /* static constexpr size_type npos = size_type(-1); */ {
    CHECK(std::is_same<decltype(string_view::npos),
                       string_view::size_type const>::value);
    CHECK(string_view::npos == string_view::size_type(-1));
    constexpr string_view::size_type npos = string_view::npos;
    (void)npos;
  }
}

// [string.view.cons], construction and assignment
TEST_CASE(SLB_INSTANTIATION "::basic_string_view", "[string.view.cons]") {
  static constexpr CharT str[] = {'f', 'o', 'o', '\0', 'b', 'a', 'r'};

  /* constexpr basic_string_view() noexcept; */ {
    constexpr string_view sv;
    CHECK(noexcept(string_view()));
    CHECK(sv.data() == nullptr);
    CHECK(sv.size() == 0);

    /* implicit */ {
      constexpr string_view isv = {};
      CHECK(isv.data() == nullptr);
      CHECK(isv.size() == 0);
    }
  }

  /* constexpr basic_string_view(const charT* str); */ {
    SLB_CONSTEXPR_CHAR_TRAITS string_view sv(str);
    CHECK(sv.data() == str);
    CHECK(sv.size() == Traits::length(str));

    /* implicit */ {
      SLB_CONSTEXPR_CHAR_TRAITS string_view isv = str;
      CHECK(isv.data() == str);
      CHECK(isv.size() == Traits::length(str));
    }
  }

  /* constexpr basic_string_view(const charT* str, size_type len); */ {
    constexpr string_view sv(str, countof(str));
    CHECK(sv.data() == str);
    CHECK(sv.size() == countof(str));

    /* implicit */ {
      constexpr string_view isv = {str, countof(str)};
      CHECK(isv.data() == str);
      CHECK(isv.size() == countof(str));
    }
  }

  /* constexpr basic_string_view(const basic_string_view&) noexcept = default;
   */
  {
    constexpr string_view src(str, countof(str));
    constexpr string_view sv(src);
    CHECK(noexcept(string_view(src)));
    CHECK(sv.data() == src.data());
    CHECK(sv.size() == src.size());
  }
}

TEST_CASE(SLB_INSTANTIATION "::operator=", "[string.view.cons]") {
  static constexpr CharT str[] = {'f', 'o', 'o'};

  // constexpr basic_string_view& operator=(const basic_string_view&) noexcept =
  //     default;
  constexpr string_view src(str, countof(str));
  string_view sv;
  sv = src;
  CHECK(noexcept(sv = src));
  CHECK(sv.data() == src.data());
  CHECK(sv.size() == src.size());

  CHECK_CXX14_CONSTEXPR(assign_op, {
    string_view src(str, countof(str));
    string_view sv;
    sv = src;
    return sv.data() == src.data() && sv.size() == src.size();
  });
}

// [string.view.iterators], iterator support
TEST_CASE(SLB_INSTANTIATION "(iterator-support)", "[string.view.iterators]") {
  static constexpr CharT str[] = {'f', 'o', 'o'};

  /* constexpr const_iterator begin() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(
        std::is_same<decltype(sv.begin()), string_view::const_iterator>::value);
    CHECK(noexcept(sv.begin()));
    CHECK(&*sv.begin() == sv.data());
    constexpr auto begin = sv.begin();
    (void)begin;

    constexpr string_view esv;
    CHECK(esv.begin() == esv.end());
  }

  /* constexpr const_iterator cbegin() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.cbegin()),
                       string_view::const_iterator>::value);
    CHECK(noexcept(sv.cbegin()));
    CHECK(sv.cbegin() == sv.begin());
    constexpr auto cbegin = sv.cbegin();
    (void)cbegin;

    constexpr string_view esv;
    CHECK(esv.cbegin() == esv.cend());
  }

  /* constexpr const_iterator end() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.end()), string_view::const_iterator>::value);
    CHECK(noexcept(sv.end()));
    CHECK(sv.end() == sv.begin() + sv.size());
    constexpr auto end = sv.end();
    (void)end;
  }

  /* constexpr const_iterator cend() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(
        std::is_same<decltype(sv.cend()), string_view::const_iterator>::value);
    CHECK(noexcept(sv.cend()));
    CHECK(sv.cend() == sv.end());
    constexpr auto cend = sv.cend();
    (void)cend;
  }

  /* constexpr const_reverse_iterator rbegin() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.rbegin()),
                       string_view::const_reverse_iterator>::value);
    CHECK(noexcept(sv.rbegin()));
    CHECK(sv.rbegin().base() == sv.end());
    SLB_CONSTEXPR_REVERSE_ITERATOR auto rbegin = sv.rbegin();
    (void)rbegin;
  }

  /* constexpr const_reverse_iterator crbegin() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    SLB_CONSTEXPR_REVERSE_ITERATOR auto crbegin = sv.crbegin();
    (void)crbegin;
  }

  /* constexpr const_reverse_iterator rend() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.rend()),
                       string_view::const_reverse_iterator>::value);
    CHECK(noexcept(sv.rend()));
    CHECK(sv.rend().base() == sv.begin());
    SLB_CONSTEXPR_REVERSE_ITERATOR auto rend = sv.rend();
    (void)rend;
  }

  /* constexpr const_reverse_iterator crend() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.crend()),
                       string_view::const_reverse_iterator>::value);
    CHECK(noexcept(sv.crend()));
    CHECK(sv.crend() == sv.rend());
    SLB_CONSTEXPR_REVERSE_ITERATOR auto crend = sv.crend();
    (void)crend;
  }
}

// [string.view.capacity], capacity
TEST_CASE(SLB_INSTANTIATION "(capacity)", "[string.view.capacity]") {
  static constexpr CharT str[] = {'f', 'o', 'o'};

  /* constexpr size_type size() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.size()), string_view::size_type>::value);
    CHECK(noexcept(sv.size()));
    CHECK(sv.size() == countof(str));
    constexpr auto size = sv.size();
    (void)size;
  }

  /* constexpr size_type length() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.length()), string_view::size_type>::value);
    CHECK(noexcept(sv.length()));
    CHECK(sv.length() == sv.size());
    constexpr auto length = sv.length();
    (void)length;
  }

  /* constexpr size_type max_size() const noexcept; */ {
    constexpr string_view sv;
    CHECK(std::is_same<decltype(sv.max_size()), string_view::size_type>::value);
    CHECK(noexcept(sv.max_size()));
    CHECK(sv.max_size() > 0);
    constexpr auto max_size = sv.max_size();
    (void)max_size;
  }

  /* [[nodiscard]] constexpr bool empty() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.empty()), bool>::value);
    CHECK(noexcept(sv.empty()));
    CHECK_FALSE(sv.empty());
    constexpr auto empty = sv.empty();
    (void)empty;

    constexpr string_view esv;
    CHECK(esv.empty());
  }
}

// [string.view.access], element access
TEST_CASE(SLB_INSTANTIATION "(element-access)", "[string.view.access]") {
  static constexpr CharT str[] = {'f', 'o', 'o'};

  /* constexpr const_reference operator[](size_type pos) const; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv[0]), string_view::const_reference>::value);
    CHECK(&sv[0] == &str[0]);
    CHECK(&sv[1] == &str[1]);
    CHECK(&sv[2] == &str[2]);
    constexpr auto&& subscript_op = sv[0];
    (void)subscript_op;
  }

  /* constexpr const_reference at(size_type pos) const; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(
        std::is_same<decltype(sv.at(0)), string_view::const_reference>::value);
    CHECK(&sv.at(0) == &str[0]);
    CHECK(&sv.at(1) == &str[1]);
    CHECK(&sv.at(2) == &str[2]);
    CHECK_THROWS_AS(sv.at(sv.size()), std::out_of_range);
    constexpr auto&& at = sv.at(0);
    (void)at;
  }

  /* constexpr const_reference front() const; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.front()),
                       string_view::const_reference>::value);
    CHECK(&sv.front() == &str[0]);
    constexpr auto&& front = sv.front();
    (void)front;
  }

  /* constexpr const_reference back() const; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(
        std::is_same<decltype(sv.back()), string_view::const_reference>::value);
    CHECK(&sv.back() == &str[countof(str) - 1]);
    constexpr auto&& back = sv.back();
    (void)back;
  }

  /* constexpr const_pointer data() const noexcept; */ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.data()), string_view::const_pointer>::value);
    CHECK(noexcept(sv.data()));
    CHECK(sv.data() == str);
    constexpr auto* data = sv.data();
    (void)data;

    constexpr string_view esv;
    CHECK(esv.data() == nullptr);
  }
}

// [string.view.modifiers], modifiers
TEST_CASE(SLB_INSTANTIATION "(modifiers)", "[string.view.modifiers]") {
  static constexpr CharT str[] = {'f', 'o', 'o'};

  /* constexpr void remove_prefix(size_type n); */ {
    string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.remove_prefix(2)), void>::value);
    sv.remove_prefix(2);
    CHECK(sv.data() == str + 2);
    CHECK(sv.size() == countof(str) - 2);

    CHECK_CXX14_CONSTEXPR(remove_prefix, {
      string_view sv(str, countof(str));
      sv.remove_prefix(2);
      return sv.data() == str + 2 && sv.size() == countof(str) - 2;
    });
  }

  /* constexpr void remove_suffix(size_type n); */ {
    string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.remove_suffix(2)), void>::value);
    sv.remove_suffix(2);
    CHECK(sv.data() == str);
    CHECK(sv.size() == countof(str) - 2);

    CHECK_CXX14_CONSTEXPR(remove_suffix, {
      string_view sv(str, countof(str));
      sv.remove_suffix(2);
      return sv.data() == str && sv.size() == countof(str) - 2;
    });
  }

  /* constexpr void swap(basic_string_view& s) noexcept; */ {
    string_view sv1(str, countof(str)), sv2;
    CHECK(std::is_same<decltype(sv1.swap(sv2)), void>::value);
    CHECK(noexcept(sv1.swap(sv2)));
    sv1.swap(sv2);
    CHECK(sv1.data() == nullptr);
    CHECK(sv1.size() == 0);
    CHECK(sv2.data() == str);
    CHECK(sv2.size() == countof(str));

    CHECK_CXX14_CONSTEXPR(swap, {
      string_view sv1(str, countof(str)), sv2;
      sv1.swap(sv2);
      return sv1.data() == nullptr && sv1.size() == 0 && sv2.data() == str &&
             sv2.size() == countof(str);
    });
  }
}

// [string.view.ops], string operations
TEST_CASE(SLB_INSTANTIATION "(string-operations)", "[string.view.ops]") {
  static constexpr CharT str_smaller[] = {'f', 'o'};
  static constexpr CharT str_less[] = {'f', 'o', 'a'};
  static constexpr CharT str[] = {'f', 'o', 'o'};
  static constexpr CharT str_greater[] = {'f', 'o', 'z'};
  static constexpr CharT str_bigger[] = {'f', 'o', 'o', 'o'};

  /* size_type copy(charT* s, size_type n, size_type pos = 0) const;*/ {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.copy(nullptr, 0)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.copy(nullptr, 0, sv.size())),
                       string_view::size_type>::value);
    CHECK(sv.copy(nullptr, 0) == 0);
    CHECK(sv.copy(nullptr, 0, sv.size()) == 0);
    CHECK_THROWS_AS(sv.copy(nullptr, 0, sv.size() + 1), std::out_of_range);

    CharT buf[3] = {'b', 'a', 'r'};
    CHECK(sv.copy(buf, sv.npos) == sv.size());
    CHECK(Traits::eq(buf[0], str[0]));
    CHECK(Traits::eq(buf[1], str[1]));
    CHECK(Traits::eq(buf[2], str[2]));

    buf[0] = 'b', buf[1] = 'a', buf[2] = 'r';
    CHECK(sv.copy(buf, sv.npos, 0) == sv.size());
    CHECK(Traits::eq(buf[0], str[0]));
    CHECK(Traits::eq(buf[1], str[1]));
    CHECK(Traits::eq(buf[2], str[2]));

    buf[0] = 'b', buf[1] = 'a', buf[2] = 'r';
    CHECK(sv.copy(buf, sv.npos, 1) == sv.size() - 1);
    CHECK(Traits::eq(buf[0], str[1]));
    CHECK(Traits::eq(buf[1], str[2]));
    CHECK(Traits::eq(buf[2], 'r'));

    buf[0] = 'b', buf[1] = 'a', buf[2] = 'r';
    CHECK(sv.copy(buf, sv.size() - 1, 0) == sv.size() - 1);
    CHECK(Traits::eq(buf[0], str[0]));
    CHECK(Traits::eq(buf[1], str[1]));
    CHECK(Traits::eq(buf[2], 'r'));
  }

  /* constexpr basic_string_view substr(size_type pos = 0, size_type n = npos)
      const; */
  {
    constexpr string_view sv(str, countof(str));
    CHECK(std::is_same<decltype(sv.substr()), string_view>::value);
    CHECK(std::is_same<decltype(sv.substr(0)), string_view>::value);
    CHECK(std::is_same<decltype(sv.substr(0, sv.npos)), string_view>::value);
    CHECK_THROWS_AS(sv.substr(sv.size() + 1, sv.npos), std::out_of_range);

    constexpr string_view sub0 = sv.substr();
    CHECK(sub0.data() == sv.data());
    CHECK(sub0.size() == sv.size());

    constexpr string_view sub1 = sv.substr(0);
    CHECK(sub1.data() == sv.data());
    CHECK(sub1.size() == sv.size());

    constexpr string_view sub2 = sv.substr(0, sv.npos);
    CHECK(sub2.data() == sv.data());
    CHECK(sub2.size() == sv.size());

    /*constexpr*/ string_view sub3 = sv.substr(1, sv.npos); // confuses gcc
    CHECK(sub3.data() == sv.data() + 1);
    CHECK(sub3.size() == sv.size() - 1);

    constexpr string_view sub4 = sv.substr(sv.size(), sv.npos);
    CHECK(sub4.data() == sv.data() + sv.size());
    CHECK(sub4.size() == 0u);

    constexpr string_view sub5 = sv.substr(0, sv.size() - 1);
    CHECK(sub5.data() == sv.data());
    CHECK(sub5.size() == sv.size() - 1);

    constexpr string_view sub6 = sv.substr(0, 1);
    CHECK(sub6.data() == sv.data());
    CHECK(sub6.size() == 1);
  }

  /* constexpr int compare(basic_string_view str) const noexcept; */
  /* constexpr int compare(const charT* s) const; */
  {
    static constexpr CharT strx[] = {'f', 'o', 'o', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv.compare(x)), int>::value);
    CHECK(noexcept(sv.compare(x)));
    CHECK(sv.compare(x) == 0);
    SLB_CONSTEXPR_CHAR_TRAITS int compare = sv.compare(x);
    (void)compare;

    constexpr string_view smaller(str_smaller, countof(str_smaller));
    CHECK(smaller.compare(sv) < 0);
    constexpr string_view less(str_less, countof(str_less));
    CHECK(less.compare(sv) < 0);
    constexpr string_view greater(str_greater, countof(str_greater));
    CHECK(greater.compare(sv) > 0);
    constexpr string_view bigger(str_bigger, countof(str_bigger));
    CHECK(bigger.compare(sv) > 0);

    CHECK(std::is_same<decltype(sv.compare(strx)), int>::value);
    CHECK(sv.compare(strx) == 0);
    SLB_CONSTEXPR_CHAR_TRAITS int compare_sz = sv.compare(strx);
    (void)compare_sz;

    CHECK(smaller.compare(strx) < 0);
    CHECK(less.compare(strx) < 0);
    CHECK(greater.compare(strx) > 0);
    CHECK(bigger.compare(strx) > 0);
  }

  /* constexpr int compare(size_type pos1, size_type n1, basic_string_view str)
         const; */
  /* constexpr int compare(size_type pos1, size_type n1, const charT* s) const;
   */
  {
    static constexpr CharT strx[] = {'o', 'o', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv.compare(1, 2, x)), int>::value);
    CHECK(sv.compare(1, 2, x) == 0);
    SLB_CONSTEXPR_CHAR_TRAITS int compare = sv.compare(1, 2, x);
    (void)compare;

    constexpr string_view smaller(str_smaller, countof(str_smaller));
    CHECK(smaller.compare(1, 1, x) < 0);
    constexpr string_view less(str_less, countof(str_less));
    CHECK(less.compare(1, 2, x) < 0);
    constexpr string_view greater(str_greater, countof(str_greater));
    CHECK(greater.compare(1, 2, x) > 0);
    constexpr string_view bigger(str_bigger, countof(str_bigger));
    CHECK(bigger.compare(1, 3, x) > 0);

    CHECK(std::is_same<decltype(sv.compare(1, 2, strx)), int>::value);
    CHECK(sv.compare(1, 2, strx) == 0);
    SLB_CONSTEXPR_CHAR_TRAITS int compare_sz = sv.compare(1, 2, strx);
    (void)compare_sz;

    CHECK(smaller.compare(1, 1, strx) < 0);
    CHECK(less.compare(1, 2, strx) < 0);
    CHECK(greater.compare(1, 2, strx) > 0);
    CHECK(bigger.compare(1, 3, strx) > 0);
  }

  /* constexpr int compare(size_type pos1, size_type n1, basic_string_view str,
         size_type pos2, size_type n2) const; */
  /* constexpr int compare(
         size_type pos1, size_type n1, const charT* s, size_type n2) const; */
  {
    static constexpr CharT strx[] = {'b', 'o', 'o', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv.compare(1, 2, x, 1, 2)), int>::value);
    CHECK(sv.compare(1, 2, x, 1, 2) == 0);
    SLB_CONSTEXPR_CHAR_TRAITS int compare = sv.compare(1, 2, x, 1, 2);
    (void)compare;

    constexpr string_view smaller(str_smaller, countof(str_smaller));
    CHECK(smaller.compare(1, 1, sv, 1, 2) < 0);
    constexpr string_view less(str_less, countof(str_less));
    CHECK(less.compare(1, 2, sv, 1, 2) < 0);
    constexpr string_view greater(str_greater, countof(str_greater));
    CHECK(greater.compare(1, 2, sv, 1, 2) > 0);
    constexpr string_view bigger(str_bigger, countof(str_bigger));
    CHECK(bigger.compare(1, 3, sv, 1, 2) > 0);

    CHECK(std::is_same<decltype(sv.compare(1, 2, strx, 1, 2)), int>::value);
    CHECK(sv.compare(1, 2, strx, 1, 2) == 0);
    SLB_CONSTEXPR_CHAR_TRAITS int compare_sz = sv.compare(1, 2, strx, 1, 2);
    (void)compare_sz;

    CHECK(smaller.compare(1, 1, strx, 1, 2) < 0);
    CHECK(less.compare(1, 2, strx, 1, 2) < 0);
    CHECK(greater.compare(1, 2, strx, 1, 2) > 0);
    CHECK(bigger.compare(1, 3, strx, 1, 2) > 0);
  }

  /* constexpr bool starts_with(basic_string_view x) const noexcept; */
  /* constexpr bool starts_with(const charT* x) const; */
  {
    static constexpr CharT strx[] = {'f', 'o', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv.starts_with(x)), bool>::value);
    CHECK(noexcept(sv.starts_with(x)));
    CHECK(sv.starts_with(x));
    SLB_CONSTEXPR_CHAR_TRAITS bool starts_with = sv.starts_with(x);
    (void)starts_with;

    CHECK_FALSE(sv.starts_with(string_view(str + 1, countof(str) - 1)));
    CHECK(sv.starts_with(string_view()));

    CHECK(std::is_same<decltype(sv.starts_with(strx)), bool>::value);
    CHECK(sv.starts_with(strx));
    SLB_CONSTEXPR_CHAR_TRAITS bool starts_with_sz = sv.starts_with(strx);
    (void)starts_with_sz;

    static constexpr CharT strnx[] = {'o', 'o', '\0'};
    static constexpr CharT stre[] = {'\0'};
    CHECK_FALSE(sv.starts_with(strnx));
    CHECK(sv.starts_with(stre));
  }

  /* constexpr bool starts_with(charT x) const noexcept; */
  {
    constexpr string_view sv(str, countof(str));
    constexpr CharT chx = 'f';
    CHECK(std::is_same<decltype(sv.starts_with(chx)), bool>::value);
    CHECK(noexcept(sv.starts_with(chx)));
    CHECK(sv.starts_with(chx));
    SLB_CONSTEXPR_CHAR_TRAITS bool starts_with = sv.starts_with(chx);
    (void)starts_with;

    CHECK_FALSE(sv.starts_with('o'));
  }

  /* constexpr bool ends_with(basic_string_view x) const noexcept; */
  /* constexpr bool ends_with(const charT* x) const; */
  {
    static constexpr CharT strx[] = {'o', 'o', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv.ends_with(x)), bool>::value);
    CHECK(noexcept(sv.ends_with(x)));
    CHECK(sv.ends_with(x));
    SLB_CONSTEXPR_CHAR_TRAITS bool ends_with = sv.ends_with(x);
    (void)ends_with;

    CHECK_FALSE(sv.ends_with(string_view(str, countof(str) - 1)));
    CHECK(sv.ends_with(string_view()));

    CHECK(std::is_same<decltype(sv.ends_with(strx)), bool>::value);
    CHECK(sv.ends_with(strx));
    SLB_CONSTEXPR_CHAR_TRAITS bool ends_with_sz = sv.ends_with(strx);
    (void)ends_with_sz;

    static constexpr CharT strnx[] = {'f', 'o', '\0'};
    static constexpr CharT stre[] = {'\0'};
    CHECK_FALSE(sv.ends_with(strnx));
    CHECK(sv.ends_with(stre));
  }

  /* constexpr bool ends_with(charT x) const noexcept; */
  {
    constexpr string_view sv(str, countof(str));
    constexpr CharT chx = 'o';
    CHECK(std::is_same<decltype(sv.ends_with(chx)), bool>::value);
    CHECK(noexcept(sv.ends_with(chx)));
    CHECK(sv.ends_with(chx));
    SLB_CONSTEXPR_CHAR_TRAITS bool ends_with = sv.ends_with(chx);
    (void)ends_with;

    CHECK_FALSE(sv.ends_with('f'));
  }
}

// [string.view.find], searching
TEST_CASE(SLB_INSTANTIATION "(searching)", "[string.view.find]") {
  static constexpr CharT str[] = {'f', 'f', 'f', 'o', 'o', '\0', 'b', 'a', 'r'};
  static constexpr CharT rstr[] = {
      'r', 'a', 'b', '\0', 'o', 'o', 'f', 'f', 'f'};

  /* constexpr size_type find(basic_string_view s, size_type pos = 0) const
        noexcept; */
  /* constexpr size_type find(const charT* s, size_type pos = 0) const; */
  /* constexpr size_type find(const charT* s, size_type pos, size_type n)
        const; */
  {
    static constexpr CharT strx[] = {'f', 'f', 'o', 'o', '\0', 'b'};
    static constexpr CharT strnx[] = {'b', 'a', 'z', '\0'};
    static constexpr CharT stro[] = {'o', '\0'};
    static constexpr CharT stre[] = {'\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx));
    CHECK(std::is_same<decltype(sv.find(x)), string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find(x, 0)), string_view::size_type>::value);
    CHECK(noexcept(sv.find(x)));
    CHECK(noexcept(sv.find(x, 0)));
    CHECK(sv.find(x) == 1);
    CHECK(sv.find(x, 0) == 1);
    CHECK(sv.find(x, 2) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find0 = sv.find(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find1 =
        sv.find(x, 0);
    (void)find0;
    (void)find1;

    constexpr string_view nx(strnx, countof(strnx) - 1);
    constexpr string_view o(stro, countof(stro) - 1);
    CHECK(sv.find(o, 0) == 3);
    CHECK(sv.find(o, 4) == 4);
    CHECK(sv.find(o, 5) == sv.npos);
    CHECK(sv.find(nx, 0) == sv.npos);
    CHECK(sv.find(string_view(), 0) == 0);

    CHECK(std::is_same<decltype(sv.find(strx)), string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find(strx, 0)),
                       string_view::size_type>::value);
    CHECK(sv.find(strx) == 1);
    CHECK(sv.find(strx, 0) == 1);
    CHECK(sv.find(strx, 2) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find0_sz =
        sv.find(strx);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find1_sz =
        sv.find(strx, 0);
    (void)find0_sz;
    (void)find1_sz;

    CHECK(sv.find(stro, 0) == 3);
    CHECK(sv.find(stro, 4) == 4);
    CHECK(sv.find(stro, 5) == sv.npos);
    CHECK(sv.find(strnx, 0) == sv.npos);
    CHECK(sv.find(stre, 0) == 0);

    CHECK(std::is_same<decltype(sv.find(strx, 0, countof(strx))),
                       string_view::size_type>::value);
    CHECK(sv.find(strx, 0, countof(strx)) == 1);
    CHECK(sv.find(strx, 2, countof(strx)) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_sn =
        sv.find(strx, 0, countof(strx));
    (void)find_sn;

    CHECK(sv.find(stro, 0, countof(stro)) == 4);
    CHECK(sv.find(stro, 5, countof(stro)) == sv.npos);
    CHECK(sv.find(strnx, 0, countof(strnx)) == sv.npos);
    CHECK(sv.find(stre, 0, 1) == 5);
  }

  /* constexpr size_type find(charT c, size_type pos = 0) const noexcept; */
  {
    constexpr string_view sv(str, countof(str));
    constexpr CharT x = 'f';
    CHECK(std::is_same<decltype(sv.find(x)), string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find(x, 0)), string_view::size_type>::value);
    CHECK(noexcept(sv.find(x)));
    CHECK(noexcept(sv.find(x, 0)));
    CHECK(sv.find(x) == 0);
    CHECK(sv.find(x, 0) == 0);
    CHECK(sv.find(x, 3) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find0 = sv.find(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find1 =
        sv.find(x, 0);
    (void)find0;
    (void)find1;

    constexpr CharT nx = 'z';
    constexpr CharT o = 'o';
    CHECK(sv.find(o, 0) == 3);
    CHECK(sv.find(o, 4) == 4);
    CHECK(sv.find(o, 5) == sv.npos);
    CHECK(sv.find(nx, 0) == sv.npos);
  }

  /* constexpr size_type rfind(basic_string_view s, size_type pos = npos)
     const noexcept; */
  /* constexpr size_type rfind(const charT* s, size_type pos = npos) const;
   */
  /* constexpr size_type rfind(const charT* s, size_type pos, size_type n)
        const; */
  {
    static constexpr CharT rstrx[] = {'b', '\0', 'o', 'o', 'f', 'f'};
    static constexpr CharT rstrnx[] = {'z', 'a', 'b', '\0'};
    static constexpr CharT rstro[] = {'o', '\0'};
    static constexpr CharT rstre[] = {'\0'};

    constexpr string_view sv(rstr, countof(str));
    constexpr string_view x(rstrx, countof(rstrx));
    CHECK(std::is_same<decltype(sv.rfind(x)), string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.rfind(x, sv.npos)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.rfind(x)));
    CHECK(noexcept(sv.rfind(x, sv.npos)));
    CHECK(sv.rfind(x) == 2);
    CHECK(sv.rfind(x, sv.npos) == 2);
    CHECK(sv.rfind(x, 2) == 2);
    CHECK(sv.rfind(x, 1) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type rfind0 = sv.rfind(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type rfind1 =
        sv.rfind(x, sv.npos);
    (void)rfind0;
    (void)rfind1;

    constexpr string_view nx(rstrnx, countof(rstrnx) - 1);
    constexpr string_view o(rstro, countof(rstro) - 1);
    CHECK(sv.rfind(o, sv.npos) == 5);
    CHECK(sv.rfind(o, 4) == 4);
    CHECK(sv.rfind(o, 3) == sv.npos);
    CHECK(sv.rfind(nx, sv.npos) == sv.npos);
    CHECK(sv.rfind(string_view(), sv.npos) == 9);

    CHECK(
        std::is_same<decltype(sv.rfind(rstrx)), string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.rfind(rstrx, sv.npos)),
                       string_view::size_type>::value);
    CHECK(sv.rfind(rstrx) == 2);
    CHECK(sv.rfind(rstrx, sv.npos) == 2);
    CHECK(sv.rfind(rstrx, 2) == 2);
    CHECK(sv.rfind(rstrx, 1) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type rfind0_sz =
        sv.rfind(rstrx);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type rfind1_sz =
        sv.rfind(rstrx, sv.npos);
    (void)rfind0_sz;
    (void)rfind1_sz;

    CHECK(sv.rfind(rstro, sv.npos) == 5);
    CHECK(sv.rfind(rstro, 4) == 4);
    CHECK(sv.rfind(rstro, 3) == sv.npos);
    CHECK(sv.rfind(rstrnx, sv.npos) == sv.npos);
    CHECK(sv.rfind(rstre, sv.npos) == 9);

    CHECK(std::is_same<decltype(sv.rfind(rstrx, sv.npos, countof(rstrx))),
                       string_view::size_type>::value);
    CHECK(sv.rfind(rstrx, sv.npos, countof(rstrx)) == 2);
    CHECK(sv.rfind(rstrx, 2, countof(rstrx)) == 2);
    CHECK(sv.rfind(rstrx, 1, countof(rstrx)) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type rfind_sn =
        sv.rfind(rstrx, sv.npos, countof(rstrx));
    (void)rfind_sn;

    CHECK(sv.rfind(rstro, sv.npos, countof(rstro)) == sv.npos);
    CHECK(sv.rfind(rstrnx, sv.npos, countof(rstrnx)) == sv.npos);
    CHECK(sv.rfind(rstre, sv.npos, 1) == 3);
  }

  /* constexpr size_type rfind(charT c, size_type pos = npos) const
  noexcept;
  /*/
  {
    constexpr string_view sv(rstr, countof(rstr));
    constexpr CharT x = 'f';
    CHECK(std::is_same<decltype(sv.rfind(x)), string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.rfind(x, sv.npos)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.rfind(x)));
    CHECK(noexcept(sv.rfind(x, sv.npos)));
    CHECK(sv.rfind(x) == 8);
    CHECK(sv.rfind(x, sv.npos) == 8);
    CHECK(sv.rfind(x, 6) == 6);
    CHECK(sv.rfind(x, 5) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type rfind0 = sv.rfind(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type rfind1 =
        sv.rfind(x, sv.npos);
    (void)rfind0;
    (void)rfind1;

    constexpr CharT nx = 'z';
    constexpr CharT o = 'o';
    CHECK(sv.rfind(o, sv.npos) == 5);
    CHECK(sv.rfind(o, 4) == 4);
    CHECK(sv.rfind(o, 3) == sv.npos);
    CHECK(sv.rfind(nx, sv.npos) == sv.npos);
  }

  /* constexpr size_type find_first_of(basic_string_view s, size_type pos =
     0) const noexcept; */
  /* constexpr size_type find_first_of(const charT* s, size_type pos = 0)
         const; */
  /* constexpr size_type find_first_of(const charT* s, size_type pos,
     size_type n) const; */
  {
    static constexpr CharT strx[] = {'f', 'b', '\0'};
    static constexpr CharT strnx[] = {'x', 'y', 'z', '\0'};
    static constexpr CharT stre[] = {'\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx));
    CHECK(std::is_same<decltype(sv.find_first_of(x)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_first_of(x, 0)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.find_first_of(x)));
    CHECK(noexcept(sv.find_first_of(x, 0)));
    CHECK(sv.find_first_of(x) == 0);
    CHECK(sv.find_first_of(x, 0) == 0);
    CHECK(sv.find_first_of(x, 3) == 5);
    CHECK(sv.find_first_of(x, 7) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_of0 =
        sv.find_first_of(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_of1 =
        sv.find_first_of(x, 0);
    (void)find_first_of0;
    (void)find_first_of1;

    constexpr string_view nx(strnx, countof(strnx) - 1);
    CHECK(sv.find_first_of(nx, 0) == sv.npos);
    CHECK(sv.find_first_of(string_view(), 0) == sv.npos);

    CHECK(std::is_same<decltype(sv.find_first_of(strx)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_first_of(strx, 0)),
                       string_view::size_type>::value);
    CHECK(sv.find_first_of(strx) == 0);
    CHECK(sv.find_first_of(strx, 0) == 0);
    CHECK(sv.find_first_of(strx, 3) == 6);
    CHECK(sv.find_first_of(strx, 7) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_of0_sz =
        sv.find_first_of(strx);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_of1_sz =
        sv.find_first_of(strx, 0);
    (void)find_first_of0_sz;
    (void)find_first_of1_sz;

    CHECK(sv.find_first_of(strnx, 0) == sv.npos);
    CHECK(sv.find_first_of(stre, 0) == sv.npos);

    CHECK(std::is_same<decltype(sv.find_first_of(strx, 0, countof(strx))),
                       string_view::size_type>::value);
    CHECK(sv.find_first_of(strx, 0, countof(strx)) == 0);
    CHECK(sv.find_first_of(strx, 3, countof(strx)) == 5);
    CHECK(sv.find_first_of(strx, 7, countof(strx)) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_of_sn =
        sv.find_first_of(strx, 0, countof(strx));
    (void)find_first_of_sn;

    CHECK(sv.find_first_of(strnx, 0, countof(strnx)) == 5);
    CHECK(sv.find_first_of(stre, 0, 1) == 5);
  }

  /* constexpr size_type find_first_of(charT c, size_type pos = 0) const
        noexcept; */
  {
    constexpr string_view sv(str, countof(str));
    constexpr CharT x = 'f';
    CHECK(std::is_same<decltype(sv.find_first_of(x)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_first_of(x, 0)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.find_first_of(x)));
    CHECK(noexcept(sv.find_first_of(x, 0)));
    CHECK(sv.find_first_of(x) == 0);
    CHECK(sv.find_first_of(x, 0) == 0);
    CHECK(sv.find_first_of(x, 3) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_of0 =
        sv.find_first_of(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_of1 =
        sv.find_first_of(x, 0);
    (void)find_first_of0;
    (void)find_first_of1;

    constexpr CharT nx = 'z';
    CHECK(sv.find_first_of(nx, 0) == sv.npos);
  }

  /* constexpr size_type find_last_of(basic_string_view s, size_type pos =
     npos) const noexcept; */
  /* constexpr size_type find_last_of(const charT* s, size_type pos = npos)
        const; */
  /* constexpr size_type find_last_of(const charT* s, size_type pos,
     size_type n) const; */
  {
    static constexpr CharT strx[] = {'f', 'b', '\0'};
    static constexpr CharT strnx[] = {'x', 'y', 'z', '\0'};
    static constexpr CharT stre[] = {'\0'};

    constexpr string_view sv(rstr, countof(rstr));
    constexpr string_view x(strx, countof(strx));
    CHECK(std::is_same<decltype(sv.find_last_of(x)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_last_of(x, sv.npos)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.find_last_of(x)));
    CHECK(noexcept(sv.find_last_of(x, sv.npos)));
    CHECK(sv.find_last_of(x) == 8);
    CHECK(sv.find_last_of(x, sv.npos) == 8);
    CHECK(sv.find_last_of(x, 5) == 3);
    CHECK(sv.find_last_of(x, 1) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_of0 =
        sv.find_last_of(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_of1 =
        sv.find_last_of(x, sv.npos);
    (void)find_last_of0;
    (void)find_last_of1;

    constexpr string_view nx(strnx, countof(strnx) - 1);
    CHECK(sv.find_last_of(nx, sv.npos) == sv.npos);
    CHECK(sv.find_last_of(string_view(), sv.npos) == sv.npos);

    CHECK(std::is_same<decltype(sv.find_last_of(strx)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_last_of(strx, sv.npos)),
                       string_view::size_type>::value);
    CHECK(sv.find_last_of(strx) == 8);
    CHECK(sv.find_last_of(strx, sv.npos) == 8);
    CHECK(sv.find_last_of(strx, 5) == 2);
    CHECK(sv.find_last_of(strx, 1) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_of0_sz =
        sv.find_last_of(strx);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_of1_sz =
        sv.find_last_of(strx, sv.npos);
    (void)find_last_of0_sz;
    (void)find_last_of1_sz;

    CHECK(sv.find_last_of(strnx, sv.npos) == sv.npos);
    CHECK(sv.find_last_of(stre, sv.npos) == sv.npos);

    CHECK(std::is_same<decltype(sv.find_last_of(strx, sv.npos, countof(strx))),
                       string_view::size_type>::value);
    CHECK(sv.find_last_of(strx, sv.npos, countof(strx)) == 8);
    CHECK(sv.find_last_of(strx, 5, countof(strx)) == 3);
    CHECK(sv.find_last_of(strx, 1, countof(strx)) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_of_sn =
        sv.find_last_of(strx, sv.npos, countof(strx));
    (void)find_last_of_sn;

    CHECK(sv.find_last_of(strnx, sv.npos, countof(strnx)) == 3);
    CHECK(sv.find_last_of(stre, sv.npos, 1) == 3);
  }

  /* constexpr size_type find_last_of(charT c, size_type pos = npos) const
         noexcept; */
  {
    constexpr string_view sv(rstr, countof(rstr));
    constexpr CharT x = 'f';
    CHECK(std::is_same<decltype(sv.find_last_of(x)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_last_of(x, sv.npos)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.find_last_of(x)));
    CHECK(noexcept(sv.find_last_of(x, sv.npos)));
    CHECK(sv.find_last_of(x) == 8);
    CHECK(sv.find_last_of(x, sv.npos) == 8);
    CHECK(sv.find_last_of(x, 5) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_of0 =
        sv.find_last_of(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_of1 =
        sv.find_last_of(x, sv.npos);
    (void)find_last_of0;
    (void)find_last_of1;

    constexpr CharT nx = 'z';
    CHECK(sv.find_last_of(nx, sv.npos) == sv.npos);
  }

  /* constexpr size_type find_first_not_of(basic_string_view s, size_type
     pos = 0) const noexcept; */
  /* constexpr size_type find_first_not_of(const charT* s, size_type pos =
     0) const; */
  /* constexpr size_type find_first_not_of(const charT* s, size_type pos,
         size_type n) const; */
  {
    static constexpr CharT strx[] = {'a', 'r', 'z', '\0'};
    static constexpr CharT strnx[] = {'f', 'b', '\0'};
    static constexpr CharT stre[] = {'\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx));
    CHECK(std::is_same<decltype(sv.find_first_not_of(x)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_first_not_of(x, 0)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.find_first_not_of(x)));
    CHECK(noexcept(sv.find_first_not_of(x, 0)));
    CHECK(sv.find_first_not_of(x) == 0);
    CHECK(sv.find_first_not_of(x, 0) == 0);
    CHECK(sv.find_first_not_of(x, 5) == 6);
    CHECK(sv.find_first_not_of(x, 7) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_not_of0 =
        sv.find_first_not_of(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_not_of1 =
        sv.find_first_not_of(x, 0);
    (void)find_first_not_of0;
    (void)find_first_not_of1;

    constexpr string_view nx(strnx, countof(strnx) - 1);
    CHECK(sv.find_first_not_of(nx, 0) == 3);
    CHECK(sv.find_first_not_of(nx, 5) == 5);
    CHECK(sv.find_first_not_of(nx, 6) == 7);
    CHECK(sv.find_first_not_of(string_view(), 0) == 0);

    CHECK(std::is_same<decltype(sv.find_first_not_of(strx)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_first_not_of(strx, 0)),
                       string_view::size_type>::value);
    CHECK(sv.find_first_not_of(strx) == 0);
    CHECK(sv.find_first_not_of(strx, 0) == 0);
    CHECK(sv.find_first_not_of(strx, 5) == 5);
    CHECK(sv.find_first_not_of(strx, 6) == 6);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type
        find_first_not_of0_sz = sv.find_first_not_of(strx);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type
        find_first_not_of1_sz = sv.find_first_not_of(strx, 0);
    (void)find_first_not_of0_sz;
    (void)find_first_not_of1_sz;

    CHECK(sv.find_first_not_of(strnx, 0) == 3);
    CHECK(sv.find_first_not_of(strnx, 5) == 5);
    CHECK(sv.find_first_not_of(strnx, 6) == 7);
    CHECK(sv.find_first_not_of(stre, 0) == 0);

    CHECK(std::is_same<decltype(sv.find_first_not_of(strx, 0, countof(strx))),
                       string_view::size_type>::value);
    CHECK(sv.find_first_not_of(strx, 0, countof(strx)) == 0);
    CHECK(sv.find_first_not_of(strx, 5, countof(strx)) == 6);
    CHECK(sv.find_first_not_of(strx, 7, countof(strx)) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type
        find_first_not_of_sn = sv.find_first_not_of(strx, 0, countof(strx));
    (void)find_first_not_of_sn;

    CHECK(sv.find_first_not_of(strnx, 0, countof(strnx)) == 3);
    CHECK(sv.find_first_not_of(strnx, 5, countof(strnx)) == 7);
    CHECK(sv.find_first_not_of(stre, 0, 1) == 0);
  }

  /* constexpr size_type find_first_not_of(charT c, size_type pos = 0) const
         noexcept; */
  {
    constexpr string_view sv(str, countof(str));
    constexpr CharT x = 'z';
    CHECK(std::is_same<decltype(sv.find_first_not_of(x)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_first_not_of(x, 0)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.find_first_not_of(x)));
    CHECK(noexcept(sv.find_first_not_of(x, 0)));
    CHECK(sv.find_first_not_of(x) == 0);
    CHECK(sv.find_first_not_of(x, 0) == 0);
    CHECK(sv.find_first_not_of(x, 3) == 3);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_not_of0 =
        sv.find_first_not_of(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_first_not_of1 =
        sv.find_first_not_of(x, 0);
    (void)find_first_not_of0;
    (void)find_first_not_of1;

    constexpr CharT nx = 'f';
    CHECK(sv.find_first_not_of(nx, 0) == 3);
  }

  /* co nstexpr size_type find_last_not_of(basic_string_view s, size_type
     pos = npos) const noexcept; */
  /* constexpr size_type find_last_not_of(const charT* s, size_type pos =
     npos) const; */
  /* constexpr size_type find_last_not_of(const charT* s, size_type pos,
         size_type n) const; */
  {
    static constexpr CharT strx[] = {'a', 'r', 'z', '\0'};
    static constexpr CharT strnx[] = {'f', 'b', '\0'};
    static constexpr CharT stre[] = {'\0'};

    constexpr string_view sv(rstr, countof(rstr));
    constexpr string_view x(strx, countof(strx));
    CHECK(std::is_same<decltype(sv.find_last_not_of(x)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_last_not_of(x, sv.npos)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.find_last_not_of(x)));
    CHECK(noexcept(sv.find_last_not_of(x, sv.npos)));
    CHECK(sv.find_last_not_of(x) == 8);
    CHECK(sv.find_last_not_of(x, sv.npos) == 8);
    CHECK(sv.find_last_not_of(x, 3) == 2);
    CHECK(sv.find_last_not_of(x, 1) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_not_of0 =
        sv.find_last_not_of(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_not_of1 =
        sv.find_last_not_of(x, sv.npos);
    (void)find_last_not_of0;
    (void)find_last_not_of1;

    constexpr string_view nx(strnx, countof(strnx) - 1);
    CHECK(sv.find_last_not_of(nx, sv.npos) == 5);
    CHECK(sv.find_last_not_of(nx, 3) == 3);
    CHECK(sv.find_last_not_of(nx, 2) == 1);
    CHECK(sv.find_last_not_of(string_view(), sv.npos) == 8);

    CHECK(std::is_same<decltype(sv.find_last_not_of(strx)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_last_not_of(strx, sv.npos)),
                       string_view::size_type>::value);
    CHECK(sv.find_last_not_of(strx) == 8);
    CHECK(sv.find_last_not_of(strx, sv.npos) == 8);
    CHECK(sv.find_last_not_of(strx, 3) == 3);
    CHECK(sv.find_last_not_of(strx, 1) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type
        find_last_not_of0_sz = sv.find_last_not_of(strx);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type
        find_last_not_of1_sz = sv.find_last_not_of(strx, sv.npos);
    (void)find_last_not_of0_sz;
    (void)find_last_not_of1_sz;

    CHECK(sv.find_last_not_of(strnx, sv.npos) == 5);
    CHECK(sv.find_last_not_of(strnx, 3) == 3);
    CHECK(sv.find_last_not_of(strnx, 2) == 1);
    CHECK(sv.find_last_not_of(stre, sv.npos) == 8);

    CHECK(std::is_same<decltype(
                           sv.find_last_not_of(strx, sv.npos, countof(strx))),
                       string_view::size_type>::value);
    CHECK(sv.find_last_not_of(strx, sv.npos, countof(strx)) == 8);
    CHECK(sv.find_last_not_of(strx, 3, countof(strx)) == 2);
    CHECK(sv.find_last_not_of(strx, 1, countof(strx)) == sv.npos);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_not_of_sn =
        sv.find_last_not_of(strx, sv.npos, countof(strx));
    (void)find_last_not_of_sn;

    CHECK(sv.find_last_not_of(strnx, sv.npos, countof(strnx)) == 5);
    CHECK(sv.find_last_not_of(strnx, 3, countof(strnx)) == 1);
    CHECK(sv.find_last_not_of(stre, sv.npos, 1) == 8);
  }

  /* constexpr size_type find_last_not_of(charT c, size_type pos = npos)
     const noexcept; */
  {
    constexpr string_view sv(rstr, countof(rstr));
    constexpr CharT x = 'z';
    CHECK(std::is_same<decltype(sv.find_last_not_of(x)),
                       string_view::size_type>::value);
    CHECK(std::is_same<decltype(sv.find_last_not_of(x, sv.npos)),
                       string_view::size_type>::value);
    CHECK(noexcept(sv.find_last_not_of(x)));
    CHECK(noexcept(sv.find_last_not_of(x, sv.npos)));
    CHECK(sv.find_last_not_of(x) == 8);
    CHECK(sv.find_last_not_of(x, sv.npos) == 8);
    CHECK(sv.find_last_not_of(x, 5) == 5);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_not_of0 =
        sv.find_last_not_of(x);
    SLB_CXX14_CONSTEXPR_CHAR_TRAITS string_view::size_type find_last_not_of1 =
        sv.find_last_not_of(x, sv.npos);
    (void)find_last_not_of0;
    (void)find_last_not_of1;

    constexpr CharT nx = 'f';
    CHECK(sv.find_last_not_of(nx, sv.npos) == 5);
  }
}

// [string.view.comparison], non-member comparison functions
TEST_CASE(SLB_INSTANTIATION "(comparison)", "[string.view.comparison]") {
  static constexpr CharT str_smaller[] = {'f', 'o'};
  static constexpr CharT str_less[] = {'f', 'o', 'a'};
  static constexpr CharT str[] = {'f', 'o', 'o'};
  static constexpr CharT strn[] = {'b', 'a', 'r'};
  static constexpr CharT str_greater[] = {'f', 'o', 'z'};
  static constexpr CharT str_bigger[] = {'f', 'o', 'o', 'o'};

  /* template<class charT, class traits>
       constexpr bool operator==(basic_string_view<charT, traits> x,
                                 basic_string_view<charT, traits> y)
     noexcept;
                                 */
  {
    static constexpr CharT strx[] = {'f', 'o', 'o', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv == x), bool>::value);
    CHECK(noexcept(sv == x));
    CHECK(sv == x);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare1 = sv == x;
    (void)compare1;

    constexpr string_view equal(str, countof(str));
    CHECK(sv == equal);
    constexpr string_view not_equal(strn, countof(strn));
    CHECK_FALSE(sv == not_equal);

    CHECK(std::is_same<decltype(strx == sv), bool>::value);
    CHECK(strx == sv);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare2 = strx == sv;
    (void)compare2;

    CHECK(strx == equal);
    CHECK_FALSE(strx == not_equal);

    CHECK(std::is_same<decltype(sv == strx), bool>::value);
    CHECK(sv == strx);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare3 = sv == strx;
    (void)compare3;

    CHECK(equal == strx);
    CHECK_FALSE(not_equal == strx);
  }

  /* template<class charT, class traits>
       constexpr bool operator!=(basic_string_view<charT, traits> x,
                                 basic_string_view<charT, traits> y)
     noexcept;
   */
  {
    static constexpr CharT strx[] = {'b', 'a', 'r', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv != x), bool>::value);
    CHECK(noexcept(sv != x));
    CHECK(sv != x);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare1 = sv != x;
    (void)compare1;

    constexpr string_view equal(str, countof(str));
    CHECK_FALSE(sv != equal);
    constexpr string_view not_equal(strn, countof(strn));
    CHECK(sv != not_equal);

    CHECK(std::is_same<decltype(strx != sv), bool>::value);
    CHECK(strx != sv);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare2 = strx != sv;
    (void)compare2;

    CHECK(strx != equal);
    CHECK_FALSE(strx != not_equal);

    CHECK(std::is_same<decltype(sv != strx), bool>::value);
    CHECK(sv != strx);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare3 = sv != strx;
    (void)compare3;

    CHECK(equal != strx);
    CHECK_FALSE(not_equal != strx);
  }

  /* template<class charT, class traits>
       constexpr bool operator< (basic_string_view<charT, traits> x,
                                 basic_string_view<charT, traits> y)
     noexcept;
   */
  {
    static constexpr CharT strx[] = {'f', 'o', 'o', 'o', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv < x), bool>::value);
    CHECK(noexcept(sv < x));
    CHECK(sv < x);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare1 = sv < x;
    (void)compare1;

    constexpr string_view smaller(str_smaller, countof(str_smaller));
    CHECK_FALSE(sv < smaller);
    constexpr string_view less(str_less, countof(str_less));
    CHECK_FALSE(sv < less);
    constexpr string_view greater(str_greater, countof(str_greater));
    CHECK(sv < greater);
    constexpr string_view bigger(str_bigger, countof(str_bigger));
    CHECK(sv < bigger);

    CHECK(std::is_same<decltype(strx < sv), bool>::value);
    CHECK_FALSE(strx < sv);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare2 = strx < sv;
    (void)compare2;

    CHECK_FALSE(strx < smaller);
    CHECK_FALSE(strx < less);
    CHECK(strx < greater);
    CHECK_FALSE(strx < bigger);

    CHECK(std::is_same<decltype(sv < strx), bool>::value);
    CHECK(sv < strx);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare3 = sv < strx;
    (void)compare3;

    CHECK(smaller < strx);
    CHECK(less < strx);
    CHECK_FALSE(greater < strx);
    CHECK_FALSE(bigger < strx);
  }

  /* template<class charT, class traits>
       constexpr bool operator> (basic_string_view<charT, traits> x,
                                 basic_string_view<charT, traits> y)
     noexcept;
   */
  {
    static constexpr CharT strx[] = {'f', 'o', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv > x), bool>::value);
    CHECK(noexcept(sv > x));
    CHECK(sv > x);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare1 = sv > x;
    (void)compare1;

    constexpr string_view smaller(str_smaller, countof(str_smaller));
    CHECK(sv > smaller);
    constexpr string_view less(str_less, countof(str_less));
    CHECK(sv > less);
    constexpr string_view greater(str_greater, countof(str_greater));
    CHECK_FALSE(sv > greater);
    constexpr string_view bigger(str_bigger, countof(str_bigger));
    CHECK_FALSE(sv > bigger);

    CHECK(std::is_same<decltype(strx > sv), bool>::value);
    CHECK_FALSE(strx > sv);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare2 = strx > sv;
    (void)compare2;

    CHECK_FALSE(strx > smaller);
    CHECK_FALSE(strx > less);
    CHECK_FALSE(strx > greater);
    CHECK_FALSE(strx > bigger);

    CHECK(std::is_same<decltype(sv > strx), bool>::value);
    CHECK(sv > strx);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare3 = sv > strx;
    (void)compare3;

    CHECK_FALSE(smaller > strx);
    CHECK(less > strx);
    CHECK(greater > strx);
    CHECK(bigger > strx);
  }

  /* template<class charT, class traits>
       constexpr bool operator<=(basic_string_view<charT, traits> x,
                                 basic_string_view<charT, traits> y)
     noexcept;
   */
  {
    static constexpr CharT strx[] = {'f', 'o', 'z', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv <= x), bool>::value);
    CHECK(noexcept(sv <= x));
    CHECK(sv <= x);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare1 = sv <= x;
    (void)compare1;

    constexpr string_view smaller(str_smaller, countof(str_smaller));
    CHECK_FALSE(sv <= smaller);
    constexpr string_view less(str_less, countof(str_less));
    CHECK_FALSE(sv <= less);
    constexpr string_view greater(str_greater, countof(str_greater));
    CHECK(sv <= greater);
    constexpr string_view bigger(str_bigger, countof(str_bigger));
    CHECK(sv <= bigger);

    CHECK(std::is_same<decltype(strx <= sv), bool>::value);
    CHECK_FALSE(strx <= sv);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare2 = strx <= sv;
    (void)compare2;

    CHECK_FALSE(strx <= smaller);
    CHECK_FALSE(strx <= less);
    CHECK(strx <= greater);
    CHECK_FALSE(strx <= bigger);

    CHECK(std::is_same<decltype(sv <= strx), bool>::value);
    CHECK(sv <= strx);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare3 = sv <= strx;
    (void)compare3;

    CHECK(smaller <= strx);
    CHECK(less <= strx);
    CHECK(greater <= strx);
    CHECK(bigger <= strx);
  }

  /* template<class charT, class traits>
       constexpr bool operator>=(basic_string_view<charT, traits> x,
                                 basic_string_view<charT, traits> y)
     noexcept;
   */
  {
    static constexpr CharT strx[] = {'f', 'o', 'a', '\0'};

    constexpr string_view sv(str, countof(str));
    constexpr string_view x(strx, countof(strx) - 1);
    CHECK(std::is_same<decltype(sv >= x), bool>::value);
    CHECK(noexcept(sv >= x));
    CHECK(sv >= x);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare1 = sv >= x;
    (void)compare1;

    constexpr string_view smaller(str_smaller, countof(str_smaller));
    CHECK(sv >= smaller);
    constexpr string_view less(str_less, countof(str_less));
    CHECK(sv >= less);
    constexpr string_view greater(str_greater, countof(str_greater));
    CHECK_FALSE(sv >= greater);
    constexpr string_view bigger(str_bigger, countof(str_bigger));
    CHECK_FALSE(sv >= bigger);

    CHECK(std::is_same<decltype(strx >= sv), bool>::value);
    CHECK_FALSE(strx >= sv);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare2 = strx >= sv;
    (void)compare2;

    CHECK(strx >= smaller);
    CHECK(strx >= less);
    CHECK_FALSE(strx >= greater);
    CHECK_FALSE(strx >= bigger);

    CHECK(std::is_same<decltype(sv >= strx), bool>::value);
    CHECK(sv >= strx);
    SLB_CONSTEXPR_CHAR_TRAITS bool compare3 = sv >= strx;
    (void)compare3;

    CHECK_FALSE(smaller >= strx);
    CHECK(less >= strx);
    CHECK(greater >= strx);
    CHECK(bigger >= strx);
  }
}

#endif
