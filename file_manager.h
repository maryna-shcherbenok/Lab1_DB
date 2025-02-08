#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include "struct.h"

/* Глобальні файлові покажчики */
extern FILE *fpAuthors;
extern FILE *fpBooks;

/* Прототипи функцій роботи з файлами */
int getAuthorRecordCount();
int getBookRecordCount();
Author readAuthor(int recNo);
void writeAuthor(int recNo, Author *a);
Book readBook(int recNo);
void writeBook(int recNo, Book *b);

#endif /* FILE_MANAGER_H */
