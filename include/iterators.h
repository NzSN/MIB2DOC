/* iterator.h
 * This file declare common interface of all iterators that used in this project */

#ifndef _ITERATORS_H_
#define _ITERATORS_H_

#include "list.h"

/* Interfaces be included:
 * (1) successor
 * (2) predecessor
 * (3) source
 * (4) sink */

#define successor(Iter)\
    _Generic((Iter), \
             sliceIter : sliceSuccessor)(Iter)

#define predecessor(Iter)\
    _Generic((Iter), \
             sliceIter : slicePredecessor)(Iter)

#define source(Iter)\
    _Generic((Iter), \
             sliceIter : sliceSource)(Iter)

#define sink(Iter_l, Iter_r)\
    _Generic((Iter_l), \
             sliceIter : sliceSink)(Iter_l, Iter_r)

PreCond((Iter), Iterator(Iter) && (Reference(Iter) || Mutable(Iter)))
#define next(Iter)\
    _Generic((Iter), \
             sliceIter * : sliceNext)(Iter)

PreCond((Iter), Iterator(Iter) && (Reference(Iter) || Mutable(Iter)))
#define prev(Iter)\
    _Generic((Iter), \
             sliceIter * : slicePrev)(Iter)

#endif /* _ITERATORS_H_ */
