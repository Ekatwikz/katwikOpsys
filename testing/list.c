#include "../myList.h"

int main(int argc, char** argv) {
    char* usageDescription = "val";

    usage_(argc == 2);
    int val = atoi(argv[1]);

    MyList* list = newMyList();
    insertValLast(list, 0);
    printf_("%ld\n", myListLength(list));
    printMyList(list, "%d ");
    insertValFirst(list, val);
    printf_("%ld\n", myListLength(list));
    printMyList(list, "%d ");

    printf_("%d ?%ld\n", popFirstVal(list), myListLength(list)); // weird?
    printf_("?%ld\n", myListLength(list));
    deleteMyList(list);

    MyList* list2 = newMyList();
    insertValLast(list2, 3);
    insertValLast(list2, 2);
    insertValLast(list2, 1);
    insertValBefore(list2, 1, 5);

    printMyList(list2, "%d ");
    sortMyList(list2);
    printMyList(list2, "%d ");
    deleteMyList(list2);
}