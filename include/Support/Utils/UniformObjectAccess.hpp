//
//
//

#ifndef PEDIGREE_UNIFORMOBJECTACCESS_HPP
#define PEDIGREE_UNIFORMOBJECTACCESS_HPP

#include <cassert>
// using assert

namespace pedigree {

template <typename T> T &ToObj(T &Obj) { return Obj; }

template <typename T> T &ToObj(T *Ptr) {
  assert(Ptr && "Pointer is null!");
  return *Ptr;
}

//

template <typename T> T *ToPtr(T &Obj) { return &Obj; }

template <typename T> T *ToPtr(T *Ptr) { return Ptr; }

} // namespace pedigree

#endif // header
