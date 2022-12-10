#ifndef MY_LIST_H
#define MY_LIST_H

#include "options.h"
#include "easyAttr.h"
#include "errorHelpers.h"
#include "easyCheck.h"
#include "littleHelpers.h"

// lol these List methods are just a slightly upgraded version from
// my Sem1 early exam prep
#ifndef LIST_TYPE
#if MUNDANE_MESSAGES
//#pragma message "LIST_TYPE undefined, defaulting to int"
#endif // MUNDANE_MESSAGES
#define LIST_TYPE int
#endif // LIST_TYPE

typedef struct MyNode_ {
	LIST_TYPE val;
	struct MyNode_ *next;
} MyNode;

typedef struct MyList_ {
	MyNode *head;
	size_t size; // number of nodes
} MyList;

extern MyNode* MY_WARN_UNUSED
	newMyNode (LIST_TYPE val);

extern MyList* MY_WARN_UNUSED
	newMyList (void);

extern int MY_NON_NULL(1, 2) MY_FORMAT(printf, 2, 0)
	printMyList(const MyList* const list, const char* restrict format);

extern size_t MY_NON_NULL(1) MY_WARN_UNUSED
	myListLength(const MyList* const list);

extern MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popAt(MyList* const list, size_t pos);

extern MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	peekNodeAt(const MyList* const list, size_t pos);

extern LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekValAt(const MyList* const list, size_t pos);

extern LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popValAt(MyList* const list, size_t pos);

extern LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekFirstVal(const MyList* const list);

extern LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekLastVal(const MyList* const list);

extern LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popFirstVal(MyList* const list);

extern LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popLastVal(MyList* const list);

extern MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popLast(MyList* const list);

extern MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popFirst(MyList* const list);

extern MyList* MY_NON_NULL(1)
	clearMyList(MyList* const list);

extern void MY_NON_NULL(1)
	deleteMyList (MyList* list);

extern MyList* MY_NON_NULL(1)
	deleteAt(MyList* const list, size_t pos);

extern MyList* MY_NON_NULL(1)
	deleteLast(MyList* const list);

extern MyList* MY_NON_NULL(1)
	deleteFirst(MyList* const list);

extern MyList* MY_NON_NULL(1, 3)
	insertAfter(MyList* const list, size_t pos, MyNode* const newMyNode);

extern MyList* MY_NON_NULL(1, 3)
	insertBefore(MyList* const list, size_t pos, MyNode* const newMyNode);

extern MyList* MY_NON_NULL(1)
	insertValAfter(MyList* const list, size_t pos, LIST_TYPE newVal);

extern MyList* MY_NON_NULL(1)
	insertValBefore(MyList* const list, size_t pos, LIST_TYPE newVal);

extern MyList* MY_NON_NULL(1, 2)
	insertLast(MyList* const list, MyNode* const node);

extern MyList* MY_NON_NULL(1, 2)
	insertFirst(MyList* const list, MyNode* const node);

extern MyList* MY_NON_NULL(1)
	insertValLast(MyList* const list, LIST_TYPE newVal);

extern MyList* MY_NON_NULL(1)
	insertValFirst(MyList* const list, LIST_TYPE newVal);

extern bool MY_NON_NULL(1, 2)
	compLessThan(const LIST_TYPE* const a, const LIST_TYPE* const b);

extern MyList* MY_NON_NULL(1)
	sortMyList_(MyList* const list, bool (*comp)(const LIST_TYPE* const a, const LIST_TYPE* const b));
#define sortMyList_1_ARG(list) sortMyList_(list, compLessThan)
#define sortMyList_2_ARGS(list, comp) sortMyList_(list, comp)
#define sortMyList_X_ARGS(x, A, B, FUNC, ...) FUNC
#define sortMyList(...) sortMyList_X_ARGS(, ##__VA_ARGS__,\
	sortMyList_2_ARGS(__VA_ARGS__),\
	sortMyList_1_ARG(__VA_ARGS__),\
	sortMyList_0_ARGS(__VA_ARGS__)) // Wimplicit-function-declaration

#endif // MY_LIST_H
