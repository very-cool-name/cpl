#include <vector>
#include <functional>
#include <iterator>
#include <iostream>
#include <cassert>

#include "segment_tree.hpp"

void test_sum() {
  std::vector<int> data = {1, 2, 3, 4, 5, 6};
  cpl::SegmentTree<int, std::plus<int>> tree(std::begin(data), std::end(data));
  for (int i = 0; i < data.size(); ++i)
    assert(tree.Get(i, i) == data[i]);
  assert(tree.Get(0, 5) == 21);
  assert(tree.Get(0, 2) == 6);
  assert(tree.Get(2, 3) == 7);
  tree.Set(0, 3);
  assert(tree.Get(0, 5) == 23);
  assert(tree.Get(0, 3) == 12);
  assert(tree.Get(4, 5) == 11);
}

template<class T>
struct greater {
  T operator()(const T& lhs, const T& rhs) {
    return std::max(lhs, rhs);
  }
};

void test_max() {
  std::vector<int> data = {1, 2, 3, 4, 5, 6};
  cpl::SegmentTree<int, greater<int>> tree(std::begin(data), std::end(data));
  for (int i = 0; i < data.size(); ++i)
    assert(tree.Get(i, i) == data[i]);
  assert(tree.Get(0, 5) == 6);
  assert(tree.Get(0, 2) == 3);
  assert(tree.Get(2, 3) == 4);
  tree.Set(0, 4);
  assert(tree.Get(0, 5) == 6);
  assert(tree.Get(0, 3) == 4);
  assert(tree.Get(4, 5) == 6);
}

int main() {
  test_sum();
  test_max();
}