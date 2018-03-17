#ifndef _36BF5C5E_B38C_4308_AE68_1ECDCB17734B_HPP
#define _36BF5C5E_B38C_4308_AE68_1ECDCB17734B_HPP

#include <array>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>

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

template <int Size>
struct Test {
  constexpr Test() = default;
  std::array<int, Size> counts;
  int no = 0;
};

}  // namespace format_detail

template <typename String, typename... Args>
std::string format(String str, Args&&... args) {
  constexpr auto Size = sizeof...(args);
  std::array<std::string, Size> fields;

  {
    std::stringstream ss;
    int i = 0;
    ((ss.str(std::string{}), ss << args, fields[i++] = ss.str()), ...);
  }

  constexpr std::string_view view(str.c_str());

  using Test = format_detail::Test<Size>;

  constexpr auto test = format_detail::parse<Test>(
      view, [](std::size_t, Test&) {},
      [](std::size_t start, std::size_t size, int index, Test& t) {
        if (index < 0) index = t.no++;

        if (index >= Size)
          throw "compile time error: replacement index out of range";

        ++t.counts[index];
      });

  auto buffer_size = view.size();

  for (auto i = 0u; i < test.counts.size(); ++i) {
    buffer_size += test.counts[i] * fields[i].size();
  }

  using boost::iostreams::basic_array;
  using boost::iostreams::stream_buffer;

  std::string result(buffer_size, 0);
  stream_buffer<basic_array<char>> buffer(result.data(), result.size());
  std::ostream stream(&buffer);

  std::size_t pos = 0;
  int no = 0;
  format_detail::parse<int>(
      view,
      [&](std::size_t start, int&) {
        stream << view.substr(pos, start - pos + 1);
        pos = start + 2;
      },
      [&](std::size_t start, std::size_t size, int index, int&) {
        if (index < 0) index = no++;
        stream << view.substr(pos, start - pos);
        stream << fields[index];
        pos = start + size;
      });

  stream << view.substr(pos);

  result.resize(stream.tellp());
  return result;
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
