#ifndef BOOKS_H
#define BOOKS_H

#include "struct.h"

/* Функції роботи з книгами (slave) */
void get_s();
void insert_s();
void update_s();
void del_s();
void calc_s();
void ut_s();

/* Зовнішні змінні для сміттєвої зони книг */
extern int booksGarbage[];
extern int booksGarbageCount;

#endif /* BOOKS_H */
