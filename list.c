#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* your list function definitions */

int insert(const int value, struct node **head) {
	// Inserts a node into the list in sorted (increasing) order. 
	// Returns 0 if successful, -1 if not.
	struct node *temp = malloc(sizeof(struct node));
	if (temp == NULL) {
		printf("Error: No free memory found. \n");
		return -1; // Malloc was unsuccessful, so insert fails.
	}
	temp -> value = value;
	temp -> next = NULL;

	if (*head != NULL) {
		struct node *iter = *head;
		struct node *prev = NULL;
		while (iter != NULL) {
			if (iter -> value > value) {
				if (prev == NULL) {
					temp -> next = iter;
					(*head) = temp;
					return 0;
				}
				else {
					prev -> next = temp;
					temp -> next = iter;
					return 0;
				}
			}
			prev = iter;
			if (iter -> next == NULL) {
				iter -> next = temp;
				return 0;
			}
			iter = iter -> next;
		}
	}
	else {
		(*head) = temp;
	}
	return 0;
}

void list_print(struct node *head) {
	// Original code by Professor Sommers, modified by Bria.
	printf("***List contents begin***\n");
	while (head != NULL) {
		printf("%d\n", head -> value);
		head = head -> next;
	}
	printf("***List contents end***\n");
}

void list_clear(struct node *list) {
	// Code by Professor Sommers from HW3
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
        free(tmp);
    }
}
