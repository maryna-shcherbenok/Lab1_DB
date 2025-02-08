#ifndef AUTHORS_H
#define AUTHORS_H

#include "struct.h"

/* Функції роботи з авторами (master) */
void get_m();
void insert_m();
void update_m();
void del_m();
void calc_m();
void ut_m();

/* Функція пошуку автора за id */
int getAuthorById(int id, Author *author, int *recNo);

/* Функція завантаження індексної таблиці авторів з файлу */
void loadAuthorIndex();

#endif /* AUTHORS_H */
