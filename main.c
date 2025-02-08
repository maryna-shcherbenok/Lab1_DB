#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"
#include "authors.h"
#include "books.h"

/* Вивід меню */
void menu() {
    printf("\n==== Menu ====\n");
    printf("1. get-m (Search and display author and their books)\n");
    printf("2. get-s (Search for a specific book for an author)\n");
    printf("3. insert-m (Add a new author)\n");
    printf("4. insert-s (Add a new book for an author)\n");
    printf("5. update-m (Update author data)\n");
    printf("6. update-s (Update book data)\n");
    printf("7. del-m (Delete an author and their books)\n");
    printf("8. del-s (Delete a book)\n");
    printf("9. calc-m (Count authors)\n");
    printf("10. calc-s (Count books)\n");
    printf("11. ut-m (Display all author records)\n");
    printf("12. ut-s (Display all book records)\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}

/* Головна функція */
int main() {
    int choice;

    /* Відкриття файлів. Якщо файли не існують, створюємо їх.
       Використовуємо розширення .fl */
    fpAuthors = fopen("authors.fl", "rb+");
    if (fpAuthors == NULL) {
        fpAuthors = fopen("authors.fl", "wb+");
        if (fpAuthors == NULL) {
            printf("Error opening file authors.fl\n");
            exit(1);
        }
    }
    fpBooks = fopen("books.fl", "rb+");
    if (fpBooks == NULL) {
        fpBooks = fopen("books.fl", "wb+");
        if (fpBooks == NULL) {
            printf("Error opening file books.fl\n");
            exit(1);
        }
    }

    /* Завантаження індексу авторів з файлу */
    loadAuthorIndex();

    do {
        menu();
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                get_m();
                break;
            case 2:
                get_s();
                break;
            case 3:
                insert_m();
                break;
            case 4:
                insert_s();
                break;
            case 5:
                update_m();
                break;
            case 6:
                update_s();
                break;
            case 7:
                del_m();
                break;
            case 8:
                del_s();
                break;
            case 9:
                calc_m();
                break;
            case 10:
                calc_s();
                break;
            case 11:
                ut_m();
                break;
            case 12:
                ut_s();
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Unknown option!\n");
                break;
        }
    } while (choice != 0);

    fclose(fpAuthors);
    fclose(fpBooks);
    return 0;
}
