#ifndef CPL_SEGMENT_TREE_HPP_INCLUDED
#define CPL_SEGMENT_TREE_HPP_INCLUDED

// Segment tree implementation based on http://e-maxx.ru/algo/segment_tree

#include <vector>
#include <ostream>
#include <algorithm>

namespace cpl {
template<class Type, class Operation>
class SegmentTree {
  public:
  //Constructs segment tree from random-access Iterator range
  template<class Iterator>
  SegmentTree(Iterator begin, Iterator end);

  void Set(int idx, Type new_value);
  Type Get(int l, int r) const;
  void Show(std::ostream& out) const;

  private:
  template<class Iterator>
  void BuildTree(int v, Iterator begin, Iterator end);

  Type Get(int v, int tl, int tr, int l, int r) const;
  void Set(int v, int tl, int tr, int idx, Type new_val);

  std::vector<Type> data_;
  int size_;
};

template<class Type, class Operation>
template<class Iterator>
SegmentTree<Type, Operation>::SegmentTree(Iterator begin, Iterator end) {
  size_ = end - begin;
  // When size_ is not power of two, there are empty cells in tree and it is not balanced
  // So reserve twice size for nodes.
  data_.resize(size_ * 4);
  BuildTree(1, begin, end);
}

template<class Type, class Operation>
void SegmentTree<Type, Operation>::Set(int idx, Type new_value) {
  Set(1, 0, size_ - 1, idx, new_value);
}

template<class Type, class Operation>
Type SegmentTree<Type, Operation>::Get(int l, int r) const {
  return Get(1, 0, size_ - 1, l, r);
}

template<class Type, class Operation>
void SegmentTree<Type, Operation>::Show(std::ostream& out) const {
  for (int i = 0; i < data_.size(); ++i) {
    out << i << ' ' << data_[i] << '\n';
  }
}

template<class Type, class Operation>
template<class Iterator>
void SegmentTree<Type, Operation>::BuildTree(int v, Iterator begin, Iterator end) {
  //v is vertex to fill
  if (begin == end) {
    return;
  } else if (begin == end - 1) { // here is leaf node
    data_[v] = *begin;
  } else { // 1.find middle 2. go to left and right subarrays 3. Invoke Operation on result
    auto middle = begin + (end - begin + 1) / 2;
    int left = v << 1;
    int right = (v << 1) + 1;
    BuildTree(left, begin, middle);
    BuildTree(right, middle, end);
    data_[v] = Operation()(data_[left], data_[right]);
  }
}

template<class Type, class Operation>
Type SegmentTree<Type, Operation>::Get(int v, int tl, int tr, int l, int r) const {
  if (l == tl && r == tr) // segment we are looking for
    return data_[v];
  if (tl == tr || r < l)
    return 0;
  int mid = (tl + tr) >> 1;
  return Operation()(Get((v << 1), tl, mid, l, std::min(r, mid)),
                     Get((v << 1) + 1, mid + 1, tr, std::max(l, mid + 1), r));
}

template<class Type, class Operation>
void SegmentTree<Type, Operation>::Set(int v, int tl, int tr, int idx, Type new_val) {
  if (tl == tr) { // leaf node
    data_[v] = new_val;
  } else { // go to left or right and set there, then update current node with Operation
    int mid = (tl + tr) >> 1;
    int left = v << 1;
    int right = (v << 1) + 1;
    if (idx <= mid)
      Set(left, tl, mid, idx, new_val);
    else
      Set(right, mid + 1, tr, idx, new_val);
    data_[v] = Operation()(data_[left], data_[right]);
  }
}
} // namespace cpl

#endif // CPL_SEGMENT_TREE_HPP_INCLUDED