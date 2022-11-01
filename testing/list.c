#define ERR_MULTIPROCESS 0
#define USAGE_STRING "value"
#include "../myList.h"

int main(int argc, char** argv) {
    printf("%d\n", USAGE(argc == 2));
    int val = atoi(argv[1]);

    MyList* list = newMyList();
    insertValLast(list, 0);
    insertValFirst(list, val);
    printMyList(list, "%d ");
    deleteMyList(list);	
}