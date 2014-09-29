#ifndef __LIST_H__
#define __LIST_H__


struct node {
	int value;
	struct node *next;
};

int insert(const int value, struct node **head);
void list_print(struct node *head);
void list_clear(struct node *list);

#endif // __LIST_H__
