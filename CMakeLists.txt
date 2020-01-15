cmake_minimum_required(VERSION 3.15)
project(DS2)

set(CMAKE_CXX_STANDARD 14)

add_executable(DS2 main.cpp unionFind.cpp hash.cpp src1/list.cpp src1/avlTree.cpp src1/node.cpp src1/treeExceptions.cpp)