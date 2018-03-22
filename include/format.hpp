#ifndef _36BF5C5E_B38C_4308_AE68_1ECDCB17734B_HPP
#define _36BF5C5E_B38C_4308_AE68_1ECDCB17734B_HPP

#include <array>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace format_detail {

constexpr int atoi(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  throw "compile time error: not a digit";
}

constexpr std::size_t stoi(std::string_view str) {
  std::size_t x = 0;
  for (auto c : str) {
    x *= 10;
    x += atoi(c);
  }
  return x;
}

template <typename Result, typename Escaped, typename Replacement>
constexpr Result parse(std::string_view const& view, Escaped escaped,
                       Replacement replacement) {
  Result result{};
  for (auto x = view.find_first_of('{'); x != std::string_view::npos;
       x = view.find_first_of('{', x)) {
    if (x == (view.size() - 1))
      throw "compile time error: non closed replacement";

    if (view[x + 1] == '{') {
      escaped(x, result);
      x += 2;
      continue;
    }

    auto start = x;
    x = view.find_first_of('}', x);
    if (x == std::string_view::npos)
      throw "compile time error: non closed replacement";

    ++x;

    auto repl = view.substr(start + 1, x - start - 2);
    int index = repl.empty() ? -1 : stoi(repl);
    replacement(start, x - start, index, result);
  }
  return result;
}

enum class Numbering { None, Manual, Automatic };

struct Test {
  constexpr Test() = default;
  Numbering numbering = Numbering::None;
  int no = 0;
};

template <typename T>
void Field_format(std::ostream& os, void const* t) {
  os << *reinterpret_cast<T const*>(t);
}

using Field_format_func = void (*)(std::ostream& os, void const*);

struct Field {
  void const* value = nullptr;
  Field_format_func func = nullptr;

  Field() = default;

  template <typename T>
  Field(T const& t) : value(&t), func(&Field_format<T>) {}

  void format(std::ostream& os) const { func(os, value); }
};

template <typename String, std::size_t Size>
struct Wrap {
  std::array<Field, Size> fields;

  std::string str() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }
};

template <typename String, std::size_t Size>
std::ostream& shift(std::ostream& os, Wrap<String, Size> const& wrap) {
  constexpr std::string_view view(String{}.c_str());

  constexpr auto test = format_detail::parse<Test>(
      view, [](std::size_t, Test&) {},
      [](std::size_t /*start*/, std::size_t /*size*/, int index, Test& test) {
        auto numbering = index < 0 ? Numbering::Automatic : Numbering::Manual;

        if (test.numbering == Numbering::None)
          test.numbering = numbering;
        else if (test.numbering != numbering)
          throw "compile time error: cannot switch field numbering type";

        if (index < 0) index = test.no++;

        if (static_cast<std::size_t>(index) >= Size)
          throw "compile time error: replacement index out of range";
      });
  (void)test;

  std::size_t pos = 0;
  int no = 0;
  format_detail::parse<int>(
      view,
      [&](std::size_t start, int&) {
        os << view.substr(pos, start - pos + 1);
        pos = start + 2;
      },
      [&](std::size_t start, std::size_t size, int index, int&) {
        if (index < 0) index = no++;
        os << view.substr(pos, start - pos);
        wrap.fields[index].format(os);
        pos = start + size;
      });

  os << view.substr(pos);
  return os;
}

template <typename String, std::size_t Size>
std::ostream& operator<<(std::ostream& os, Wrap<String, Size> const& wrap) {
  return shift(os, wrap);
}

}  // namespace format_detail

template <typename String, typename... Args>
auto format(String, Args const&... args) {
  return format_detail::Wrap<String, sizeof...(Args)>{args...};
}

#define CONSTEXPR_STRING(s)                                 \
  ([] {                                                     \
    struct tmp {                                            \
      static constexpr decltype(auto) c_str() { return s; } \
    };                                                      \
    return tmp{};                                           \
  }())

// ultra compile time fast format version
#define FORMAT(s)                                                              \
  [](auto&&... args) {                                                         \
    return format(CONSTEXPR_STRING(s), std::forward<decltype(args)>(args)...); \
  }

#endif
