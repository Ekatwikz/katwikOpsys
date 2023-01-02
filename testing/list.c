#include "../myList.h"
#include "errorHelpers.h"

int main(int argc, char** argv) {
    char* usageDescription = "val";

    usage_(argc == 2);
    int val = atoi(argv[1]);

    MyList* list = newMyList();
    insertValLast(list, 0);
    insertValFirst(list, val);
    printMyList(list, "%d ");
    deleteMyList(list);	
}