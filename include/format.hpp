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
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>

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

  constexpr bool escaped(string_view::size_type& pos) const {
    if (pos >= view.size() || view[pos + 1] != view[pos]) {
      return false;
    }
    pos += 2;
    return true;
  }

  constexpr bool indexed(string_view::size_type& pos, int& index) const {
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

  constexpr auto step(string_view::size_type pos) const {
    return view.find_first_of(string_view{"{}"}, pos);
  }

  // spirit grammar :
  // *(*(!char_("{}")) >> ("{{" | ('{' > *int_ > '}') | "}}" | eps[error()]))
  template <typename Visitor>
  constexpr Visitor parse() const {
    Visitor visitor{};

    for (auto pos = step(0); pos != string_view::npos; pos = step(pos)) {
      auto b = pos;
      if (view[pos] == '}') {
        if (escaped(pos)) {
          visitor.escape(string_view{view.data() + b, pos - b}, '}');
          continue;
        }
        throw error{"unmatched '}'"};
      }

      if (escaped(pos)) {
        visitor.escape(string_view{view.data() + b, pos - b}, '{');
        continue;
      }

      int index = -1;
      if (indexed(pos, index)) {
        visitor.field(string_view{view.data() + b, pos - b}, index);
        continue;
      }
      throw error("unmatched '{'");
    }

    return visitor;
  }
};

template <int Size>
struct count_visitor {
  int total = 0;
  int indices = 0;
  int up = -1;
  std::array<int, Size> fields;

  constexpr count_visitor() : fields{} {}

  constexpr void escape(string_view const& view, char) { ++total; }
  constexpr void field(string_view const& view, int index) {
    if (index < 0) {
      index = indices;
    }

    up = std::max(index, up);

    if (index < Size) {
      ++fields[index];
    }

    ++indices;
    ++total;
  }
};

std::string_view make_string_view(char const* b, char const* e) {
  return std::string_view{b, static_cast<std::string_view::size_type>(e - b)};
}

}  // namespace format_impl

template <typename String, typename... Args>
std::string format(String str, Args&&... args) {
  using namespace format_impl;
  using namespace boost::hana;
  using std::string_view;

  constexpr auto Size = sizeof...(args);
  std::array<std::string, Size> fields;

  {
    auto tuple = make_tuple(std::forward<Args&&>(args)...);
    auto indices = make_range(size_c<0>, size_c<Size>);
    std::stringstream ss;
    for_each(indices, [&](auto x) {
      constexpr auto index = value(x);
      ss.str(std::string{});
      ss << tuple[x];
      fields[index] = ss.str();
    });
  }

  constexpr auto view = string_view(str.c_str());
  constexpr auto counter = parser{view}.parse<count_visitor<Size>>();

  if constexpr (counter.total == 0) {
    return str.c_str();
  } else {
    static_assert(counter.up < static_cast<int>(Size),
                  "replacement index out of range");

    auto buffer_size = view.size();
    for (auto i = 0u; i < fields.size(); ++i) {
      buffer_size += counter.fields[i] * fields[i].size();
    }

    using boost::iostreams::basic_array;
    using boost::iostreams::stream_buffer;

    std::string result(buffer_size, 0);
    stream_buffer<basic_array<char>> buffer(result.data(), result.size());
    std::ostream stream(&buffer);

    auto parser = format_impl::parser{view};
    auto count = 0;
    auto prev = 0;
    for (auto pos = parser.step(0); pos != string_view::npos;
         pos = parser.step(pos)) {
      stream << make_string_view(view.data() + prev, view.data() + pos);
      auto b = pos;
      if (parser.escaped(pos)) {
        stream << view[b];
      } else {
        int index;
        parser.indexed(pos, index);
        if (index < 0) {
          index = count;
        }
        ++count;
        stream << fields[index];
      }
      prev = pos;
    }

    stream << make_string_view(view.data() + prev, view.data() + view.size());

    result.resize(stream.tellp());
    return result;
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
