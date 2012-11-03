//
// lock.h
//
// written by sjrct
//

#ifndef LOCK_H
#define LOCK_H

void getlock(void *, long);
int trylock(void *, long);
int unlock(void *, long);

#endif
