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
             sliceIter*   : sliceSuccessor,\
             SYNTAX_iter* : syntaxSucessor,\
             INDEX_iter*  : indexSucessor)(Iter)

#define predecessor(Iter)\
    _Generic((Iter), \
             sliceIter*   : slicePredecessor,\
             SYNTAX_iter* : syntaxPredecessor,\
             INDEX_iter*  : indexPredecessor)(Iter)

#define source(Iter)\
    _Generic((Iter), \
             sliceIter*   : sliceSource,\
             SYNTAX_iter* : sliceSource,\
             INDEX_iter*  : sliceSource)(Iter)

#define sink(Iter_l, Iter_r)\
    _Generic((Iter_l), \
             sliceIter*   : sliceSink,\
             SYNTAX_iter* : sliceSink,\
             INDEX_iter*  : sliceSink)(Iter_l, Iter_r)

PreCond((Iter), Iterator(Iter) && (Reference(Iter) || Mutable(Iter)))
#define next(Iter)\
    _Generic((Iter), \
             sliceIter*   : sliceNext,\
             SYNTAX_iter* : syntaxNext,\
             INDEX_iter*  : indexNext)(Iter)

PreCond((Iter), Iterator(Iter) && (Reference(Iter) || Mutable(Iter)))
#define prev(Iter)\
    _Generic((Iter), \
             sliceIter*   : slicePrev,\
             SYNTAX_iter* : syntaxPrev,\
             INDEX_iter*  : indexPrev)(Iter)

#endif /* _ITERATORS_H_ */
