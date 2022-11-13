#include "../myList.h"
#include "errorHelpers.h"
//USAGE_STRING="val"

int main(int argc, char** argv) {
    printf("%d\n", USAGE(argc == 2));
    int val = atoi(argv[1]);

    MyList* list = newMyList();
    insertValLast(list, 0);
    insertValFirst(list, val);
    printMyList(list, "%d ");
    deleteMyList(list);	
}