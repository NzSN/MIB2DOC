/* iterator.h
 * This file declare common interface of all iterators that used in this project */

#define successor(Iter) \
    _Generic((Iter),\
             slice : sliceNext)(Iter)
