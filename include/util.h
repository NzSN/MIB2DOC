#ifndef _UTIL_H_
#define _UTIL_H

#include "dispatcher.h"
#include "hash.h"

int stringToIdentList(char *str, hashMap *map, char seperator);
int tableRecognize(char *buffer, int size);
int entryRecognize(char *buffer, int size);
char * randStr(int length);
char * numberToStr(int num);

void * Malloc(size_t size);
_Bool isStringEqual(const char *strLeft, const char *strRight);

#endif /* _UTIL_H_ */
