#ifndef CPL_TREAP_HPP_INCLUDED
#define CPL_TREAP_HPP_INCLUDED

#include <memory>
#include <random>
#include <chrono>

namespace cpl {

template<class Key>
class Treap {
 public:
  using Ptr = std::unique_ptr<Treap>;
  
  static Ptr Create(const Key& k) { return Ptr(new Treap<Key>(k)); }
  
  //leaves left and right empty
  static void Merge(Ptr& left, Ptr& right, Ptr& result);
  //leaves tree empty
  static void Split(Ptr& tree, int key, Ptr& left, Ptr& right);
  static void Insert(Ptr& tree, const Key& key);
  static bool Find(Ptr& tree, const Key& key);
  template<class F>
  static void Traverse(const Ptr& tree, F f);
  
  void Count(int& left, int& right) {
    if (left_) {
      ++left;
      left_->Count(left, right);
    }
    if (right_) {
      ++right;
      right_->Count(left, right);
    }
  }
  const Key& key() { return key_; }
  
 private:
  Treap(const Key& k);
  
  Key key_;
  int priority_;
  Ptr left_;
  Ptr right_;
};

template<class Key>
Treap<Key>::Treap(const Key& k)
  : key_(k) {
#ifdef DEFAULT_SEED
  static std::default_random_engine generator;
#else
  static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
#endif
  static std::uniform_int_distribution<int> distribution;
  priority_ = distribution(generator);
}

template<class Key>
void Treap<Key>::Merge(Ptr& left, Ptr& right, Ptr& result) {
  if (!left || !right) {
    result = (left) ? std::move(left) : std::move(right);
    return;
  }
  Ptr to_merge;
  if (left->priority_ > right->priority_) {
    result = std::move(left);
    if (result->right_)
      to_merge = std::move(result->right_);
    Merge(to_merge, right, result->right_);
  } else {
    result = std::move(right);
    if (result->left_)
      to_merge = std::move(result->left_);
    Merge(left, to_merge, result->left_);
  }
}

template<class Key>
void Treap<Key>::Split(Ptr& tree, int key, Ptr& left, Ptr& right) {
  if (!tree)
    return;
  if (tree->key_ > key) {
    right = std::move(tree);
    Ptr to_split = std::move(right->left_);
    Split(to_split, key, left, right->left_);
  } else {
    left = std::move(tree);
    Ptr to_split = std::move(left->right_);
    Split(to_split, key, left->right_, right);
  }
}

template<class Key>
void Treap<Key>::Insert(Ptr& tree, const Key& key) {
  Ptr mid = Create(key);
  if (!tree) {
    tree = std::move(mid);
    return;
  }
  Ptr left, right;
  Split(tree, key, left, right);
  Ptr leftmid;
  Merge(left, mid, leftmid);
  Merge(leftmid, right, tree);
}

template<class Key>
bool Treap<Key>::Find(Ptr& tree, const Key& key) {
  Ptr* t = &tree;
  while(*t) {
    if ((*t)->key_ == key)
      return true;
    if ((*t)->key_ < key)
      t = &((*t)->right_);
    else
      t = &((*t)->left_);
  }
  return false;
}

template<class Key>
template<class F>
void Treap<Key>::Traverse(const Ptr& tree, F f) {
  if(!tree)
    return;
  Traverse(tree->left_, f);
  f(tree->key_);
  Traverse(tree->right_, f);
}

} //namespace cpl

#endif // CPL_TREAP_HPP_INCLUDED
