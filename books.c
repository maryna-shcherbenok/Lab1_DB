#include <stdio.h>
#include <string.h>
#include "books.h"
#include "struct.h"
#include "file_manager.h"
#include "authors.h"

/* Визначення сміттєвої зони для книг */
int booksGarbage[MAX_BOOKS];
int booksGarbageCount = 0;

/* Для виводу статистики за авторами (використовуємо індекс із authors.c) */
extern AuthorIndex authorIndexArray[];
extern int authorIndexCount;

/* get-s: Пошук конкретної книги для заданого автора */
void get_s() {
    int authId, bookId;
    printf("Enter author id: ");
    scanf("%d", &authId);
    getchar();
    Author a;
    int aRec;
    if (!getAuthorById(authId, &a, &aRec)) {
        printf("Author with id=%d not found!\n", authId);
        return;
    }
    printf("Enter book id to search: ");
    scanf("%d", &bookId);
    getchar();

    int bookRec = a.firstBook;
    int found = 0;
    while (bookRec != -1) {
        Book b = readBook(bookRec);
        if (!b.isDeleted && b.id == bookId) {
            printf("Found book:\n");
            printf("id: %d\nTitle: %s\nPublisher: (%d) %s\nISBN: %s\nGenre: %s\nAvailability: %d\nPage Count: %d\nAuthor id: %d\n",
                   b.id, b.title, b.publisherId, b.publisherName, b.isbn, b.genre, b.availability, b.pageCount, b.authorId);
            found = 1;
            break;
        }
        bookRec = b.nextBook;
    }
    if (!found)
        printf("Book with id=%d not found for author with id=%d.\n", bookId, authId);
}

/* insert-s: Додавання нової книги для заданого автора */
void insert_s() {
    int authId;
    printf("Enter author id for which to add a book: ");
    scanf("%d", &authId);
    getchar();
    Author a;
    int aRec;
    if (!getAuthorById(authId, &a, &aRec)) {
        printf("Author with id=%d not found!\n", authId);
        return;
    }
    Book b;
    printf("Enter book id: ");
    scanf("%d", &b.id);
    getchar();
    printf("Enter book title: ");
    fgets(b.title, TITLE_LEN, stdin);
    b.title[strcspn(b.title, "\n")] = '\0';
    printf("Enter publisher id: ");
    scanf("%d", &b.publisherId);
    getchar();  // Очищення буфера вводу
    printf("Enter publisher name: ");
    fgets(b.publisherName, PUB_NAME_LEN, stdin);
    b.publisherName[strcspn(b.publisherName, "\n")] = '\0';

    /* Запит нових атрибутів книги */
    printf("Enter ISBN: ");
    fgets(b.isbn, ISBN_LEN, stdin);
    b.isbn[strcspn(b.isbn, "\n")] = '\0';

    printf("Enter genre: ");
    fgets(b.genre, GENRE_LEN, stdin);
    b.genre[strcspn(b.genre, "\n")] = '\0';

    printf("Enter availability (1 - available, 0 - not available): ");
    scanf("%d", &b.availability);
    getchar();

    printf("Enter page count: ");
    scanf("%d", &b.pageCount);
    getchar();

    /* Встановлюємо id автора книги */
    b.authorId = authId;

    b.isDeleted = 0;
    /* Вставка книги у список: нова книга вказує на поточний перший запис */
    b.nextBook = a.firstBook;
    int bookRec;
    if (booksGarbageCount > 0) {
        bookRec = booksGarbage[--booksGarbageCount];
        fseek(fpBooks, bookRec * sizeof(Book), SEEK_SET);
    } else {
        bookRec = getBookRecordCount();
        fseek(fpBooks, 0, SEEK_END);
    }
    fwrite(&b, sizeof(Book), 1, fpBooks);
    fflush(fpBooks);

    /* Оновлення запису автора: нова книга стає першою в списку */
    a.firstBook = bookRec;
    a.bookCount++;
    writeAuthor(aRec, &a);
    printf("Book added at record number %d for author id=%d\n", bookRec, authId);
}

/* update-s: Оновлення даних книги */
void update_s() {
    int authId, bookId;
    printf("Enter author id for updating a book: ");
    scanf("%d", &authId);
    getchar();
    Author a;
    int aRec;
    if (!getAuthorById(authId, &a, &aRec)) {
        printf("Author with id=%d not found!\n", authId);
        return;
    }
    printf("Enter book id to update: ");
    scanf("%d", &bookId);
    getchar();

    int prevRec = -1;
    int currRec = a.firstBook;
    int found = 0;
    while (currRec != -1) {
        Book b = readBook(currRec);
        if (!b.isDeleted && b.id == bookId) {
            found = 1;
            printf("Updating book data (title, publisher info, ISBN, genre, availability, page count).\n");
            printf("Enter new title: ");
            fgets(b.title, TITLE_LEN, stdin);
            b.title[strcspn(b.title, "\n")] = '\0';
            printf("Enter new publisher id: ");
            scanf("%d", &b.publisherId);
            getchar();
            printf("Enter new publisher name: ");
            fgets(b.publisherName, PUB_NAME_LEN, stdin);
            b.publisherName[strcspn(b.publisherName, "\n")] = '\0';
            printf("Enter new ISBN: ");
            fgets(b.isbn, ISBN_LEN, stdin);
            b.isbn[strcspn(b.isbn, "\n")] = '\0';
            printf("Enter new genre: ");
            fgets(b.genre, GENRE_LEN, stdin);
            b.genre[strcspn(b.genre, "\n")] = '\0';
            printf("Enter new availability (1 or 0): ");
            scanf("%d", &b.availability);
            getchar();
            printf("Enter new page count: ");
            scanf("%d", &b.pageCount);
            getchar();
            /* authorId залишається незмінним */
            writeBook(currRec, &b);
            printf("Book data updated.\n");
            break;
        }
        prevRec = currRec;
        currRec = b.nextBook;
    }
    if (!found)
        printf("Book with id=%d not found for author with id=%d.\n", bookId, authId);
}

/* del-s: Логічне видалення книги */
void del_s() {
    int authId, bookId;
    printf("Enter author id: ");
    scanf("%d", &authId);
    getchar();
    Author a;
    int aRec;
    if (!getAuthorById(authId, &a, &aRec)) {
        printf("Author with id=%d not found!\n", authId);
        return;
    }
    printf("Enter book id to delete: ");
    scanf("%d", &bookId);
    getchar();

    int currRec = a.firstBook, prevRec = -1;
    int found = 0;
    while (currRec != -1) {
        Book b = readBook(currRec);
        if (!b.isDeleted && b.id == bookId) {
            b.isDeleted = 1;
            writeBook(currRec, &b);
            booksGarbage[booksGarbageCount++] = currRec;
            if (prevRec == -1) {
                a.firstBook = b.nextBook;
            } else {
                Book prevBook = readBook(prevRec);
                prevBook.nextBook = b.nextBook;
                writeBook(prevRec, &prevBook);
            }
            a.bookCount--;
            writeAuthor(aRec, &a);
            printf("Book deleted.\n");
            found = 1;
            break;
        }
        prevRec = currRec;
        currRec = b.nextBook;
    }
    if (!found)
        printf("Book with id=%d not found for author with id=%d.\n", bookId, authId);
}

/* calc-s: Підрахунок активних книг та книг для кожного автора */
void calc_s() {
    int total = 0;
    int recCount = getBookRecordCount();
    for (int i = 0; i < recCount; i++) {
        Book b = readBook(i);
        if (!b.isDeleted)
            total++;
    }
    printf("Active books in total: %d\n", total);
    printf("Book count per author (according to bookCount field):\n");

    extern AuthorIndex authorIndexArray[];
    extern int authorIndexCount;
    for (int i = 0; i < authorIndexCount; i++) {
        Author a = readAuthor(authorIndexArray[i].recNo);
        if (!a.isDeleted)
            printf("Author id=%d, name=%s, books: %d\n", a.id, a.name, a.bookCount);
    }
}

/* ut-s: Вивід усіх записів файлу books.fl (разом із службовими полями) */
void ut_s() {
    int recCount = getBookRecordCount();
    printf("=== Content of file books.fl ===\n");
    for (int i = 0; i < recCount; i++) {
        Book b = readBook(i);
        printf("Record %d: id=%d, title=%s, publisherId=%d, publisherName=%s, isbn=%s, genre=%s, availability=%d, pageCount=%d, authorId=%d, nextBook=%d, isDeleted=%d\n",
               i, b.id, b.title, b.publisherId, b.publisherName, b.isbn, b.genre, b.availability, b.pageCount, b.authorId, b.nextBook, b.isDeleted);
    }
}
