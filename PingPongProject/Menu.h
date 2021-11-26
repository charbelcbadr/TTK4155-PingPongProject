/*
 * Menu.h
 *
 * Created: 04.10.2021 18:17:44
 *  Author: charbelb
 */ 


#ifndef MENU_H_
#define MENU_H_

//Structure for the creating the Circular linked list
typedef struct Node {
	char* data;
	int line;
	int column;
	struct Node* next;
	struct Node* prev;
}Node;

Node* create_new_node(char* data,int size);
Node* displayMenu();
void append_linked_list(char* data);
void displayArrow(int line,int column);
void clearArrow(int line,int column);
Node* Sub_displayMenu();
Node* Sub_displayMenu_contrast();

#endif /* MENU_H_ */