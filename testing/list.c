#include "../myList.h"

int main(int argc, char** argv) {
    char* usageDescription = "val";

    usage_(argc == 2);
    int val = atoi(argv[1]);

    MyList* list = newMyList();
    insertValLast(list, 0);
    printf_("%ld\n", myListLength(list));
    insertValFirst(list, val);
    printf_("%ld\n", myListLength(list));
    printMyList(list, "%d ");
    printf_("%d ?%ld\n", popFirstVal(list), myListLength(list));
    printf_("?%ld\n", myListLength(list));
    deleteMyList(list);	
}