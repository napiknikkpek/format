#ifndef _0039D484_AE0F_4DF2_8C7B_3FAFB9E0094E_HPP
#define _0039D484_AE0F_4DF2_8C7B_3FAFB9E0094E_HPP

#include <array>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include <boost/hana/for_each.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/value.hpp>

namespace format_impl {

struct error : public std::runtime_error {
  error(std::string const& msg) : std::runtime_error(msg) {}
};

using std::string_view;

constexpr int count(string_view const& view) {
  int result = 0;
  string_view::size_type pos = 0;
  while (true) {
    pos = view.find('{', pos);
    pos = view.find('}', pos);
    if (pos == string_view::npos) {
      return result;
    }
    ++result;
  }
}

struct element {
  int index;
  std::string_view holder;
};

constexpr int to_int(char c) {
  if (c >= '0' && c <= '9') {
    return int{c} - int{'0'};
  }
  throw error("not a number");
}

template <typename Iterator>
constexpr int to_int(Iterator b, Iterator e) {
  int result = 0;
  for (; b != e; ++b) {
    result *= 10;
    result += to_int(*b);
  }
  return result;
}

std::string_view make_string_view(char const* b, char const* e) {
  return std::string_view{b, static_cast<std::string_view::size_type>(e - b)};
}

template <int N>
constexpr std::array<element, N> split(string_view const& view) {
  std::array<element, N> result{};

  string_view::size_type pos = 0;

  enum class spec_t { none, nondigits, digits };
  spec_t spec = spec_t::none;

  for (int i = 0; i < N; ++i) {
    auto b = view.find('{', pos);
    auto e = view.find('}', b);
    result[i].holder = string_view{view.data() + b, e - b + 1};
    if (e - b == 1) {
      if (spec == spec_t::digits) {
        throw error{"mix spec"};
      }
      spec = spec_t::nondigits;
      result[i].index = i;
    } else {
      if (spec == spec_t::nondigits) {
        throw error{"mix spec"};
      }
      spec = spec_t::digits;
      result[i].index = to_int(view.data() + b + 1, view.data() + e);
    }
    pos = e;
  }

  return result;
}
}  // namespace format_impl

template <typename String, typename... Args>
std::string format(String str, Args const&... args) {
  using namespace format_impl;
  using namespace boost::hana;
  using std::string_view;

  constexpr auto view = string_view(str.c_str());

  constexpr auto N = format_impl::count(view);

  if (N == 0) {
    return str.c_str();
  }

  if constexpr (N > 0) {
    constexpr auto elements = format_impl::split<N>(view);

    std::stringstream ss;

    auto tuple = make_tuple(args...);

    static_assert(value(size(tuple)) <= N);

    ss << make_string_view(view.data(), elements.front().holder.data())
       << tuple[size_c<elements[0].index>];

    auto seq = make_range(int_c<1>, int_c<N>);

    for_each(seq, [&](auto x) {
      constexpr auto index = value(x);
      auto const& prev = elements[index - 1].holder;
      auto const& cur = elements[index].holder;
      ss << make_string_view(prev.data() + prev.size(), cur.data())
         << tuple[size_c<elements[index].index>];
    });

    auto last = elements.back().holder;
    ss << make_string_view(last.data() + last.size(), view.end());

    return ss.str();
  }
}

#endif
