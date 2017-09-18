#ifndef _0039D484_AE0F_4DF2_8C7B_3FAFB9E0094E_HPP
#define _0039D484_AE0F_4DF2_8C7B_3FAFB9E0094E_HPP

#include <array>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include <boost/hana/for_each.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/value.hpp>

namespace format_impl {

struct error : public std::runtime_error {
  error(std::string const& msg) : std::runtime_error(msg) {}
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

using std::string_view;

struct parser {
  string_view const& view;

  constexpr bool escaped(string_view::size_type& pos, char c) {
    if (pos >= view.size() || view[pos + 1] != c) {
      return false;
    }
    pos += 2;
    return true;
  }

  constexpr bool found_field(string_view::size_type& pos, int& index) {
    if (pos >= view.size()) {
      return false;
    }
    if (view[pos + 1] == '}') {
      index = -1;
      pos += 2;
      return true;
    }
    auto b = pos;
    auto e = view.find('}', b);
    if (e == string_view::npos) {
      return false;
    }
    index = to_int(view.begin() + b + 1, view.begin() + e);
    pos = e + 1;
    return true;
  }

  // spirit grammar :
  // *(*(!char_("{}")) >> ("{{" | ('{' > *int_ > '}') | "}}" | eps[error()]))
  template <typename Visitor>
  constexpr Visitor parse() {
    Visitor visitor{};
    string_view::size_type pos = 0;

    while (true) {
      pos = view.find_first_of(string_view("{}"), pos);

      if (pos == string_view::npos) {
        return visitor;
      }

      auto b = pos;

      if (view[pos] == '}') {
        if (escaped(pos, '}')) {
          visitor.escape(string_view{view.data() + b, pos - b}, '}');
          continue;
        }
        throw error{"unmatched '}'"};
      }

      if (escaped(pos, '{')) {
        visitor.escape(string_view{view.data() + b, pos - b}, '{');
        continue;
      }

      int index = -1;
      if (found_field(pos, index)) {
        visitor.field(string_view{view.data() + b, pos - b}, index);
        continue;
      }
      throw error("unmatched '{'");
    }
  }
};

struct count_visitor {
  int count = 0;

  constexpr void escape(string_view const&, char) { ++count; }
  constexpr void field(string_view const&, int) { ++count; }
};

struct replacement {
  bool is_index;
  int index;
  char escaped;
  std::string_view holder;

  template <typename Container>
  std::ostream& stream(std::ostream& os, Container const& container) const {
    if (is_index) {
      return os << container[index];
    } else {
      return os << escaped;
    }
  }
};

template <int N>
struct replacement_visitor {
  std::array<replacement, N> replacements;
  int count = 0;
  int index_count = 0;

  enum class spec_t { none, empty, numeric };
  spec_t spec = spec_t::none;

  constexpr void escape(string_view const& view, char c) {
    auto& rep = replacements[count++];
    rep.is_index = false;
    rep.escaped = c;
    rep.holder = view;
  }

  constexpr void field(string_view const& view, int index) {
    if (index < 0) {
      if (spec == spec_t::numeric) {
        throw error{"mix field format specification"};
      }
      spec = spec_t::empty;
      index = index_count++;
    } else {
      if (spec == spec_t::empty) {
        throw error{"mix field format specification"};
      }
      spec = spec_t::numeric;
    }
    auto& rep = replacements[count++];
    rep.is_index = true;
    rep.index = index;
    rep.holder = view;
  }
};

std::string_view make_string_view(char const* b, char const* e) {
  return std::string_view{b, static_cast<std::string_view::size_type>(e - b)};
}

template <typename Iterator, typename T, typename Op>
constexpr T accumulate(Iterator b, Iterator e, T init, Op op) {
  for (; b != e; ++b) {
    init = op(init, *b);
  }
  return init;
}

}  // namespace format_impl

template <typename String, typename... Args>
std::string format(String str, Args const&... args) {
  using namespace format_impl;
  using namespace boost::hana;
  using std::string_view;

  constexpr auto Size = sizeof...(args);
  std::array<std::string, Size> fields;

  auto tuple = make_tuple(args...);
  auto indices = make_range(size_c<0>, size_c<Size>);
  std::stringstream ss;
  for_each(indices, [&](auto x) {
    constexpr auto index = value(x);
    ss.str(std::string{});
    ss << tuple[x];
    fields[index] = ss.str();
  });

  constexpr auto view = string_view(str.c_str());
  constexpr auto N = parser{view}.parse<count_visitor>().count;

  if constexpr (N == 0) {
    return str.c_str();
  } else {
    constexpr auto replacements =
        parser{view}.parse<replacement_visitor<N>>().replacements;

    constexpr auto up = accumulate(replacements.begin(), replacements.end(), -1,
                                   [](auto init, auto const& rep) {
                                     if (!rep.is_index) {
                                       return init;
                                     }
                                     return std::max(init, rep.index);
                                   });

    static_assert(up < static_cast<int>(Size),
                  "replacement index out of range");

    ss.str(std::string{});

    ss << make_string_view(view.data(), replacements.front().holder.data());
    replacements.front().stream(ss, fields);

    for (auto i = 1; i < N; ++i) {
      auto const& prev = replacements[i - 1].holder;
      auto const& cur = replacements[i].holder;
      ss << make_string_view(prev.data() + prev.size(), cur.data());
      replacements[i].stream(ss, fields);
    }

    auto const& last = replacements.back().holder;
    ss << make_string_view(last.data() + last.size(), view.end());

    return ss.str();
  }
}

#define CONSTEXPR_STRING(s)                                 \
  ([] {                                                     \
    struct tmp {                                            \
      static constexpr decltype(auto) c_str() { return s; } \
    };                                                      \
    return tmp{};                                           \
  }())

#define FORMAT(s)                                           \
  [](auto&&... args) {                                      \
    return format(CONSTEXPR_STRING(s),                      \
                  std::forward<decltype(args)&&>(args)...); \
  }

#endif
