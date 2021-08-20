#ifndef KLEE_RUST_H
#define KLEE_RUST_H

#include <string>

namespace klee {
std::string demangle_legacy_rust_symbol(std::string mangled);
}

#endif /* KLEE_RUST_H */