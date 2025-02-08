#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"

FILE *fpAuthors = NULL;
FILE *fpBooks = NULL;

/* Обчислює кількість записів у файлі авторів */
int getAuthorRecordCount() {
    fseek(fpAuthors, 0, SEEK_END);
    long size = ftell(fpAuthors);
    return (int)(size / sizeof(Author));
}

/* Обчислює кількість записів у файлі книг */
int getBookRecordCount() {
    fseek(fpBooks, 0, SEEK_END);
    long size = ftell(fpBooks);
    return (int)(size / sizeof(Book));
}

/* Зчитування запису автора за номером запису */
Author readAuthor(int recNo) {
    Author a;
    fseek(fpAuthors, recNo * sizeof(Author), SEEK_SET);
    fread(&a, sizeof(Author), 1, fpAuthors);
    return a;
}

/* Запис даних автора за номером запису */
void writeAuthor(int recNo, Author *a) {
    fseek(fpAuthors, recNo * sizeof(Author), SEEK_SET);
    fwrite(a, sizeof(Author), 1, fpAuthors);
    fflush(fpAuthors);
}

/* Зчитування запису книги за номером запису */
Book readBook(int recNo) {
    Book b;
    fseek(fpBooks, recNo * sizeof(Book), SEEK_SET);
    fread(&b, sizeof(Book), 1, fpBooks);
    return b;
}

/* Запис даних книги за номером запису */
void writeBook(int recNo, Book *b) {
    fseek(fpBooks, recNo * sizeof(Book), SEEK_SET);
    fwrite(b, sizeof(Book), 1, fpBooks);
    fflush(fpBooks);
}
