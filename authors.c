#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "authors.h"
#include "struct.h"       // містить визначення структур
#include "file_manager.h" // для роботи з файлами
#include "books.h"        // для оголошення зовнішніх змінних booksGarbage

/* Зовнішні змінні з файлу books.c */
extern int booksGarbage[MAX_BOOKS];
extern int booksGarbageCount;

/* Індекс авторів та зона для логічно вилучених записів */
AuthorIndex authorIndexArray[MAX_AUTHORS];
int authorIndexCount = 0;
int authorsGarbage[MAX_AUTHORS];
int authorsGarbageCount = 0;

/* Локальна функція для порівняння записів індексу */
static int cmpAuthorIndex(const void *a, const void *b) {
    AuthorIndex *ai = (AuthorIndex *)a;
    AuthorIndex *bi = (AuthorIndex *)b;
    return (ai->key - bi->key);
}

/* Локальна функція для додавання запису до індексної таблиці */
static void addAuthorIndex(int key, int recNo) {
    if (authorIndexCount >= MAX_AUTHORS) {
        printf("Індекс авторів переповнений!\n");
        return;
    }
    authorIndexArray[authorIndexCount].key = key;
    authorIndexArray[authorIndexCount].recNo = recNo;
    authorIndexCount++;
    qsort(authorIndexArray, authorIndexCount, sizeof(AuthorIndex), cmpAuthorIndex);
}

/* Локальна функція для видалення запису з індексної таблиці */
static void removeAuthorIndex(int key) {
    int i, j;
    for (i = 0; i < authorIndexCount; i++) {
        if (authorIndexArray[i].key == key) {
            for (j = i; j < authorIndexCount - 1; j++) {
                authorIndexArray[j] = authorIndexArray[j+1];
            }
            authorIndexCount--;
            break;
        }
    }
}

/* Локальна функція для пошуку у індексній таблиці за ключем */
static int findAuthorInIndex(int key) {
    int low = 0, high = authorIndexCount - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (authorIndexArray[mid].key == key)
            return authorIndexArray[mid].recNo;
        else if (authorIndexArray[mid].key < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

/* Функція пошуку автора за id */
int getAuthorById(int id, Author *author, int *recNo) {
    int r = findAuthorInIndex(id);
    if (r == -1) return 0;
    *recNo = r;
    *author = readAuthor(r);
    if (author->isDeleted) return 0;
    return 1;
}

/* Функція завантаження індексної таблиці авторів з файлу */
void loadAuthorIndex() {
    int recCount = getAuthorRecordCount();
    authorIndexCount = 0;
    for (int i = 0; i < recCount; i++) {
        Author a = readAuthor(i);
        if (!a.isDeleted) {  // додаємо лише активні записи
            addAuthorIndex(a.id, i);
        }
    }
}

/* get-m: Вивід інформації про автора та його книги */
void get_m() {
    int id;
    printf("Enter author id for search: ");
    scanf("%d", &id);
    getchar();  // зчитуємо символ нового рядка після числа
    Author a;
    int recNo;
    if (!getAuthorById(id, &a, &recNo)) {
        printf("Author with id=%d not found!\n", id);
        return;
    }
    printf("=== Author Data ===\n");
    printf("id: %d\nName: %s\nBirth Year: %d\nPhone: %s\nEmail: %s\nBook Count: %d\n",
           a.id, a.name, a.birth_year, a.phone, a.email, a.bookCount);
    printf("First book record: %d\n", a.firstBook);

    int bookRec = a.firstBook;
    if (bookRec == -1) {
        printf("No books found for this author.\n");
        return;
    }
    printf("=== Author's Books ===\n");
    while (bookRec != -1) {
        Book b = readBook(bookRec);
        if (!b.isDeleted) {
            printf("Book id: %d | Title: %s | Publisher: (%d) %s | nextBook: %d\n",
                   b.id, b.title, b.publisherId, b.publisherName, b.nextBook);
        }
        bookRec = b.nextBook;
    }
}

/* insert-m: Додавання нового автора */
void insert_m() {
    Author a;
    printf("Enter author id: ");
    scanf("%d", &a.id);
    getchar(); // очищення залишкового '\n'

    printf("Enter author name: ");
    fgets(a.name, NAME_LEN, stdin);
    a.name[strcspn(a.name, "\n")] = '\0';  // видаляємо символ нового рядка

    printf("Enter birth year: ");
    scanf("%d", &a.birth_year);
    getchar(); // очищення залишкового '\n'

    printf("Enter author phone: ");
    fgets(a.phone, PHONE_LEN, stdin);
    a.phone[strcspn(a.phone, "\n")] = '\0';

    printf("Enter author email: ");
    fgets(a.email, EMAIL_LEN, stdin);
    a.email[strcspn(a.email, "\n")] = '\0';

    a.firstBook = -1;
    a.bookCount = 0;
    a.isDeleted = 0;

    int recNo;
    if (authorsGarbageCount > 0) {
        recNo = authorsGarbage[--authorsGarbageCount];
        fseek(fpAuthors, recNo * sizeof(Author), SEEK_SET);
    } else {
        recNo = getAuthorRecordCount();
        fseek(fpAuthors, 0, SEEK_END);
    }
    fwrite(&a, sizeof(Author), 1, fpAuthors);
    fflush(fpAuthors);

    addAuthorIndex(a.id, recNo);
    printf("Author added at record number %d\n", recNo);
}

/* update-m: Оновлення даних автора */
void update_m() {
    int authId;
    printf("Enter author id to update: ");
    scanf("%d", &authId);
    getchar();  // очищення залишкового '\n'

    Author a;
    int aRec;
    if (!getAuthorById(authId, &a, &aRec)) {
        printf("Author with id=%d not found!\n", authId);
        return;
    }
    printf("Updating author data (name, birth year, phone, email).\n");

    printf("Enter new name: ");
    fgets(a.name, NAME_LEN, stdin);
    a.name[strcspn(a.name, "\n")] = '\0';

    printf("Enter new birth year: ");
    scanf("%d", &a.birth_year);
    getchar();

    printf("Enter new phone: ");
    fgets(a.phone, PHONE_LEN, stdin);
    a.phone[strcspn(a.phone, "\n")] = '\0';

    printf("Enter new email: ");
    fgets(a.email, EMAIL_LEN, stdin);
    a.email[strcspn(a.email, "\n")] = '\0';

    writeAuthor(aRec, &a);
    printf("Author data updated.\n");
}

/* del-m: Логічне видалення автора та його книг */
void del_m() {
    int authId;
    printf("Enter author id to delete: ");
    scanf("%d", &authId);
    getchar();

    Author a;
    int aRec;
    if (!getAuthorById(authId, &a, &aRec)) {
        printf("Author with id=%d not found!\n", authId);
        return;
    }
    a.isDeleted = 1;
    writeAuthor(aRec, &a);
    authorsGarbage[authorsGarbageCount++] = aRec;
    removeAuthorIndex(authId);

    int bookRec = a.firstBook;
    while (bookRec != -1) {
        Book b = readBook(bookRec);
        if (!b.isDeleted) {
            b.isDeleted = 1;
            writeBook(bookRec, &b);
            booksGarbage[booksGarbageCount++] = bookRec;
        }
        bookRec = b.nextBook;
    }
    printf("Author and all their books have been logically deleted.\n");
}

/* calc-m: Підрахунок активних авторів */
void calc_m() {
    int total = 0;
    int recCount = getAuthorRecordCount();
    for (int i = 0; i < recCount; i++) {
        Author a = readAuthor(i);
        if (!a.isDeleted)
            total++;
    }
    printf("Active authors: %d\n", total);
}

/* ut-m: Вивід усіх записів файлу authors.fl (разом із службовими полями) */
void ut_m() {
    int recCount = getAuthorRecordCount();
    printf("=== Content of file authors.fl ===\n");
    for (int i = 0; i < recCount; i++) {
        Author a = readAuthor(i);
        printf("Record %d: id=%d, name=%s, birth_year=%d, phone=%s, email=%s, firstBook=%d, bookCount=%d, isDeleted=%d\n",
               i, a.id, a.name, a.birth_year, a.phone, a.email, a.firstBook, a.bookCount, a.isDeleted);
    }
}
