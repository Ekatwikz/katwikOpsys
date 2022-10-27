#ifndef MY_LIST_H
#define MY_LIST_H

#include "options.h"
#include "easyAttr.h"
#include "errorHelpers.h"
#include "easyCheck.h"

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
	MyNode *head,
	       *tail; // okay so let's get real
	// the only real reason to have a tail pointer in a singly
	// linked list is to have fast operations for the ends, but I
	// never used this lol, probably TODO: remove this lol
} MyList;

MyNode* MY_WARN_UNUSED
newMyNode (LIST_TYPE val) {
	MyNode *newMyNode;
	ERR_NULL( newMyNode = malloc(sizeof(MyNode)) );

	newMyNode->val = (LIST_TYPE) val;
	newMyNode->next = NULL;
	return newMyNode;
}

MyList* MY_WARN_UNUSED
newMyList (void) {
	MyList* list;

	// calloc to correctly set NULL pointers
	ERR_NULL( list = calloc(1, sizeof(MyList)) );
	return list;
}

int MY_NON_NULL(1, 2) MY_FORMAT(printf, 2, 0)
	printMyList(const MyList* const list, const char* restrict format) {
		NON_ATTR_IF(!list) {
			DBGonly(reason "\n");
		}

        int totalPrinted = 0;
		for (MyNode* node = list->head; node; node = node->next) {
			totalPrinted += printf_(format, node->val);
		}
		totalPrinted += printf_("\n");

		return totalPrinted;
	}

size_t MY_NON_NULL(1) MY_WARN_UNUSED
myListLength(const MyList* const list) {
	size_t length = 0;
	for (MyNode* node = list->head; node; node = node->next) {
		++length;
	}

	return length;
}

MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popAt(MyList* const list, size_t pos) {
		MyNode *prev = NULL, *node = list->head;

		// is this redundant btw? TODO: remove and check lol
		if (!list->head) {
			return NULL; // Can't delete from empty list
		}

		for (size_t i = 0; i < pos; ++i) {
			if (node) {
				prev = node;
				node = node->next;
			} else {
				return NULL; // Can't delete outside of list
			}
		}

		// idk there's a few SIGSEGVs without this lol
		// it's getting pretty messy now, I should rewrite this whole function tbh
		if (!node) {
			return NULL;
		}

		// If there's a previous node relink, otherwise we're deleting the head node, so update that
		prev ?
			(prev->next = node->next) :
			(list->head = node->next);

		// If we're deleting the tail node, update that
		if (!node->next) {
			list->tail = prev;
		}

		// clean up and return
		node->next = NULL;
		return node;
	}

MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	peekNodeAt(const MyList* const list, size_t pos) {
		MyNode* node = list->head;

		if (!list->head) {
			return NULL; // peek empty list
		}

		for (size_t i = 0; i < pos; ++i) {
			if (node) {
				node = node->next;
			} else {
				return NULL; // peek outside list
			}
		}

		return node;
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekValAt(const MyList* const list, size_t pos) {
		MyNode* node;

		// TODO: make this fancy or something lol
		if (NULL == (node = peekNodeAt(list, pos)) ) {
			errno = EINVAL;
			ERR("Can't peek value outside list");
		}

		return node->val;
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popValAt(MyList* const list, size_t pos) {
		MyNode* node;
		LIST_TYPE val;

		if (NULL == (node = popAt(list, pos)) ) {
			errno = EINVAL;
			ERR("Can't pop value outside list"); // ?
		}

		val = node->val;
		FREE(node); // we do a little leakn't

		return val;
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekFirstVal(const MyList* const list) {
		return peekValAt(list, 0);
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekLastVal(const MyList* const list) {
		size_t sz = myListLength(list);
		return peekValAt(list, sz - 1);
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popFirstVal(MyList* const list) {
		return popValAt(list, 0);
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popLastVal(MyList* const list) {
		size_t sz = myListLength(list);
		return popValAt(list, sz - 1);
	}

MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popLast(MyList* const list) {
		size_t sz = myListLength(list);
		return popAt(list, sz - 1);
	}

MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popFirst(MyList* const list) {
		return popAt(list, 0);
	}

MyList* MY_NON_NULL(1)
	clearMyList(MyList* const list) {
		for (MyNode* first; (first = popFirst(list));) {
			FREE(first);
		}

		return list;
	}

void MY_NON_NULL(1)
	deleteMyList (MyList* list) {
		clearMyList(list);
		FREE(list);
	}

MyList* MY_NON_NULL(1)
	deleteAt(MyList* const list, size_t pos) {
		MyNode* node = popAt(list, pos);
		FREE(node);
		return list;
	}

MyList* MY_NON_NULL(1)
	deleteLast(MyList* const list) {
		MyNode* node = popLast(list);
		FREE(node);
		return list;
	}

MyList* MY_NON_NULL(1)
	deleteFirst(MyList* const list) {
		MyNode* node = popAt(list, 0);
		FREE(node);
		return list;
	}

MyList* MY_NON_NULL(1, 3)
	insertAfter(MyList* const list, size_t pos, MyNode* const newMyNode) {
		MyNode *node = list->head;

		for (size_t i = 0; i < pos; ++i) { // Find the node we're inserting after
			if (node && node->next) { // Keep moving if this is a node and there's a next one
				node = node->next;
			} else {
				break; // will by default insert after the last one
			}
		}

		node == list->tail ? (list->tail = newMyNode) : (newMyNode->next = node->next);

		// Be careful in case we insertAfter in an empty list
		!node ? (list->head = newMyNode) : (node->next = newMyNode);
		return list;
	}

MyList* MY_NON_NULL(1, 3)
	insertBefore(MyList* const list, size_t pos, MyNode* const newMyNode) {
		MyNode *prev = NULL, *node = list->head;

		for (size_t i = 0; i < pos; ++i) { // Find the node we're inserting before
			if (node && node->next) {
				prev = node;
				node = node->next;
			} else {
				break; // will by default insert before the last one
			}
		}

		node == list->head ? (list->head = newMyNode) : (prev->next = newMyNode);

		// Be careful in case we insertBefore in an empty list
		!node ? (list->tail = newMyNode) : (newMyNode->next = node);
		return list;
	}

MyList* MY_NON_NULL(1)
	insertValAfter(MyList* const list, size_t pos, LIST_TYPE newVal) {
		// NB: returns list after allocation then insertion
		return insertAfter(list, pos, newMyNode(newVal));
	}

MyList* MY_NON_NULL(1)
	insertValBefore(MyList* const list, size_t pos, LIST_TYPE newVal) {
		return insertBefore(list, pos, newMyNode(newVal));
	}

MyList* MY_NON_NULL(1, 2)
	insertLast(MyList* const list, MyNode* const node) {
		size_t sz = myListLength(list);
		insertAfter(list, sz - 1, node);
		return list;
	}

MyList* MY_NON_NULL(1, 2)
	insertFirst(MyList* const list, MyNode* const node) {
		insertBefore(list, 0, node);
		return list;
	}
MyList* MY_NON_NULL(1)
	insertValLast(MyList* const list, LIST_TYPE newVal) {
		return insertLast(list, newMyNode(newVal));
	}

MyList* MY_NON_NULL(1)
	insertValFirst(MyList* const list, LIST_TYPE newVal) {
		return insertFirst(list, newMyNode(newVal));
	}

#endif // MY_LIST_H