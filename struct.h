#ifndef STRUCT_H
#define STRUCT_H

#define MAX_AUTHORS     1000
#define MAX_BOOKS       1000

#define NAME_LEN        50
#define TITLE_LEN       100
#define PUB_NAME_LEN    50
#define PHONE_LEN       20
#define EMAIL_LEN       50
#define ISBN_LEN        20
#define GENRE_LEN       50

/* Структура запису автора (master-файл) */
typedef struct {
    int id;                 // id автора (ключ)
    char name[NAME_LEN];    // П.І.Б. автора
    int birth_year;         // рік народження
    char phone[PHONE_LEN];  // Номер телефону автора
    char email[EMAIL_LEN];  // Пошта автора
    int firstBook;          // номер першої книги або -1, якщо книг немає
    int bookCount;          // кількість книг даного автора
    int isDeleted;          // 0 – запис активний, 1 – логічно вилучений
} Author;

/* Структура запису книги (slave-файл) */
typedef struct {
    int id;                     // id книги (унікальний ідентифікатор книги)
    char title[TITLE_LEN];      // Назва книги
    int publisherId;            // id видавництва
    char publisherName[PUB_NAME_LEN]; // Назва видавництва
    int nextBook;               // номер наступної книги даного автора або -1
    int isDeleted;              // 0 – активний, 1 – логічно вилучений

    // Нові атрибути книги:
    char isbn[ISBN_LEN];        // ISBN книги
    char genre[GENRE_LEN];      // Жанр книги
    int availability;           // Статус наявності книги (1 – є, 0 – немає)
    int pageCount;              // Кількість сторінок
    int authorId;               // Ідентифікатор автора книги (для зручності)
} Book;

/* Структура індексної таблиці для авторів */
typedef struct {
    int key;    // id автора
    int recNo;  // номер запису у файлі authors.fl
} AuthorIndex;

#endif /* STRUCT_H */

