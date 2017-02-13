#include "treap.hpp"

#include <cassert>

int main() {
  using Treap = cpl::Treap<int>;
  using TreapPtr = Treap::Ptr;

  //Insert & Find test
  TreapPtr t;
  for (int j = 5; j > 0; --j) {
    Treap::Insert(t, j);
    assert(Treap::Find(t, j));
  }
  
  //Split test
  TreapPtr tl, tr;
  Treap::Split(t, 2, tl, tr);
  for (int j = 1; j <= 2; ++j) {
    assert(Treap::Find(tl, j));
  }
  for (int j = 3; j <= 5; ++j) {
    assert(Treap::Find(tr, j));
  }

  //Merge test
  Treap::Merge(tl, tr, t);
  for (int j = 5; j > 0; --j) {
    assert(Treap::Find(t, j));
  }
}