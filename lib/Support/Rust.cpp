#include "klee/Support/Rust.h"

namespace klee {

std::string demangle_legacy_rust_symbol(std::string mangled) {
  // This func will not perform format check since it is designed
  // to only accept symbols from the compiler

  // Cut the useful segment out
  size_t start = 0;
  start = mangled.find("ZN");
  if (start == std::string::npos || start + 2 == mangled.size())
    return mangled;
  start += 2;
  size_t end = 0;
  end = mangled.find(".llvm.");
  mangled = mangled.substr(start, end - start);

  // Split string
  std::string demangled;
  while (mangled.size() && mangled[0] != 'E') {
    size_t segment_len = 0;
    size_t segment_start = 0;
    segment_len = (size_t)std::stoul(mangled, &segment_start, 10);
    size_t index = 0;
    index = segment_start;
    if (mangled[segment_start] == 'h')
      break;
    if (demangled.size())
      demangled.append("::");
    while (index < segment_len + segment_start) {
      if (mangled[index] == '$') {
        // escape part
        index++;
        std::string escaped;
        escaped = mangled.substr(index, mangled.find('$', index) - index);
        if (escaped == "SP")
          demangled.push_back('@');
        else if (escaped == "BP")
          demangled.push_back('*');
        else if (escaped == "RF")
          demangled.push_back('&');
        else if (escaped == "LT")
          demangled.push_back('<');
        else if (escaped == "GT")
          demangled.push_back('>');
        else if (escaped == "LP")
          demangled.push_back('(');
        else if (escaped == "RP")
          demangled.push_back(')');
        else if (escaped == "C")
          demangled.push_back(',');
        else if (escaped[0] == 'u')
          // unicode character
          demangled.push_back((char)std::stoul(
              escaped.substr(1, std::string::npos), nullptr, 16));
        else
          demangled.append(escaped);
        index = index + escaped.size() + 1;
      } else if (mangled[index] == '.') {
        if (mangled[index + 1] == '.') {
          demangled.append("::");
          index += 2;
        } else {
          demangled.push_back('.');
          index++;
        }
      } else {
        demangled.push_back(mangled[index++]);
      }
    }
    mangled = mangled.substr(segment_start + segment_len, std::string::npos);
  }

  return demangled;
}

} // namespace klee
