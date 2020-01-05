#ifndef TREE_EXCEPTIONS_
#define TREE_EXCEPTIONS_

#include <iostream>

namespace Tree {
class KeyExists : public std::exception {};
class KeyNotExists : public std::exception {};
class TreeIsEmpty : public std::exception {};
}  // namespace Tree

#endif  // TREE_EXCEPTIONS_
